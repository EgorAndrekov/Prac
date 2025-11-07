#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

static char** list = NULL;
static int list_size = 0;
static int list_capacity = 0;

void init_list() {
    list = malloc(SIZE * sizeof(char*));
    list_capacity = SIZE;
    list_size = 0;
}

void add_word(const char* word) {
    if (list == NULL) init_list();
    
    if (list_size >= list_capacity - 1) {
        list_capacity += SIZE;
        list = realloc(list, list_capacity * sizeof(char*));
    }
    
    list[list_size] = malloc(strlen(word) + 1);
    strcpy(list[list_size], word);
    list_size++;
}

void sort_list() {
    for (int i = 0; i < list_size - 1; i++) {
        for (int j = 0; j < list_size - i - 1; j++) {
            if (strcmp(list[j], list[j + 1]) > 0) {
                char* temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }
}

void print_list() {
    for (int i = 0; i < list_size; i++) {
        printf("%s ", list[i]);
    }
    printf("\n");
}

void print_size(){
    printf("%d\n", list_size);
}

void clear_list() {
    if (list == NULL) return;
    
    for (int i = 0; i < list_size; i++) {
        free(list[i]);
    }
    free(list);
    list = NULL;
    list_size = 0;
    list_capacity = 0;
}

int get_list_size() {
    return list_size;
}