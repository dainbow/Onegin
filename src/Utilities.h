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

//-------------------------------------------------------------------------------------------------------
//! Checks if symbol is cyrillic
//!
//! @param [in] sym Symbol which will be checked
//!
//! @return True if cyrillic, false in other cases
//-------------------------------------------------------------------------------------------------------

bool IsCyrillic(const uint16_t sym);

//-------------------------------------------------------------------------------------------------------
//! Writes string which ends on '\0' or '\n' in stream
//!
//! @param [in] str Pointer to the beginning of the string
//! @param [in] stream Pointer to the stream
//!
//! @return Returns 1 in case of success or EOF
//-------------------------------------------------------------------------------------------------------

int MyFPuts(const uint8_t *str, FILE *stream);
#endif
