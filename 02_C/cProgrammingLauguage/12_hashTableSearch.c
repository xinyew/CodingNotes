/*
 * Linked list structure
 */
struct linkedList {
    struct linkedList *next;    // Pointing to the next Node
    char *name; // String of name
    char *defn; // String of definition
};

/*
 * Head files
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Definitions
 */
#define HASHSIZE 101

/*
 * Global variable hashTable
 * Has the capacity of storing HASHSIZE heads of defined linked list
 */
static struct linkedList *hashTable[HASHSIZE];

/*
 * Hash function to find hash value of the string
 * ----------------------------------------------
 * @param s: string to be calculated
 * @return hashval: the calculated hash value
 */
unsigned hash(char *s) {
    unsigned hashval;   // The unsigned number to be returned
    for (hashval = 0; *s != '\0'; s++) {
        // Loop through the string until '\0'
        hashval = *s + 31 * hashval;
    }
    return hashval % HASHSIZE;
}

/*
 * Lookup function to find target in the hash table
 * ----------------------------------------------
 * @param s: target string to be found
 * @return np: the address of the node storing the target or NULL if not found
 */
struct linkedList *lookUp(char *s) {
    struct linkedList *np;  // Helper linked list pointer to find target

    for (np = hashTable[hash(s)]; np != NULL; np = np -> next) {
        // Go to the right index of the pointer array, then loop through the linked list until find target
        if (strcmp(s, np -> name) == 0) {
            return np;  // If found, return the address of target
        }
    }
    return np;  // If not found, return NULL
}

/*
 * String duplication function to store a string in a safe address
 * ---------------------------------------------------------------
 * @param s: string to be stored
 * @return char: the new address of the stored string
 */
char *strDup(char *);

/*
 * Install function to add (name, defn) pairs into the hash table
 * --------------------------------------------------------------
 * @param name: the name to be replaced
 * @param defn: the replace texts for the names
 */
struct linkedList *install(char *name, char *defn) {
    struct linkedList *np;  // Helper pointer to replace
    unsigned hashVal;   // Hash value to find index

    if ((np = lookUp(name)) == NULL) {
        // If not found
        np = (struct linkedList *) malloc(sizeof(*np)); // Allocate space for the helper pointer
        if (np == NULL || (np -> name = strDup(name)) == NULL) {
            // Not enough space for new node or string
            return NULL;
        }
        hashVal = hash(name);   // Find the index of hash table to insert
        np -> next = hashTable[hashVal];    // Connect the existing linked list with the pointer
        hashTable[hashVal] = np;    // Insert the pointer
    } else {
        // If already exists
        free((void *) np -> defn);  // Clear previous definition and free the space of storing previous definition
    }
    if ((np -> defn = strDup(defn)) == NULL) {
        //  If there is not enough space for inserting new definitions
        return NULL;
    }
    return np;
}

char *strDup(char *s) {
    char *p;

    p = (char *) malloc(strlen(s) + 1);
    if (p != NULL) {
        strcpy(p, s);
    }
    return p;
}
