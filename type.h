#ifndef TYPE_H
#define TYPE_H

typedef struct {
    int typeId;
    char name[50];
    int numInputPins;
    int numOutputPins;
    float delay;
} Type;

int parseTypeFile(const char* filename, Type** types, int* count);
Type* findTypeById(Type* types, int typeCount, int typeId);


#endif // TYPE_H
