#include<stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <pthread.h>
#include <math.h>
#include <time.h>
#define _POSIX_C_SOURCE 200809L
#define MAX_VALUE 65536

struct list_node_s {
    int data;
    struct list_node_s* next;
};


struct list_node_s* head_pp = NULL;


int noOfnodes;
int noOfOperations;
int noOfMember;
int noOfDelete;
int noOfInsert;

int Insert(int value, struct list_node_s** head_pp);
int Member(int value, struct list_node_s* head_pp);
int Delete(int value, struct list_node_s** head_pp);
int generateRandomValue();
void doFunctions();
long getCurrentTime(void);
void createList(int);


int main(int argc, char const *argv[])
{
    long startTime, endsTime, elapsedTime;
    noOfnodes = atoi(argv[1]);
    noOfOperations = atoi(argv[2]);
    noOfMember = strtod(argv[3], NULL) * noOfOperations;
    noOfInsert = strtod(argv[4], NULL) * noOfOperations;
    noOfDelete = strtod(argv[5], NULL) * noOfOperations;

    createList(noOfnodes);
    startTime = getCurrentTime();
    doFunctions();
    endsTime = getCurrentTime();
    elapsedTime = endsTime - startTime;
	
    printf("%ld", elapsedTime);	
    return 0;
}

void createList(int noOfVariables) {
    srand (time(NULL));
    int Inserted = 0;
    int i;
    for (i = 0; i < noOfVariables; i++) {
        int value = generateRandomValue();
        Inserted = Insert(value,&head_pp);
        if (!Inserted) {
            i--;
        }
    }
}

int generateRandomValue() {
    int value = rand() % MAX_VALUE;
    return value;
}

void doFunctions(){
  
    for (long i = 0; i < noOfOperations; i++) {
        if (i < noOfInsert) {
	    int random_number = generateRandomValue();
	    Insert(random_number, &head_pp);
	} else if (i < noOfInsert +noOfDelete) {
	    int random_number = generateRandomValue();
	    Delete(random_number, &head_pp);
	} else {
	    int random_number = generateRandomValue();
	    Member(random_number, head_pp);
	}
    }

}
long getCurrentTime() {
    long milliseconds; 
    time_t seconds; 
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);
    seconds = spec.tv_sec;
    milliseconds = spec.tv_sec * 1000 + spec.tv_nsec / 1.0e6 ;  // Convert nanoseconds to milliseconds
     	
    return milliseconds;
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
        temp_p = (struct list_node_s*)malloc(sizeof(struct list_node_s));
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

int Member(int value, struct list_node_s* head) {
    struct list_node_s* curr_p = head;

    while (curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;
    if (curr_p == NULL || curr_p->data < value) {
        return 0;
    } else {
        return 1;
    }
}

  
