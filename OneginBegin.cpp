#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

const size_t STR_LEN = 200;
const size_t STR_AMOUNT = 400;

const char END_OF_PART[] = "------------------END OF PART------------------\n";
const char INPUT_FILE[] = "Onegin.txt";
const char OUTPUT_FILE[] = "output.txt";

size_t buffLen = STR_LEN;
size_t buffAmount = STR_AMOUNT;

bool IsCyrillic(const char sym);
int LetterCmp(const char *str1, const char *str2);
void SwapPtr(void **str1, void **str2);
void GnomeSort(void **original, int (*FuncComp)(const void*, const void*));
void PrintHelp(int argc, char *argv[]);
int InversedLetterCmp(const char *str1, const char *str2);
int GetLine(char **lineptr, size_t *n, FILE *stream);

int main(int argc, char *argv[]) {
    PrintHelp(argc, argv);

    FILE *input = fopen(INPUT_FILE, "r");
    assert(input != nullptr);

    FILE *output = fopen(OUTPUT_FILE, "w");
    assert(output != nullptr);

    char **original = (char**)calloc(buffAmount, sizeof(char*));
    assert(original != nullptr);

    size_t curStr = 0;

    while((original[curStr] = (char*)calloc(buffLen, sizeof(char))) != nullptr) {
        if (GetLine(&original[curStr], &buffLen, input) == -1) {
            break;
        }

        fputs(original[curStr], output);

        if (++curStr == buffAmount) {
            char **buffOriginal = original;
            buffAmount += STR_LEN;

            original = (char**)calloc(buffAmount, sizeof(char*));
            assert(original != nullptr);

            for(size_t bufStr = 0; bufStr < curStr; bufStr++) {
                original[bufStr] = buffOriginal[bufStr];
                free(buffOriginal[bufStr] = nullptr);
            }
            free(buffOriginal);
        }
    }
    buffAmount = curStr + 1;
    fputs(END_OF_PART, output);

    GnomeSort((void **)original, (int (*) (const void*, const void*))LetterCmp);

    curStr = 0;
    while(fputs(original[curStr++], output) != EOF);
    fputs(END_OF_PART, output);

    GnomeSort((void **)original, (int (*) (const void*, const void*))InversedLetterCmp);

    curStr = 0;
    while(fputs(original[curStr], output) != EOF) {
        free(original[curStr++] = nullptr);
    }
    fputs(END_OF_PART, output);

    free(original = nullptr);

    fclose(input = nullptr);
    fclose(output = nullptr);
}

//-------------------------------------------------------------------------------------------------------
//! Writes current string of stream file in string variable
//!
//! @param [out] linePtr Pointer to the pointer where string will be written
//! @param [out] memSize Pointer to the variable where beginning value of strlen is stored
//! @param [in] stream Pointer to the stream where from string will be read
//!
//! @return Returns amount of written symbols or -1 if EOF
//!
//! @note Function uses dynamic memory and can increase it.
//-------------------------------------------------------------------------------------------------------

int GetLine(char **linePtr, size_t *memSize, FILE *stream) {
    assert(linePtr != nullptr);
    assert(memSize != nullptr);
    assert(stream != nullptr);

    int curChar = 0;

    if ((curChar = fgetc(stream)) == EOF) {
        return -1;
    }

    char *buffPtr = *linePtr;
    char *ptr = buffPtr;

    size_t buffMemSize = *memSize;
    size_t diffPtr = 0;

    while(curChar != EOF) {
        if ((diffPtr = ptr - buffPtr) > (buffMemSize - 1)) {
            buffMemSize += STR_LEN;

            buffPtr = (char*)calloc(buffMemSize, sizeof(char));
            assert(buffPtr != nullptr);

            ptr = buffPtr + diffPtr;

            if (buffPtr == nullptr) {
                return -1;
            }
        }
        *ptr++ = (char)curChar;

        if (curChar == '\n') {
            break;
        }
        curChar = fgetc(stream);
    }

    *ptr++ = '\0';
    *linePtr = buffPtr;
    *memSize = buffMemSize;

    return ptr - buffPtr - 1;
}

//-------------------------------------------------------------------------------------------------------
//! Prints short manual for program
//!
//! @param [in] argc amount of program arguments
//! @param [in] argv array of program arguments
//!
//! @note Program always have 1 argument - its location
//!       If program doesn't find help.txt it will print REALLY short manual
//-------------------------------------------------------------------------------------------------------

void PrintHelp(int argc, char *argv[]) {
    assert(isfinite(argc));
    assert(argv != nullptr);

    for (int argIdx = 1; argIdx < argc; argIdx++) {
        if (strcmp(argv[argIdx], "/?") == 0) {
            FILE *help = fopen("help.txt", "r");

            if (help == nullptr) {
                printf("help.txt not found. REALLY short description: this program writes in output.txt\n1. Original \"Eugene Onegin\"\n2. Sorted strings of it from the beginning\n3. Sorted strings of it from the end");
            }
            else {
                char strOutput[STR_LEN] = "";

                while(fgets(strOutput, STR_LEN, help) != 0) {
                    puts(strOutput);
                }

                fclose(help);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------
//! Checks if symbol is cyrillic
//!
//! @param [in] sym Symbol which will be checked
//!
//! @return True if cyrillic, false in other cases
//-------------------------------------------------------------------------------------------------------

bool IsCyrillic(const char sym) {
    assert(isfinite(sym));

    return (sym >= 'À' && sym <= 'ÿ');
}

//-------------------------------------------------------------------------------------------------------
//! Compares letters of Russian and English alphabet of str1 and str2 from the end of each string
//!
//! @param [in] str1 First string which will be compared
//! @param [in] str2 Second string which will be compared
//!
//! @return Returns 0 if strings are equal, otherwise returns distance between
//!         first different characters (can be >0 or <0)
//!
//! @note Keep in mind that this function skips all non-letter symbols/
//-------------------------------------------------------------------------------------------------------

int InversedLetterCmp(const char *str1, const char *str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    assert(str1 != str2);

    int idx1 = strlen(str1) - 1, idx2 = strlen(str2) - 1;

	while (idx1 != -1 && idx2 != -1) {
		while (!(IsCyrillic(str1[idx1]) || isalpha(str1[idx1]))) {
			idx1--;
		}
		while (!(IsCyrillic(str2[idx2]) || isalpha(str2[idx2]))) {
			idx2--;
		}

		if (str1[idx1] != str2[idx2]) {
			return str1[idx1] - str2[idx2];
		}
		idx1--;
		idx2--;
	}

    return 0;
}

//-------------------------------------------------------------------------------------------------------
//! Compares letters of Russian and English alphabet of str1 and str2 from the beginning of each string
//!
//! @param [in] str1 First string which will be compared
//! @param [in] str2 Second string which will be compared
//!
//! @return Returns 0 if strings are equal, otherwise returns distance between
//!         first different characters (can be >0 or <0)
//!
//! @note Keep in mind that this function skips all non-letter symbols/
//-------------------------------------------------------------------------------------------------------

int LetterCmp(const char *str1, const char *str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    assert(str1 != str2);

    int idx1 = 0, idx2 = 0;

	while (str1[idx1] != '\0' && str2[idx2]  != '\0') {
		while (!(IsCyrillic(str1[idx1]) || isalpha(str1[idx1]))) {
			idx1++;
		}
		while (!(IsCyrillic(str2[idx2]) || isalpha(str2[idx2]))) {
			idx2++;
		}
		if (str1[idx1] != str2[idx2]) {
			return str1[idx1] - str2[idx2];
		}
		idx1++;
		idx2++;
	}

    return 0;
}

//-------------------------------------------------------------------------------------------------------
//! Swaps two pointers
//!
//! @param [in] ptr1 Pointer to the first pointer
//! @param [in] ptr2 Pointer to the second pointer
//!
//! @note You can swap strings by this function
//-------------------------------------------------------------------------------------------------------

void SwapPtr(void **ptr1, void **ptr2) {
    assert(ptr1 != nullptr);
    assert(ptr2 != nullptr);

    assert(ptr1 != ptr2);

    void *temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

//-------------------------------------------------------------------------------------------------------
//! Sorts originalay of strings in lexicographic order.
//!
//! @param [in] original pointer to the pointer of originalay which is being sorted
//! @param [in] FuncComp pointer to the function which will sort original
//!
//! @note Complexity of algorithm is O(n^2)
//-------------------------------------------------------------------------------------------------------

void GnomeSort(void **original, int (*FuncComp)(const void*, const void*)) {
    assert(original != nullptr);
	assert(FuncComp != nullptr);

    size_t curStr = 0;

    while (curStr < buffAmount) {
        if (curStr == 0 || (*FuncComp)(original[curStr], original[curStr - 1]) >= 0) {
            curStr++;
        }
        else {
            SwapPtr(original + curStr, original + curStr - 1);
            curStr--;
        }
    }
}
