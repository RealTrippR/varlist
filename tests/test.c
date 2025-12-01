#include <llvl_env.89.h>
#include <stdio.h>
#include <string.h>
#include "test_common.h"
#define VAR_FILE "example.varlist"



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



    
    env_size_t needed_bytes = 0;


    printf("ENV_CHECK_VALIDITY MUST ALSO ENSURE THAT NO MORE THAN 300 DIGITS ARE PRESENT IN A NUMBER.\n");
    
    env_i32 offendingLines[256];
    ENV_RESULT result = ENV_CHECK_VALIDITY(fcontents, flen, offendingLines, sizeof(offendingLines));
     if (result<0) {
        printf(".env structure is invalid.\n");

        printf("offending lines:\n");
        env_i32 i;
        for (i=0; i < 256; ++i) {
            if (offendingLines[i]==-1) {
                break;
            }
            printf(" %d",offendingLines[i]);
        }
        printf("\n");

        return -2;
    }



    result = ENV_PARSE(fcontents, flen, &needed_bytes, NULL);
    if (result<0) {
        printf("Failed to parse .env file: %s\n", VAR_FILE);
        return -2;
    }

    if (needed_bytes>0) 
    {
        env_i8 structure_buffer[needed_bytes];
        result = ENV_PARSE(fcontents, flen, &needed_bytes, structure_buffer);

        if (result<0) {
            printf("Failed to parse .env file: %s\n", VAR_FILE);
            return -2;
        }
        
        printf("Successfully parsed .env file: %s", VAR_FILE);

        print_nodes((ENV_NODE_BASE*)structure_buffer,structure_buffer+needed_bytes);
    } 
    else {
        printf(".env file is empty.\n");
    }








    return 0;
}