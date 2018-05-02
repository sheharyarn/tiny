//
// Created by Sheharyar Naseer on 4/9/18.
//

#include "globals.h"
#include "utils.h"
#include "errors.h"


void printToken(TokenType token, const char * tokenString) {
    switch (token) {
        case ASSIGN:  fprintf(listing, ":=\n");   break;
        case LT:      fprintf(listing, "<\n");    break;
        case EQ:      fprintf(listing, "=\n");    break;
        case LPAREN:  fprintf(listing, "(\n");    break;
        case RPAREN:  fprintf(listing, ")\n");    break;
        case SEMI:    fprintf(listing, ";\n");    break;
        case PLUS:    fprintf(listing, "+\n");    break;
        case MINUS:   fprintf(listing, "-\n");    break;
        case TIMES:   fprintf(listing, "*\n");    break;
        case OVER:    fprintf(listing, "/\n");    break;
        case ENDFILE: fprintf(listing, "EOF\n");  break;

        case IF:
        case THEN:
        case ELSE:
        case END:
        case REPEAT:
        case UNTIL:
        case READ:
        case WRITE:
            fprintf(listing, "reserved word: %s\n", tokenString);
            break;

        case NUM:
            fprintf(listing, "NUM, val= %s\n", tokenString);
            break;

        case ID:
            fprintf(listing, "ID, name= %s\n", tokenString);
            break;

        case ERROR:
            fprintf(listing, "ERROR: %s\n", tokenString);
            break;

        default: // SHOULD NEVER HAPPEN
            fprintf(listing, "Unknown Token: %d\n", token);
            // printErrorUnknownToken(token);
            break;
    }
}




TreeNode * newStatementNode(StatementKind kind) {
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));

    if (t == NULL) {
        printErrorOOM();
    } else {
        for (int i=0; i < MAX_NODE_CHILDREN; i++)
            t->child[i] = NULL;

        t->sibling = NULL;
        t->nodeKind = StatementK;
        t->lineNumber = lineNumber;
        t->kind.statement = kind;
    }

    return t;
}




TreeNode * newExpressionNode(ExpressionKind kind) {
    TreeNode * t = (TreeNode *) malloc(sizeof(TreeNode));

    if (t == NULL) {
        printErrorOOM();
    } else {
        for (int i=0; i < MAX_NODE_CHILDREN; i++)
            t->child[i] = NULL;

        t->sibling = NULL;
        t->nodeKind = ExpressionK;
        t->lineNumber = lineNumber;
        t->kind.expression = kind;
        t->type = Void;
    }

    return t;
}




char * copyString(char * string) {
    if (string == NULL)
        return NULL;

    int length = strlen(string) + 1;
    char * copy = (char *) malloc(length);

    if (copy == NULL)
        printErrorOOM();
    else
        strcpy(copy, string);

    return copy;
}




#define INDENT   indentCount+=2;
#define UNINDENT indentCount-=2;

static int indentCount = 0;

static void printSpaces(void) {
    for (int i=0; i < indentCount; i++)
        fprintf(listing, " ");
}

void printTree(TreeNode * tree) {
    INDENT;

    while (tree != NULL) {
        printSpaces();

        if (tree->nodeKind == StatementK) {
            switch (tree->kind.statement) {
                case IfK:       fprintf(listing, "If\n");                               break;
                case RepeatK:   fprintf(listing, "Repeat\n");                           break;
                case AssignK:   fprintf(listing, "Assign to: %s\n", tree->attr.name);   break;
                case ReadK:     fprintf(listing, "Read: %s\n", tree->attr.name);        break;
                case WriteK:    fprintf(listing, "Write\n");                            break;
                default:        fprintf(listing, "Unknown Statement Kind\n");           break;
            }

        } else if (tree->nodeKind == ExpressionK) {
            switch (tree->kind.expression) {
                case OpK:
                    fprintf(listing, "Op: ");
                    printToken(tree->attr.op, "\0");
                    break;
                case ConstK:
                    fprintf(listing, "const: %d\n", tree->attr.val);
                    break;
                case IdK:
                    fprintf(listing, "Id: %s\n", tree->attr.name);
                    break;
                default:
                    fprintf(listing, "Unknown Expression Kind\n");
                    break;
            }

        } else {
            fprintf(listing, "Unknown Node Kind\n");
        }

        for (int i=0; i < MAX_NODE_CHILDREN; i++)
            printTree(tree->child[i]);

        tree = tree->sibling;
    }

    UNINDENT;
}
