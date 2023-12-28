#include "function.h"
#include <stdio.h>
#include <stdlib.h>

int parseFunctionFile(const char* filename, Function** functions, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    int capacity = 10; // Initial capacity for functions array
    *functions = malloc(sizeof(Function) * capacity);
    if (*functions == NULL) {
        perror("Failed to allocate memory for functions");
        fclose(file);
        return -1;
    }

    *count = 0;
    while (fscanf(file, "%d %u %u", &((*functions)[*count].typeId), &((*functions)[*count].inputBits), &((*functions)[*count].outputBits)) == 3) {
        (*count)++;
        // Resize the array if necessary
        if (*count >= capacity) {
            capacity *= 2; // Double the capacity
            *functions = realloc(*functions, sizeof(Function) * capacity);
            if (*functions == NULL) {
                perror("Failed to reallocate memory for functions");
                fclose(file);
                return -1;
            }
        }
    }

    fclose(file);
    return 0;
}