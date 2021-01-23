#include <stdio.h>
#include <stdlib.h>

void print_file(FILE *in_fp, FILE *out_fp) {
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    while ((nread = getline(&line, &len, in_fp)) != -1) {
        fprintf(out_fp, "%s", line);
    }
    free(line);
}

int main(int argc, char *argv[]) {
    for (int idx = 1; idx < argc; ++idx) {
        char *filename = argv[idx];

        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            printf("wcat: cannot open file\n");
            return 1;
        }
        print_file(fp, stdout);
    }
}
