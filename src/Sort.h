#ifndef _SORT_H_
#define _SORT_H_

#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>

//-------------------------------------------------------------------------------------------------------
//! Swaps two memory areas
//!
//! @param [in] mem1 Pointer to the first memory area
//! @param [in] mem2 Pointer to the second memory area
//! @param [in] size Size in bytes of swapping variables
//-------------------------------------------------------------------------------------------------------

void SwapMem(char *mem1, char *mem2, size_t size);

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

int LetterStrCmp(const struct String *str1, const struct String *str2);

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

int InversedLetterStrCmp(const struct String *str1, const struct String *str2);

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

void MyQsort(void *arr, size_t number, size_t size, int (*comparator) (const void*, const void*));
#endif
