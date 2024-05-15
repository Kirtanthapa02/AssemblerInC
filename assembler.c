#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbolTable.h"
#include "pass1.h"

int main()
{
    const char* inputfile = "sic_code.asm";
    const char* intermediatefile = "intermediate.asm";
    AssemblerPass1(inputfile,intermediatefile);
    AssemblerPass2(intermediatefile);

    return 0;
}