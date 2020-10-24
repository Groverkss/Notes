#include <stdio.h>
#include <stdbool.h>
#include "libs.h"

float *probabilities = NULL;
int companies, zones, students;

Student **students_t; 
Company **companies_t;
Zone **zones_t;

int main(void) {
    /* Get input */
    scanf("%d %d %d", &companies, &zones, &students);

    probabilities = Malloc(sizeof(float) * companies);
    for (int i = 0; i < companies; i++) {
        scanf("%f", &probabilities[i]);
    }

    if (companies == 0 || zones == 0) {
        cprintf(RED, "The simulation will never end!\n");
        return 0;
    }

    companies_t = Malloc(sizeof(Company*) * companies);
    zones_t = Malloc(sizeof(Zone*) * zones);
    students_t = Malloc(sizeof(Student*) * students);

    /* Create Company threads */
    for (int i = 0; i < companies; i++) {
        Company *thread_args = Malloc(sizeof(Company)); 

        thread_args->index = i;
        thread_args->prob = probabilities[i];
        thread_args->batches = 0;
        thread_args->batch_capacity = 0;

        companies_t[i] = thread_args;
        pthread_t company_t;
        Pthread_create(&company_t, NULL, company_worker, thread_args);
    }

    /* Create Zone threads */
    for (int i = 0; i < zones; i++) {
        Zone *thread_args = Malloc(sizeof(Zone));

        thread_args->index = i;
        thread_args->prob = 0;
        thread_args->capacity = 0;
        thread_args->phase = EMPTY;
        thread_args->slots = 0;
        pthread_mutex_init(&thread_args->lock, NULL); 
        thread_args->students = NULL;

        zones_t[i] = thread_args;
        pthread_t zone_t;
        Pthread_create(&zone_t, NULL, zone_worker, thread_args);
    }

    pthread_t student_threads[students];

    /* Create Student threads */
    for (int i = 0; i < students; i++) {
        Student *thread_args = Malloc(sizeof(Student));

        thread_args->index = i;
        thread_args->phase = 0;

        students_t[i] = thread_args;
        pthread_t student_t;
        Pthread_create(&student_t, NULL, student_worker, thread_args);
        student_threads[i] = student_t;
    }

    /* Wait for all students to finish */
    for (int i = 0; i < students; i++) {
        Pthread_join(student_threads[i], NULL);
    }

    cprintf(GREEN, "Simulation Over\n");
}
