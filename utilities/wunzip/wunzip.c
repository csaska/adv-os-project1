#include <stdio.h>
#include <stdint.h>
#include <stdbool.h> 

const uint32_t CHUNK_SIZE = 1024;

int main(int argc, char *argv[]) {
    if (argc == 1) {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }

    char chunk[CHUNK_SIZE];
    size_t curr_size = 0;

    for (int idx = 1; idx < argc; ++idx) {
        char *filename = argv[idx];

        // open file stream
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) {
            printf("wunzip: cannot open file\n");
            return 1;
        }

        while (true) {
            // TODO: is there a way to read in larger chunks? This seems like a ton of file I/O that could be avoided

            // read char count
            uint32_t char_count;
            size_t n_bytes = fread(&char_count, sizeof(uint32_t), 1, fp);
            if (n_bytes <= 0) {
                break;
            }
            if (n_bytes != 1) {
                printf("wunzip: file format error\n");
                return 1;
            }

            // read char value
            char char_val;
            n_bytes = fread(&char_val, sizeof(char), 1, fp);
            if (n_bytes <= 0) {
                printf("wunzip: file format error\n");
                return 1;
            }
            if (n_bytes != 1) {
                printf("wunzip: file format error\n");
                return 1;
            }

            for (uint32_t i = 0; i < char_count; i++) {
                if (curr_size == CHUNK_SIZE) {
                    fwrite(chunk, 1, curr_size, stdout);
                    curr_size = 0;
                }
                chunk[curr_size] = char_val;
                ++curr_size;
            }
        }
    }

    if (curr_size > 0) {
        fwrite(chunk, 1, curr_size, stdout);
    }
}



// read binary file in sizeof(buffer) bytes chunks
//  while ((bytes_read = fread(chunk, 1, sizeof(chunk), fp)) > 0) {
//      // file format guarantees file size being 5 byte multiple
//      if (bytes_read % DECODE_SIZE != 0) {
//          printf("wunzip: decode error - expected file with size of %d byte multiple\n", DECODE_SIZE);
//          return 1;
//      }

//      // decode chunk
//      //  char decoded_chunk;
//      //  size_t decoded_bytes;
//      //  decode(chunk, bytes_read, &decoded_chunk, &decoded_bytes);

//      // flush to buffer
//      printf("Read %zu bytes\n", bytes_read);
//      printf("Chunk: '%s'", chunk);
//      fwrite(chunk, bytes_read, 1, stdout);
//      return 0;
//  }
