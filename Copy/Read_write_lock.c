#include<stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct list_node_s {
    int data;
    struct list_node_s* next;
};

int member_op_per_thread;
int insert_op_per_thread;
int delete_op_per_thread;

int Member(int value, struct list_node_s* head_p);
int Delete(int value, struct list_node_s** head_pp);
int Insert(int value, struct list_node_s** head_pp);
struct list_node_s* create(int data,struct list_node_s* curr_p);
void* start_operations(void* thread);
int is_valuein_array(int val, int *arr, int size());
struct list_node_s* first_node;
pthread_mutex_t lock;

int main(int args, char* argv[]){

    int number_of_nodes =  strtol(argv[1], NULL ,10);
    int operations =  strtol(argv[2], NULL ,10);
    double member_fraction = atof(argv[3]);
    double insert_fraction = atof(argv[4]);
    double delete_fraction = atof(argv[5]);

    int *tenarray = (int *) malloc(sizeof(int) * number_of_nodes);
    int i;
    int j;
    int num;
    int swap;

    srand(time(NULL));

    for (i = 0; i < number_of_nodes; i++)
    {
        num = getRandom();
        while (is_valuein_array(num, tenarray, number_of_nodes)==0){
            num = getRandom();
        }
        tenarray[i] = num;
    }

    for (i=0; i<number_of_nodes - 1; i++)
    {
        for (j=0; j<number_of_nodes - 1; j++)
        {
            if(tenarray[j] > tenarray[j+1])
            {
                swap = tenarray[j];
                tenarray[j] = tenarray[j+1];
                tenarray[j+1] = swap;

            }
        }
    }

    first_node = malloc(sizeof(struct list_node_s));;
    int header_data = tenarray[0];
    first_node->data = header_data;

    struct list_node_s* current_node = first_node;

    for (i = 1; i < number_of_nodes; i++)
    {
        current_node = create(tenarray[i], current_node);
    }



    int thread;
    pthread_t* thread_handles;

    int thread_count = strtol(argv[6], NULL ,10);

    int op_per_thread = operations/thread_count;
    member_op_per_thread = (int) (op_per_thread * member_fraction);
    insert_op_per_thread = (int) (op_per_thread * insert_fraction);
    delete_op_per_thread = (int) (op_per_thread * delete_fraction);

    thread_handles = (pthread_t*) malloc(thread_count * sizeof(pthread_t));

    clock_t start = clock();
    for (thread =0; thread < thread_count; thread++)
        pthread_create(&thread_handles[thread], NULL, start_operations, (void*) thread);

    for (thread =0; thread < thread_count; thread++)
        pthread_join(thread_handles[thread], NULL);

    clock_t end = clock();

    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time = %fs\n", time_spent);


}

int is_valuein_array(int val, int *arr, int size()){
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return 0;
    }
    return 1;
}


int getRandom() {
    return  rand() % 65535;
}
void* start_operations(void* thread){

    int random_number;
    int member_op = member_op_per_thread;
    int insert_op = insert_op_per_thread;
    int delete_op = delete_op_per_thread;
    while(1) {

        if (member_op != 0) {
            random_number = getRandom();
            pthread_rwlock_rdlock(&lock);
            Member(random_number, first_node);
            member_op--;
            pthread_rwlock_unlock(&lock);
        }
        if (insert_op != 0) {
            random_number = getRandom();
            pthread_rwlock_wrlock(&lock);
            Insert(random_number, &first_node);
            insert_op--;
            pthread_rwlock_unlock(&lock);
        }
        if (delete_op != 0) {
            random_number = getRandom();
            pthread_rwlock_wrlock(&lock);
            Delete(random_number, &first_node);
            delete_op--;
            pthread_rwlock_unlock(&lock);
        }

        if (delete_op == 0 && insert_op == 0 && member_op == 0) {
            break;
        }

    }

    return NULL;
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



int Insert(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value) {
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL) {
            *head_pp = temp_p;
        }else {
            pred_p->next = temp_p;
        }
        return 1;
    } else {
        return 0;
    }

}

int Delete(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value) {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value) {
        if (pred_p == NULL) {
            *head_pp = curr_p->next;
        } else {
            pred_p->next = curr_p->next;
        }
        free(curr_p);
        return 1;
    } else {
        return 0;
    }
}

struct list_node_s* create(int data,struct list_node_s* curr_p)
{
    struct list_node_s* new_node = malloc(sizeof(struct list_node_s));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    curr_p->next = new_node;

    return new_node;
}
