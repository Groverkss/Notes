#ifndef _STRUCTDEFS_
#define _STRUCTDEFS_

typedef struct {
    int type;
    int index;
    int state;
    int sing;
} Stage;

typedef struct {
    int index;
    int state;  // 0 wait -1 leave 1 tshirt
    char name[50];
    int stage;  // can be of 3 types : (0 for acoustic),(1 for elctric),(2 can
                // use both stages),(3 for singer)
    int arrival_time;
    int flag;
    char instrument;
} Performer;

Performer *performers;
Stage *stages;

pthread_mutex_t *lock_stage, *lock_flag;
pthread_cond_t *cond_stage;

pthread_t *performer_t;

sem_t acoustic_musician, electric_musician, total_singer, coordinators;

int n_people, acoustic_stages, electric_stages, total_stages;
int time_1, time_2, anger_time, coord;

#endif
