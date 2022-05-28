#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct node {
	int data;
	struct node *prev;
	struct node *next;
}Buffer;

Buffer *head = NULL, *tail = NULL;

void Print_Buffer(){
	Buffer *cur = head;
	if (head == NULL) {
		assert(tail == NULL);
		printf("Empty list \n");
		return;
	}
    printf("Current buffer = [");
	while(cur != NULL) {
        printf("%d", cur->data);
        cur = cur->next;
        if (cur != NULL){
            printf(", ");
        }
	}
	printf("]\n");
	return;
}

void Add_Last(int data)
{
	Buffer *newnode; 

	if (newnode = (Buffer *) malloc(sizeof (Buffer))) {
		newnode->data = data;
		newnode->next = NULL;
		newnode->prev = NULL;
	} else {
		printf("unable to allocate memory \n");
		return;
	}
		
	if (tail == NULL) {
		assert (tail == NULL);
		head = tail = newnode;
	} else {
		newnode->prev = tail;
		tail->next = newnode;
		tail = newnode;
	}
	
	return;
}

void Delete_First()
{
	Buffer *temp = head;

	if (head == NULL) {
		assert(tail == NULL);
		printf("Empty list\n");
		return;
	}

	if (head == tail) {
		head = tail = NULL;
	} else {
		head = head->next;
		head->prev = NULL;
	}
	
	free(temp);
	
	return;
}


int main()
{

	Add_Last(5); Print_Buffer();
	Add_Last(6); Print_Buffer();
	Add_Last(7); Print_Buffer();
	Add_Last(8); Print_Buffer();
	Add_Last(9); Print_Buffer();
    Delete_First(); Print_Buffer();

	

	return 0;
}