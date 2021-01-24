#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h> 

struct RleEntry {
    char curr_char;
    int32_t count;
    bool valid;
};

void write_rle_entry(struct RleEntry *rle_ptr, FILE *out_fp) {
    fwrite((const void*) & rle_ptr->count, sizeof(int32_t), 1, out_fp);
    fwrite((char *)(&rle_ptr->curr_char), sizeof(char), 1, out_fp);
    //  printf("%d%c", rle_ptr->count, rle_ptr->curr_char);
}

void encode(char *src, struct RleEntry *rle_ptr) {
    if (rle_ptr == NULL) {
        printf("wzip: program error\n");
        exit(1);
    }

    // if first file, initialize RleEntry with first char in file
    size_t i = 0;
    if (rle_ptr->valid == false) {
        rle_ptr->curr_char = src[0];
        rle_ptr->count = 1;
        rle_ptr->valid = true;
        ++i;
    }

    while (src[i] != '\0') {
        if (src[i] == rle_ptr->curr_char) {
            ++(rle_ptr->count);

            // TODO: need to check we aren't overflowing uint32_t
        } else {
            // write binary rle_count
            write_rle_entry(rle_ptr, stdout);

            // reset rle count
            rle_ptr->count = 1;
            rle_ptr->curr_char = src[i];
        }
        ++i;
    }
}

//  void encode_to_stream(FILE *in_fp, FILE* out_fp) {
//      char *line = NULL;
//      size_t len = 0;
//      ssize_t nread = 0;
//      while ((nread = getline(&line, &len, in_fp)) != -1) {
//          encode(line);
//      }
//      free(line);
//  }

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    struct RleEntry *rle_ptr = (struct RleEntry*)malloc(sizeof(struct RleEntry)); 
    rle_ptr->valid = false;

    for (int idx = 1; idx < argc; ++idx) {
        char *filename = argv[idx];

        // open file stream
        FILE *fp = fopen(filename, "r");
        if (fp == NULL) {
            printf("wzip: cannot open file\n");
            return 1;
        }

        // encode each line of file
        char *line = NULL;
        size_t len = 0;
        ssize_t nread = 0;
        while ((nread = getline(&line, &len, fp)) != -1) {
            // TODO: may be cleaner if encode returned a linked list of RleEntry's. That would
            //       allow the caller to determine how the file is writtne. Awkward that write_rle_entry
            //       is called from within encode (by the callee) and after the for loop (by the caller).
            encode(line, rle_ptr);
        }
        free(line);
    }

    // write last entry 
    write_rle_entry(rle_ptr, stdout);

    free(rle_ptr);
}
