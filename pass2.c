#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pass1.h"
#include "symbolTable.h"

void AssemblerPass2(const char *IntermediateFile)
{
    SymbolTable symbol;
    initSymbolTable(&symbol);
    FILE *readfile,*writefile;
    char line[MAX_LINE_LENGTH];
    int LocCTR = 0,start_address=0;
    readfile = fopen(IntermediateFile,"r");
    writefile = fopen("object_program.txt","w");
    if (readfile == NULL)
    {
        printf("Error opening file.\n");
        return;
    }
    while(fgets(line,MAX_LINE_LENGTH,readfile) != NULL)
    {
        char label[50] = {'\0'};
        char opcode[50] = {'\0'};
        char operand[50] = {'\0'};
        char object_code[50] = {'\0'};

        if(line[0]=='\t')
        {
            sscanf(line,"%s %s %s",label,opcode,operand);
        }
        else if(sscanf(line,"%X %s %s %s",&LocCTR,label,opcode,operand)==4)
        {

        }
        else
        {
            label[0] = '\0';
            sscanf(line,"%X %s %s",&LocCTR,opcode,operand);
        }

        if(strcmp(opcode,"START")==0)
        {
            start_address = atoi(operand);
            fprintf(writefile,"H^%s^%06X^%06X",label,start_address,program_length);
            continue;
        }
        else if(strcmp(opcode,"END")==0)
        {
            fprintf(writefile,"\nE^%06X\n",start_address);
            break;
        }
        char *optab_value = search_optab(opcode);
        if(optab_value != NULL)
        {
            int symbol_value = lookupValue(&sym_label,operand);
            if(symbol_value != -1)
            {
                char temp_value[50];
                strcpy(object_code,optab_value);
                sprintf(temp_value,"%X",symbol_value);
                strcat(object_code,temp_value);
            }
            else if(strstr(operand, ",X") != NULL)
            {
                char operand_copy[20];
                strcpy(operand_copy,operand);
                char *token,hex_value[10];
                token = strtok(operand,",");
                symbol_value = lookupValue(&sym_label,token);
                char *address = do_index_addressing(symbol_value);
                int decimal_value = strtol(address, NULL, 2);
                sprintf(hex_value,"%X",decimal_value);
                strcpy(object_code,optab_value);
                strcat(object_code,hex_value);
            }
            else
            {
                strcpy(object_code,optab_value);
                strcat(object_code,"0000");
            }
        }
        else if((strcmp(opcode,"BYTE")==0))
        {
            if(strstr(operand, "C\'") != NULL){
                char* extracted_characters = extract_chars(operand);
                char* hex_code = string_to_hex(extracted_characters);
                writeTextRecord(writefile,hex_code,LocCTR,opcode);
                //printf("%X\t\t%s\t\t%s\t\t%s\t\t%s\n",LocCTR,label,opcode,operand,hex_code);
                continue;
            }
            else if(strstr(operand, "X\'") != NULL)
            {
                char* extracted_characters = extract_chars(operand);
                writeTextRecord(writefile,extracted_characters,LocCTR,opcode);
                //printf("%X\t\t%s\t\t%s\t\t%s\t\t%s\n",LocCTR,label,opcode,operand,extracted_characters);
                continue;
            }
        }
        else if((strcmp(opcode,"WORD")==0))
        {
            int op = atoi(operand);
            sprintf(object_code,"%X",op);
        }
        if(strlen(object_code)<6)
        {
            makeSixDigit(object_code);
        }
        //printf("%s\n",opcode);
        writeTextRecord(writefile,object_code,LocCTR,opcode);
        //printf("%X\t\t%s\t\t%s\t\t%s\t\t%s\n",LocCTR,label,opcode,operand,object_code);
    }
    fclose(readfile);
    fclose(writefile);
}
void makeSixDigit(char* input) {
    int length = strlen(input);

    if (length < 6) {
        int leadingZeroes = 6 - length;
        memmove(input + leadingZeroes, input, length + 1);
        memset(input, '0', leadingZeroes);
    }
}
void writeTextRecord(FILE *write,char *object_code, int start_address, char* opcode)
{
    static int len = 0;
    static int flag = 1;
    static int temp_address = 0;

    if(strcmp(opcode,"RESB")==0||strcmp(opcode,"RESW")==0)
    {
        len = 0;
        return;
    }
    else if(len==60)
    {
        len = 0;
    }
    else if(strcmp(object_code,"F1")==0)
    {
        len = len + 4;
    }
    if(len==0 && flag ==1)
    {
        temp_address = start_address;
        flag = 0;
    }
    else if(len == 0)
    {
        int text_length = start_address - temp_address;
        temp_address = start_address;
        fprintf(write,"\nT^%06X^%X^",start_address,text_length);
    }
    fprintf(write,"%s^",object_code);
    len = len + strlen(object_code);
}
char* string_to_hex(const char *str) {
    char *hex_string = (char *)malloc(strlen(str) * 2 + 1);
    
    if (hex_string == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    
    for (int i = 0; i < strlen(str); i++) {
        sprintf(hex_string + i * 2, "%02x", str[i]);
    }
    hex_string[strlen(str) * 2] = '\0';
    
    return hex_string;
}

char *do_index_addressing(int num) {
    char *binary = (char *)malloc(16 * sizeof(char));
    char *result = (char *)malloc(strlen(binary) + 2);
    if (binary == NULL) {
        printf("Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
    binary[15] = '\0';
    int bitIndex = 14;
    while (bitIndex >= 0) {
        int bit = num & 1;
        binary[bitIndex] = (bit == 0) ? '0' : '1';
        num >>= 1;
        bitIndex--;
    }
    result[0] = '1';
    strcpy(result + 1, binary);


    return result;
}