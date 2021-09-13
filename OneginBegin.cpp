#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <stdint.h>

const char END_OF_PART[] = "------------------------------------------------------\n";
const char INPUT_FILE[] = "Onegin.txt";
const char OUTPUT_FILE[] = "output.txt";
const char HELP_FILE[] = "help.txt";

typedef unsigned char uchar;

const uchar UPPERCASE_A = 'À';
const uchar LOWERCASE_YA = 'ÿ';

bool IsCyrillic(const uint16_t sym);
void SwapPtr(void *str1, void *str2, size_t size);

void CountStr(struct text*);
void FillIdxArr(struct text*);

void MyQsort(void *arr, size_t number, size_t size, int (*comparator) (const void*, const void*));

void PrintHelp(int argc, char *argv[]);
void WriteIdxArr(struct text*, FILE* output);
int MyFPuts(const uchar *str, FILE *stream);

int LetterStrCmp(struct strAndLen *str1, struct strAndLen *str2);
int InversedLetterStrCmp(struct strAndLen *str1, struct strAndLen *str2);

struct strAndLen {
    uchar *ptr;
    size_t ptrLen;
};

struct text {
    struct strAndLen *idxArr;
    uchar *buffer;
    size_t bufSize;
    size_t strAmount;
};

int main(int argc, char *argv[]) {
    PrintHelp(argc, argv);

    int input = open(INPUT_FILE, O_RDONLY, 0);
    assert(input != -1);

    struct stat inputStat;
    fstat(input, &inputStat);

    struct text onegin;
    onegin.bufSize = inputStat.st_size;

    onegin.buffer = (uchar*)calloc(onegin.bufSize + 1, sizeof(onegin.buffer[0]));
    assert(onegin.buffer != nullptr);

    read(input, onegin.buffer, onegin.bufSize);
    CountStr(&onegin);

    onegin.idxArr = (struct strAndLen*)calloc(onegin.strAmount, sizeof(onegin.idxArr[0]));
    assert(onegin.idxArr != nullptr);

    FillIdxArr(&onegin);
    qsort(onegin.idxArr, onegin.strAmount, sizeof(onegin.idxArr[0]), (int (*) (const void*, const void*))LetterStrCmp);

    FILE *output = fopen(OUTPUT_FILE, "w");
    WriteIdxArr(&onegin, output);

    MyQsort(onegin.idxArr, onegin.strAmount, sizeof(onegin.idxArr[0]), (int (*) (const void*, const void*))InversedLetterStrCmp);

    WriteIdxArr(&onegin, output);
    fputs((const char*)onegin.buffer, output);

    close(input);
    fclose(output);

    free(onegin.buffer);
    free(onegin.idxArr);
}

//-------------------------------------------------------------------------------------------------------
//! Checks if symbol is cyrillic
//!
//! @param [in] sym Symbol which will be checked
//!
//! @return True if cyrillic, false in other cases
//-------------------------------------------------------------------------------------------------------

bool IsCyrillic(const uint16_t sym) {
    assert(isfinite(sym));

    return (sym >= UPPERCASE_A && sym <= LOWERCASE_YA);
}

//-------------------------------------------------------------------------------------------------------
//! Swaps two pointers
//!
//! @param [in] ptr1 Pointer to the first pointer
//! @param [in] ptr2 Pointer to the second pointer
//! @param [in] size Size in bytes of swapping variables
//!
//! @note You can swap strings by this function
//-------------------------------------------------------------------------------------------------------

void SwapPtr(void *ptr1, void *ptr2, size_t size) {
    assert(ptr1 != nullptr);
    assert(ptr2 != nullptr);

    if (ptr1 != ptr2) {
        uint64_t *firstBig = (uint64_t *)ptr1, *secondBig = (uint64_t *)ptr2, tmpBig;
        size_t curByte = 0;

        for (; size / sizeof(uint64_t); size -= sizeof(uint64_t), curByte++) {
            tmpBig = firstBig[curByte];
            firstBig[curByte] = secondBig[curByte];
            secondBig[curByte] = tmpBig;
        }

        unsigned char *firstLittle = (unsigned char *)((char*)ptr1 + curByte * sizeof(uint64_t)), *secondLittle = (unsigned char *)((char*)ptr2 + curByte * sizeof(uint64_t)), tmpLittle;
        for (; size; size--, curByte++) {
            tmpLittle = firstLittle[curByte];
            firstLittle[curByte] = secondLittle[curByte];
            secondLittle[curByte] = tmpLittle;
        }
    }
}

//-------------------------------------------------------------------------------------------------------
//! Counts strings in structure's buffer and set structure's strAmount.
//!
//! @param [in] text Pointer to the text structure
//-------------------------------------------------------------------------------------------------------

void CountStr(struct text *text) {
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

void FillIdxArr(struct text *text) {
    text->idxArr[0].ptr = &text->buffer[0];

    for (size_t curStrBuf = 0, curStrIdx = 1; curStrBuf < text->bufSize; curStrBuf++) {
        if (text->buffer[curStrBuf - 1] == '\n') {
            text->idxArr[curStrIdx].ptr = &text->buffer[curStrBuf];
            text->idxArr[curStrIdx - 1].ptrLen = text->idxArr[curStrIdx].ptr - text->idxArr[curStrIdx - 1].ptr - 1;
            curStrIdx++;
        }
    }
    text->idxArr[text->strAmount - 1].ptrLen = &text->buffer[text->bufSize] - text->idxArr[text->strAmount - 1].ptr - 1;
}

//-------------------------------------------------------------------------------------------------------
//! Sorts array using quicksort algorithm
//!
//! @param [in] arr pointer to the array which is being sorted
//! @param [in] memory size of each element of arr in bytes
//! @param [in] size amount of array's elements
//! @param [in] comparator Pointer to the function which will compare elements
//!
//! @note Average complexity of algorithm is O(n*(log n))
//-------------------------------------------------------------------------------------------------------

void MyQsort (void *arr, size_t number, size_t size, int (*comparator) (const void*, const void*)) {
    assert(arr != nullptr);
    assert(comparator!= nullptr);

    assert(isfinite(number));
    assert(isfinite(size));

    long int left = 0;
    long int right = number - 1;
    char* mid =*((char**)((char*)arr + (number / 2) * size));

    do {
        while(comparator((char*)arr + left * size, &mid) < 0) {
            left++;
        }
        while(comparator((char*)arr + right * size, &mid) > 0) {
            right--;
        }

        if (left <= right) {
            SwapPtr((char*)arr + left * size, (char*)arr + right * size, size);

            left++;
            right--;
        }
    } while (left <= right);

    if (right > 0) {
        MyQsort(arr, right + 1, size, comparator);
    }

    if (left < (long)number) {
        MyQsort((char*)arr + left * size, number - left, size, comparator);
    }
}

//-------------------------------------------------------------------------------------------------------
//! Prints short manual for program
//!
//! @param [in] argc amount of command line arguments
//! @param [in] argv array of command line arguments
//!
//! @note Program always has 1 argument - its location
//!       If program doesn't find help.txt it will print REALLY short manual
//-------------------------------------------------------------------------------------------------------

void PrintHelp(int argc, char *argv[]) {
    assert(argv != nullptr);

    for (int argIdx = 1; argIdx < argc; argIdx++) {
        if (strcmp(argv[argIdx], "/?") == 0) {
            FILE* help = fopen(HELP_FILE, "r");

            if (help == nullptr) {
                printf("%s not found. REALLY short description: this program writes in output.txt\n1. Original \"Eugene Onegin\"\n2. Sorted strings of it from the beginning\n3. Sorted strings of it from the end", HELP_FILE);
            }
            else {
                int chr = 0;

                while ((chr = fgetc(help)) != EOF) {
                    fputc(chr, stdout);
                }
                fclose(help);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------
//! Writes each string of the structure text idxArr
//!
//! @param [in] text Pointer to the text structure
//! @param [in] output Pointer to the output stream
//-------------------------------------------------------------------------------------------------------

void WriteIdxArr(struct text *text, FILE* output) {
    for (size_t curStr = 0; curStr < text->strAmount; curStr++) {
        MyFPuts(text->idxArr[curStr].ptr, output);
    }
    fprintf(output, "%s\n", END_OF_PART);
}

//-------------------------------------------------------------------------------------------------------
//! Compares letters of Russian and English alphabet of two strAndLen structures from the beginning
//!
//! @param [in] str1 Pointer to the first sAL structure
//! @param [in] str2 Pointer to the second sAl structure
//!
//! @return Returns 0 if strings are equal, otherwise returns distance between
//!         first different characters (can be >0 or <0)
//!
//! @note Keep in mind that this function skips all non-letter symbols
//-------------------------------------------------------------------------------------------------------

int LetterStrCmp(struct strAndLen* str1, struct strAndLen* str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    if (str1->ptr == str2->ptr) {
        return 0;
    }

    int idx1 = 0, idx2 = 0;
    int len1 = str1->ptrLen, len2 = str2->ptrLen;

	while (idx1 < len1 && idx2 < len2) {
		while (!(IsCyrillic(str1->ptr[idx1]) || isalpha(str1->ptr[idx1])) && idx1 < len1) {
			idx1++;
		}
		while (!(IsCyrillic(str2->ptr[idx2]) || isalpha(str2->ptr[idx2])) && idx2 < len2) {
			idx2++;
		}

		if (str1->ptr[idx1] != str2->ptr[idx2] && idx1 < len1 && idx2 < len2) {
			return str1->ptr[idx1] - str2->ptr[idx2];
		}
		idx1++;
		idx2++;
	}

    return 0;
}

//-------------------------------------------------------------------------------------------------------
//! Compares letters of Russian and English alphabet of two strAndLen structures from the end
//!
//! @param [in] str1 Pointer to the first sAL structure
//! @param [in] str2 Pointer to the second sAl structure
//!
//! @return Returns 0 if strings are equal, otherwise returns distance between
//!         first different characters (can be >0 or <0)
//!
//! @note Keep in mind that this function skips all non-letter symbols
//-------------------------------------------------------------------------------------------------------

int InversedLetterStrCmp(struct strAndLen *str1, struct strAndLen *str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    if(str1->ptr == str2->ptr){
        return 0;
    }

    int idx1 = str1->ptrLen - 1, idx2 = str2->ptrLen - 1;

	while (idx1 > -1 && idx2 > -1) {
		while (!(IsCyrillic(str1->ptr[idx1]) || isalpha(str1->ptr[idx1])) && idx1 > -1) {
			idx1--;
		}
		while (!(IsCyrillic(str2->ptr[idx2]) || isalpha(str2->ptr[idx2])) && idx2 > -1) {
			idx2--;
		}

		if (str1->ptr[idx1] != str2->ptr[idx2] && idx1 > -1 && idx2 > -1) {
			return str1->ptr[idx1] - str2->ptr[idx2];
		}
		idx1--;
		idx2--;
	}

    return 0;
}

//-------------------------------------------------------------------------------------------------------
//! Writes string which ends on '\0' or '\n' in stream
//!
//! @param [in] str Pointer to the beginning of the string
//! @param [in] stream Pointer to the stream
//!
//! @return Returns 1 in case of success or EOF
//-------------------------------------------------------------------------------------------------------

int MyFPuts(const uchar *str, FILE *stream) {
    assert(str != nullptr);
    assert(stream != nullptr);

    for (int i = 0; str[i] != '\0' && str[i] != '\n' && str[i] != EOF; i++) {
        fputc(str[i], stream);
    }
    fputc('\n', stream);

    return 1;
}
