#ifndef GATE_H
#define GATE_H

#include "type.h"

typedef struct {
    int gateId;
    int typeId;
} Gate;

int parseGateFile(const char* filename, Gate** gates, int* count);

#endif // GATE_H
