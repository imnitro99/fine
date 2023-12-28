#include "connection.h"
#include <stdio.h>
#include <stdlib.h>

int parseConnectionFile(const char* filename, Connection** connections, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    int capacity = 10; // Initial capacity for connections array
    *connections = malloc(sizeof(Connection) * capacity);
    if (*connections == NULL) {
        perror("Failed to allocate memory for connections");
        fclose(file);
        return -1;
    }

    *count = 0;
    while (fscanf(file, "%d %d %d %d", &((*connections)[*count].fromGateId), &((*connections)[*count].fromPin), &((*connections)[*count].toGateId), &((*connections)[*count].toPin)) == 4) {
        (*count)++;
        // Resize the array if necessary
        if (*count >= capacity) {
            capacity *= 2; // Double the capacity
            *connections = realloc(*connections, sizeof(Connection) * capacity);
            if (*connections == NULL) {
                perror("Failed to reallocate memory for connections");
                fclose(file);
                return -1;
            }
        }
    }

    fclose(file);
    return 0;
}