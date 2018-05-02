//
// Created by Sheharyar Naseer on 4/2/18.
//


#include "globals.h"
#include "utils.h"
#include "scanner.h"

/* States in Scanner DFA */
typedef enum StateType {
    START, INASSIGN, INCOMMENT, INNUM, INID, DONE
} StateType;

/* Lexeme of identifier or reserved word */
char tokenString[MAX_TOKEN_LENGTH + 1];


/* Buffer references */
static char lineBuffer[BUFFER_LENGTH];          /* holds the current line */
static int linePosition = 0;                    /* current position in line buffer */
static int bufferSize = 0;                      /* current size of buffer string */




/**
 * Fetches the next non-blank character from lineBuffer,
 * reading in a new line if it is exhausted
 */
static char getNextChar(void) {
    if (!(linePosition < bufferSize)) {
        lineNumber++;

        if (fgets(lineBuffer, BUFFER_LENGTH - 1, source)) {
            // If new line is available
            if (EchoSource)
                fprintf(listing, "%4d: %s", lineNumber, lineBuffer);

            bufferSize = strlen(lineBuffer);
            linePosition = 0;
            return lineBuffer[linePosition++];

        } else {
            // End line processing
            return EOF;
        }
    } else {
        return lineBuffer[linePosition++];
    }
}




/**
 * Backtrack one character in line buffer
 */
static void ungetNextChar(void) {
    linePosition--;
}




/**
 * Lookup table of reserved words
 */
static struct { char * str; TokenType tok; } reservedWords[RESERVED_COUNT] = {
    {"if", IF}, {"then", THEN}, {"else", ELSE}, {"end", END},
    {"repeat", REPEAT}, {"until", UNTIL},
    {"read", READ}, {"write", WRITE}
};




/**
 * Look up an identifier to see if it's a reserved word
 * (using linear search)
 */
static TokenType reservedLookup(char * s) {
    for (int i = 0; i < RESERVED_COUNT; i++)
        if (!strcmp(s, reservedWords[i].str))
            return reservedWords[i].tok;

    return ID;
}




/**
 * Return the next token in the source file
 */
TokenType getToken(void) {
    int save;                       // Flag to indicate save to tokenString
    int tokenStringIndex = 0;       // Index for storing into tokenString
    TokenType currentToken;         // Current token to be returned
    StateType state = START;        // Current state -- Always begins at START

    while (state != DONE) {
        char c = getNextChar();
        save = TRUE;

        switch (state) {
            case START:
                if      (isdigit(c))  state = INNUM;
                else if (isalpha(c))  state = INID;
                else if (c == ':')    state = INASSIGN;

                else if ((c == ' ') || (c == '\t') || (c == '\n'))
                    save = FALSE;
                else if (c == '{') {
                    save = FALSE;
                    state = INCOMMENT;

                } else {
                    state = DONE;
                    switch (c) {
                        case EOF:
                            save = FALSE;
                            currentToken = ENDFILE;
                            break;

                        case '=':   currentToken = EQ;       break;
                        case '<':   currentToken = LT;       break;
                        case '+':   currentToken = PLUS;     break;
                        case '-':   currentToken = MINUS;    break;
                        case '*':   currentToken = TIMES;    break;
                        case '/':   currentToken = OVER;     break;
                        case '(':   currentToken = LPAREN;   break;
                        case ')':   currentToken = RPAREN;   break;
                        case ';':   currentToken = SEMI;     break;
                        default:    currentToken = ERROR;    break;
                    }
                }
                break;


            case INCOMMENT:
                save = FALSE;
                if (c == '}')
                    state = START;
                break;

            case INASSIGN:
                state = DONE;
                if (c == '=') {
                    currentToken = ASSIGN;
                } else {
                    ungetNextChar();
                    save = FALSE;
                    currentToken = ERROR;
                }
                break;

            case INNUM:
                if (!isdigit(c)) {
                    ungetNextChar();
                    save = FALSE;
                    state = DONE;
                    currentToken = NUM;
                }
                break;

            case INID:
                if (!isalpha(c)) {
                    ungetNextChar();
                    save = FALSE;
                    state = DONE;
                    currentToken = ID;
                }
                break;

            case DONE:
            default:
                // Should never happen
                fprintf(listing, "Scanner Bug: state = %d\n", state);
                state = DONE;
                currentToken = ERROR;
                break;
        }

        if ((save) && (tokenStringIndex <= MAX_TOKEN_LENGTH))
            tokenString[tokenStringIndex++] = c;

        if (state == DONE) {
            tokenString[tokenStringIndex] = '\0';
            if (currentToken == ID)
                currentToken = reservedLookup(tokenString);
        }
    }

    if (TraceScan) {
        fprintf(listing, "\t%d: ", lineNumber);
        printToken(currentToken, tokenString);
    }

    return currentToken;
}