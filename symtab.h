//
// Created by Sheharyar Naseer on 4/28/18.
//

#ifndef TINY_SYMTAB_H
#define TINY_SYMTAB_H


// Insert line numbers and memory locations into the symbol table
// loc = memory location is only inserted the first time
// otherwise ignored
void st_insert(char * name, int lineNo, int loc);


// Returns the memory location of a variable or -1 if not found
int st_lookup(char * name);


// Print a formatted listing of the symbol table contents to
// the listing file
void printSymTab(FILE * listing);


#endif //TINY_SYMTAB_H
