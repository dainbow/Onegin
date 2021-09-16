#ifndef _STRUCT_WORKS_H_
#define _STRUCT_WORKS_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>

struct String {
    uint8_t *value;
    size_t length;
};

struct Text {
    struct String *strings;
    uint8_t *buffer;
    size_t bufSize;
    size_t strAmount;
};

void TextFromFile(struct Text *text, const char* input);
void CountStrAmount(struct Text*);
void FillIdxArr(struct Text*);
void WriteStrings(const struct Text*, FILE* output);
void DestroyText(struct Text*);
#endif
