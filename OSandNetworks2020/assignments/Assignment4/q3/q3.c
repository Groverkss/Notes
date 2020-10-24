#include "libs.h"

int main(void) {
    /* Take Input */
    scanf("%d %d %d %d %d %d %d", &n_people, &acoustic_stages, &electric_stages,
            &coord, &time_1, &time_2, &anger_time);
    total_stages = acoustic_stages + electric_stages;

    /* Allocate Memory */
    performers = Malloc(sizeof(Performer) * n_people);
    stages = Malloc(sizeof(Stage) * total_stages);
    performer_t = Malloc(sizeof(pthread_t) * n_people);
    lock_stage = Malloc((sizeof(pthread_mutex_t) * total_stages));
    cond_stage = Malloc(sizeof(pthread_cond_t) * total_stages);
    lock_flag = Malloc((sizeof(pthread_mutex_t) * n_people));

    /* Initialise semaphores */
    sem_init(&acoustic_musician, 0, acoustic_stages);
    sem_init(&electric_musician, 0, electric_stages);
    sem_init(&total_singer, 0, total_stages);
    sem_init(&coordinators, 0, coord);

    /* Initialise stages */
    for (int i = 0; i < acoustic_stages; i++) {
        stages[i].sing = -1;
        stages[i].type = 0;
        stages[i].state = 0;
    }

    for (int i = 0; i < electric_stages; i++) {
        stages[i + acoustic_stages].sing = -1;
        stages[i + acoustic_stages].state = 0;
        stages[i + acoustic_stages].type = 1;
    }

    /* Take Musician input */
    for (int i = 0; i < n_people; i++) {
        scanf("%s %c %d", performers[i].name, &performers[i].instrument,
                &performers[i].arrival_time);
        type(i, performers[i].instrument);

        /* Initialize performers */
        performers[i].index = i;
        performers[i].flag = 0;
        performers[i].state = 0;
    }

    pthread_t thread_t[n_people];

    /* Create threads */
    for (int i = 0; i < n_people; i++) {
        if (performers[i].stage != 3) {
            /* Not musician */
            Pthread_create(&thread_t[i], NULL, musician_worker,
                    &performers[i].index);
        } else {
            /* Musician */
            Pthread_create(&thread_t[i], NULL, singer_worker,
                    &performers[i].index);
        }
    }

    /* Wait for all performances to end */
    for (int i = 0; i < n_people; i++) {
        Pthread_join(thread_t[i], NULL);
    }

    cprintf(GREEN, "*** SIMULATION OVER ***\n");
}
