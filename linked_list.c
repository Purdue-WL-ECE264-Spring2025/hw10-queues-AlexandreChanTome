#include "linked_list.h"
#include <stdlib.h>

// Function: new_node
// Purpose: Dynamically allocates memory for a new node in the linked list, initializes it with the provided value, and sets its next pointer to NULL
struct list_node *new_node(size_t value)
{
    // Allocate memory for one node of type struct list_node
    struct list_node *node = malloc(sizeof(struct list_node));

    // Check if memory allocation was successful (i.e., node is not NULL)
    if (node != NULL)
    {
        // Set the value field of the node to the input value
        node->value = value;

        // Since this is a new node, it doesn't point to any other node yet
        node->next = NULL;
    }

    // Return the pointer to the new node (could be NULL if malloc failed)
    return node;
}

// Function: insert_at_head
// Purpose: Inserts a new node with the given value at the **beginning** (head) of the list
void insert_at_head(struct linked_list *list, size_t value)
{
    // Step 1: Create a new node using the value provided
    struct list_node *node = new_node(value);

    // Step 2: If node creation failed (e.g., due to memory allocation issues), exit early
    if (node == NULL)
    {
        return;
    }

    // Step 3: Check if the list is currently empty
    if (list->head == NULL)
    {
        // If the list has no elements, set head to the new node
        list->head = node;
    }
    else
    {
        // Otherwise, insert the new node before the current head
        node->next = list->head;  // new node points to the current head
        list->head = node;  // head is updated to the new node
    }
}

// Function: insert_at_tail
// Purpose: Inserts a new node with the given value at the **end** (tail) of the list
void insert_at_tail(struct linked_list *list, size_t value)
{
    // Step 1: Create a new node with the given value
    struct list_node *node = new_node(value);

    // Step 2: Check if memory allocation failed
    if (node == NULL)
    {
        return;
    }

    // Step 3: If the list is currently empty
    if (list->head == NULL)
    {
        // The new node becomes the head of the list
        list->head = node;
    }
    
    else
    {
        // Otherwise, traverse the list to find the last node (tail)
        struct list_node *current = list->head;

        // Keep moving to the next node until we find the last node
        while (current->next != NULL)
        {
            current = current->next;
        }

        // Once we're at the last node, append the new node
        current->next = node;
    }
}

// Function: remove_from_head
// Purpose: Removes the node at the head of the list and returns its value
size_t remove_from_head(struct linked_list *list)
{
    // Step 1: Check if the list is empty
    if (list->head == NULL)
    {
        // Nothing to remove, return 0 as a default value
        return 0;
    }

    // Step 2: Save the current head node in a temporary pointer
    struct list_node *temp = list->head;

    // Step 3: Retrieve the value stored in the node
    size_t value = temp->value;

    // Step 4: Update the head to the next node in the list
    list->head = list->head->next;

    // Step 5: Free the memory used by the removed node
    free(temp);

    // Step 6: Return the value that was stored in the removed node
    return value;
}

// Function: remove_from_tail
// Purpose: Removes the node at the end (tail) of the list and returns its value
size_t remove_from_tail(struct linked_list *list)
{
    // Step 1: Check if the list is empty
    if (list->head == NULL)
    {
        // Nothing to remove, return 0
        return 0;
    }

    // Step 2: Check if the list has only one node
    if (list->head->next == NULL)
    {
        // Store the value, free the node, and set head to NULL
        size_t value = list->head->value;
        free(list->head);
        list->head = NULL;
        return value;
    }

    // Step 3: Traverse the list to find the second-to-last node
    struct list_node *current = list->head;

    // Loop until current->next->next is NULL (meaning current->next is the last node)
    while (current->next->next != NULL)
    {
        current = current->next;
    }

    // Step 4: Save the value of the last node
    size_t value = current->next->value;

    // Step 5: Free the last node and update current->next to NULL
    free(current->next);
    current->next = NULL;

    // Step 6: Return the value of the removed node
    return value;
}

// Function: free_list
// Purpose: Frees the memory of all nodes in the list to prevent memory leaks
void free_list(struct linked_list list)
{
    // Start at the head of the list
    struct list_node *current = list.head;
    struct list_node *next;

    // Traverse through the list
    while (current != NULL)
    {
        // Save a reference to the next node
        next = current->next;

        // Free the current node
        free(current);

        // Move to the next node
        current = next;
    }

    // All nodes are now freed
}

// Function: dump_list
// Purpose: Prints out the values of the list in a readable format (for debugging)
// Note: This is a helper function — do not modify it.
void dump_list(FILE *fp, struct linked_list list)
{
    fprintf(fp, "[ "); // Opening bracket for the list output

    // Iterate through the list and print each value
    for (struct list_node *cur = list.head; cur != NULL; cur = cur->next)
    {
        fprintf(fp, "%zu ", cur->value);
    }

    fprintf(fp, "]\n"); // Closing bracket and newline
}
