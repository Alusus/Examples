#include <stdio.h>
#include <string.h>

void searchPattern(char *text, char *pattern) {
    int textSize = strlen(text);
    int patternSize = strlen(pattern);

    for (int i = 0; i <= textSize - patternSize; i++) {
        int j;
        for (j = 0; j < patternSize; j++) {
            if (text[i + j] != pattern[j])
                break;
        }

        if (j == patternSize) {
            printf("Pattern found at index %d\n", i);
        }
    }
}

int main() {
    char text[100];
    char pattern[20];

    printf("Enter the text: ");
    fgets(text, sizeof(text), stdin);
    text[strcspn(text, "\n")] = '\0';  // remove newline character

    printf("Enter the pattern: ");
    fgets(pattern, sizeof(pattern), stdin);
    pattern[strcspn(pattern, "\n")] = '\0';  // remove newline character

    searchPattern(text, pattern);

    return 0;
}
