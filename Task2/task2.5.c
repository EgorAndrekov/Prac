#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct WordNode {
    char *word;
    struct WordNode *next;
} WordNode;

WordNode *create_node(const char *word) {
    WordNode *node = malloc(sizeof(WordNode));
    node->word = strdup(word);
    node->next = NULL;
    return node;
}

void free_list(WordNode *head) {
    while (head) {
        WordNode *temp = head;
        head = head->next;
        free(temp->word);
        free(temp);
    }
}

char *reverse_string(char *str) {
    if (str == NULL) {
        return NULL;
    }
    
    int length = strlen(str);
    int i, j;
    char temp;
    
    for (i = 0, j = length - 1; i < j; i++, j--) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
    
    return str;
}

int main() {
    char line[1000];
    fgets(line, sizeof(line), stdin);
    WordNode *head = NULL, *tail = NULL;
    char *token = strtok(line, " \t\n");
    while (token) {
        WordNode *new_node = create_node(token);
        if (!head) head = new_node;
        else tail->next = new_node;
        tail = new_node;
        token = strtok(NULL, " \t\n");
    }
    if (!head) return 0;
    const char *last_word = tail->word;
    WordNode *prev = NULL, *curr = head;
    while (curr) {
        if (curr != tail && strcmp(curr->word, last_word) == 0) {
            if (prev) prev->next = curr->next;
            else head = curr->next;
            WordNode *to_free = curr;
            curr = curr->next;
            free(to_free->word);
            free(to_free);
        } else {
            prev = curr;
            curr = curr->next;
        }
    }
    curr = head;
    while (curr) {
        curr->word = reverse_string(curr->word);
        printf("%s", curr->word);
        if (curr->next) printf(" ");
        curr = curr->next;
    }
    printf("\n");
    free_list(head);
    return 0;
}