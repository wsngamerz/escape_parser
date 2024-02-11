#ifndef ESCAPE_PARSER_H
#define ESCAPE_PARSER_H

// some C preprocessor magic to help define the escape character and escape sequences
#define ESCAPE_CHAR '\x1B'
#define ESCAPE_CHAR_STRING "\x1B"
#define STRINGIFY(X) #X
#define ESCAPE_SGR(X) ESCAPE_CHAR_STRING "[" STRINGIFY(X) "m"


typedef enum {
    ESCAPE_PARSE_STATE_START,           // q0
    ESCAPE_PARSE_STATE_ESCAPE,          // q1
    ESCAPE_PARSE_STATE_LEFT_BRACKET,    // q2 - start of CSI sequences
    ESCAPE_PARSE_STATE_QUESTION_MARK,   // q3 - start of private sequences
    ESCAPE_PARSE_STATE_PRIVATE_2,       // q4
    ESCAPE_PARSE_STATE_PRIVATE_25,      // q5
    ESCAPE_PARSE_STATE_CURSOR_SHOW,     // q6 - accepting state
    ESCAPE_PARSE_STATE_CURSOR_HIDE,     // q7 - accepting state
    ESCAPE_PARSE_STATE_ARG0_REMAIN,     // q8
    ESCAPE_PARSE_STATE_ARG0_3,          // q9
    ESCAPE_PARSE_STATE_ARG0_6,          // q10
    ESCAPE_PARSE_STATE_DELETE_KEY,      // q11 - accepting state
    ESCAPE_PARSE_STATE_CURSOR_REQ,      // q12 - accepting state
    ESCAPE_PARSE_STATE_SGR_SEQUENCE,    // q13 - accepting state
    ESCAPE_PARSE_STATE_ARG0_SEMI,       // q14
    ESCAPE_PARSE_STATE_ARG1,            // q15
    ESCAPE_PARSE_STATE_CURSOR_RES,      // q16 - accepting state
    ESCAPE_PARSE_STATE_CURSOR_SET,      // q17 - accepting state
    ESCAPE_PARSE_STATE_ARGN_SEMI,       // q18
    ESCAPE_PARSE_STATE_ARGN,            // q19
    ESCAPE_PARSE_STATE_ARROW_UP,        // q20 - accepting state
    ESCAPE_PARSE_STATE_ARROW_DOWN,      // q21 - accepting state
    ESCAPE_PARSE_STATE_ARROW_RIGHT,     // q22 - accepting state
    ESCAPE_PARSE_STATE_ARROW_LEFT,      // q23 - accepting state
    ESCAPE_PARSE_STATE_CLEAR_SCREEN,    // q24 - accepting state
    ESCAPE_PARSE_STATE_CLEAR_LINE,      // q25 - accepting state

} EscapeParseState;

typedef struct {
    EscapeParseState current_state;
} EscapeParseResult;

//
// Escape parser functions
//

EscapeParseResult *init_escape_parser();

void free_escape_parser(EscapeParseResult *result);

void reset_escape_parser(EscapeParseResult *result);

int step_escape_parser(EscapeParseResult *result, char c);

int escape_strlen(const char *str);

#endif //ESCAPE_PARSER_H
