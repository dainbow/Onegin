#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

const int STR_LEN = 200;
const int STR_AMOUNT = 4000;
const char END_OF_PART[] = "------------------END OF PART------------------\n";

size_t StrLen(const char *str);
void StrReverse(char **str);
bool IsCyrillic(const char sym);
int LetterCmp(const char *str1, const char *str2);
void SwapPtr(void **str1, void **str2);
void GnomeSort(void **original, int (*FuncComp)(const void*, const void*));
void PrintHelp(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    PrintHelp(argc, argv);

	FILE *input = fopen("Onegin.txt", "r");
	FILE *output = fopen("output.txt", "w");

	assert(input != nullptr);
	assert(output != nullptr);

    char **original = (char**)calloc(STR_AMOUNT, sizeof(char*));

    assert(original != nullptr);

    for (int curStr = 0; curStr < STR_AMOUNT; curStr++) {
        original[curStr] = (char*)calloc(STR_LEN, sizeof(char));

        assert(original[curStr] != nullptr);

        fgets(original[curStr], STR_LEN, input);
        fputs(original[curStr], output);
    }

    fputs(END_OF_PART, output);

    GnomeSort((void **)original, (int (*) (const void*, const void*))LetterCmp);

    for (int curStr = 0; curStr < STR_AMOUNT; curStr++) {
        fputs(original[curStr], output);

        StrReverse(&original[curStr]);
    }

    fputs(END_OF_PART, output);

    GnomeSort((void **)original, (int (*) (const void*, const void*))LetterCmp);

    for (int curStr = 0; curStr < STR_AMOUNT; curStr++) {
        StrReverse(&original[curStr]);
        fputs(original[curStr], output);

        free(original[curStr]);
        original[curStr] = nullptr;
    }

    fputs(END_OF_PART, output);

    free(original);
    original = nullptr;

    fclose(input);
    fclose(output);
}

//----------------------------------------------------------------------------
//! Prints short manual for program
//!
//! @param [in] argc amount of program arguments
//! @param [in] argv originalay of program arguments
//!
//! @note Program always have 1 argument - its location
//!       If program doesn't find help.txt it will print REALLY short manual
//----------------------------------------------------------------------------

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

//----------------------------------------------------------------------------
//! Counts length of string
//!
//! @param [in] str String which will be counted
//!
//! @return Length of string
//!
//! @note Program returns length of string withount zero symbol.
//----------------------------------------------------------------------------

size_t StrLen(const char *str) {
    assert(str != nullptr);

    size_t len = 0;

    while (str[len++] != '\0') {
		;
	}

    return len - 1;
}

//----------------------------------------------------------------------------
//! Reverses string
//!
//! @param [in] str Pointer to the string which will be reversed
//----------------------------------------------------------------------------

void StrReverse(char **str) {
	assert(str != nullptr);

    int strLen = StrLen(*str);

	char temp[STR_LEN] = "";
	strcpy(temp, *str);

	*str = (char*)calloc(STR_LEN, sizeof(char));

	for (int iter = 0; iter < strLen; iter++) {
		*(*str + iter) = *(temp + strLen - 1 - iter);
	}


}

//----------------------------------------------------------------------------
//! Checks if symbol is cyrillic
//!
//! @param [in] sym Symbol which will be checked
//!
//! @return True if cyrillic, false in other cases
//----------------------------------------------------------------------------

bool IsCyrillic(const char sym) {
    assert(isfinite(sym));

    return (sym >= 'À' && sym <= 'ÿ');
}

//----------------------------------------------------------------------------
//! Compares letters of Russian and English alphabet of str1 and str2
//!
//! @param [in] str1 First string which will be compared
//! @param [in] str2 Second string which will be compared
//!
//! @return Returns 0 if strings are equal, otherwise returns distance between
//!         first different characters (can be >0 or <0)
//!
//! @note Keep in mind that this function skips all non-letter symbols/
//----------------------------------------------------------------------------


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

//----------------------------------------------------------------------------
//! Swaps two pointers
//!
//! @param [in] ptr1 Pointer to the first pointer
//! @param [in] ptr2 Pointer to the second pointer
//!
//! @note You can swap strings by this function
//----------------------------------------------------------------------------

void SwapPtr(void **ptr1, void **ptr2) {
    assert(ptr1 != nullptr);
    assert(ptr2 != nullptr);

    assert(ptr1 != ptr2);

    void *temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

//----------------------------------------------------------------------------
//! Sorts originalay of strings in lexicographic order.
//!
//! @param [in] original pointer to the pointer of originalay which is being sorted
//! @param [in] FuncComp pointer to the function which will sort original
//!
//! @note Complexity of algorithm is O(n^2)
//----------------------------------------------------------------------------

void GnomeSort(void **original, int (*FuncComp)(const void*, const void*)) {
    assert(original != nullptr);
	assert(FuncComp != nullptr);

    int curStr = 0;

    while (curStr < STR_AMOUNT) {
        if (curStr == 0 || (*FuncComp)(*(original + curStr), *(original + curStr - 1)) >= 0) {
            curStr++;
        }
        else {
            SwapPtr(original + curStr, original + curStr - 1);
            curStr--;
        }
    }
}
