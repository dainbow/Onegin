#ifndef _UTILITIES_H_
#define _UTILITIES_H_

#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <string.h>

const uint8_t UPPERCASE_A = 'À';
const uint8_t LOWERCASE_YA = 'ÿ';
bool IsCyrillic(const uint16_t sym);

void CommandLineArgs(int argc, char *argv[]);
int MyFPuts(const uint8_t *str, FILE *stream);
#endif
