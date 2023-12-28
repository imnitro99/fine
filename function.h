#ifndef FUNCTION_H
#define FUNCTION_H

typedef struct {
    int typeId;
    unsigned int inputBits;
    unsigned int outputBits;
} Function;

int parseFunctionFile(const char* filename, Function** functions, int* count);

#endif // FUNCTION_H
