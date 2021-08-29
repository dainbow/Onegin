#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <locale.h>
#include <ctype.h>
#include <math.h>

const int STR_LEN = 200;
const int STR_AMOUNT = 4000;

size_t StrLen(const char *str) {
    assert(str != nullptr);

    size_t len = 0;

    while (str[len++] != '\0') {
		;
	}

    return len - 1;
}

void StrReverse(char **str) {
	assert(str != nullptr);

    int strLen = StrLen(*str);
	char *temp = *str;

	*str = (char*)calloc(STR_LEN, sizeof(char));

	for (int iter = 0; iter < strLen; iter++) {
		*(*str + iter) = *(temp + strLen - 1 - iter);
	}
}

bool IsAlphaRus(const char sym) {
    assert(isfinite(sym));

    return (sym >= 'А' && sym <= 'я') ? 1 : 0;
}

int StrCmp(const char *str1, const char *str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    assert(str1 != str2);

    int ind1 = 0, ind2 = 0;

	while (str1[ind1] != '\0' && str2[ind2]  != '\0') {
		while (!(IsAlphaRus(str1[ind1]) || isalpha(str1[ind1]))) {
			ind1++;
		}
		while (!(IsAlphaRus(str2[ind2]) || isalpha(str2[ind2]))) {
			ind2++;
		}
		if (str1[ind1] != str2[ind2]) {
			return str1[ind1] - str2[ind2];
		}
		ind1++;
		ind2++;
	}
    return 0;
}

void SwapStr(char **str1, char **str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    assert(str1 != str2);

    char *temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}

//--------------------------------------
//! Sorts array of strings in lexicographic order.
//!
//! @param [in] arr pointer to the pointer of array which is being sorted
//!
//! @note Complexity of algorithm is O(n^2)
//--------------------------------------

void GnomeSort(char **arr) {
    assert(arr != nullptr);

    int curStr = 0;

    while (curStr < STR_AMOUNT) {
        if (curStr == 0 || StrCmp(*(arr + curStr), *(arr + curStr - 1)) >= 0) {
            curStr++;
        }
        else {
            SwapStr(arr + curStr, arr + curStr - 1);
            curStr--;
        }
    }
}

int main() {
	FILE *input = fopen("Onegin.txt", "r");
	FILE *output = fopen("OneginOutput.txt", "w");

	assert(input != nullptr);
	assert(output != nullptr);

    char **arr = (char**)calloc(STR_AMOUNT, sizeof(char*));
    assert(arr != nullptr);

    for (int curStr = 0; curStr < STR_AMOUNT; curStr++) {
        arr[curStr] = (char*)calloc(STR_LEN, sizeof(char));
        assert(arr[curStr] != nullptr);

        fgets(arr[curStr], STR_LEN, input);
        fputs(arr[curStr], output);
    }

    GnomeSort(arr);

    for (int curStr = 0; curStr < STR_AMOUNT; curStr++) {
        fputs(arr[curStr], output);
        free(arr[curStr]);
    }

    for (int curStr = 0; curStr < STR_AMOUNT; curStr++) {
        arr[curStr] = (char*)calloc(STR_LEN, sizeof(char));
        assert(arr[curStr] != nullptr);

        fgets(arr[curStr], STR_LEN, input);
        StrReverse(&arr[curStr]);
    }

    GnomeSort(arr);

    for (int curStr = 0; curStr < STR_AMOUNT; curStr++) {
        StrReverse(&arr[curStr]);
        fputs(arr[curStr], output);

        free(arr[curStr]);
    }

    free(arr);

    fclose(input);
    fclose(output);

}
