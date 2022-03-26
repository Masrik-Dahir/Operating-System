#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct node {
   int data;
   int key;
   struct node *next;
};

struct node *head = NULL;
struct node *current = NULL;

//display the list
void printList() {
   struct node *ptr = head;
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%d)\n", ptr->data);
      ptr = ptr->next;
   }
	
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

//delete first item
struct node* deleteFirst() {

   //save reference to first link
   struct node *tempLink = head;
	
   //mark next to first link as first 
   head = head->next;
	
   //return the deleted link
   return tempLink;
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

//find a link with given key
struct node* find(int key) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if data found, return the current Link
   return current;
}



void main() {
   insertFirst(10);
   insertFirst(20);
   insertFirst(30);
   insertFirst(1);
   insertFirst(40);
   insertFirst(56); 

   printf("Original List:\n"); 
	
   //print list
   printList();

   deleteFirst();

    printf("\n");
   printList();

}