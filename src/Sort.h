#ifndef _SORT_H_
#define _SORT_H_

#include <string.h>
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <ctype.h>

void SwapMem(char *mem1, char *mem2, size_t size);

int LetterStrCmp(const struct String *str1, const struct String *str2);
int InversedLetterStrCmp(const struct String *str1, const struct String *str2);

void MyQsort(void *arr, size_t number, size_t size, int (*comparator) (const void*, const void*));
#endif
