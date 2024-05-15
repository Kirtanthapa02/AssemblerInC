#ifndef PASS1_H
#define PASS1_H

#define MAX_LINE_LENGTH 1000

extern int program_length;

void AssemblerPass1(const char* code_file,const char* intermediate_File);
void AssemblerPass2(const char *IntermediateFile);
char *do_index_addressing(int num);
char* extract_chars(const char *str);
char* string_to_hex(const char *str) ;
void writeTextRecord(FILE *write,char *object_code, int start_address,char *opcode);
void makeSixDigit(char* input);

#endif