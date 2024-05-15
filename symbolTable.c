#include "symbolTable.h"
#include <stdio.h>
#include <string.h>

void initSymbolTable(SymbolTable *symtab) {
    symtab->count = 0;
}

void insertEntry(SymbolTable *symtab, const char *key, int address) {
    if (symtab->count < MAX_ENTRIES) {
        for (int i = 0; i < symtab->count; i++) {
            if (strcmp(symtab->entries[i].key, key) == 0) {
                printf("Error: Key '%s' already exists in the symbol table\n", key);
                return;
            }
        }
        strcpy(symtab->entries[symtab->count].key, key);
        symtab->entries[symtab->count].address = address;
        symtab->count++;
        //printf("Inserted entry: key='%s', address='%X'\n", key, address);
    } else {
        printf("Error: Symbol table full\n");
    }
}

void printSymbolTable(SymbolTable *symtab) {
    printf("Symbol Table Contents:\n");
    printf("Index\tKey\t\tAddress\n");
    printf("-----\t------------\t--------\n");
    for (int i = 0; i < symtab->count; i++) {
        printf("%d\t%s\t\t%X\n", i, symtab->entries[i].key, symtab->entries[i].address);
    }
}

int lookupValue(SymbolTable *symtab, const char *label) {
    for (int i = 0; i < symtab->count; i++) {
        if (strcmp(symtab->entries[i].key, label) == 0) {
            return symtab->entries[i].address;
        }
    }
    return -1;
}

char *search_optab(const char *opcode) {
    FILE *optab_fp = fopen("optab.txt", "r");
    if (optab_fp == NULL) {
        perror("Error opening OPTAB file");
    }

    char line[MAX_ENTRIES];
    char mnemonic[10], opcode_value[10];
    while (fgets(line, sizeof(line), optab_fp)) {
        sscanf(line, "%s %s", mnemonic, opcode_value);
        if (strcmp(mnemonic, opcode) == 0) {
            fclose(optab_fp);
            return strdup(opcode_value);
        }
    }
    fclose(optab_fp);
    return NULL;
}