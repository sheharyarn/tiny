//
// Created by Sheharyar Naseer on 4/9/18.
//

#ifndef TINY_UTILS_H
#define TINY_UTILS_H


// Print a token and it's lexeme to the listing file
void printToken(TokenType, const char*);

// Create a new statement node for Syntax Tree
TreeNode * newStatementNode(StatementKind);

// Create a new expression node for Syntax Tree
TreeNode * newExpressionNode(ExpressionKind);

// Make a pure copy of an existing string
char * copyString(char *);

// Print the Syntax Tree to the listing file
void printTree(TreeNode *);


#endif //TINY_UTILS_H
