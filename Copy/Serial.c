#include<stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct list_node_s {
    int data;
    struct list_node_s* next;
};


struct list_node_s* create(int data,struct list_node_s* curr_p);
int Member(int value, struct list_node_s* head);
int Delete(int value, struct list_node_s** head);
int Insert(int value, struct list_node_s** head_pp);
int is_valuein_array(int val, int *arr, int size());
struct list_node_s* first_node;

void* start_operations(int member_operations, int insert_operations, int delete_operations){

    int random_number;

    while(1) {

        if (member_operations != 0) {
            random_number = rand() % 65535;
            Member(random_number, first_node);
            member_operations--;
        }
        if (insert_operations != 0) {
            random_number = rand() % 65535;
            Insert(random_number, &first_node);
            insert_operations--;
        }
        if (delete_operations != 0) {
            random_number = rand() % 65535;
            Delete(random_number, &first_node);
            delete_operations--;
        }

        if (delete_operations == 0 && insert_operations == 0 && member_operations == 0) {
            break;
        }

    }

    return NULL;
}


int main(int args, char* argv[]){

    int number_of_nodes =  strtol(argv[1], NULL ,10);
    int operations =  strtol(argv[2], NULL ,10);
    double member_fraction = atof(argv[3]);
    double insert_fraction = atof(argv[4]);

    int *tenarray = (int *) malloc(sizeof(int) * number_of_nodes);
    int i;
    int j;
    int num;
    int swap;

    int member_operations = (int)(member_fraction * operations);
    int insert_operations = (int)(insert_fraction * operations);
    int delete_opertions = operations - (member_operations + insert_operations);

    srand(time(NULL));

    for (i = 0; i < number_of_nodes; i++)
    {
        num = rand() % 65535;
        while (is_valuein_array(num, tenarray, number_of_nodes)==0){
            num = rand() % 65535;
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


    clock_t start = clock();

    start_operations(member_operations, insert_fraction, delete_opertions);

    clock_t end = clock();

    double time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Time = %fs\n", time);


}

int is_valuein_array(int val, int *arr, int size()){
    int i;
    for (i=0; i < size; i++) {
        if (arr[i] == val)
            return 0;
    }
    return 1;
}


int Insert(int value, struct list_node_s** head){
    struct list_node_s* curr_p = *head;
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
