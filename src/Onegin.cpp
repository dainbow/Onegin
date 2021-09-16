#include "Sort.h"
#include "Text.h"
#include "Utilities.h"
#include "Onegin.h"

int main(int argc, char *argv[]) {
    ReadCommandLineArgs(argc, argv);

    struct Text onegin;
    ReadTextFromFile(&onegin, INPUT_FILE);

    CountStrAmount(&onegin);

    onegin.strings = (struct String*)calloc(onegin.strAmount, sizeof(onegin.strings[0]));
    assert(onegin.strings != nullptr);

    FillStrings(&onegin);
    MyQsort(onegin.strings, onegin.strAmount, sizeof(onegin.strings[0]), (int (*) (const void*, const void*))LetterStrCmp);

    FILE *output = fopen(OUTPUT_FILE, "w");
    assert(output != nullptr);
    PrintStrings(&onegin, output);

    MyQsort(onegin.strings, onegin.strAmount, sizeof(onegin.strings[0]), (int (*) (const void*, const void*))InversedLetterStrCmp);

    PrintStrings(&onegin, output);
    fputs((const char*)onegin.buffer, output);

    fclose(output);
    DestroyText(&onegin);
}

void ReadCommandLineArgs(int argc, char *argv[]) {
    for (int argIdx = 1; argIdx < argc; argIdx++) {
        if (strcmp(argv[argIdx], "/?") == 0) {
			PrintHelp();
        }
    }
}

void PrintHelp() {
	int help = open(HELP_FILE, O_RDONLY | O_BINARY, 0);

	if (help == -1) {
		printf("%s not found. REALLY short description: this program writes in output.txt\n1. Sorted strings of \"Eugene Onegin\" from the beginning\n2. Sorted strings of it from the end\n3. Original \"Eugene Onegin\", I didn't touch Pushkin)", HELP_FILE);
	}
	else {
		struct stat helpStat;
		fstat(help, &helpStat);
		
		char bufHelp[helpStat.st_size] = "";
		read(help, &bufHelp, helpStat.st_size - 3);
		
		printf("%s\n", bufHelp);
		
		close(help);
	}
}
