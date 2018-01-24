#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t lock;
struct list_node_s {
    int data;
    struct list_node_s* next;
};

struct do_op_arg {

    int member_op_per_thread ;
    int insert_op_per_thread ;
    int delete_op_per_thread ;
    struct list_node_s* first_node ;
    int thread_id;

};

int Insert(int value, struct list_node_s** head){
    struct list_node_s* curr_p = *head;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = (struct list_node_s*)malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL) {
            *head = temp_p;
        }else {
            pred_p->next = temp_p;
        }
        return 1;
    } else {
        return 0;
    }

}

int Delete(int value, struct list_node_s** head) {
    struct list_node_s* curr_p = *head;
    struct list_node_s* pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head = curr_p->next;
        } else {
            pred_p->next = curr_p->next;
        }
        free(curr_p);
        return 1;
    } else {
        return 0;
    }
}

int Member(int value, struct list_node_s* head_p) {
    struct list_node_s* curr_p = head_p;

    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;
    if (curr_p == NULL || curr_p->data < value) {
        return 0;
    } else {
        return 1;
    }

}

 void *doOperations(void* arguments){

        struct do_op_arg* args = (struct do_op_arg *) arguments;

        int local_mem_op =  args->member_op_per_thread;
        int local_ins_op = args->insert_op_per_thread;
        int local_del_op = args->delete_op_per_thread;
        struct list_node_s* first_node = args->first_node;

        // printf("Thread Id : %d\n", args->thread_id);
        // printf("member_op_per_thread : %d\n", local_mem_op);

        while(1){
            int random_number ;
            if (local_mem_op != 0) {
                pthread_rwlock_rdlock(&lock);
                random_number = rand();
                // printf("%d",Member(random_number, first_node));
                Member(random_number,first_node);
                local_mem_op--;
                pthread_rwlock_unlock(&lock);
            }
            if (local_ins_op != 0) {
                random_number = rand();
                pthread_rwlock_wrlock(&lock);
                // printf("%d",Insert(random_number, &first_node));
                Insert(random_number,&first_node);
                local_ins_op--;
                pthread_rwlock_unlock(&lock);
            }
            if (local_del_op != 0) {
                random_number = rand();
                pthread_rwlock_wrlock(&lock);
                Delete(random_number, &first_node);
                local_del_op--;
                pthread_rwlock_unlock(&lock);
            }

            if (local_mem_op == 0 && local_ins_op == 0 && local_del_op == 0) {
                break;
            }
        }


 }


 void initializeList(struct list_node_s** first_node, int limit){

    int local_limit = limit;
    int random_number = rand();

    while(1){

        if( Insert(random_number,first_node)==1){
            local_limit--;
        }
        if(local_limit<=0){
            break;
        }

    }

 }

 long getCurrentTime() {
    long milliseconds; 
    time_t seconds; 
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);
    seconds = spec.tv_sec;
    milliseconds = seconds* 1000 + spec.tv_nsec / 1.0e6 ;  // Convert nanoseconds to milliseconds
        
    return milliseconds;
}

int main(int argc, char const *argv[])
{

    struct list_node_s* first_node;
    first_node = malloc(sizeof(struct list_node_s));

    // for(int i=0;i<1000;i++){
    //  int random_number = rand() % 65535;
    //  int result = Insert(random_number, &first_node);
    //  printf("%d",result);
    // }

    // printf("%d",Member(24,first_node));

    int nodes =  strtol(argv[1], NULL ,10);
    int operations =  strtol(argv[2], NULL ,10);
    double member_fraction = strtod(argv[3], NULL);
    double insert_fraction = strtod(argv[4], NULL);
    double delete_fraction = strtod(argv[5],NULL);
    int threadcount = strtod(argv[6],NULL);

    if(member_fraction+insert_fraction+delete_fraction != 1.0){
        return 0;
    }
    // printf("Enter the number of threads : \n");
    // scanf("%d",&threadcount);
    // printf("%d\n", threadcount);
    // initializeList(&first_node, nodes);

    int mem_op = (int)(member_fraction*operations);
    int ins_op = (int)(insert_fraction*operations);
    int del_op = (int)(delete_fraction*operations);
    // printf("mem_op : %d, ins_op : %d, del_op :%d \n", mem_op,ins_op,del_op);

    // time_t time = time();
    // doOperations(mem_op,ins_op,del_op,first_node);
    // time = time()- time;
    // clock_t end_time = clock();
    // double time_difference = ((double)time);
    // printf("Execution Time : %f\n",time_difference);

    // time_t start_time,end_time;
    // time(&start_time);
    long start_time,end_time,elapsed_time;

    pthread_t* thread_handles;
    thread_handles = (pthread_t*) malloc(threadcount * sizeof(pthread_t));


    struct do_op_arg *args =malloc(sizeof(struct do_op_arg));;

    args->member_op_per_thread = (int) (mem_op/threadcount);
    args->insert_op_per_thread = (int) (ins_op/threadcount);
    args->delete_op_per_thread = (int) (del_op /threadcount);
    args->first_node = first_node; 


    start_time = getCurrentTime();
    for(int thread = 0; thread< threadcount;thread++){
        args->thread_id = thread;
    
        pthread_create(&thread_handles[thread], NULL, doOperations, (void *)args);

    }

    for (int thread =0; thread < threadcount; thread++){
        pthread_join(thread_handles[thread], NULL);
    }

    // doOperations((void *)args);
    // time(&end_time);
    // float diff_t = difftime(end_time,start_time);

    end_time = getCurrentTime();
    elapsed_time = end_time -start_time;

    printf("%li\n", elapsed_time);
    return 0;
}





