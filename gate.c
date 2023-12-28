#include "gate.h"
#include <stdio.h>
#include <stdlib.h>

int parseGateFile(const char* filename, Gate** gates, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    int capacity = 10; // Initial capacity for gates array
    *gates = malloc(sizeof(Gate) * capacity);
    if (*gates == NULL) {
        perror("Failed to allocate memory for gates");
        fclose(file);
        return -1;
    }

    *count = 0;
    while (fscanf(file, "%d %d", &((*gates)[*count].gateId), &((*gates)[*count].typeId)) == 2) {
        (*count)++;
        // Resize the array if necessary
        if (*count >= capacity) {
            capacity *= 2; // Double the capacity
            *gates = realloc(*gates, sizeof(Gate) * capacity);
            if (*gates == NULL) {
                perror("Failed to reallocate memory for gates");
                fclose(file);
                return -1;
            }
        }
    }

    fclose(file);
    return 0;
}