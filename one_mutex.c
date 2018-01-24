#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t one_mutex_lock;
struct list_node_s {
    int data;
    struct list_node_s* next;
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

 void doOperations(int mem_op, int ins_op, int del_op, struct list_node_s* first_node){

        int local_mem_op = mem_op;
        int local_ins_op = ins_op;
        int local_del_op = del_op;
        while(1){
            int random_number ;
            if (local_mem_op != 0) {
                pthread_mutex_lock(&one_mutex_lock);
                random_number = rand();
                printf("%d",Member(random_number, first_node));
                local_mem_op--;
                pthread_mutex_unlock(&one_mutex_lock);
            }
            if (local_ins_op != 0) {
                random_number = rand();
                pthread_mutex_lock(&one_mutex_lock);
                printf("%d",Insert(random_number, &first_node));
                local_ins_op--;
                pthread_mutex_unlock(&one_mutex_lock);
            }
            if (local_del_op != 0) {
                random_number = rand();
                pthread_mutex_lock(&one_mutex_lock);
                Delete(random_number, &first_node);
                local_del_op--;
                pthread_mutex_unlock(&one_mutex_lock);
            }

            if (local_mem_op == 0 && local_ins_op == 0 && local_del_op == 0) {
                break;
            }
        }

 }

int main(int argc, char const *argv[])
{

	struct list_node_s* first_node;

	// for(int i=0;i<1000;i++){
	// 	int random_number = rand() % 65535;
	// 	int result = Insert(random_number, &first_node);
	// 	printf("%d",result);
	// }

	// printf("%d",Member(24,first_node));

    int nodes =  strtol(argv[1], NULL ,10);
    int operations =  strtol(argv[2], NULL ,10);
    double member_fraction = strtod(argv[3], NULL);
    double insert_fraction = strtod(argv[4], NULL);
    double delete_fraction = strtod(argv[5],NULL);

    if(member_fraction+insert_fraction+delete_fraction != 1.0){
        return 0;
    }

    int mem_op = (int)(member_fraction*operations);
    int ins_op = (int)(insert_fraction*operations);
    int del_op = (int)(delete_fraction*operations);


    // time_t time = time();
    // doOperations(mem_op,ins_op,del_op,first_node);
    // time = time()- time;
    // clock_t end_time = clock();
    // double time_difference = ((double)time);
    // printf("Execution Time : %f\n",time_difference);

    // time_t start_time,end_time;
    // time(&start_time);
    doOperations(mem_op,ins_op,del_op,first_node);
    // time(&end_time);
    // double diff_t = difftime(end_time,start_time);
	return 0;
}





