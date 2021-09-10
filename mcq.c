#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ASSERT(condition, ...)                          \
    if (!(condition)) {                                 \
        fprintf(stderr, "\033[0;31merror:\033[0m ");    \
        fprintf(stderr, __VA_ARGS__);                   \
        fprintf(stderr, "\n");                          \
        exit(1);                                        \
    }

#define USAGE "\033[32musage:\033[0m mcq FILE"

#define FmtNormal "\033[0m"
#define FmtPrompt "\033[32m"

#define FmtChoice "\033[33m"
#define FmtQuestion "\033[34m"
#define FmtNumber "\033[33m"

void match_answer(size_t answer, size_t *score)
{
    printf(FmtPrompt "Enter your answer:" FmtNormal " ");

    size_t choice = 0;
    ASSERT(scanf("%zu", &choice) == 1, "user entered invalid choice");

    if (choice == answer + 1) *score += 1;
}

int main(int argc, char **argv)
{
    ASSERT(argc == 2, "incorrect usage\n" USAGE);
    FILE *file = fopen(argv[1], "r");
    ASSERT(file, "could not open file '%s'", argv[1]);

    char *input = NULL;
    size_t n = 0;
    ssize_t length;

    size_t score = 0;
    size_t total = 0;
    size_t index = 0;
    size_t answer = 0;
    bool pending = false;
    while ((length = getline(&input, &n, file)) != -1) {
        if (input[length - 1] == '\n') length -= 1;

        if (length == 0) {
            match_answer(answer, &score);
            pending = false;
        } else {
            switch (input[0]) {
            case 'Q':
                index = 0;
                pending = true;
                total++;
                break;
            case '-':
                index++;
                break;
            case 'X':
                answer = index++;
                break;
            }

            if (index == 0) {
                printf(FmtQuestion "Q) ");
            } else {
                printf(FmtChoice "%zu) ", index);
            }

            printf("%.*s\n", (int) length - 2, input + 2);
        }
    }
    if (pending) match_answer(answer, &score);

    fclose(file);
    if (input) free(input);

    printf("You scored " FmtNumber "%zu" FmtNormal "/" FmtNumber "%zu" FmtNormal ".\n", score, total);
    return 0;
}
