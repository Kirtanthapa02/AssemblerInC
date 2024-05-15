#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pass1.h"
#include "symbolTable.h"

int program_length = 0;
SymbolTable sym_label;
void AssemblerPass1(const char* code_file,const char* intermediate_File)
{
    FILE *read,*write;
    char line[MAX_LINE_LENGTH];
    read = fopen(code_file, "r");
    write = fopen(intermediate_File,"w");
    if (read == NULL)
    {
        printf("Error reading file.\n");
        return;
    }

    int LocCTR = 0000;
    int Start_address = 0;

    initSymbolTable(&sym_label);

    while (fgets(line, MAX_LINE_LENGTH, read) != NULL)
    {
        char label[50], opcode[50], operand[50];
        if (line[0] != ' ')
        {
            sscanf(line, "%s %s %s", label, opcode, operand);
        }
        else if (sscanf(line, "%s %s", opcode, operand) == 2)
        {
            label[0] = '\0';
        }
        else if (sscanf(line, "%s", opcode) == 1)
        {
            label[0] = '\0'; 
            operand[0] = '\0';
        }

        if (strcmp(opcode, "START") == 0)
        {
            fprintf(write,"\t\t\t%s\t\t%s\t\t%s\n",label,opcode,operand);
            Start_address = atoi(operand);
            LocCTR = Start_address;
            continue;
        }
        else if (strcmp(opcode, "END") == 0)
        {
            fprintf(write,"%X\t\t%s\t\t%s\t\t%s\n",LocCTR,label,opcode,operand);
            break;
        }
        fprintf(write,"%X\t\t%s\t\t%s\t\t%s\n",LocCTR,label,opcode,operand);

        if (strlen(label) > 0)
        {
            int sym_value = lookupValue(&sym_label, label);
            if (sym_value == -1)
            {
                insertEntry(&sym_label, label, LocCTR);
            }
            else
            {
                printf("Symbol already present.\n");
            }
        }

        if (strcmp(opcode, "START") != 0)
        {
            const char *optab_value = search_optab(opcode);
            if (optab_value == NULL)
            {
                if (strcmp(opcode, "WORD") == 0)
                {
                    LocCTR += 3;
                }
                else if (strcmp(opcode, "RESW") == 0)
                {
                    LocCTR += 3 * atoi(operand);
                }
                else if (strcmp(opcode, "RESB") == 0)
                {
                    LocCTR += atoi(operand);
                }
                else if (strcmp(opcode, "BYTE") == 0)
                {
                    char* extracted_character = extract_chars(operand);
                    if(strcmp(extracted_character,"F1")==0)
                    {
                        LocCTR += 1;
                    }
                    else
                    {
                        int const_length = strlen(operand) - 3;
                        LocCTR += const_length;
                    }
                }
                else
                {
                    printf("Opcode doesn't exist.\n");
                }
            }
            else
            {
                LocCTR += 3;
            }
        }
    }
    program_length = LocCTR - Start_address;
    //printf("Length of program is %X\n", program_length);
    fclose(read);
    fclose(write);
}
char* extract_chars(const char *str) {
    int index = 0;
    int i = 0;
    char *extracted_chars = (char *)malloc(strlen(str) * sizeof(char));

    if (extracted_chars == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    while (str[i] != '\0') {
        if (str[i] == '\'') {
            i++;
            while (str[i] != '\'' && str[i] != '\0') {
                extracted_chars[index++] = str[i++];
            }
            break;
        }
        i++;
    }
    extracted_chars[index] = '\0';
    return extracted_chars;
}