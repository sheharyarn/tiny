//
// Created by Sheharyar Naseer on 4/29/18.
//

#include "globals.h"
#include "errors.h"
#include "symtab.h"
#include "analyzer.h"


// Define a function type referencing functions that take
// a TreeNode as the only argument and return void
typedef void (* OrderCallback)(TreeNode *);


// counter for variable memory locations
static int location = 0;


// generic syntax tree traversal routine
// accepts two functions (preProc & postProc) as arguments
// which are applied in pre and post order traversals of
// the tree t
static void traverse(TreeNode * t, OrderCallback preOrder, OrderCallback postOrder) {
    if (t != NULL) {
        preOrder(t);

        // TODO:
        // Make sure this works without the block
        // Recursively traverse all children
        for (int i=0; i < MAX_NODE_CHILDREN; i++)
            traverse(t -> child[i], preOrder, postOrder);

        postOrder(t);

        // Traverse the next node
        traverse(t -> sibling, preOrder, postOrder);
    }
}



// Do nothing callback (when we only want one of pre/post
// and not both)
static void emptyCallback(TreeNode * t) {
    // ¯\_(ツ)_/¯
}


// Insert identifiers stored in t into the symbol table
static void insertNode(TreeNode * t) {
    switch (t -> nodeKind) {

        // For Statements
        case StatementK:
            switch (t -> kind.statement) {
                case AssignK:
                case ReadK:
                    if (st_lookup(t -> attr.name) == -1)
                        // not yet in the table, treat as new definition
                        st_insert(t->attr.name, t->lineNumber, location++);

                    else
                        // already in table, ignore location, just add line number
                        st_insert(t->attr.name, t->lineNumber, 0);

                    break;
                default:
                    break;
            }
            break;


        // For Expressions
        case ExpressionK:
            switch (t -> kind.expression) {
                case IdK:
                    if (st_lookup(t -> attr.name) == -1)
                        // not yet in the table, treat as new definition
                        st_insert(t->attr.name, t->lineNumber, location++);

                    else
                        // already in table, ignore location, just add line number
                        st_insert(t->attr.name, t->lineNumber, 0);

                    break;
                default:
                    break;
            }
            break;


        default:
            break;
    }
}



// Perform type checking at a single tree node
static void checkNode(TreeNode * t) {
    switch (t -> nodeKind) {

        case StatementK:
            switch (t -> kind.statement) {
                case IfK:
                    if (t->child[0]->type == Integer)
                        printErrorType(t->child[0], "if test is not boolean");
                    break;

                case AssignK:
                    if (t->child[0]->type != Integer)
                        printErrorType(t->child[0], "assignment of non integer value");
                    break;

                case WriteK:
                    if (t->child[0]->type != Integer)
                        printErrorType(t->child[0], "write of non integer value");
                    break;

                case RepeatK:
                    // Make sure my change here works
                    if (t->child[1]->type != Boolean)
                        printErrorType(t->child[1], "repeat test is not boolean");
                    break;

                default:
                    break;
            }
            break;


        case ExpressionK:
            switch (t -> kind.expression) {
                case OpK:
                    if ((t->child[0]->type != Integer) || (t->child[1]->type != Integer))
                        printErrorType(t, "Operation applied to non integer");

                    if ((t->attr.op == EQ) || (t->attr.op == LT))
                        t->type = Boolean;
                    else
                        t->type = Integer;
                    break;

                case ConstK:
                case IdK:
                    t->type = Integer;
                    break;

                default:
                    break;
            }
            break;


        default:
            break;
    }
}




// Construct Symbol Table by pre-order traversal of the AST
void buildSymbolTable(TreeNode * ast) {
    traverse(ast, insertNode, emptyCallback);

    if (TraceAnalyze) {
        fprintf(listing, "\nSymbol Table:\n\n");
        printSymTab(listing);
    }
}


// Perform type checking by post order traversal
void typeCheck(TreeNode * ast) {
    traverse(ast, emptyCallback, checkNode);
}
