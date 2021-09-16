#include <assert.h>
#include <string.h>
#include <sys\stat.h>
#include <fcntl.h>

#include "StructWorks.h"
#include "Utilities.h"
#include "Onegin.h"

void TextFromFile(struct Text *text, const char* inputFile) {
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

//-------------------------------------------------------------------------------------------------------
//! Counts strings in structure's buffer and set structure's strAmount.
//!
//! @param [in] text Pointer to the text structure
//-------------------------------------------------------------------------------------------------------

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

//-------------------------------------------------------------------------------------------------------
//! Fills idxArr of the text structure with pointers on the beginning of each string and count length
//! of each string.
//!
//! @param [in] text Pointer to the text structure
//!
//! @note You must allocate memory to idxArr (strAndLen structure) before using this function
//-------------------------------------------------------------------------------------------------------

void FillIdxArr(struct Text *text) {
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

//-------------------------------------------------------------------------------------------------------
//! Writes each string of the structure text idxArr
//!
//! @param [in] text Pointer to the text structure
//! @param [in] output Pointer to the output stream
//-------------------------------------------------------------------------------------------------------

void WriteIdxArr(const struct Text *text, FILE* output) {
    assert(text != nullptr);
    assert(output != nullptr);

    for (size_t curStr = 0; curStr < text->strAmount; curStr++) {
        MyFPuts(text->strings[curStr].value, output);
    }
    fprintf(output, "%s\n", END_OF_PART);
}

//-------------------------------------------------------------------------------------------------------
//! Frees all dynamic arrays of the text structure
//!
//! @param [in] text Pointer to the text structure
//-------------------------------------------------------------------------------------------------------

void DestroyText(struct Text *text) {
    assert(text != nullptr);

    free(text->buffer);
    text->buffer = nullptr;

    free(text->strings);
    text->strings = nullptr;
}

