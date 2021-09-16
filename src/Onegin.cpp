#include <assert.h>

#include "Sort.h"
#include "StructWorks.h"
#include "Utilities.h"
#include "Onegin.h"

int main(int argc, char *argv[]) {
    CommandLineArgs(argc, argv);

    struct Text onegin;
	TextFromFile(&onegin, INPUT_FILE);
	
    CountStrAmount(&onegin);

    onegin.strings = (struct String*)calloc(onegin.strAmount, sizeof(onegin.strings[0]));
    assert(onegin.strings != nullptr);

    FillIdxArr(&onegin);
    MyQsort(onegin.strings, onegin.strAmount, sizeof(onegin.strings[0]), (int (*) (const void*, const void*))LetterStrCmp);

    FILE *output = fopen(OUTPUT_FILE, "w");
    assert(output != nullptr);
    WriteIdxArr(&onegin, output);

    MyQsort(onegin.strings, onegin.strAmount, sizeof(onegin.strings[0]), (int (*) (const void*, const void*))InversedLetterStrCmp);

    WriteIdxArr(&onegin, output);
    fputs((const char*)onegin.buffer, output);

    fclose(output);
    DestroyText(&onegin);
}
