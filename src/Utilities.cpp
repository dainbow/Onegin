#include <stdint.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>

#include "StructWorks.h"
#include "Utilities.h"
#include "Onegin.h"

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
//! Prints short manual for program
//!
//! @param [in] argc amount of command line arguments
//! @param [in] argv array of command line arguments
//!
//! @note Program always has 1 argument - its location
//!       If program doesn't find help.txt it will print REALLY short manual
//-------------------------------------------------------------------------------------------------------

void CommandLineArgs(int argc, char *argv[]) {
    for (int argIdx = 1; argIdx < argc; argIdx++) {
        if (strcmp(argv[argIdx], "/?") == 0) {
            FILE* help = fopen(HELP_FILE, "r");

            if (help == nullptr) {
                printf("%s not found in %s folder. REALLY short description: this program writes in output.txt\n1. Sorted strings of \"Eugene Onegin\" from the beginning\n2. Sorted strings of it from the end\n3. Original \"Eugene Onegin\", I didn't touch Pushkin)", HELP_FILE, argv[0]);
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
//! Writes string which ends on '\0' or '\n' in stream
//!
//! @param [in] str Pointer to the beginning of the string
//! @param [in] stream Pointer to the stream
//!
//! @return Returns 1 in case of success or EOF
//-------------------------------------------------------------------------------------------------------

int MyFPuts(const uint8_t *str, FILE *stream) {
    assert(str != nullptr);
    assert(stream != nullptr);

    for (int i = 0; str[i] != '\0' && str[i] != '\n' && str[i] != EOF; i++) {
        fputc(str[i], stream);
    }
    fputc('\n', stream);

    return 1;
}
