#include <assert.h>
#include <string.h>
#include <sys\stat.h>

#include "Text.h"
#include "Utilities.h"
#include "Onegin.h"

void ReadTextFromFile(struct Text *text, const char* inputFile) {
    assert(text != nullptr);
    assert(inputFile != nullptr);

    int input = open(inputFile, O_RDONLY, 0);
    assert(input != -1);

    struct stat inputStat;
    fstat(input, &inputStat);

    text->bufSize = inputStat.st_size;
    text->buffer = (uint8_t*)calloc(text->bufSize + 1, sizeof(text->buffer[0]));
    assert(text->buffer != nullptr);

    read(input, text->buffer, text->bufSize);

    close(input);
}

void CountStrAmount(struct Text *text) {
    assert(text != nullptr);

    size_t strCount = 0;

    for (size_t curChr = 0; curChr < (text->bufSize); curChr++) {
        if (text->buffer[curChr] == '\n') {
            strCount++;
        }

        if (text->buffer[curChr] == '\r') {
            text->buffer[curChr] = '\0';
        }
    }

    text->strAmount = strCount;
}

void FillStrings(struct Text *text) {
    assert(text != nullptr);

    text->strings[0].value = &text->buffer[1];
    for (size_t curStrBuf = 1, curStrIdx = 1; curStrBuf < text->bufSize; curStrBuf++) {
        if (text->buffer[curStrBuf - 1] == '\n') {
            text->strings[curStrIdx].value = &text->buffer[curStrBuf];
            text->strings[curStrIdx - 1].length = text->strings[curStrIdx].value - text->strings[curStrIdx - 1].value - 1;

            curStrIdx++;
        }
    }
    text->strings[text->strAmount - 1].length = &text->buffer[text->bufSize] - text->strings[text->strAmount - 1].value - 1;
}

void PrintStrings(const struct Text *text, FILE* output) {
    assert(text != nullptr);
    assert(output != nullptr);

    for (size_t curStr = 0; curStr < text->strAmount; curStr++) {
        MyFPuts(text->strings[curStr].value, output);
    }
    fprintf(output, "%s\n", END_OF_PART);
}

void DestroyText(struct Text *text) {
    assert(text != nullptr);

    free(text->buffer);
    text->buffer = nullptr;

    free(text->strings);
    text->strings = nullptr;
}

