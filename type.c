#include "type.h"
#include <stdio.h>
#include <stdlib.h>

Type* findTypeById(Type* types, int typeCount, int typeId) {
    for (int i = 0; i < typeCount; i++) {
        if (types[i].typeId == typeId) {
            return &types[i];
        }
    }
    return NULL; // Type not found
}

int parseTypeFile(const char* filename, Type** types, int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return -1;
    }

    int capacity = 10; // Initial capacity for types array
    *types = malloc(sizeof(Type) * capacity);
    if (*types == NULL) {
        perror("Failed to allocate memory for types");
        fclose(file);
        return -1;
    }

    *count = 0;
    while (fscanf(file, "%d %49s %d %d %f", &((*types)[*count].typeId), (*types)[*count].name, &((*types)[*count].numInputPins), &((*types)[*count].numOutputPins), &((*types)[*count].delay)) == 5) {
        (*count)++;
        // Resize the array if necessary
        if (*count >= capacity) {
            capacity *= 2; // Double the capacity
            *types = realloc(*types, sizeof(Type) * capacity);
            if (*types == NULL) {
                perror("Failed to reallocate memory for types");
                fclose(file);
                return -1;
            }
        }
    }

    fclose(file);
    return 0;
}