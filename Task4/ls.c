#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

void print_file_attributes(struct stat fileStat, char *filename, int f_g){
    if (S_ISREG(fileStat.st_mode))
        printf("-");
    else if (S_ISDIR(fileStat.st_mode))
        printf("d");
    else if (S_ISLNK(fileStat.st_mode))
        printf("l");
    else
        printf("?");

    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");

    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");

    printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group *gr = getgrgid(fileStat.st_gid);
    printf(" %s", pw->pw_name);

    if (f_g){
        printf(" %s", gr->gr_name);
    }
    printf(" %ld", fileStat.st_size);

    printf(" %s", filename);

    if (S_ISLNK(fileStat.st_mode)){
        char link[1024];
        ssize_t l = readlink(filename, link, sizeof(link) - 1);
        if (l != -1)
        {
            link[l] = '\0';
            printf(" -> %s", link);
        }
    }

    printf("\n");
}

void print_directory_contents(const char *dir_path, int f_l, int f_g){
    DIR *dir;
    struct dirent *content;
    int first = 1;

    dir = opendir(dir_path);


    while ((content = readdir(dir)) != NULL){
        if (strcmp(content->d_name, ".") == 0 || strcmp(content->d_name, "..") == 0)
            continue;

        if (f_l){
            char filePath[1024];
            snprintf(filePath, sizeof(filePath), "%s/%s", dir_path, content->d_name);

            struct stat fileStat;
            if (lstat(filePath, &fileStat) == -1) {
                continue;
            }
            print_file_attributes(fileStat, content->d_name, f_g);
        }
        else{
            if (!first)
            {
                printf(" ");
            }
            printf("%s", content->d_name);
            first = 0;
        }
    }
    
    if (!f_l){
        printf("\n");
    }
    
    closedir(dir);
}

void list_dir_recursive(const char *f_p, int f_l, int f_g, int is_first){
    DIR *dir;
    struct dirent *content;

    dir = opendir(f_p);

    if (is_first){
        printf("%s:\n", f_p);
    }
    else{
        printf("\n%s:\n", f_p);
    }

    print_directory_contents(f_p, f_l, f_g);

    rewinddir(dir);
    while ((content = readdir(dir)) != NULL){
        if (strcmp(content->d_name, ".") == 0 || strcmp(content->d_name, "..") == 0)
            continue;

        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s", f_p, content->d_name);

        struct stat fileStat;
        if (lstat(filePath, &fileStat) == -1){
            continue;
        }

        if (S_ISDIR(fileStat.st_mode)){
            list_dir_recursive(filePath, f_l, f_g, 0);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]){
    int f_R = 0;
    int f_l = 0;
    int f_g = 0;

    for (int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-R") == 0){
            f_R = 1;
        }
        else if (strcmp(argv[i], "-l") == 0){
            f_l = 1;
        }
        else if (strcmp(argv[i], "-g") == 0){
            f_g = 1;
        }
    }

    if (f_R){
        list_dir_recursive(".", f_l, f_g, 1);
    }
    else{
        print_directory_contents(".", f_l, f_g);
    }

    return 0;
}