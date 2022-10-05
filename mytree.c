#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
int d_count = 0;
int file_count = 0;

void p_Mode(char *name, char *n) {
    DIR *dir_info;
    struct dirent *dir_entry;
    struct stat sb;

    stat(name, &sb);
    switch (sb.st_mode & S_IFMT) {
    case S_IFBLK:
        printf("b");
        file_count++;
        break;
    case S_IFCHR:
        printf("c");
        file_count++;
        break;
    case S_IFDIR:
        if (!strcmp(n, "current"))
            printf("l");
        else {
            printf("d");
            d_count++;
        }
        break;
    case S_IFIFO:
        printf("p");
        file_count++;
        break;
    case S_IFLNK:
        printf("l");
        break;
    case S_IFSOCK:
        printf("s");
        file_count++;
        break;
    default:
        printf("-");
        file_count++;
        break;
    }

    if (sb.st_mode & S_IRUSR)
        printf("%s", "r");
    else {
        printf("%s", "-");
    }
    if (sb.st_mode & S_IWUSR)
        printf("%s", "w");
    else {
        printf("%s", "-");
    }
    if (sb.st_mode & S_IXUSR)
        printf("%s", "x");
    else {
        printf("%s", "-");
    }

    if (sb.st_mode & S_IRGRP)
        printf("%s", "r");
    else {
        printf("%s", "-");
    }
    if (sb.st_mode & S_IWGRP)
        printf("%s", "w");
    else {
        printf("%s", "-");
    }
    if (sb.st_mode & S_IXGRP)
        printf("%s", "x");
    else {
        printf("%s", "-");
    }

    if (sb.st_mode & S_IROTH)
        printf("%s", "r");
    else {
        printf("%s", "-");
    }
    if (sb.st_mode & S_IWOTH)
        printf("%s", "w");
    else {
        printf("%s", "-");
    }
    if (sb.st_mode & S_IXOTH)
        printf("%s", "x");
    else {
        printf("%s", "-");
    }
}

void bubbleSort(char **word, int len) {

    char temp[100];

    for (int i = len - 1; i > 0; i--) {

        for (int t = 0; t < i; t++) {

            int check = strcmp(word[t], word[t + 1]);
            if (check > 0) {
                strcpy(temp, word[t + 1]);
                strcpy(word[t + 1], word[t]);
                strcpy(word[t], temp);
            }
        }
    }
}

void open_dir(char *name, int inside, int c, int n) {
    DIR *dir_info;
    struct dirent *dir_entry;
    struct stat sb;
    char *check;
    char *b_name;
    char *file_name[100];
    int count = 0;
    double size = 0;
    int len = 0;
    int p = 0;
    dir_info = opendir(name);
    if (NULL != dir_info) {
        while (dir_entry = readdir(dir_info)) {
            if (strncmp(dir_entry->d_name, ".", 1)) {
                file_name[count] = dir_entry->d_name;
                count++;
            }
        }

        bubbleSort(file_name, count);

        for (int i = 0; i < count; i++) {
            if (c - 1 == n) {
                for (int j = 0; j < inside - 1; j++) {
                    printf("│");
                    printf("   ");
                }
                printf("   ");
            } else {
                for (int j = 0; j < inside; j++) {
                    printf("│");
                    printf("   ");
                }
            }
            if (i < count - 1) {
                printf("├");
            } else {
                printf("└");
            }
            printf("──");

            b_name = (char *)calloc(100, sizeof(char) * 100);
            strcat(b_name, name);
            strcat(b_name, "/");
            strcat(b_name, file_name[i]);
            dir_entry = readdir(dir_info);
            stat(b_name, &sb);
            printf("[ %ld %ld ", (long)sb.st_ino, (unsigned long)sb.st_dev);
            p_Mode(b_name, file_name[i]);
            if ((unsigned long)sb.st_size > 1000000) {
                size = (double)sb.st_size / 1000000;
                p = 2;

            } else if ((unsigned long)sb.st_size > 1000) {
                size = (double)sb.st_size / 1000;
                p = 1;
            } else {
                size = (double)sb.st_size;
                p = 0;
            }
            if (p == 2) {
                if (size / 10 < 1)
                    printf(" %s      %.1fM]", getpwuid(sb.st_uid)->pw_name,
                           size);
                else
                    printf(" %s      %.0fM]", getpwuid(sb.st_uid)->pw_name,
                           size);
            } else if (p == 1) {
                if (size / 10 < 1) {
                    size = size - 0.05;
                    printf(" %s      %.1fK]", getpwuid(sb.st_uid)->pw_name,
                           size);
                } else
                    printf(" %s      %.0fK]", getpwuid(sb.st_uid)->pw_name,
                           size);

            } else {
                printf(" %s      %.0f]", getpwuid(sb.st_uid)->pw_name, size);
            }
            if (!strcmp(file_name[i], "current")) {
                printf(" %s -> %s\n", file_name[i], file_name[i - 2]);
            } else {
                printf(" %s\n", file_name[i]);
            }
            open_dir(b_name, inside + 1, count, i);
        }
        closedir(dir_info);
    }
}

void main() {
    int i = 0, p = 0;
    double size = 0;
    char name[] = ".";
    struct stat sb;
    stat(name, &sb);
    printf("[ %ld %ld ", (long)sb.st_ino, (unsigned long)sb.st_dev);
    p_Mode(".", ".");
    if ((unsigned long)sb.st_size > 1000000) {
        size = (double)sb.st_size / 1000000;
        p = 2;

    } else if ((unsigned long)sb.st_size > 1000) {
        size = (double)sb.st_size / 1000;
        p = 1;
    } else {
        size = (double)sb.st_size;
        p = 0;
    }
    if (p == 2) {
        if (size / 10 < 1)
            printf(" %s      %.1fM] %s\n", getpwuid(sb.st_uid)->pw_name, size,
                   ".");
        else
            printf(" %s      %.0fM] %s\n", getpwuid(sb.st_uid)->pw_name, size,
                   ".");
    } else if (p == 1) {
        if (size / 10 < 1) {
            size = size - 0.05;
            printf(" %s      %.1fK] %s\n", getpwuid(sb.st_uid)->pw_name, size,
                   ".");
        } else
            printf(" %s      %.0fK] %s\n", getpwuid(sb.st_uid)->pw_name, size,
                   ".");

    } else {
        printf(" %s      %.0f] %s\n", getpwuid(sb.st_uid)->pw_name, size, ".");
    }
    open_dir(name, 0, 0, 0);
    printf("\n\n%d directories, %d files\n", d_count + 2, file_count);
}
