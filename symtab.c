//
// Created by Sheharyar Naseer on 4/28/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Size of the hashtable
// but why 211 tho?
#define SIZE 211

// Shift -> Power of 2 used as multiplier in hash function
#define SHIFT 4

// Hash function
static int hash(char * key) {
    int temp = 0;
    int i = 0;

    while (key[i] != '\0') {
        temp = ((temp << SHIFT) + key[i]) % SIZE;
        ++i;
    }

    return temp;
}


// The list of line numbers of the source code in which a
// variable is referenced
typedef struct LineListRec {
    int lineNo;
    struct LineListRec * next;
} * LineList;


// The record in the bucket list for each variable, including
// name, assigned memory location and the list of line numbers
// in which it appears in the source code
typedef struct BucketListRec {
    char * name;
    LineList lines;
    int memloc; // memory location of the variable
    struct BucketListRec * next;
} * BucketList;


// Hash Table
// (with linked list for handling collisions)
static BucketList hashTable[SIZE];


// Inserts line numbers and memory locations into the symbol
// table. Loc = Memory location is inserted only the first
// time, otherwise ignored
void st_insert(char * name, int lineNo, int loc) {
    int h = hash(name);

    BucketList l = hashTable[h];

    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l->next;

    if (l == NULL) {
        // var is not yet in the table
        l = (BucketList) malloc(sizeof(struct BucketListRec));
        l -> name = name;
        l -> memloc = loc;
        l -> lines = (LineList) malloc(sizeof(struct LineListRec));
        l -> lines -> lineNo = lineNo;
        l -> lines -> next = NULL;

        // put the newest variable at head, and make the rest it's tail
        l -> next = hashTable[h];
        hashTable[h] = l;

    } else {
        // found in table, so just add line number
        LineList t = l -> lines;

        // iterate to the end of lines and add it there
        while (t -> next != NULL)
            t = t -> next;

        t -> next = (LineList) malloc(sizeof(struct LineListRec));
        t -> next -> lineNo = lineNo;
        t -> next -> next = NULL;
    }
}



// Return the memory location of variable if found else -1
int st_lookup(char * name) {
    int h = hash(name);
    BucketList l = hashTable[h];

    while ((l != NULL) && (strcmp(name, l->name) != 0))
        l = l -> next;

    if (l == NULL)
        return -1;
    else
        return l -> memloc;
}



// Print a formatted listing of the symbol table contents
// to the listing file
void printSymTab(FILE * listing) {
    fprintf(listing, "Variable Name   Location   Line Numbers\n");
    fprintf(listing, "-------------   --------   -----------\n");

    for (int i=0; i<SIZE; i++) {
        if (hashTable[i] != NULL) {
            BucketList l = hashTable[i];

            while (l != NULL) {
                LineList t = l -> lines;
                fprintf(listing, "%-15s ", l->name);
                fprintf(listing, "%-9d  ", l->memloc);

                while (t != NULL) {
                    fprintf(listing, "%-3d ", t->lineNo);
                    t = t -> next;
                }

                fprintf(listing, "\n");
                l = l -> next;
            }
        }
    }
}