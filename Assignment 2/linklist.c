#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

struct buffer {
   int data;
   int key;
   struct buffer *next;
};

struct buffer *head = NULL;
struct buffer *current = NULL;

//display the list
void Print_Buffer() {
   struct buffer *ptr = head;
	
   //start from the beginning
   while(ptr != NULL) {
      printf("(%d)\n", ptr->data);
      ptr = ptr->next;
   }
	
}

//insert link at the first location
void Insert_First(int data) {
   //create a link
   struct buffer *link = (struct buffer*) malloc(sizeof(struct buffer));
	
   link->data = data;
	
   //point it to old first buffer
   link->next = head;
	
   //point first to new first buffer
   head = link;
}

//delete first item
struct buffer* Deleter_First() {

   //save reference to first link
   struct buffer *tempLink = head;
	
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
   struct buffer *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//find a link with given key
struct buffer* find(int key) {

   //start from the first link
   struct buffer* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->key != key) {
	
      //if it is last buffer
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
   Insert_First(10);
   Insert_First(20);
   Insert_First(30);
   Insert_First(1);
   Insert_First(40);
   Insert_First(56); 

   printf("Original List:\n"); 
	
   //print list
   Print_Buffer();

   Deleter_First();

    printf("\n");
   Print_Buffer();

}