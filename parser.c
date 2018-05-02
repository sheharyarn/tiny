//
// Created by Sheharyar Naseer on 4/28/18.
//

#include "globals.h"
#include "utils.h"
#include "scanner.h"
#include "errors.h"
#include "parser.h"


// Holds current token
static TokenType token;


// Function Prototypes for recursive calls
static TreeNode * stmt_sequence(void);
static TreeNode * statement(void);
static TreeNode * if_stmt(void);
static TreeNode * repeat_stmt(void);
static TreeNode * assign_stmt(void);
static TreeNode * read_stmt(void);
static TreeNode * write_stmt(void);
static TreeNode * exp(void);
static TreeNode * simple_exp(void);
static TreeNode * term(void);
static TreeNode * factor(void);



static void match(TokenType expected) {
    if (token == expected) {
        token = getToken();
    } else {
        printErrorSyntaxUnexpected();
        printToken(token, tokenString);
        fprintf(listing, "       ");
    }
}



TreeNode * stmt_sequence(void) {
    TreeNode * t = statement();
    TreeNode * p = t;

    while ((token != ENDFILE) && (token != END) && (token != ELSE) && (token != UNTIL)) {
        TreeNode * q;
        match(SEMI);
        q = statement();

        if (q != NULL) {
            if (t == NULL) {
                t = p = q;
            } else {
                // p cannot be null either
                p -> sibling = q;
                p = q;
            }
        }
    }

    return t;
}


TreeNode * statement(void) {
    TreeNode * t = NULL;

    switch (token) {
        case IF:     t = if_stmt();     break;
        case REPEAT: t = repeat_stmt(); break;
        case ID:     t = assign_stmt(); break;
        case READ:   t = read_stmt();   break;
        case WRITE:  t = write_stmt();  break;

        default:
            printErrorSyntaxUnexpected();
            printToken(token, tokenString);
            token = getToken();
            break;
    }

    return t;
}


TreeNode * if_stmt(void) {
    TreeNode * t = newStatementNode(IfK);

    match(IF);
    if (t != NULL)
        t -> child[0] = exp();

    match(THEN);
    if (t != NULL)
        t -> child[1] = stmt_sequence();

    if (token == ELSE) {
        match(ELSE);
        t -> child[2] = stmt_sequence();
    }

    match(END);
    return t;
}


TreeNode * repeat_stmt(void) {
    TreeNode * t = newStatementNode(RepeatK);

    match(REPEAT);
    if (t != NULL)
        t -> child[0] = stmt_sequence();

    match(UNTIL);
    if (t != NULL)
        t -> child[1] = exp();

    return t;
}


TreeNode * assign_stmt(void) {
    TreeNode * t = newStatementNode(AssignK);

    if ((t != NULL) && (token == ID))
        t -> attr.name = copyString(tokenString);

    match(ID);
    match(ASSIGN);

    if (t != NULL)
        t -> child[0] = exp();

    return t;
}


TreeNode * read_stmt(void) {
    TreeNode * t = newStatementNode(ReadK);

    match(READ);
    if ((t != NULL) && (token == ID))
        t -> attr.name = copyString(tokenString);

    match(ID);
    return t;
}


TreeNode * write_stmt(void) {
    TreeNode * t = newStatementNode(WriteK);

    match(WRITE);
    if (t != NULL)
        t -> child[0] = exp();

    return t;
}


TreeNode * exp(void) {
    TreeNode * t = simple_exp();

    if ((token == LT) || (token == EQ)) {
        TreeNode * p = newExpressionNode(OpK);

        if (p != NULL) {
            p -> child[0] = t;
            p -> attr.op = token;
            t = p;
        }

        match(token);
        if (t != NULL)
            t -> child[1] = simple_exp();
    }

    return t;
}


TreeNode * simple_exp(void) {
    TreeNode * t = term();

    while ((token == PLUS) || (token == MINUS)) {
        TreeNode * p = newExpressionNode(OpK);

        if (p != NULL) {
            p -> child[0] = t;
            p -> attr.op = token;
            t = p;
            match(token);
            t -> child[1] = term();
        }
    }

    return t;
}


TreeNode * term(void) {
    TreeNode * t = factor();

    while ((token == TIMES) || (token == OVER)) {
        TreeNode * p = newExpressionNode(OpK);

        if (p != NULL) {
            p -> child[0] = t;
            p -> attr.op = token;
            t = p;
            match(token);
            t -> child[1] = factor();
        }
    }

    return t;
}


TreeNode * factor(void) {
    TreeNode * t = NULL;

    switch (token) {
        case NUM:
            t = newExpressionNode(ConstK);
            if ((t != NULL) && (token == NUM))
                t -> attr.val = atoi(tokenString);

            match(NUM);
            break;

        case ID:
            t = newExpressionNode(IdK);

            if ((t != NULL) && (token == ID))
                t -> attr.name = copyString(tokenString);

            match(ID);
            break;

        case LPAREN:
            match(LPAREN);
            t = exp();
            match(RPAREN);
            break;

        default:
            printErrorSyntaxUnexpected();
            printToken(token, tokenString);
            token = getToken();
            break;
    }

    return t;
}



/**
 * Actual Parser Implementation
 */
TreeNode * parse(void) {
    TreeNode * t;
    token = getToken();
    t = stmt_sequence();

    if (token != ENDFILE)
        printErrorSyntax("Code ends before file\n");

    return t;
}