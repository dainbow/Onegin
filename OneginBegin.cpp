#include <stdio.h>
#include <stdlib.h>

const int STRLEN = 200;
const int STRAMOUNT = 3470;

int strcmp(char *str1, char *str2) {
    for (int i = 0; i < STRLEN; i++) {
        if (str1[i] != str2[i]) {
            return (int)str1[i] - (int)str2[i];
        }
    }
    return 0;
}

void swapstr(char **str1, char **str2) {
    char *temp = *str1;
    *str1 = *str2;
    *str2 = temp;
}

int main() {
    FILE *f;
    char *arr[STRAMOUNT];
    f = fopen("Onegin.txt","r");

    for (int j = 0; j < STRAMOUNT; j++) {
        arr[j] = (char*)calloc(STRLEN, sizeof(char));
        fgets(arr[j], STRLEN, f);
    }
    fclose(f);

    int i = 0;

    while (i < STRAMOUNT) {
        if (i == 0 || strcmp(arr[i], arr[i - 1]) >= 0) i++;
        else {
            swapstr(&arr[i], &arr[i-1]);
            i--;
        }
    }

    f = fopen("OutputBegin.txt", "w");
    for (int k = 0; k < STRAMOUNT; k++) {
        fputs(arr[k], f);
    }
}
