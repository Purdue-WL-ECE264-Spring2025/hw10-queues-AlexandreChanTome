#include "linked_list.h"
#include <stdlib.h>

// To create a new node with the given value
struct list_node *new_node(size_t value) {
    struct list_node *node = malloc(sizeof(struct list_node));
    if (node != NULL) {
        node->value = value;     // To assign the given value to the new node
        node->next = NULL;       // To initialize the next pointer to NULL
    }
    return node; // To return the newly created node
}

// To insert a new node at the head of the list
void insert_at_head(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node == NULL) {
        return;  // To exit if memory allocation fails
    }

    if (list->head == NULL) {
        // To handle the case when the list is empty
        list->head = node;
    } else {
        // To insert the node at the beginning of the list
        node->next = list->head;
        list->head = node;
    }
}

// To insert a new node at the tail of the list
void insert_at_tail(struct linked_list *list, size_t value) {
    struct list_node *node = new_node(value);
    if (node == NULL) {
        return;  // To exit if memory allocation fails
    }

    if (list->head == NULL) {
        // To handle the case when the list is empty
        list->head = node;
    } else {
        // To traverse to the end of the list
        struct list_node *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        // To append the new node at the end
        current->next = node;
    }
}

// To remove and return the value from the head of the list
size_t remove_from_head(struct linked_list *list) {
    if (list->head == NULL) {
        // To handle the case when the list is empty
        return 0;
    }

    struct list_node *temp = list->head;
    size_t value = temp->value;

    list->head = list->head->next; // To update the head to the next node
    free(temp);                    // To free memory of the removed node

    return value; // To return the removed value
}

// To remove and return the value from the tail of the list
size_t remove_from_tail(struct linked_list *list) {
    if (list->head == NULL) {
        // To handle the case when the list is empty
        return 0;
    }

    if (list->head->next == NULL) {
        // To handle the case when the list has only one node
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }

    // To traverse to the second-to-last node
    struct list_node *current = list->head;
    while (current->next->next != NULL) {
        current = current->next;
    }

    // To retrieve and remove the last node
    size_t value = current->next->value;
    free(current->next);
    current->next = NULL;

    return value; // To return the removed value
}

// To free all nodes in the linked list
void free_list(struct linked_list list) {
    struct list_node *current = list.head;
    struct list_node *next;

    while (current != NULL) {
        next = current->next; // To keep track of the next node
        free(current);        // To free the current node
        current = next;       // To move to the next node
    }
}

// Utility function to help you debugging, do not modify
// To print the values in the list to the given file pointer
void dump_list(FILE *fp, struct linked_list list) {
    fprintf(fp, "[ ");
    for (struct list_node *cur = list.head; cur != NULL; cur = cur->next) {
        fprintf(fp, "%zu ", cur->value);
    }
    fprintf(fp, "]\n");
}
