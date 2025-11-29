#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int count_words(char *str){
    int count = 0;
    int fl_space = 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            fl_space = 0;
        } else {
            if (!fl_space) {
                count++;
                fl_space = 1;
            }
        }
    }
    return count;
}

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && (str[len-1] == '\n' || str[len-1] == '\r')) {
        str[len-1] = '\0';
        if (len > 1 && str[len-2] == '\r') {
            str[len-2] = '\0';
        }
    }
}

void cat(const char *filename, int pr_n, int pr_w, int *line_n){
    FILE *file = fopen(filename, "r");


    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL){
        if (pr_n){
            if (line[0] == '\n' || (line[0] == '\r' && line[1] == '\n')){
                printf("%6d\n", (*line_n)++);
            }
            else{
                printf("%6d  %s", (*line_n)++, line);
            }
        } else if (pr_w) {
            char temp_line[1024];
            strcpy(temp_line, line);

            remove_newline(temp_line);
            
            int word_count = count_words(temp_line);
            printf("%s %d\n", temp_line, word_count);
        }
        else{
            printf("%s", line);
        }
    }
    fclose(file);
}

int main(int argc, char *argv[]){
    int pr_n = 0;
    int pr_w = 0;
    int index_strt = 1;

    if (strcmp(argv[1], "-n") == 0){
        pr_n = 1;
        index_strt = 2;
        
    }

    if (strcmp(argv[1], "-w") == 0){
        pr_w = 1;
        index_strt = 2;
        
    }

    int line_n = 1;
    for (int i = index_strt; i < argc; i++){
        cat(argv[i], pr_n, pr_w, &line_n);
    }
    
    return 0;
}