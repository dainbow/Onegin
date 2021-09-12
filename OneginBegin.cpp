#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include <fcntl.h>
#include <windows.h>

const char END_OF_PART[] = "------------------------------------------------------\n";
const char INPUT_FILE[] = "Onegin.txt";
const char OUTPUT_FILE[] = "output.txt";
const char HELP_FILE[] = "help.txt";

const unsigned char BIG_A = 'À';
const unsigned char LIL_YA = 'ÿ';

bool IsCyrillic(const unsigned int sym);
void SwapPtr(void **str1, void **str2);

void MyQsort(unsigned char **arr, int memory, long int size, int (*comparator) (const void*, const void*));
void PrintHelp(int argc, char *argv[]);

int LetterCmp(const unsigned char *str1, const unsigned char *str2);
int InversedLetterCmp(const unsigned char *str1, const unsigned char *str2);

int MyPuts(const unsigned char *str);
int MyFPuts(const unsigned char *str, FILE *stream);
size_t MyStrLen(const unsigned char *str);

int main(int argc, char *argv[]) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    PrintHelp(argc, argv);

    int input = open(INPUT_FILE, O_RDONLY, 0);
    assert(input != -1);

    long fileSize = _filelength(input);
    unsigned char *buf = (unsigned char*)calloc(fileSize, sizeof(buf[0]));
    read(input, buf, fileSize);

    size_t strCount = 0;

    for (int curCh = 1; curCh < fileSize; curCh++) {
        if (buf[curCh] == '\n') {
            strCount++;
        }
    }
    printf("%d %ld\n", strCount, fileSize);

    unsigned char **idxArr = (unsigned char**)calloc(strCount, sizeof(idxArr[0]));

    for (int curStrBuf = 0, curStrIdx = 0; curStrBuf < fileSize; curStrBuf++) {
        if (curStrBuf == 0 || buf[curStrBuf-1] == '\n') {
            idxArr[curStrIdx] = &buf[curStrBuf];
            curStrIdx++;
        }
    }

    MyQsort(idxArr, 4, strCount, (int (*) (const void*, const void*))LetterCmp);

    FILE *output = fopen(OUTPUT_FILE, "w");

    for (size_t curStr = 0; curStr < strCount; curStr++) {
        MyFPuts(idxArr[curStr], output);
    }
    fprintf(output, "%s\n", END_OF_PART);


    MyQsort(idxArr, 4, strCount, (int (*) (const void*, const void*))InversedLetterCmp);

    for (size_t curStr = 0; curStr < strCount; curStr++) {
        MyFPuts(idxArr[curStr], output);
    }
    fprintf(output, "%s\n", END_OF_PART);

    fputs((const char*)buf, output);

    free(buf);
    free(idxArr);
    close(input);
}

size_t MyStrLen(const unsigned char *str) {
    assert(str != nullptr);

    size_t len = 0;

    while (str[len++] != '\n' && str[len] != '\0' && str[len] != EOF);

    return len - 1;
}

int MyFPuts(const unsigned char *str, FILE *stream) {
    assert(str != nullptr);
    assert(stream != nullptr);

    int i = 0;
    while (str[i] != '\0' && str[i] != '\n' && str[i] != EOF) {
        fputc(str[i], stream);
        i++;
    }
    fputc('\n', stream);

    if (str[i] == '\0' || str[i] == '\n') {
        return 1;
    }

    return EOF;
}

int MyPuts(const unsigned char *str) {
    assert(str != nullptr);

    int i = 0;
    while (str[i] != '\0' && str[i] != '\n' && str[i] != EOF) {
        printf("%c", str[i++]);
    }
    printf("\n");

    if (str[i] == '\0' || str[i] == '\n') {
        return 1;
    }

    return EOF;
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
//! Checks if symbol is cyrillic
//!
//! @param [in] sym Symbol which will be checked
//!
//! @return True if cyrillic, false in other cases
//-------------------------------------------------------------------------------------------------------

bool IsCyrillic(const unsigned int sym) {
    assert(isfinite(sym));

    return (sym >= BIG_A && sym <= LIL_YA);
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

int InversedLetterCmp(const unsigned char *str1, const unsigned char *str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    if(str1 == str2){
        return 0;
    }

    int idx1 = MyStrLen(str1) - 1, idx2 = MyStrLen(str2) - 1;

	while (idx1 > -1 && idx2 > -1) {
		while (!(IsCyrillic(str1[idx1]) || isalpha(str1[idx1])) && idx1 > -1) {
			idx1--;
		}
		while (!(IsCyrillic(str2[idx2]) || isalpha(str2[idx2])) && idx2 > -1) {
			idx2--;
		}

		if (str1[idx1] != str2[idx2] && idx1 > -1 && idx2 > -1) {
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

int LetterCmp(const unsigned char *str1, const unsigned char *str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    if (str1 == str2) {
        return 0;
    }

    int idx1 = 0, idx2 = 0;
    int len1 = MyStrLen(str1), len2 = MyStrLen(str2);

	while (idx1 < len1 && idx2 < len2) {
		while (!(IsCyrillic(str1[idx1]) || isalpha(str1[idx1])) && idx1 < len1) {
			idx1++;
		}
		while (!(IsCyrillic(str2[idx2]) || isalpha(str2[idx2])) && idx2 < len2) {
			idx2++;
		}

		if (str1[idx1] != str2[idx2] && idx1 < len1 && idx2 < len2) {
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

    if (ptr1 != ptr2) {
        void *temp = *ptr1;
        *ptr1 = *ptr2;
        *ptr2 = temp;
    }
}

//-------------------------------------------------------------------------------------------------------
//! Sorts originalay of strings in lexicographic order.
//!
//! @param [in] original pointer to the pointer of originalay which is being sorted
//! @param [in] FuncComp pointer to the function which will sort original
//!
//! @note Complexity of algorithm is O(n^2)
//-------------------------------------------------------------------------------------------------------

void MyQsort (unsigned char **arr, int memory, long int size, int (*comparator) (const void*, const void*)) {
    assert(arr != nullptr);
    assert(comparator!= nullptr);

    assert(isfinite(memory));
    assert(isfinite(size));

    long int left = 0;
    long int right = size - 1;
    long int mid = size / 2;

    do {
        while(comparator(arr[left], arr[mid]) < 0) {
            left++;
        }
        while(comparator(arr[right], arr[mid]) > 0) {
            right--;
        }

        if (left <= right) {
            SwapPtr((void**)(arr + left), (void**)(arr + right));

            left++;
            right--;
        }
    } while (left <= right);

    if (right > 0) {
        MyQsort(arr, 4, right + 1, comparator);
    }

    if (left < size) {
        MyQsort(&arr[left], 4, size - left, comparator);
    }
}
