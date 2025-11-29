#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int f_s = 0;

    int original_link_index = 1;
    int new_link_index = 2;

    if (strcmp(argv[1], "-s") == 0){
        f_s = 1;
        original_link_index = 2;
        new_link_index = 3;

    }

    char *original_link = argv[original_link_index];
    char *new_link = argv[new_link_index];
    int res;
    if (f_s){
        res = symlink(original_link, new_link);
    }
    else{
        res = link(original_link, new_link);
    }

}