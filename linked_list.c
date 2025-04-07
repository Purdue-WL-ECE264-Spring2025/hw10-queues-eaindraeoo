#include "linked_list.h"

#include <stdlib.h>

struct list_node *new_node(size_t value) { 
  struct list_node *no = malloc(sizeof(struct list_node));
  if (!no) {
    // Memory allocation failed
    return NULL;
  }
  no->value = value;
  no->next = NULL;
  return no;
}

// Insert a new node at the head of the linked list
void insert_at_head(struct linked_list *list, size_t value) {
  struct list_node *no = new_node(value);
  if (!no) {
    // Handle memory allocation failure if needed
    return;
  }
  no->next = list->head; // Point new node to the current head
  list->head = no; // Update the head to the new node
}

void insert_at_tail(struct linked_list *list, size_t value) {
  struct list_node *no = new_node(value);
  if (!no) {
    // Handle memory allocation failure if needed
    return;
  }
  
  if (list->head == NULL) {
    // If the list is empty, set both head and tail to the new node
    list->head = no;
    return;
  }

  struct list_node *cur = list->head;
  while (cur->next != NULL) {
    cur = cur->next; // Traverse to the end of the list
  }
  cur->next = no; // Link the new node at the end
}

// Remove and return value from head of list
size_t remove_from_head(struct linked_list *list) { 
  if (list->head == NULL) {
    // List is empty, nothing to remove
    return 0; // or some error value, depending on your design
  }
  struct list_node *temp = list->head; // Store the current head
  size_t value = temp->value; // Get the value to return
  
  list->head = temp->next; // Move head to the next node
  free(temp); // Free the old head node
  return value;
}

size_t remove_from_tail(struct linked_list *list) { 
  if (list->head == NULL) {
    // List is empty, nothing to remove
    return 0; // or some error value, depending on your design
  }
  struct list_node *cur = list->head;
  struct list_node *prev = NULL;
  
  while (cur->next != NULL) {
    prev = cur; // Keep track of the previous node
    cur = cur->next; // Move to the next node
  }

  size_t value = cur->value; // Get the value to return

  if (prev == NULL)
  {
    list->head = NULL; // The list had only one node
  } else {
    // If there are more than one node, unlink the tail
    prev->next = NULL; // Set the previous node's next to NULL
  }

  free(cur); // Free the old tail node
  return value; // Return the value of the removed node
}

void free_list(struct linked_list list) {
  struct list_node *cur = list.head;
  while (cur != NULL) {
    struct list_node *next = cur->next; // Store the next node
    free(cur); // Free the current node
    cur = next; // Move to the next node
  }
}

// Utility function to help you debugging, do not modify
void dump_list(FILE *fp, struct linked_list list) {
  fprintf(fp, "[ ");
  for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
    fprintf(fp, "%zu ", cur->value);
  }
  fprintf(fp, "]\n");
}
