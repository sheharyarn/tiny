#include "globals.h"
#include "errors.h"
#include "utils.h"

#define NO_PARSE    FALSE   // Set to TRUE for a scanner-only compiler
#define NO_ANALYZE  FALSE   // Set to TRUE for a parser-only compiler
#define NO_CODE     TRUE    // Set to TRUE to not generate code

#include "scanner.h"
#include "parser.h"
#include "analyzer.h"


// Global Variables
int lineNumber = 0;
FILE * source;
FILE * listing;
FILE * code;


// Tracing Flags
int EchoSource   = TRUE;
int TraceScan    = TRUE;
int TraceParse   = TRUE;
int TraceAnalyze = TRUE;
int TraceCode    = TRUE;
int Error        = FALSE;


int main_c(int argc, char * argv[]) {
    TreeNode * syntaxTree;
    char pgm[50];   // Source Code Filename

    if (argc != 2)
        exitErrorUsage(argv[0]);

    strcpy(pgm, argv[1]);
    if (strchr(pgm, '.') == NULL)
        strcat(pgm, ".tny");

    source = fopen(pgm, "r");
    if (source == NULL)
        exitErrorFNF(pgm);

    listing = stdout;   // Output listing to screen
    fprintf(listing, "\nTINY COMPILATION: %s\n", pgm);


#if NO_PARSE
    // Scanner-Only Mode
    while (getToken() != ENDFILE);

#else
    // Parse Code to create AST
    syntaxTree = parse();
    if (TraceParse) {
        fprintf(listing, "\nSyntax Tree:\n");
        printTree(syntaxTree);
    }
#endif


#if !NO_ANALYZE
    //
    if (! Error) {
        if (TraceAnalyze)
            fprintf(listing,"\nBuilding Symbol Table...\n");

        buildSymbolTable(syntaxTree);

        if (TraceAnalyze)
            fprintf(listing,"\nChecking Types...\n");

        typeCheck(syntaxTree);

        if (TraceAnalyze)
            fprintf(listing,"\nType Checking Finished\n");
    }
#endif


    fclose(source);
    return 0;
}




// Manually Inject Command-line arguments

#define argc 2
#define binary "tiny"
#define directory "tiny-codes/"
#define source "sample.tiny"

int main(void) {
    const char * argv[argc] = { binary, directory source };
    return main_c(argc, argv);
}