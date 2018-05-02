//
// Created by Sheharyar Naseer on 4/9/18.
//

#ifndef TINY_ERRORS_H
#define TINY_ERRORS_H

void printErrorSyntax(char *);
void printErrorSyntaxUnexpected(void);

void printErrorType(TreeNode *, char *);

void printErrorOOM();

void exitErrorUsage(const char *);

void exitErrorFNF(const char *);



#endif //TINY_ERRORS_H
