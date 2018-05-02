//
// Created by Sheharyar Naseer on 4/11/18.
//

#include "globals.h"
#include "errors.h"


void printErrorSyntax(char * message) {
    fprintf(listing, "\n>>> ");
    fprintf(listing, "Syntax Error at line %d: %s", lineNumber, message);
    Error = TRUE;
}

void printErrorSyntaxUnexpected(void) {
    printErrorSyntax("unexpected token -> ");
}


void printErrorType(TreeNode * t, char * message) {
    fprintf(listing, "Type Error at line %d: %s\n", t -> lineNumber);
    Error = TRUE;
}



void printErrorOOM() {
    fprintf(listing, "Out of Memory Error at line %d\n", lineNumber);
}

void exitErrorUsage(const char * name) {
    fprintf(stderr, "usage: %s <filename>\n", name);
    exit(1);
}

void exitErrorFNF(const char * file) {
    fprintf(stderr, "File %s not found\n", file);
    exit(1);
}



//void printErrorUnknown(const char * error) {
//    fprintf(listing, "Unknown %s at Line %d\n", error, lineNumber);
//}
//
//void printErrorUnknownToken(const TokenType token) {
//    fprintf(listing, "Unknown Token '%s' at Line %d\n", token, lineNumber);
//}
