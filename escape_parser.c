#include <malloc.h>

#include "escape_parser.h"

/**
 * Initialize the escape parser state
 *
 * @return EscapeParseResult* - pointer to the parser state
 */
EscapeParseResult *init_escape_parser() {
    EscapeParseResult *result = malloc(sizeof(EscapeParseResult));
    reset_escape_parser(result);
    return result;
}

/**
 * Step the escape parser state.
 *
 * Notes:
 * - this parser should be able to handle consecutive escape sequences and string (character by character) that contain
 *   no escape sequences. The only way this parser can fail is if the escape sequence is incomplete.
 * - if the escape sequence is complete, this method does not reset the parser state. It is up to the caller to reset.
 *
 * @param result EscapeParseResult* - pointer to the current state
 * @param c char - the next character to parse
 * @return int - 0 if the parse is not complete, 1 if the parse is complete, -1 if the parse fails (incomplete, invalid, unknown, etc.)
 */
int step_escape_parser(EscapeParseResult *result, char c) {
    switch (result->current_state) {
        case ESCAPE_PARSE_STATE_START:
            if (c == ESCAPE_CHAR) {
                result->current_state = ESCAPE_PARSE_STATE_ESCAPE;
                return 0;
            }
            return 1;
        case ESCAPE_PARSE_STATE_ESCAPE:
            if (c == '[') {
                result->current_state = ESCAPE_PARSE_STATE_LEFT_BRACKET;
                return 0;
            }
            return 1;
        case ESCAPE_PARSE_STATE_LEFT_BRACKET:
            switch (c) {
                case '?':
                    result->current_state = ESCAPE_PARSE_STATE_QUESTION_MARK;
                    return 0;
                case 'A':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_UP;
                    return 1;
                case 'B':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_DOWN;
                    return 1;
                case 'C':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_RIGHT;
                    return 1;
                case 'D':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_LEFT;
                    return 1;
                case 'J':
                    result->current_state = ESCAPE_PARSE_STATE_CLEAR_SCREEN;
                    return 1;
                case 'K':
                    result->current_state = ESCAPE_PARSE_STATE_CLEAR_LINE;
                    return 1;
                case '3':
                    result->current_state = ESCAPE_PARSE_STATE_ARG0_3;
                    return 0;
                case '6':
                    result->current_state = ESCAPE_PARSE_STATE_ARG0_6;
                    return 0;
                default:
                    if (c >= '0' && c <= '9') {
                        result->current_state = ESCAPE_PARSE_STATE_ARG0_REMAIN;
                        return 0;
                    }
                    return -1;
            }
        case ESCAPE_PARSE_STATE_QUESTION_MARK:
            if (c == '2') {
                result->current_state = ESCAPE_PARSE_STATE_PRIVATE_2;
                return 0;
            }
            return -1;
        case ESCAPE_PARSE_STATE_PRIVATE_2:
            if (c == '5') {
                result->current_state = ESCAPE_PARSE_STATE_PRIVATE_25;
                return 0;
            }
            return -1;
        case ESCAPE_PARSE_STATE_PRIVATE_25:
            switch(c) {
                case 'h':
                    result->current_state = ESCAPE_PARSE_STATE_CURSOR_SHOW;
                    return 1;
                case 'l':
                    result->current_state = ESCAPE_PARSE_STATE_CURSOR_HIDE;
                    return 1;
                default:
                    return -1;
            }
        case ESCAPE_PARSE_STATE_ARG0_REMAIN:
            if (c >= '0' && c <= '9') {
                return 0; // remain in this state
            }

            switch (c) {
                case ';':
                    result->current_state = ESCAPE_PARSE_STATE_ARG0_SEMI;
                    return 0;
                case 'm':
                    result->current_state = ESCAPE_PARSE_STATE_SGR_SEQUENCE;
                    return 1;
                case 'A':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_UP;
                    return 1;
                case 'B':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_DOWN;
                    return 1;
                case 'C':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_RIGHT;
                    return 1;
                case 'D':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_LEFT;
                    return 1;
                case 'J':
                    result->current_state = ESCAPE_PARSE_STATE_CLEAR_SCREEN;
                    return 1;
                case 'K':
                    result->current_state = ESCAPE_PARSE_STATE_CLEAR_LINE;
                    return 1;
                default:
                    return -1;
            }
        case ESCAPE_PARSE_STATE_ARG0_3:
            if (c >= '0' && c <= '9') {
                result->current_state = ESCAPE_PARSE_STATE_ARG0_REMAIN;
                return 0;
            }

            switch (c) {
                case '~':
                    result->current_state = ESCAPE_PARSE_STATE_DELETE_KEY;
                    return 1;
                case 'm':
                    result->current_state = ESCAPE_PARSE_STATE_SGR_SEQUENCE;
                    return 1;
                case ';':
                    result->current_state = ESCAPE_PARSE_STATE_ARG0_SEMI;
                    return 0;
                case 'A':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_UP;
                    return 1;
                case 'B':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_DOWN;
                    return 1;
                case 'C':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_RIGHT;
                    return 1;
                case 'D':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_LEFT;
                    return 1;
                case 'J':
                    result->current_state = ESCAPE_PARSE_STATE_CLEAR_SCREEN;
                    return 1;
                case 'K':
                    result->current_state = ESCAPE_PARSE_STATE_CLEAR_LINE;
                    return 1;
                default:
                    return -1;
            }
        case ESCAPE_PARSE_STATE_ARG0_6:
            if (c >= '0' && c <= '9') {
                result->current_state = ESCAPE_PARSE_STATE_ARG0_REMAIN;
                return 0;
            }

            switch (c) {
                case 'n':
                    result->current_state = ESCAPE_PARSE_STATE_CURSOR_REQ;
                    return 1;
                case ';':
                    result->current_state = ESCAPE_PARSE_STATE_ARG0_SEMI;
                    return 0;
                case 'A':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_UP;
                    return 1;
                case 'B':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_DOWN;
                    return 1;
                case 'C':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_RIGHT;
                    return 1;
                case 'D':
                    result->current_state = ESCAPE_PARSE_STATE_ARROW_LEFT;
                    return 1;
                case 'J':
                    result->current_state = ESCAPE_PARSE_STATE_CLEAR_SCREEN;
                    return 1;
                case 'K':
                    result->current_state = ESCAPE_PARSE_STATE_CLEAR_LINE;
                    return 1;
                default:
                    return -1;
            }
        case ESCAPE_PARSE_STATE_ARG0_SEMI:
            if (c >= '0' && c <= '9') {
                result->current_state = ESCAPE_PARSE_STATE_ARG1;
                return 0;
            }
            return -1;
        case ESCAPE_PARSE_STATE_ARG1:
            if (c >= '0' && c <= '9') {
                return 0; // remain in this state
            }

            switch (c) {
                case 'm':
                    result->current_state = ESCAPE_PARSE_STATE_SGR_SEQUENCE;
                    return 1;
                case 'R':
                    result->current_state = ESCAPE_PARSE_STATE_CURSOR_RES;
                    return 1;
                case 'H':
                    result->current_state = ESCAPE_PARSE_STATE_CURSOR_SET;
                    return 1;
                case ';':
                    result->current_state = ESCAPE_PARSE_STATE_ARGN_SEMI;
                    return 0;
                default:
                    return -1;
            }
        case ESCAPE_PARSE_STATE_ARGN_SEMI:
            if (c >= '0' && c <= '9') {
                result->current_state = ESCAPE_PARSE_STATE_ARGN;
                return 0;
            }
            return -1;
        case ESCAPE_PARSE_STATE_ARGN:
            if (c >= '0' && c <= '9') {
                return 0; // remain in this state
            }

            switch (c) {
                case 'm':
                    result->current_state = ESCAPE_PARSE_STATE_SGR_SEQUENCE;
                    return 1;
                case ';':
                    result->current_state = ESCAPE_PARSE_STATE_ARGN_SEMI;
                    return 0;
                default:
                    return -1;
            }
        default:
            return -1;
    }
}

/**
 * Reset the escape parser state
 *
 * @param result EscapeParseResult* - pointer to the parser state
 */
void reset_escape_parser(EscapeParseResult *result) {
    result->current_state = ESCAPE_PARSE_STATE_START;
}

/**
 * Free the escape parser state
 *
 * @param result EscapeParseResult* - pointer to the parser state
 */
void free_escape_parser(EscapeParseResult *result) {
    free(result);
}

/**
 * Calculate the length of a string (visible characters) that contains escape sequences
 *
 * @param str const char* - the string to calculate the length of
 * @return int - the length of the string
 */
int escape_strlen(const char *str) {
    int len = 0;
    EscapeParseResult *result = init_escape_parser();

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        int status = step_escape_parser(result, c);

        if (status == 1) {
            if (result->current_state == ESCAPE_PARSE_STATE_START) {
                len++;
            }

            reset_escape_parser(result);
        }
    }

    free_escape_parser(result);
    return len;
}