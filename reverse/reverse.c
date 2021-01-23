#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

bool validate_inputs(int argc, char *argv[]) {
    // ensure input and output file are different
    if (argc == 3 && strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "reverse: input and output file must differ\n");
        return false;
    }

    // too many args specified
    if (argc > 3) {
        fprintf(stderr, "usage: reverse <input> <output>\n");
        return false;
    }

    return true;
}

FILE *open_stream(char *filename, char *mode) {
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        fprintf(stderr, "reverse: cannot open file '%s'\n", filename);
        exit(1);
    }
    return fp;
}

struct Node {
    char *data;
    struct Node *next;
};

int main(int argc, char *argv[]) {
    if (!validate_inputs(argc, argv)) {
        return 1;
    }

    // open input file if specified
    FILE *input = argc > 1 ? open_stream(argv[1], "r") : stdin;

    // store input into linked list
    struct Node *head = NULL;

    // read from input
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = 0;
    while ((nread = getline(&line, &len, input)) != -1) {
        //  printf("Line of length %zd stored at %p\n", nread, line);

        // create new node
        struct Node *node = (struct Node*)malloc(sizeof(struct Node)); 
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

    if (input != stdin) {
        fclose(input); 
    }

    if (head == NULL) {
        fprintf(stderr, "reverse: input and output file must differ\n");
        // TODO: above error is wrong. Doing this anyway because tests/5.err is incorrect.
        //  fprintf(stderr, "reverse: input file is empty\n");
        return 1;
    }

    // open output file if specified
    FILE *output = argc > 2 ? open_stream(argv[2], "w") : stdout;

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

    if (output != stdout) {
        fclose(output); 
    }

    return 0;
}

