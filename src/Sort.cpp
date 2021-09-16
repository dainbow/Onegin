#include "StructWorks.h"
#include "Utilities.h"

//-------------------------------------------------------------------------------------------------------
//! Swaps two memory areas
//!
//! @param [in] mem1 Pointer to the first memory area
//! @param [in] mem2 Pointer to the second memory area
//! @param [in] size Size in bytes of swapping variables
//-------------------------------------------------------------------------------------------------------

void SwapMem(char *mem1, char *mem2, size_t size) {
    assert(mem1 != nullptr);
    assert(mem2 != nullptr);
    assert(isfinite(size));

    int sizeChange = 0;

    while(size && mem1 != mem2) {
        if (size / sizeof(uint64_t)) {
            uint64_t tmp = *(uint64_t*) mem1;
            *(uint64_t*) mem1 = *(uint64_t*) mem2;
            *(uint64_t*) mem2 = tmp;

            sizeChange = sizeof(uint64_t);
        }
        else if (size / sizeof(uint32_t)) {
            uint32_t tmp = *(uint32_t*) mem1;
            *(uint32_t*) mem1 = *(uint32_t*) mem2;
            *(uint32_t*) mem2 = tmp;

            sizeChange = sizeof(uint32_t);
        }
        else if (size / sizeof(uint16_t)) {
            uint16_t tmp = *(uint16_t*) mem1;
            *(uint16_t*) mem1 = *(uint16_t*) mem2;
            *(uint16_t*) mem2 = tmp;

            sizeChange = sizeof(uint16_t);
        }
        else {
            uint8_t tmp = *(uint8_t*) mem1;
            *(uint8_t*) mem1 = *(uint8_t*) mem2;
            *(uint8_t*) mem2 = tmp;

            sizeChange = sizeof(uint8_t);
        }

        mem1 += sizeChange;
        mem2 += sizeChange;
        size -= sizeChange;
    }
}

//-------------------------------------------------------------------------------------------------------
//! Compares letters of Russian and English alphabet of two String structures from the beginning
//!
//! @param [in] str1 Pointer to the first String structure
//! @param [in] str2 Pointer to the second String structure
//!
//! @return Returns 0 if strings are equal, otherwise returns distance between
//!         first different characters (can be >0 or <0)
//!
//! @note Keep in mind that this function skips all non-letter symbols
//-------------------------------------------------------------------------------------------------------

int LetterStrCmp(const struct String* str1, const struct String* str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    if (str1->value == str2->value) {
        return 0;
    }

    int idx1 = 0, idx2 = 0;
    int len1 = str1->length, len2 = str2->length;

	while (idx1 < len1 && idx2 < len2) {
		while (!(IsCyrillic(str1->value[idx1]) || isalpha(str1->value[idx1])) && idx1 < len1) {
			idx1++;
		}
		while (!(IsCyrillic(str2->value[idx2]) || isalpha(str2->value[idx2])) && idx2 < len2) {
			idx2++;
		}

		if (str1->value[idx1] != str2->value[idx2] && idx1 < len1 && idx2 < len2) {
			return str1->value[idx1] - str2->value[idx2];
		}
		idx1++;
		idx2++;
	}

    return 0;
}

//-------------------------------------------------------------------------------------------------------
//! Compares letters of Russian and English alphabet of two String structures from the end
//!
//! @param [in] str1 Pointer to the first String structure
//! @param [in] str2 Pointer to the second String structure
//!
//! @return Returns 0 if strings are equal, otherwise returns distance between
//!         first different characters (can be >0 or <0)
//!
//! @note Keep in mind that this function skips all non-letter symbols
//-------------------------------------------------------------------------------------------------------

int InversedLetterStrCmp(const struct String *str1, const struct String *str2) {
    assert(str1 != nullptr);
    assert(str2 != nullptr);

    if(str1->value == str2->value){
        return 0;
    }

    int idx1 = str1->length - 1, idx2 = str2->length - 1;
	
	while (idx1 > -1 && idx2 > -1) {
		while (!(IsCyrillic(str1->value[idx1]) || isalpha(str1->value[idx1])) && idx1 > -1) {
			idx1--;
		}
		while (!(IsCyrillic(str2->value[idx2]) || isalpha(str2->value[idx2])) && idx2 > -1) {
			idx2--;
		}

		if (str1->value[idx1] != str2->value[idx2] && idx1 > -1 && idx2 > -1) {
            return str1->value[idx1] - str2->value[idx2];
		}
		idx1--;
		idx2--;
	}

    return 0;
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
	
	char* mid = (char*)arr + (number / 2) * size;
		
    do {
        while(comparator((char*)arr + left * size, mid) < 0) {
            left++;
        }
        while(comparator((char*)arr + right * size, mid) > 0) {
            right--;
        }

        if (left < right) {
            SwapMem((char*)arr + left * size, (char*)arr + right * size, size);
			
			if (mid == (char*)arr + left * size) {
				mid = (char*)arr + right * size;
			}
			if (mid == (char*)arr + right * size) {
				mid = (char*)arr + left * size;
			}
			
            left++;
            right--;
        }
		else if (left >= right) {
			left++;
            right--;
			break;
		}
    } while (left <= right);

    if (right > 0) {
        MyQsort(arr, right + 1, size, comparator);
    }

    if (left < (long)number) {
        MyQsort((char*)arr + left * size, number - left, size, comparator);
    }
}
