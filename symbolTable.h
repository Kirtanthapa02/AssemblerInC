#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#define MAX_ENTRIES 100

struct SymbolEntry {
    char key[50];
    int address;
};

typedef struct{
    struct SymbolEntry entries[MAX_ENTRIES];
    int count;
} SymbolTable;
extern SymbolTable sym_label;
void initSymbolTable(SymbolTable *symtab);
void insertEntry(SymbolTable *symtab, const char *key,int address);
unsigned int lookupAddress(SymbolTable *symtab, const char *key);
void printSymbolTable(SymbolTable *symtab);
int lookupValue(SymbolTable *symtab, const char *label);
char *search_optab(const char *opcode);
#endif
