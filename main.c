#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define MAXTHREAD 10  /* define # readers */


void access_database();     /* prototypes */
void non_access_database();

void* reader(void*);
void* writer(void*);

sem_t q;        /* establish que */
int rc = 0;     /* number of processes reading or wanting to */
int wc = 0;
int write_request = 0;

int main()
{
    pthread_t readers[MAXTHREAD],writerTh;
    int index;
    int ids[MAXTHREAD]; /* readers and initialize mutex, q and db-set them to 1 */
    sem_init (&q,0,1);
    for(index = 0; index < MAXTHREAD; index ++)
    {
        ids[index]=index+1;                             /* create readers and error check */
        if(pthread_create(&readers[index],0,reader,&ids[index])!=0){
            perror("Cannot create reader!");
            _exit(1);
        }
    }
    if(pthread_create(&writerTh,0,writer,0)!=0){
        perror("Cannot create writer");     /* create writers and error check */
        _exit(1);
    }

    pthread_join(writerTh,0);
    sem_destroy (&q);
    return 0;
}

void* reader(void*arg)          /* readers function to read */
{
    int index = *(int*)arg;
    int can_read;
    while(1){
        can_read = 1;

        sem_wait(&q);
        if(wc == 0 && write_request == 0)  rc++;
        else                               can_read = 0;
        sem_post(&q);

        if(can_read) {
            access_database();
            printf("Thread %d reading\n", index);
            sleep(index);

            sem_wait(&q);
            rc--;
            sem_post(&q);
        }

        sched_yield();
    }
    return 0;
}
;
void* writer(void*arg)      /* writer's function to write */
{
    int can_write;
    while(1){
        can_write = 1;
        non_access_database();

        sem_wait (&q);
        if(rc == 0)   wc++;
        else          { can_write = 0; write_request = 1; }
        sem_post(&q);

        if(can_write) {
            access_database();
            printf("Writer is now writing...Number of readers: %d\n",rc);

            sleep(3);

            sem_wait(&q);
            wc--;
            write_request = 0;
            sem_post(&q);
        }

        sched_yield();
    }
    return 0;
}

void access_database()
{

}


void non_access_database()
{

}