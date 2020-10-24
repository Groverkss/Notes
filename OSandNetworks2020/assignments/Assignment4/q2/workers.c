#include "libs.h"

const int PHARMA_MIN_TIME_MAKE = 2;
const int PHARMA_MAX_TIME_MAKE = 5;

const int PHARMA_BATCH_MIN = 1;
const int PHARMA_BATCH_MAX = 5;

const int PHARMA_CAP_MIN = 10;
const int PHARMA_CAP_MAX = 20;

const int STUDENT_WAIT_MIN = 1;
const int STUDENT_WAIT_MAX = 8;

int get_random(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

bool flip_coin(float p) {
    return ((float) rand() / (float) RAND_MAX) <= p;
}

volatile int REMAINING_STUDENTS = 0;
pthread_mutex_t student_lock = PTHREAD_MUTEX_INITIALIZER;

void *company_worker(void *arg) {
    int index_arg = ((Company*)arg)->index;
    while(true) {
        /* Calculate number of batches and batch capacity */
        companies_t[index_arg]->batches = get_random(PHARMA_BATCH_MIN, PHARMA_BATCH_MAX);
        companies_t[index_arg]->batch_capacity = get_random(PHARMA_CAP_MIN, PHARMA_CAP_MAX);

        /* Wait for vacine to be made */
        cprintf(CYAN, "Pharmaceutical Company %d is preparing %d batches of "
                "which have success probability %f\n", companies_t[index_arg]->index + 1,
                companies_t[index_arg]->batches, companies_t[index_arg]->prob);
        int wait_time = get_random(PHARMA_MIN_TIME_MAKE, PHARMA_MAX_TIME_MAKE);
        sleep(wait_time);
        cprintf(CYAN, "Pharmaceutical Company %d has prepared %d batches of "
                "which have success probability %f. Waiting for all vaccines"
                "to be used to resume production.\n", companies_t[index_arg]->index + 1,
                companies_t[index_arg]->batches, companies_t[index_arg]->prob);

        while(companies_t[index_arg]->batches != 0) {
            for (int i = 0; i < zones; i++) {
                /* Check if zone empty */

                /* Use try-lock to acquire */
                if (pthread_mutex_trylock(&zones_t[i]->lock)) {
                    continue; 
                }

                if (zones_t[i]->phase == EMPTY) {
                    /* Refill zones */
                    zones_t[i]->capacity = companies_t[index_arg]->batch_capacity;
                    zones_t[i]->prob = companies_t[index_arg]->prob;
                    zones_t[i]->phase = READY;
                    /* Release lock */
                    pthread_mutex_unlock(&zones_t[i]->lock);

                    /* Did this after unlocking becacuse I/O inside a lock
                     * is just bad */
                    cprintf(CYAN, "Pharmaceutical Company %d is delivering a"
                            "vaccine batch to Vaccination Zones %d which has"
                            "success probability %f\n", companies_t[index_arg]->index + 1, i + 1,
                            companies_t[index_arg]->prob);
                    cprintf(CYAN, "Pharmaceutical Company %d has delivered "
                            "vaccines to Vaccination zone %d, resuming"
                            "vaccinations now\n", companies_t[index_arg]->index + 1, i + 1);
                    companies_t[index_arg]->batches--;

                    /* Refill if finished */
                    if (companies_t[index_arg]->batches == 0) {
                        break;
                    }
                } else {
                    /* Release lock */
                    pthread_mutex_unlock(&zones_t[i]->lock);
                }

                
            }
        } 
        cprintf(CYAN, "All the vaccines prepared by Pharmaceutical Company %d "
                "are emptied. Resuming production now.\n", companies_t[index_arg]->index + 1);
    }      
}

void *zone_worker(void *arg) {
    int index_arg = ((Zone*)arg)->index;
    while(true) {
        if (zones_t[index_arg]->capacity == 0) {
            /* Mark that this zones needs vaccine */ 
            zones_t[index_arg]->phase = EMPTY;
            cprintf(YELLOW, "Vaccination Zones %d has run out of vaccines\n",
                    zones_t[index_arg]->index + 1);

            while(zones_t[index_arg]->phase == EMPTY) {
                /* Wait for vaccine to be there */
            }
        }

        /* The phase is set to READY which no other resource tries to
         * acquire. So no need to acquire lock as no matter what it wont
         * be modified */

        /* Mark that Vacination phase has started and allocate queue */ 
        zones_t[index_arg]->students = queue_init(30);
        int slot_max = (zones_t[index_arg]->capacity > 8) ? 8 : zones_t[index_arg]->capacity;
        zones_t[index_arg]->slots = get_random(1, slot_max);

        /* Remove alloacted slots from capacity */
        zones_t[index_arg]->capacity -= zones_t[index_arg]->slots;

        cprintf(YELLOW, "Vaccination Zones %d is ready to vaccinate with "
                "%d slots\n", zones_t[index_arg]->index + 1, zones_t[index_arg]->slots);
        zones_t[index_arg]->phase = VACCINATING;
        cprintf(YELLOW, "Vaccination Zone %d entering Vaccination Phase\n", 
                zones_t[index_arg]->index + 1);

        while((zones_t[index_arg]->slots != 0 && REMAINING_STUDENTS != 0) || (queue_empty(zones_t[index_arg]->students))) {
            /* Wait for slots to be filled */
        }

        pthread_mutex_lock(&zones_t[index_arg]->lock);
        zones_t[index_arg]->phase = WORKING;

        /* Add remaining slots */
        zones_t[index_arg]->phase += zones_t[index_arg]->slots;

        /* Do vaccination on students */
        while(!queue_empty(zones_t[index_arg]->students)) {
            int student_index = *(int*)queue_front(zones_t[index_arg]->students);
            queue_pop(zones_t[index_arg]->students); 

            /* Vaccinate student */
            if (flip_coin(zones_t[index_arg]->prob)) {
                /* Vaccinated */
                students_t[student_index]->phase = 4;
                cprintf(RED, "Student %d has tested positive for antibodies\n",
                        students_t[student_index]->index + 1);
                cprintf(RED, "Student %d on Vaccination Zone %d has "
                        "been vaccinated which has success probability %f\n",
                        students_t[student_index]->index + 1, zones_t[index_arg]->index + 1, zones_t[index_arg]->prob);
            } else {
                /* Not Vaccinated */
                cprintf(RED, "Student %d has tested negative for antibodies\n",
                        students_t[student_index]->index + 1);
                students_t[student_index]->phase++;
            }
        }

        /* Mark queue NULL */
        zones_t[index_arg]->students = NULL;
        pthread_mutex_unlock(&zones_t[index_arg]->lock);
    }
}

void *student_worker(void *arg) {
    int index_arg = ((Student*)arg)->index;

    /* Sleep for a while */
    int wait_time = get_random(STUDENT_WAIT_MIN, STUDENT_WAIT_MAX);
    sleep(wait_time);

    /* Try three times */ 
    for (int try = 0; try < 3; try++) {
        int current_phase = students_t[index_arg]->phase;

        pthread_mutex_lock(&student_lock);
        REMAINING_STUDENTS++;
        pthread_mutex_unlock(&student_lock);

        cprintf(BLUE,
                "Student %d has arrived for his %d round of Vaccinatiion\n",
                students_t[index_arg]->index + 1, students_t[index_arg]->phase + 1);

        cprintf(BLUE,
                "Student %d is waiting to be allocated "
                "a slot on a Vaccination Zone\n",
                students_t[index_arg]->index + 1);

        int flag = false;
        while(true) {
            for (int i = 0; i < zones; i++) {
                /* Try to acquire zone lock */
                if (pthread_mutex_trylock(&zones_t[i]->lock)) {
                    continue;
                }

                if (zones_t[i]->phase == VACCINATING && zones_t[i]->slots != 0) {
                    cprintf(BLUE, "Student %d assigned a slot on the "
                            "Vaccination Zone %d and waiting to be vaccinated\n",
                            students_t[index_arg]->index + 1, i + 1);
                    queue_push(zones_t[i]->students, &index_arg);
                    zones_t[i]->slots--;
                    /* Release lock */
                    pthread_mutex_unlock(&zones_t[i]->lock);

                    pthread_mutex_lock(&student_lock);
                    REMAINING_STUDENTS--;
                    pthread_mutex_unlock(&student_lock);


                    flag = true;
                    break;
                } else {
                    /* Release lock */
                    pthread_mutex_unlock(&zones_t[i]->lock);
                }
            } 

            if (flag) {
                break;
            }
        }

        while(students_t[index_arg]->phase == current_phase) {
            /* Wait for vaccination */
        }

        /* Check if phase is done and return if done */
        if (students_t[index_arg]->phase == 4) {
            return NULL;
        }
    }    

    /* Send back home */
    cprintf(RED, "Student %d has been sent home :(\n", index_arg + 1);
    return NULL;
}
