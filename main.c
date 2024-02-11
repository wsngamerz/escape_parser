#include <stdio.h>
#include <string.h>

#include "escape_parser.h"

// uncomment to enable debug line-by-line testing
// #define DEBUG

const char *test_sequences[] = {
        "Hello, World!",
        ESCAPE_SGR(31) "Hello, World!" ESCAPE_SGR(0),
        ESCAPE_SGR(31) "Hello, World!" ESCAPE_SGR(0) "Hello, World!",
        ESCAPE_CHAR_STRING "[A",
        ESCAPE_CHAR_STRING "[B" ESCAPE_CHAR_STRING "[C" ESCAPE_CHAR_STRING "[D",
        ESCAPE_CHAR_STRING "[1;2;3;4;5;6;7;8;9;10;11;12;13;14;15;16;17;18;19;20;21;22;23;24;25;26;27;28;29;30;31;32;33;34;35m",
        ESCAPE_SGR(1;31) "H" ESCAPE_SGR(32) "e" ESCAPE_SGR(33) "l" ESCAPE_SGR(34) "l" ESCAPE_SGR(35) "o"
        ESCAPE_SGR(36) ", " ESCAPE_SGR(37) "W" ESCAPE_SGR(90) "o" ESCAPE_SGR(91) "r" ESCAPE_SGR(92) "l" ESCAPE_SGR(93)
        "d" ESCAPE_SGR(94) "!" ESCAPE_SGR(0),
};
#define NUM_TEST_SEQUENCES (sizeof(test_sequences) / sizeof(test_sequences[0]))

// forward declarations
void test_sequence(const char *sequence);
void line_by_line_test(const char *sequence);
void print_escaped_string(const char *sequence);

int main() {
    printf("Escape parser tests\n");

    for (int i = 0; i < NUM_TEST_SEQUENCES; i++) {
        printf("\n================================\n");
        test_sequence(test_sequences[i]);
#ifdef DEBUG
        line_by_line_test(test_sequences[i]);
#endif
    }

    printf("\n\n");
    for (int i = 0; i < 256; i++) {
        printf(ESCAPE_CHAR_STRING "[48;5;%dm ", i);
        if (i % 64 == 63) {
            printf(ESCAPE_SGR(0) "\n");
        }
    }

    return 0;
}

void test_sequence(const char *sequence) {
    printf("String:\t\t\t'");
    print_escaped_string(sequence);
    printf("'\n");
    printf("String Length:\t%lu\n", strlen(sequence));

    printf("Visible String:\t'%s" ESCAPE_SGR(0) "'\n", sequence);
    printf("Visible Chars:\t%d\n", escape_strlen(sequence));
}

void print_escaped_string(const char *sequence) {
    for (int i = 0; sequence[i] != '\0'; i++) {
        char c = sequence[i];
        if (c == ESCAPE_CHAR) {
            printf("\\e");
        } else {
            printf("%c", c);
        }
    }
}

void line_by_line_test(const char *sequence) {
    // test the escape parser character by character
    EscapeParseResult *result = init_escape_parser();
    for (int i = 0; sequence[i] != '\0'; i++) {
        char c = sequence[i];
        int status = step_escape_parser(result, c);

        printf("- %d:\t '%c'", i, c);
        switch (status) {
            case 0:
                printf("(Incomplete - %d)", result->current_state);
                break;
            case 1:
                printf("(Complete - %d)", result->current_state);
                reset_escape_parser(result);
                break;
            case -1:
                printf("(Failed)");
                reset_escape_parser(result);
                break;
            default:
                printf("(Unknown status)");
                break;
        }
        printf("\n");
    }
    free_escape_parser(result);
}
