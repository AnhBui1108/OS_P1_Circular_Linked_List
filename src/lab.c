/**Update this file with the starter code**/

#include "lab.h"


list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {
    list_t *list = (list_t *)malloc(sizeof(list_t));
    if (list != NULL) {
        // Set function pointers
        list->destroy_data = destroy_data;
        list->compare_to = compare_to;
        list->size = 0;

        // Allocate memory for the sentinel node
        list->head = (node_t *)malloc(sizeof(node_t));
        if (list->head != NULL) {
            // Initialize sentinel node
            list->head->data = NULL;  // Sentinel node doesn't store data
            list->head->next = list->head;  // Point to itself
            list->head->prev = list->head;  // Point to itself
        } else {
            free(list);  // Free list if sentinel node allocation fails
            return NULL;
        }
    }
    return list;
}


void list_destroy(list_t **list) {
    if (list != NULL && *list != NULL) {
    
        node_t *current = (*list)->head->next; // Start with the first data node
        while (current != (*list)->head) {     // Stop when we reach the sentinel node
            printf("Freeing node: %p, next: %p\n", (void *)current, (void *)current->next);
            node_t *next = current->next;      // Save the next node
            if ((*list)->destroy_data != NULL) {
                (*list)->destroy_data(current->data); // Free node's data
            }
            free(current); // Free the current node
            current = next; // Move to the next node
        }
        
        free((*list)->head); // Free the sentinel node
        free(*list);         // Free the list struct
        *list = NULL;        // Avoid dangling pointers
    }
}


list_t *list_add(list_t *list, void *data) {
    if (!list) return NULL; // Check for NULL list
    if (!data) return list; // Do not add NULL data

    // Create a new node
    node_t *newNode = (node_t *)malloc(sizeof(node_t));
    if (!newNode) return list; // Allocation failure

    newNode->data = data;

    if (list->size == 0) { // First data node
        newNode->next = list->head;
        newNode->prev = list->head;
        list->head->next = newNode;
        list->head->prev = newNode;
    } else { // Non-empty list
        newNode->next = list->head->next;
        newNode->prev = list->head;

        list->head->next->prev = newNode;
        list->head->next = newNode;
    }

    list->size++; 
    return list;
}


void *list_remove_index(list_t *list, size_t index) {
    if (!list || index >= list->size || !list->head) return NULL;

    // Start from the first data node (not the sentinel)
    node_t *current = list->head->next;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }

    // Save the data to return
    void *data = current->data;

    // Update links to remove the current node
    current->prev->next = current->next;
    current->next->prev = current->prev;

    // Special case: Removing the last node
    if (list->size == 1) {
        list->head->next = list->head;
        list->head->prev = list->head;
    }

    // Free the node
    free(current);
    list->size--;

    return data;
}


int list_indexof(list_t *list, void *data) {
    if (!list || !data || !list->head) return -1; // Invalid inputs

    node_t *current = list->head->next; // Start at the first data node
    int index = 0;

    while (current != list->head) { // Stop when we return to the sentinel node
        if (current->data && list->compare_to(current->data, data) == 0) {
            return index; // Found the matching data
        }
        current = current->next; // Move to the next node
        index++;
    }
    return -1;
}
