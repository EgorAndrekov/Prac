#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"

#define BUFFER_SIZE 256
#define SYMBOL_BUFFER_SIZE 16

typedef enum {
    START, WORD, SPECIAL, GREATER, GREATER2, 
    PIPE, PIPE2, AMPERSAND, AMPERSAND2
} State;

static char read_buffer[BUFFER_SIZE];
static int buffer_pos = 0;
static int buffer_size = 0;

int getsym() {
    if (buffer_pos >= buffer_size) {
        buffer_size = fread(read_buffer, 1, BUFFER_SIZE, stdin);
        buffer_pos = 0;
        if (buffer_size == 0) return EOF;
    }
    return read_buffer[buffer_pos++];
}

int is_word_char(int ch) {
    return isalnum(ch) || ch == '$' || ch == '\'' || ch == '`' || ch == '/' || ch == ',';
}

int is_special_char(int ch) {
    return ch == '|' || ch == '&' || ch == ';' || ch == '>' || ch == '<' || ch == '(' || ch == ')';
}

int main() {
    init_list();
    
    State state = START;
    int c;
    char* current_word = NULL;
    int word_size = 0;
    int word_capacity = 0;
    
    while ((c = getsym()) != EOF) {
        switch (state) {
            case START:
                if (c == ' ' || c == '\t') {
                    continue;
                } else if (c == '\n') {
                    if (current_word != NULL && word_size > 0) {
                        current_word[word_size] = '\0';
                        add_word(current_word);
                        free(current_word);
                        current_word = NULL;
                        word_size = 0;
                        word_capacity = 0;
                    }
                
                    if (get_list_size() > 0) {
                        print_size();
                        print_list();
                        sort_list();
                        print_list();
                        clear_list();
                    }
                } else if (is_word_char(c)) {
                    state = WORD;
                    if (current_word == NULL) {
                        current_word = malloc(SYMBOL_BUFFER_SIZE);
                        word_capacity = SYMBOL_BUFFER_SIZE;
                    }
                    if (word_size >= word_capacity - 1) {
                        word_capacity += SYMBOL_BUFFER_SIZE;
                        current_word = realloc(current_word, word_capacity);
                    }
                    current_word[word_size++] = c;
                } else if (is_special_char(c)) {
                    state = SPECIAL;
                    if (current_word == NULL) {
                        current_word = malloc(SYMBOL_BUFFER_SIZE);
                        word_capacity = SYMBOL_BUFFER_SIZE;
                    }
                    if (word_size >= word_capacity - 1) {
                        word_capacity += SYMBOL_BUFFER_SIZE;
                        current_word = realloc(current_word, word_capacity);
                    }
                    current_word[word_size++] = c;
                    
                    if (c == '>') state = GREATER;
                    else if (c == '|') state = PIPE;
                    else if (c == '&') state = AMPERSAND;
                }
                break;
                
            case WORD:
                if (is_word_char(c)) {
                    if (word_size >= word_capacity - 1) {
                        word_capacity += SYMBOL_BUFFER_SIZE;
                        current_word = realloc(current_word, word_capacity);
                    }
                    current_word[word_size++] = c;
                } else {
                    current_word[word_size] = '\0';
                    add_word(current_word);
                    free(current_word);
                    current_word = NULL;
                    word_size = 0;
                    word_capacity = 0;
                    
                    state = START;
                    if (buffer_pos > 0) buffer_pos--;
                }
                break;
                
            case GREATER:
                if (c == '>') {
                    if (word_size >= word_capacity - 1) {
                        word_capacity += SYMBOL_BUFFER_SIZE;
                        current_word = realloc(current_word, word_capacity);
                    }
                    current_word[word_size++] = c;
                    state = GREATER2;
                } else {
                    current_word[word_size] = '\0';
                    add_word(current_word);
                    free(current_word);
                    current_word = NULL;
                    word_size = 0;
                    word_capacity = 0;
                    
                    state = START;
                    if (buffer_pos > 0) buffer_pos--;
                }
                break;
                
            case GREATER2:
                current_word[word_size] = '\0';
                add_word(current_word);
                free(current_word);
                current_word = NULL;
                word_size = 0;
                word_capacity = 0;
                
                state = START;
                if (buffer_pos > 0) buffer_pos--;
                break;
                
            case PIPE:
                if (c == '|') {
                    if (word_size >= word_capacity - 1) {
                        word_capacity += SYMBOL_BUFFER_SIZE;
                        current_word = realloc(current_word, word_capacity);
                    }
                    current_word[word_size++] = c;
                    state = PIPE2;
                } else {
                    current_word[word_size] = '\0';
                    add_word(current_word);
                    free(current_word);
                    current_word = NULL;
                    word_size = 0;
                    word_capacity = 0;
                    
                    state = START;
                    if (buffer_pos > 0) buffer_pos--;
                }
                break;
                
            case PIPE2:
                current_word[word_size] = '\0';
                add_word(current_word);
                free(current_word);
                current_word = NULL;
                word_size = 0;
                word_capacity = 0;
                
                state = START;
                if (buffer_pos > 0) buffer_pos--;
                break;
                
            case AMPERSAND:
                if (c == '&') {
                    if (word_size >= word_capacity - 1) {
                        word_capacity += SYMBOL_BUFFER_SIZE;
                        current_word = realloc(current_word, word_capacity);
                    }
                    current_word[word_size++] = c;
                    state = AMPERSAND2;
                } else {
                    current_word[word_size] = '\0';
                    add_word(current_word);
                    free(current_word);
                    current_word = NULL;
                    word_size = 0;
                    word_capacity = 0;
                    
                    state = START;
                    if (buffer_pos > 0) buffer_pos--;
                }
                break;
                
            case AMPERSAND2:
                current_word[word_size] = '\0';
                add_word(current_word);
                free(current_word);
                current_word = NULL;
                word_size = 0;
                word_capacity = 0;
                
                state = START;
                if (buffer_pos > 0) buffer_pos--;
                break;
                
            case SPECIAL:
                current_word[word_size] = '\0';
                add_word(current_word);
                free(current_word);
                current_word = NULL;
                word_size = 0;
                word_capacity = 0;
                
                state = START;
                if (buffer_pos > 0) buffer_pos--;
                break;
        }
    }
    
    if (current_word != NULL && word_size > 0) {
        current_word[word_size] = '\0';
        add_word(current_word);
        free(current_word);
    }
    
    if (get_list_size() > 0) {
        print_size();
        print_list();
        sort_list();
        print_list();
    }
    
    clear_list();
    return 0;
}
