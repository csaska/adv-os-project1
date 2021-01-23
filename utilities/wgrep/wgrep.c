#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void search_file(FILE *fp, char *search_term) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    while ((nread = getline(&line, &len, fp)) != -1) {
        if (strstr(line, search_term) != NULL) {
            printf("%s", line);
        }
    }
    free(line);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("wgrep: searchterm [file ...]\n");
        return 1;
    }

    char *search_term = argv[1];

    if (argc == 2) {
        // search stdin
        search_file(stdin, search_term);
    } else {
        // search list of files
        for (int idx = 2; idx < argc; ++idx) {
            char *filename = argv[idx];

            FILE *fp = fopen(filename, "r");
            if (fp == NULL) {
                printf("wgrep: cannot open file\n");
                return 1;
            }

            search_file(fp, search_term);
        }
    }
}
