#include <llvl_var.89.h>
#include <stdio.h>
#include <string.h>
#include "test_common.h"
#define VAR_FILE "edge_cases.varlist"



int main(argc, argv)
    const int argc;
    const char* const* argv;
{
    FILE* fptr = fopen(VAR_FILE, "rb");

    if (!fptr) {
        printf("Failed to open file: %s\n", VAR_FILE);
        return -1;
    }

    fseek(fptr, 0, SEEK_END);
    int flen = ftell(fptr);
    rewind(fptr);
    char fcontents[flen];
    fread(fcontents, 1, flen, fptr);
    fclose(fptr);



    
    var_size_t structure_size = 0;


    printf("ENV_CHECK_VALIDITY MUST ALSO ENSURE THAT NO MORE THAN 300 DIGITS ARE PRESENT IN A NUMBER.\n");
    
    var_i32 offendingLines[256];
    VAR_RESULT result = VAR_CHECK_VALIDITY(fcontents, flen, offendingLines, sizeof(offendingLines));
     if (result<0) {
        printf(".env structure is invalid.\n");

        printf("offending lines:\n");
        var_i32 i;
        for (i=0; i < 256; ++i) {
            if (offendingLines[i]==-1) {
                break;
            }
            printf(" %d",offendingLines[i]);
        }
        printf("\n");

        return -2;
    }



    result = VAR_PARSE(fcontents, flen, &structure_size, NULL);
    if (result<0) {
        printf("Failed to parse .env file: %s\n", VAR_FILE);
        return -2;
    }

    if (structure_size>0) 
    {
        var_i8 structure_buffer[structure_size];
        result = VAR_PARSE(fcontents, flen, &structure_size, structure_buffer);

        if (result<0) {
            printf("Failed to parse .env file: %s\n", VAR_FILE);
            return -2;
        }
        
        printf("Successfully parsed .env file: %s", VAR_FILE);

        var_size_t string_size=0;
        result = VAR_STORE_STRINGS(structure_buffer, structure_size, NULL, &string_size, false);

        var_i8 str_buffer[string_size];
        result = VAR_STORE_STRINGS(structure_buffer, structure_size, str_buffer, &string_size, false);




        print_nodes((VAR_NODE_BASE*)structure_buffer,structure_buffer+structure_size);
    } 
    else {
        printf(".env file is empty.\n");
    }








    return 0;
}