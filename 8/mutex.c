#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include<pthread.h>
#define NUM_EMPLOYEES 2

pthread_mutex_t a_mutex = PTHREAD_ERRORCHECK_MUTEX_INITIALIZER;
struct employee{
    int number;
    int id;
    char first_name[20];
    char last_name[30];
    char department[30];
    int room_number;
};
struct employee employees[]={
    {1,12345678,"three","zhang","accounting",101},
    {2,87654321,"four","li","programes",202}
};
struct employee winner;
void copy_employee(struct employee*from,struct employee*to){
    int rc;
    rc =pthread_mutex_lock(&a_mutex);
    
    to->number = from->number;
    to->id = from->id;
    strcpy(to->first_name,from->first_name);
    strcpy(to->last_name, from->last_name);
    strcpy(to->department, from->department);
    to->room_number = from->room_number;
    
    rc= pthread_mutex_unlock(&a_mutex);
}
void * do_loop(void* data)
{
    int my_num = *((int*)data);
    while (1) {
        copy_employee(&employees[my_num-1], &winner);
    }
}
int main(int argc,char*argv[]){
    int i;
    int thr_id1;
    int thr_id2;
    pthread_t pthread1;
    pthread_t pthread2;
    int num1 =1;
    int num2 =2;
    struct employee eotd;
    struct employee* worker;
    copy_employee(&employees[0], &winner);
    
    thr_id1 = pthread_create(&pthread1, NULL, do_loop, (void*)&num1);
    thr_id2 = pthread_create(&pthread2, NULL, do_loop, (void*)&num2);
    for (i=0; i<10000; i++) {
        copy_employee(&winner, &eotd);
        worker = &employees[eotd.number-1];
        if (eotd.id !=worker->id) {
            printf("mismatching 'id',%d !=%d (loop) '%d')\n",eotd.id,worker->id,i);
        }
        if(strcmp(eotd.first_name, worker->first_name)!=0){
            
            printf("mismatching 'frist_name',%s != %s(loop '%d')\n",eotd.first_name,worker->first_name,i);
        }
        if(strcmp(eotd.last_name, worker->last_name)!=0){
            printf("mismatching 'last_name',%s != %s(loop '%d')\n",eotd.last_name,worker->last_name,i);
        }
        if(strcmp(eotd.department, worker->department)!=0){
            printf("mismatching 'department',%s != %s(loop '%d')\n",eotd.department,worker->department,i);
        }
        if (eotd.room_number != worker->room_number) {
            printf("mismatching 'room_number',%d !=(loop '%d'\n");
            exit(0);
        }
    }
    printf("employees contents was always consistnt \n");
    return 0;
}
