//
// Created by Sheharyar Naseer on 4/29/18.
//

#ifndef TINY_ANALYZER_H
#define TINY_ANALYZER_H


// Construct the symbol table using pre-order traversal
// of the syntax tree
void buildSymbolTable(TreeNode *);


// Perform type-checking using post-order traversal of
// the syntax tree
void typeCheck(TreeNode *);

#endif //TINY_ANALYZER_H
