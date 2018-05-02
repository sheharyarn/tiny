//
// Created by Sheharyar Naseer on 4/2/18.
//

#ifndef TINY_SCANNER_H
#define TINY_SCANNER_H


/* Define the maximum length of language tokens */
#define MAX_TOKEN_LENGTH 50

/* Define the length of the input buffer for source code lines */
#define BUFFER_LENGTH 256


/* Array to store the lexeme of each token */
extern char tokenString[MAX_TOKEN_LENGTH + 1];

/* Function to return the next token in source file */
TokenType getToken(void);


#endif //TINY_SCANNER_H
