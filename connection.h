#ifndef CONNECTION_H
#define CONNECTION_H

typedef struct {
    int fromGateId;
    int fromPin;
    int toGateId;
    int toPin;
} Connection;

int parseConnectionFile(const char* filename, Connection** connections, int* count);

#endif // CONNECTION_H
