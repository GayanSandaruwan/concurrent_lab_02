#include<stdio.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[])
{

	struct list_node_s* first_node;

	for(int i=0;i<1000;i++){
		int random_number = rand() % 65535;
		int result = Insert(random_number, &first_node);
		printf("%d",result);
	}

	return 0;
}



