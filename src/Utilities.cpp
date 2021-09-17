#include "Utilities.h"
#include "Onegin.h"

bool IsCyrillic(const uint16_t sym) {
    assert(isfinite(sym));

    return (sym >= UPPERCASE_A && sym <= LOWERCASE_YA);
}

int MyFPuts(const uint8_t *str, FILE *stream) {
    assert(str != nullptr);
    assert(stream != nullptr);

    for (int i = 0; str[i] != '\0' && str[i] != '\n'; i++) {
        fputc(str[i], stream);
    }
    fputc('\n', stream);

    return 1;
}

size_t CountFileSize (int fd) {
	assert(fd != -1);
	
	struct stat fileStat = {};
    fstat(fd, &fileStat);
	
	return fileStat.st_size;
}