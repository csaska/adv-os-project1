#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include <sys/stat.h>


FILE* open_stream(char *filename, char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", filename);
        exit(1);
    }
    return fp;
}

bool is_same_file(int fd1, int fd2) {
    // https://stackoverflow.com/questions/12502552/can-i-check-if-two-file-or-file-descriptor-numbers-refer-to-the-same-file
    struct stat stat1, stat2;
    if(fstat(fd1, &stat1) < 0) {
        fprintf(stderr, "reverse: cannot stat input file to verify doesn't match output");
        exit(1);
    }
    if(fstat(fd2, &stat2) < 0) {
        fprintf(stderr, "reverse: cannot stat output file to verify doesn't match input");
        exit(1);
    }
    return stat1.st_dev == stat2.st_dev && stat1.st_ino == stat2.st_ino;
}

struct Node {
    char *data;
    struct Node *next;
};

struct Node* read_input_to_list(FILE *input) {
    struct Node *head = NULL;

    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    while ((nread = getline(&line, &len, input)) != -1) {
        // create new node
        struct Node *node = (struct Node*)malloc(sizeof(struct Node)); 
        if (node == NULL) {
            fprintf(stderr, "reverse: malloc failed");
            exit(1);
        }
        node->data = line;

        // add node to the front of the list
        if (head != NULL) {
            node->next = head;
        }
        head = node; 

        // unset line and len so getline allocates a new buffer
        line = NULL;
        len = 0;
    }

    // TODO: check if getline() failed to allocate memory

    return head;
}

void print_list(struct Node *head, FILE *output) {
    //  if (head == NULL) {
    //      //  fprintf(stderr, "reverse: input file is empty\n");
    //      return 1;
    //  }

    // print linked list
    struct Node *curr_node = head;
    while(curr_node != NULL) {
        fprintf(output, "%s", curr_node->data);

        // free char buffer we just printed
        free(curr_node->data);

        // free current node
        struct Node* temp_node = curr_node->next;
        free(curr_node);
        curr_node = temp_node;
    }
}

int main(int argc, char *argv[]) {
    // too many args specified
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return 1;
    }

    // open input and output file if specified
    FILE *input = argc > 1 ? open_stream(argv[1], "r") : stdin;
    FILE *output = argc > 2 ? open_stream(argv[2], "w") : stdout;

    // validate input and output file are different
    if (argc == 3 && is_same_file(fileno(input), fileno(output))) {
        fprintf(stderr, "reverse: input and output file must differ\n");
        return 1;
    }

    // store input into linked list
    struct Node *head = read_input_to_list(input);

    if (input != stdin) {
        fclose(input); 
    }

    print_list(head, output);

    if (output != stdout) {
        fclose(output); 
    }

    return 0;
}

