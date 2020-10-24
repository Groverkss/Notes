#ifndef _WORKERS_
#define _WORKERS_

extern int companies, zones, students;
extern Student **students_t; 
extern Company **companies_t;
extern Zone **zones_t;

void *company_worker(void *args);

void *zone_worker(void *args);

void *student_worker(void *args);

#define EMPTY 0
#define READY 1
#define VACCINATING 2
#define WORKING 3

#endif
