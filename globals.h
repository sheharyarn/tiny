//
// Created by Sheharyar Naseer on 4/2/18.
//

#ifndef TINY_GLOBALS_H
#define TINY_GLOBALS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//#include "errors.h"

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#define RESERVED_COUNT 8
#define MAX_NODE_CHILDREN 3


typedef enum TokenType {
    /* Book-keeping Tokens */
    ENDFILE, ERROR,

    /* Reserved Words */
    IF, THEN, ELSE, END, REPEAT, UNTIL, READ, WRITE,

    /* MULTICHARACTER TOKENS */
    ID, NUM,

    /* SPECIAL SYMBOLS */
    ASSIGN, EQ, LT, PLUS, MINUS, TIMES, OVER, LPAREN, RPAREN, SEMI

} TokenType;


/* Externally accessible references */
extern FILE * source;       /* Source code text file */
extern FILE * listing;      /* Listing output file */
extern FILE * code;         /* Code Text File for TM Simulator */

extern int lineNumber;      /* Source line number for listing */



/**
 * Syntax Tree for Parsing
 */

typedef enum NodeKind       { StatementK, ExpressionK }                 NodeKind;
typedef enum StatementKind  { IfK, RepeatK, AssignK, ReadK, WriteK }    StatementKind;
typedef enum ExpressionKind { OpK, ConstK, IdK }                        ExpressionKind;

// Check expression type
typedef enum ExpressionType  { Void, Integer, Boolean }                 ExpressionType;

// Define Node
typedef struct TreeNode {
    struct TreeNode * child[MAX_NODE_CHILDREN];
    struct TreeNode * sibling;

    int lineNumber;
    NodeKind nodeKind;
    ExpressionType type;

    union { StatementKind statement; ExpressionKind expression; } kind;
    union { TokenType op; int val; char * name; } attr;
} TreeNode;



/**
 * Flags for Tracing
 */

// "EchoSource = TRUE" causes the source program to be echoed to the
// listing file with line numbers during parsing
extern int EchoSource;

// "TraceScan = TRUE" causes token information to be printed to the
// listing file as each token is recognized by the scanner
extern int TraceScan;

// "TraceParse = TRUE" causes the syntax tree to be printed to the
// listing file in linearized form (using indents for children)
extern int TraceParse;

// "TraceAnalyze = TRUE" causes symbol table inserts and lookups to
// be reported to the listing file
extern int TraceAnalyze;

// "TraceCode = TRUE" causes comments to be written to the TM code
// file as code is generated
extern int TraceCode;

// "Error = TRUE" prevents further passes if an error occurs
extern int Error;



#endif //TINY_GLOBALS_H
