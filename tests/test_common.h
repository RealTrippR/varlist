#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <llvl_env.89.h>
#include <stdio.h>
#include <string.h>

void print_data( const void* data, env_i64 len) {
    const int LINE_WIDTH = 40;
    int printlen = 0;
    env_i64 i;
    printf("\nDATA:\n");

    for (i=0; i < len; ++i) {
        char c =  ((char*)data)[i];
        printf("%02hhx ",c);
        if ((i+1)%LINE_WIDTH==0) {
            printf("\n");
            env_i64 j = 0;
            for (i-LINE_WIDTH; j < i; ++j) {
                char c = ((char*)data)[j];
                if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c>='0' && c<='9')) {
                    //printf("%c",c);
                }
                //printf(" ");
            }
            printf("\n");
        }
    }
    printf("\n");
}



void print_nodes( ENV_NODE_BASE* base , void* end) {
    printf("\n=== NODES ===\n");
    while (base)
    {
        print_data(base, ENV_SIZEOF_NODE(base));

        if (base->type == ENV_NODE_TYPE_STRING) {
            ENV_NODE_STRING* as_string = (ENV_NODE_STRING*)base;
            printf("\t === NODE: STRING ===\n");
            printf("\t NAME: %.*s\n", as_string->nameLength, as_string->name);
            printf("\t STRING: %.*s\n", as_string->valueLength, as_string->value);
            printf("\n");
        }
        else if (base->type == ENV_NODE_TYPE_I32) {
            ENV_NODE_I32* as_i32 = (ENV_NODE_I32*)base;
            printf("\t === NODE: I32 ===\n");
            printf("\t NAME: %.*s\n", as_i32->nameLength, as_i32->name);
            printf("\t VALUE: %d\n", as_i32->value);
            printf("\n");
        }
        else if (base->type == ENV_NODE_TYPE_I64) {
            ENV_NODE_I64* as_i64 = (ENV_NODE_I64*)base;
            printf("\t === NODE: I64 ===\n");
            printf("\t NAME: %.*s\n", as_i64->nameLength, as_i64->name);
            printf("\t VALUE: %lld\n", as_i64->value);
            printf("\n");
        }
        else if (base->type == ENV_NODE_TYPE_F32) {
            ENV_NODE_F32* as_f32 = (ENV_NODE_F32*)base;
            printf("\t === NODE: F32 ===\n");
            printf("\t NAME: %.*s\n", as_f32->nameLength, as_f32->name);
            printf("\t VALUE: %f\n", as_f32->value);
            printf("\n");
        }
        else if (base->type == ENV_NODE_TYPE_F64) {
            ENV_NODE_F64* as_f64 = (ENV_NODE_F64*)base;
            printf("\t === NODE: F64 ===\n");
            printf("\t NAME: %.*s\n", as_f64->nameLength, as_f64->name);
            printf("\t VALUE: %lf\n", as_f64->value);
            printf("\n");
        }
        if (base->type < 1 || base->type > 5) {
            return;
        }

        base = base + ENV_SIZEOF_NODE(base);
        if ((char*)base>=(char*)end-1)
            break;
    }
}


#endif