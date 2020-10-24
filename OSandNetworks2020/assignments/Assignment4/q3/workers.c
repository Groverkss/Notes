#include "libs.h"

const int EXTRA_WAIT_TIME = 2;

void perform_musician(int index, int type) {
    /* Get a random wait time */
    int wait_time = get_random(time_1, time_2); 

    for (int i = 0; i < total_stages; i++) {
        /* We know one of the stages we want is free, so try to get that */
        if (stages[i].state == 0 && stages[i].type == type) {
            if (pthread_mutex_trylock(&lock_stage[i]))
                continue;

            /* Performing now */
            stages[i].state = 1;

            pthread_mutex_unlock(&lock_stage[i]);

            char *stage_type = stages[i].type ? "electric" : "acoustic";

            cprintf(YELLOW, "%s is performing %c at %s stage %d for %d seconds\n",
                performers[index].name, performers[index].instrument, stage_type,
                stages[i].index, wait_time);
            /* Sleep for the waiting time */
            sleep(wait_time);

            /* If someone joined, wait for the additional time */
            if (stages[i].state == 3) {
                cprintf(YELLOW, "%s joined %s, peformance extended by 2 seconds\n",
                    performers[stages[i].sing].name, performers[index].name);
                sleep(EXTRA_WAIT_TIME);

                pthread_mutex_lock(&lock_stage[i]);

                stages[i].state = 0;
                stages[i].sing = -1;

                /* Signal the singer that performance is done */
                pthread_cond_signal(&cond_stage[i]);
                pthread_mutex_unlock(&lock_stage[i]);

                cprintf(RED, "%s's performance at %s stage %d has ended\n",
                    performers[index].name, stage_type, stages[i].index);
                performers[index].state = 1;

                break;
            }

            pthread_mutex_lock(&lock_stage[i]);
            stages[i].state = 0;
            pthread_mutex_unlock(&lock_stage[i]);

            performers[index].state = 1;

            cprintf(RED, "%s's performance at %s stage %d has ended\n",
                performers[index].name, stage_type, stages[i].index);
            break;
        }
    }
}

void *musician_newthread(void *arg) {
    int index = *((int *)arg);

    /* Get current time and mark absolute time till which to wait */
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        return NULL;
    ts.tv_sec += anger_time;

    int sem_return;
    
    while ((sem_return = 
                sem_timedwait(&electric_musician, &ts)) == -1 && errno == EINTR)
        continue;

    if (performers[index].flag) {
        /* Got electric stage, do nothing */

        if (sem_return == -1)
            /* Waited too much time here */
            return NULL;

        sem_post(&electric_musician);
        return NULL;
    }

    /* Perform at acoustic stage */
    pthread_mutex_lock(&lock_flag[index]);
    performers[index].flag = 1;
    pthread_mutex_unlock(&lock_flag[index]);

    perform_musician(index, 1);

    return NULL;
}

void *musician_worker(void *arg) {
    int index = *((int *)arg);

    sleep(performers[index].arrival_time);
    cprintf(GREEN, "Musician %s has arrived with instrument %c\n",
            performers[index].name, performers[index].instrument);

    struct timespec ts;

    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        return NULL;

    ts.tv_sec += anger_time;
    int s;

    if (performers[index].stage == 0) {
        while ((s = sem_timedwait(&acoustic_musician, &ts)) == -1 &&
                errno == EINTR)
            continue;

        if (s == -1) {
                Error(index);
        } else {
            perform_musician(index, 0);
            sem_post(&acoustic_musician);
        }
    }

    else if (performers[index].stage == 1) {
        while ((s = sem_timedwait(&electric_musician, &ts)) == -1 &&
                errno == EINTR)
            continue;

        if (errno == ETIMEDOUT) {
                Error(index);
        } else {
            perform_musician(index, 1);
            sem_post(&electric_musician);
        }
    }

    else if (performers[index].stage == 2) {
        pthread_create(&performer_t[index], NULL, musician_newthread,
                &index);

        while ((s = sem_timedwait(&acoustic_musician, &ts)) == -1 &&
                errno == EINTR)
            continue;

        if (performers[index].flag == 0) {
            if (s != -1) {
                pthread_mutex_lock(&lock_flag[index]);
                performers[index].flag = 1;
                pthread_mutex_unlock(&lock_flag[index]);

                perform_musician(index, 0);

                sem_post(&acoustic_musician);
            } else {
                if (errno == ETIMEDOUT)
                    Error(index);
            }
        } else {
            if (s != -1)
                sem_post(&acoustic_musician);
        }

        pthread_join(performer_t[index], NULL);
    }

    if (performers[index].state == 1) {
        Tshirt(index);
    }
    return NULL;
}

void *singer_worker(void *arg) {
    int index = *((int *)arg);

    sleep(performers[index].arrival_time);
    cprintf(RED, "Singer %s has arrived\n",
            performers[index].name);

    struct timespec ts;

    if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
        return NULL;

    ts.tv_sec += anger_time;
    int s;

restart_to_avoid_deadlock:
    while ((s = sem_timedwait(&total_singer, &ts)) == -1 && errno == EINTR)
        continue;

    if (s == -1)
        Error(index);
    else {
        int wait_time = get_random(time_1, time_2);

        int acquire = 0;
        for (int i = 0; i < total_stages; i++) {
            if (stages[i].state == 0) {
                if (pthread_mutex_trylock(&lock_stage[i]))
                    continue;
                acquire = 1;
                stages[i].state = 2;
                pthread_mutex_unlock(&lock_stage[i]);

                char *stage_type = stages[i].type ? "electric" : "acoustic";

                if (stages[i].type == 0)
                    sem_wait(&acoustic_musician);
                else
                    sem_wait(&electric_musician);

                cprintf(RED, "Singer %s is performing at %s stage %d for %d seconds\n",
                        performers[index].name, stage_type, stages[i].index,
                        wait_time);

                sleep(wait_time);

                if (stages[i].type == 0)
                    sem_post(&acoustic_musician);
                else
                    sem_post(&electric_musician);

                if (pthread_mutex_lock(&lock_stage[i]))
                    stages[i].state = 0;
                pthread_mutex_unlock(&lock_stage[i]);

                cprintf(RED, "%s's performance at %s stage %d has ended\n",
                        performers[index].name, stage_type, stages[i].index);
                performers[index].state = 1;

                break;
            } else if (stages[i].state == 1) {
                if (pthread_mutex_trylock(&lock_stage[i]))
                    continue;

                stages[i].state = 3;
                stages[i].sing = index;

                char *stage_type = stages[i].type ? "electric" : "acoustic";

                pthread_cond_wait(&cond_stage[i], &lock_stage[i]);
                pthread_mutex_unlock(&lock_stage[i]);

                cprintf(RED, "%s's performance at %s stage %d has ended\n",
                        performers[index].name, stage_type, stages[i].index);
                performers[index].state = 1;
                break;
            }
        }

        sem_post(&total_singer);
        if (!acquire) {
            goto restart_to_avoid_deadlock;
        }
    }

    if (performers[index].state == 1) {
        Tshirt(index);
    }
    return NULL;
}
