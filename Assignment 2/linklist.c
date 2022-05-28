#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node {
   int data;
   int key;
   struct node *next;
   struct node *prev;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList() {
   struct node *ptr = head;
	printf("Current buffer = [");
   //start from the beginning
   while(ptr != NULL) {
      printf("%d", ptr->data);
      ptr = ptr->next;
      if (ptr != NULL){
          printf(", ");
      }
   }
   printf("]\n\n");
}

//insert link at the first location
void insertFirst(int data) {
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
	
   link->data = data;
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;
}

//is list empty
bool isEmpty() {
   return head == NULL;
}

int length() {
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//delete a link with given key
struct node* delete() {


}

void reverse(struct node** head_ref) {
   struct node* prev   = NULL;
   struct node* current = *head_ref;
   struct node* next;
	
   while (current != NULL) {
      next  = current->next;
      current->next = prev;   
      prev = current;
      current = next;
   }
	
   *head_ref = prev;
}

void main() {
   insertFirst(10);
   insertFirst(20);
   insertFirst(30);
   insertFirst(1);
   insertFirst(40);
   insertFirst(56); 

   printf("Original List: "); 
	
   //print list
   printList();

   delete(4);
   
   printList();
}