/*
Robins Free of Charge & Open Source Public License 25

Copyright (C), 2025 - Tripp R. All rights reserved.

Permission for this software, the "software" being source code, binaries, and documentation,
shall hereby be granted, free of charge, to be used for any purpose, including commercial applications,
modification, merging, and redistrubution. The software is provided 'as-is' and comes without any
express or implied warranty. This license is valid under the following restrictions:

1. The origin of the software must not be misrepresentented; the true author(s) of the software
must be attributed as such. This applies every alteration of the "software", the name(s)
of the authors(s) of any alterations must be appended to the list of names of
the author(s) of the version of the preceding software which the alteration is based upon.

2. This license must be included in all redistributions of the software source.

3. All distributions of altered forms of the software must be clearly marked as such.

4. The author(s) of this software and all subsequent alterations hold no responsibility for any
damages that may result from use of the software.

5. The software shall not be used for the purpose of training LLMs ("Large Language Models"),
be included in datasets used for the purpose of training AI, or be used in the advancement of any
form of Artificial Intelligence.
*/

#include "../llvl_var.89.h"
#include "../pow_lookup.89.h"
#include <stdint.h>
#include "../deps/streql/streqlasm.h"
#ifndef NDEBUG
#include <string.h>
#endif

#if defined(__GNUC__) || defined(_WIN64)
    #if defined(__x86_64__) || defined(_M_X64)
        #define C_VAR_IS_ASM___WIN_OR_GCC_x64
    #endif
#endif



#if defined(C_VAR_IS_ASM___WIN_OR_GCC_x64)
extern char _var_is_space_c_entry(char);
static char var_is_space(char c) {
    return _var_is_space_c_entry(c);
}
#else
static char var_is_space(char c) {
    if (c==32 || c==13 ||c==10||c==9||c==12) {
        return 1;
    }
    return 0;
}
#endif

var_i32 _var_atoi32(char* a, var_i64 digits) 
{
    char m = 0;
    char neg = 0;
    if (*a=='-') {
        a++;
        digits--;
        neg = 1;
    }
    if (digits>1&&*(a+1) == 'o') {
        m = 1;
        a+=2;
        digits-=2;
    }
    else if (digits>1&&*(a+1) == 'x') {
        m = 2;
        a+=2;
        digits-=2;
    }
    var_i32 n = 0;
    var_i8 i;
    for (i=0; i<digits ;++i) {
        var_i64 f;
        var_i8 v;
        switch (m) {
        case 0:
            f = n_pow10(digits-i-1);
            v = (a[i]-48);
            break;
        case 1:
            f = n_pow8(digits-i-1);
            v = (a[i]-48);
            break;
        case 2:
            f = n_pow16(digits-i-1);
            // hex is a special case.
           
            if (a[i]>='a') {
                v = 10 + a[i]-'a';
            }
            else if (a[i]>='A') {
                v = 10 + a[i]-'A';
            } else {
                v = (a[i]-48);
            }
            break;
        }
        n += v * f;
    }
    if (neg) {
        n=-n;
    }
    return n;
}

var_i64 _var_atoi64(a, digits) 
char *a;
var_i64 digits;
{
    char m = 0;
    char neg = 0;
    if (*a=='-') {
        a++;
        digits--;
        neg = 1;
    }
    if (digits>1&&*(a+1) == 'o') {
        m = 1;
        a+=2;
        digits-=2;
    }
    else if (digits>1&&*(a+1) == 'x') {
        m = 2;
        a+=2;
        digits-=2;
    }
    var_i64 n = 0;
    var_i8 i;
    for (i=0; i<digits ;++i) {
        var_i64 f;
        var_i8 v;
        switch (m) {
        case 0:
            f = n_pow10(digits-i-1);
            v = (a[i]-48);
            break;
        case 1:
            f = n_pow8(digits-i-1);
            v = (a[i]-48);
            break;
        case 2:
            f = n_pow16(digits-i-1);
            // hex is a special case.
            v = (a[i]-48);
            if (v>'a') {
                v = 10 + v-'a';
            }
            break;
        }
        n += v * f;
    }
    if (neg) {
        n=-n;
    }
    return n;
}

var_f32 _var_atof32(a, len) 
char *a;
var_i64 len;
{
    char neg = 0;
    if (*a=='-') {
        a++;
        len--;
        neg = 1;
    }
    var_i8 i;
    // get . indx
    var_i8 dt_i = len;
    for (i=0; i<len ;++i) {
        if (a[i]=='.') {
            dt_i = i;
            break;
        }
    }

    var_f32 f = 0;
    for (i=0; i<len ;++i) {
        if (i<dt_i) {
            double p = n_pow10d(dt_i-i-1);
            f += (a[i]-48) * p;
        }
        else if (i>dt_i) {
            double p = n_pow10d(i-dt_i);
            f += (a[i]-48) / p;
        }
    }
    if (neg) {
        f=-f;
    }
    return f;
}

var_f64 _var_atof64(a, len) 
char *a;
var_i64 len;
{
    char neg = 0;
    if (*a=='-') {
        a++;
        len--;
        neg = 1;
    }
    var_i16 i;
    // get . indx
    var_i16 dt_i = len;
    for (i=0; i<len ;++i) {
        if (a[i]=='.') {
            dt_i = i;
            break;
        }
    }

    var_f64 f = 0;
    for (i=0; i<len ;++i) {
        if (i<dt_i) {
            double p = n_pow10d(dt_i-i-1);
            f += (a[i]-48) * p;
        }
        else if (i>dt_i) {
            double p = n_pow10d(i-dt_i);
            f += (a[i]-48) / p;
        }
    }
    if (neg) {
        f=-f;
    }
    return f;
}

VAR_RESULT VAR_CHECK_VALIDITY(data, data_len, offending_line_buffer, offending_line_buffer_size)
    const var_i8*   data;
    var_size_t      data_len;
    var_i32*        offending_line_buffer;
    var_i32         offending_line_buffer_size;
{
    var_i64 offending_line_count=0;
    var_i64 offending_line_buffer_max_count = offending_line_buffer_size/sizeof(var_i32);

    var_i32* offending_line_buffer_end = offending_line_buffer + offending_line_buffer_size;

    var_i32 i;
    for (i=0; i<offending_line_buffer_max_count; ++i) {
        offending_line_buffer[i]=-1;
    }

    const var_i8* data_end = data + data_len;
    
    const var_i8* key = NULL;
    var_i16 keyLength = 0;

    const var_i8* value = NULL;
    var_i64 valueLength = 0;

    var_i8 state = 0; // state 0: key
                      // state 1: transient
                      // state 2: value

    var_i32 line = 1;
    var_i32 last_offending_line = -1;

    var_i32 negativeSignCount;

    for (; data<data_end; ++data)
    {
        // first check if this line is a key
        // it is only if it has a ':'
        const char* tmp = data;
        for (; tmp < data_end; ++tmp) {
            if (*tmp == '\n') {
                data=tmp;
                state = 0;
                break; // skip to next line
            } else if (*tmp == ':') {
                // is valid key/value pair
                break;
            }
        }


        if (state == 0 && *data == '<') {
            const char* end = NULL;
            const char* cursor = data;
            for (; cursor<data_end; ++cursor)
            {
                if (*cursor == '\n' || *cursor == ':') {
                    break;
                }
                if (*cursor == '>') {
                    end = cursor;
                    break;
                }
            }
            if (!end) {
                if (offending_line_buffer_max_count && last_offending_line != line && offending_line_count <= offending_line_buffer_max_count-1) {
                    offending_line_buffer[offending_line_count] = line;
                    offending_line_count++;
                    last_offending_line=line;
                }
            } else {
                if (end - data < 4) {
                    if (offending_line_buffer_max_count && last_offending_line != line && offending_line_count <= offending_line_buffer_max_count-1) {
                        offending_line_buffer[offending_line_count] = line;
                        offending_line_count++;
                        last_offending_line=line;
                    }
                } else {
                    const char* interior = data+1;
                    if (!(strneql(interior, "i32",3) || strneql(interior, "i64",3) || strneql(interior, "f32",3) || strneql(interior, "f64",3) || strneql(interior, "str",3))) {
                        if (offending_line_buffer_max_count && last_offending_line != line && offending_line_count <= offending_line_buffer_max_count-1) {
                            offending_line_buffer[offending_line_count] = line;
                            offending_line_count++;
                            last_offending_line=line;
                        }
                    }
                }

                data=end;
            }
        } else {
            if (*data == '\n' || data == data_end-1) {
                line++;
                valueLength = 0;
                state = 0;
            }
            else if (state == 0) {
                if (*data==':') {
                    state = 1;
                } 
                else if (!var_is_space(*data)) {
                    if (keyLength==0) {
                        key = data;
                    }
                    // check key validity
                    if (!((*data<='Z' && *data>='A')||(*data<='z' && *data>='a')||(*data <= '9' && *data >= '0')||*data=='_')) {
                        if (offending_line_buffer_max_count && last_offending_line != line && offending_line_count <= offending_line_buffer_max_count-1) {
                            offending_line_buffer[offending_line_count] = line;
                            offending_line_count++;
                            last_offending_line=line;
                        }
                    } 
                    
                    keyLength++;

                    if (keyLength>=UINT16_MAX) {
                        if (offending_line_buffer_max_count && last_offending_line != line && offending_line_count <= offending_line_buffer_max_count-1) {
                            offending_line_buffer[offending_line_count] = line;
                            offending_line_count++;
                            last_offending_line=line;
                        }
                    }
                }
            } else if (state==1) {
                if (!var_is_space(*data)) {
                    state = 2;
                    value = data;
                }
            } else {
                valueLength++;
                if (valueLength>=UINT64_MAX) {
                    if (offending_line_buffer_max_count && last_offending_line != line && offending_line_count <= offending_line_buffer_max_count-1) {
                        offending_line_buffer[offending_line_count] = line;
                        offending_line_count++;
                        last_offending_line=line;
                    }
                }
            }
        }
    }

    offending_line_buffer[offending_line_count] = -1;

    if (offending_line_count>0) {
        return VAR_ERR_INVALID;
    }
    return VAR_SUCCESS;
}


var_i8 VAR_SIZEOF_NODE(const void* node)
{
    var_i8 tbl[] = {
        0,
        sizeof(VAR_NODE_I32),
        sizeof(VAR_NODE_I64),
        sizeof(VAR_NODE_F32),
        sizeof(VAR_NODE_F64),
        sizeof(VAR_NODE_STRING)
    };

    var_i8 t = *(var_i8*)node;
    return tbl[t];
}


VAR_RESULT VAR_STORE_STRINGS(void *structure, var_size_t structure_size, void *string_buffer, var_i64 *string_size, char null_terminate)
{
    const void* structure_buffer_end = (char*)structure + structure_size;
    if (!string_buffer) {
        while (structure < structure_buffer_end)
        {
            VAR_NODE_BASE* as_base = structure;
            if (as_base->type == VAR_NODE_TYPE_STRING) {
                VAR_NODE_STRING* as_str = (VAR_NODE_STRING*)structure;
                (*string_size) += as_str->valueLength;
                (*string_size) += as_str->nameLength;
                if (null_terminate) {
                    (*string_size)++;
                }
            }
            else if (as_base->type == VAR_NODE_TYPE_I32 || as_base->type == VAR_NODE_TYPE_F32) {
                VAR_NODE_I32* as_i32 = (VAR_NODE_I32*)as_base;
                (*string_size) += as_i32->nameLength;
            }
            else if (as_base->type == VAR_NODE_TYPE_I64 || as_base->type == VAR_NODE_TYPE_F64) {
                VAR_NODE_F64* as_i64 = (VAR_NODE_F64*)as_base;
                (*string_size) += as_i64->nameLength;
            }

            structure = (char*)structure + VAR_SIZEOF_NODE(structure);    
        }
        
        
    } else {
        while (structure < structure_buffer_end)
        {
            VAR_NODE_BASE* as_base = structure;
            if (as_base->type == VAR_NODE_TYPE_STRING) {
                VAR_NODE_STRING* as_str = (VAR_NODE_STRING*)structure;
                memcpy(string_buffer, as_str->value, as_str->valueLength);
                as_str->value = string_buffer;
                string_buffer = (char*)string_buffer + as_str->valueLength;
                if (null_terminate) {
                    *((char*)string_buffer) = '\0';
                    string_buffer = (char*) string_buffer + 1;
                }

                memcpy(string_buffer, as_str->name, as_str->nameLength);
                as_str->name = string_buffer;
                string_buffer = (char*)string_buffer + as_str->nameLength;
                if (null_terminate) {
                    *((char*)string_buffer) = '\0';
                    string_buffer = (char*) string_buffer + 1;
                }
            }
            else if (as_base->type == VAR_NODE_TYPE_I32 || as_base->type == VAR_NODE_TYPE_F32) {
                VAR_NODE_I32* as_i32 = (VAR_NODE_I32*)as_base;
                
                memcpy(string_buffer, as_i32->name, as_i32->nameLength);
                as_i32->name = string_buffer;
                string_buffer = (char*)string_buffer + as_i32->nameLength;
                if (null_terminate) {
                    *((char*)string_buffer) = '\0';
                    string_buffer = (char*) string_buffer + 1;
                }
            }
            // VAR_NODE_TYPE_F64 or VAR_NODE_TYPE_I64
            else {
                VAR_NODE_I64* as_i64 = (VAR_NODE_I64*)as_base;

                memcpy(string_buffer, as_i64->name, as_i64->nameLength);
                as_i64->name = string_buffer;
                string_buffer = (char*)string_buffer + as_i64->nameLength;
                if (null_terminate) {
                    *((char*)string_buffer) = '\0';
                    string_buffer = (char*) string_buffer + 1;
                }
            }
            var_i8 s = VAR_SIZEOF_NODE(structure);
            structure = (char*)structure + s;
        }
    }
    return VAR_SUCCESS;
}






#if defined( __GNUC__ )
__attribute__((ms_abi))
#endif
extern int _var_parse_arm64();

#if !defined(C_VAR_IS_ASM___WIN_OR_GCC_x64)
    static int var_parse();

    static int finalize_pair(uint8_t* key_begin, uint16_t key, uint8_t* val, uint64_t val_len, char forced_type, var_i8* node_buffer);
    
#endif

VAR_RESULT VAR_PARSE(data, data_len, length_used, node_buffer)
    const var_i8*   data; 
    var_size_t      data_len; 
    var_size_t*     length_used;
    var_i8*         node_buffer;
{

    #if defined(C_VAR_IS_ASM___WIN_OR_GCC_x64)
    VAR_RESULT res = _var_parse_arm64(data, data_len, length_used, node_buffer);
    #else
    VAR_RESULT res = var_parse(data,data_len,length_used,node_buffer);
    #endif

    return res;
}

#if !defined(C_VAR_IS_ASM___WIN_OR_GCC_x64)

static int var_parse(data, data_len, length_used, node_buffer) 
    const var_i8*   data; 
    var_size_t      data_len; 
    var_size_t*     length_used;
    var_i8*         node_buffer;
{
    *length_used = 0;
    const char* key = 0;
    uint16_t key_len = 0;
    
    const char* value = 0;
    size_t value_len = 0;

    char state = 0; // 0: key
                    // 1: transient
                    // 2: value

    char forced_type_state = 0; // 0: not forced
                                // 1: i64 
                                // 2: i32
                                // 3: f64
                                // 4: f32
                                // 5: str
    const var_i8* cur = data;
    const var_i8* data_end = data + data_len;
    while (cur <= data_end)
    {
        if (*cur == '\r' || *cur == '\n' || cur == data_end) {

            if (state == 2 && key_len > 0 && value_len > 0) {
                int n = finalize_pair(key, key_len, value, value_len, forced_type_state, node_buffer);;
                *length_used += n;
                if (node_buffer) {
                    node_buffer+=n;
                }
            }

            state=0;
            forced_type_state = 0;
            key = 0;
            key_len = 0;
            value = 0;
            value_len = 0;
        }
        else {
            if (state == 0) {
                char is_space = var_is_space(*cur);
                if (!key && !is_space && *cur!=':') {
                    key = cur;
                }
                if (*cur == ':' || *cur == '<') {
                    state = 1;
                } else if (!is_space) {
                    key_len++;
                }
            }
            if (state == 1) {
                if (*cur == '<') {
                    cur++;
                    if (cur[0] == 's') {
                        forced_type_state = 5;
                    } else
                    if (cur[0] == 'i'){
                        if (cur[2] == '2') {
                            forced_type_state = 2;
                        } else {
                            forced_type_state = 1;
                        }
                    } else {
                        if (cur[2] == '2') {
                            forced_type_state = 4;
                        } else {
                            forced_type_state = 3;
                        }
                    }
                    cur+=4;
                } 
                else if (!var_is_space(*cur) && *cur!=':') {
                    value = cur;
                    state = 2;
                    value_len++;
                }
            }
            else if (state == 2) {
                value_len++;
            }
        }

        cur++;
    }

    return VAR_SUCCESS;
}



// return values:
// 0: not numeric
// 1: decimal
// 2: float
// 'o': octal
// 'x': hex
static char is_numeric(uint8_t* val, uint64_t val_len)
{
    if (*val == '-') {
        val++;
        val_len--;
    }

    if (val_len > 1) {
        if (val[0] == '0' && val[1] == 'x') {
            return 'x';
        } else
        if (val[0] == '0' && val[1] == 'o') {
            return 'o';
        }
    }


    char f = 0;
    char has_period = 0;
    uint8_t* val_end = val + val_len;
    while (val < val_end)
    {
          if (*val == '.') {
            if (has_period) {
                return 0;
            }
            has_period = 1;
            f = 1;
        } else
        if (*val < '0' || *val > '9') {
            return 0;
        }
      
        val++;
    }
    return f+1;
}


static int finalize_pair(key, key_len, val, val_len, type_state, node_buffer)
    uint8_t* key;
    uint16_t key_len;
    uint8_t* val;
    uint64_t val_len;
    char     type_state;
    var_i8*  node_buffer;
{
    if (type_state == 0) {
        // deduce type
        char type = is_numeric(val,val_len);
        char is_32 = 1;
        if (type==0) {
            goto as_str;
        } else
        if (type==1) { // int
            var_i64 nlen = val_len;
            var_i8* n = val;
            if (val[0] == '-') {
                nlen--;
                n++;
            }

            // compare to i32 max (ASCII)
            var_i8* cmp_to = "2147483647";
            if (nlen < strlen(cmp_to)) {
                goto as_i32;
            } else if (nlen > strlen(cmp_to)) {
                goto as_i64;
            }

            var_i8* cur = n+nlen-1;
            var_i8* cmp = cmp_to + strlen(cmp_to)-1;
            for (; cur >= n; cur--) {
                if (*cur < *cmp) {
                    goto as_i32;
                }
                if (*cur > *cmp) {
                    goto as_i64;
                }
                if (cmp == cmp_to){
                    break;
                }
                cmp--;
            }
            goto as_i32; // equal
        } else
        if (type==2) { // float
            if (val_len < 10) {
                goto as_f32;
            }
             goto as_f64;
        } else
        if (type=='o') {
            // compare to i32 max (as octal)
            var_i64 nlen = val_len-2;
            var_i8* n = val+2;
            if (val[0] == '-') {
                nlen--;
                n++;
            }

            // compare to i32 max
            var_i8* cmp_to = "17777777777";
            if (nlen < strlen(cmp_to)) {
                goto as_i32;
            } else if (nlen > strlen(cmp_to)) {
                goto as_i64;
            }

            var_i8* cur = n+nlen-1;
            var_i8* cmp = cmp_to + strlen(cmp_to)-1;
            for (; cur >= n; cur--) {
                if (*cur < *cmp) {
                    goto as_i32;
                }
                if (*cur > *cmp) {
                    goto as_i64;
                }
                if (cmp == cmp_to){
                    break;
                }
                cmp--;
            }
            goto as_i32; // equal
        } else
        if (type=='x') {
            // compare to i32 max (as hex)

            var_i64 nlen = val_len-2;
            var_i8* n = val+2;
            if (val[0] == '-') {
                nlen--;
                n++;
            }

            // compare to i32 max
            var_i8* cmp_to = "7FFFFFFF";
            if (nlen < strlen(cmp_to)) {
                goto as_i32;
            } else if (nlen > strlen(cmp_to)) {
                goto as_i64;
            }

            var_i8* cur = n+nlen-1;
            var_i8* cmp = cmp_to + strlen(cmp_to)-1;
            for (; cur >= n; cur--) {
                if (*cur < *cmp) {
                    goto as_i32;
                }
                if (*cur > *cmp) {
                    goto as_i64;
                }
                if (cmp == cmp_to){
                    break;
                }
                cmp--;
            }
            goto as_i32; // equal
        }     
    }
    else if (type_state == 1) {
as_i64:
        // i64
        if (node_buffer) {
            VAR_NODE_I64* s = (VAR_NODE_I64*)node_buffer;
            s->name = key;
            s->nameLength = key_len;
            s->type = VAR_NODE_TYPE_I64;
            s->value = _var_atoi64(val, val_len);
        }
        return sizeof(VAR_NODE_I64);
    }
    else if (type_state == 2) {
as_i32:
        // i32
        if (node_buffer) {
            VAR_NODE_I32* s = (VAR_NODE_I32*)node_buffer;
            s->name = key;
            s->nameLength = key_len;
            s->type = VAR_NODE_TYPE_I32;
            s->value = _var_atoi32(val, val_len);
        }
        return sizeof(VAR_NODE_I32);
    }
    else if (type_state == 3) {
as_f64:
        // f64
        if (node_buffer) {
            VAR_NODE_F64* s = (VAR_NODE_F64*)node_buffer;
            s->name = key;
            s->nameLength = key_len;
            s->type = VAR_NODE_TYPE_F64;
            s->value = _var_atof64(val, val_len);
        }
        return sizeof(VAR_NODE_F64);
    }
    else if (type_state == 4) {
as_f32:
        // f32
        if (node_buffer) {
            VAR_NODE_F32* s = (VAR_NODE_F32*)node_buffer;
            s->name = key;
            s->nameLength = key_len;
            s->type = VAR_NODE_TYPE_F32;
            s->value = _var_atof32(val, val_len);
        }
        return sizeof(VAR_NODE_F32);
    }
    else if (type_state == 5) {
as_str:
        // str
        if (node_buffer) {
            VAR_NODE_STRING* s = (VAR_NODE_STRING*)node_buffer;
            s->name = key;
            s->nameLength = key_len;
            s->type = VAR_NODE_TYPE_STRING;
            s->value = val;
            s->valueLength = val_len;
        }
        return sizeof(VAR_NODE_STRING);
    }
    return 0;
}

#endif

VAR_NODE_I32* VAR_GET_NODE_I32(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size)
{
    VAR_NODE_BASE* base = (VAR_NODE_BASE*)structure_buffer;
    const void* end = structure_buffer_size + structure_buffer;
    int namelen = strlen(name);
    while (base)
    {
        if (base->type == VAR_NODE_TYPE_I32) {
            VAR_NODE_I32* as_i32 = (VAR_NODE_I32*)base;
            if (as_i32->nameLength == namelen && strneql(as_i32->name, name, namelen)) {
                return as_i32;
            }
        }
        base = base + VAR_SIZEOF_NODE(base);
        if ((char*)base>=(char*)end-1)
            break;
    }
    return NULL;
}

VAR_NODE_I64* VAR_GET_NODE_I64(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size)
{
    VAR_NODE_BASE* base = (VAR_NODE_BASE*)structure_buffer;
    const void* end = structure_buffer_size + structure_buffer;
    int namelen = strlen(name);
    while (base)
    {
        if (base->type == VAR_NODE_TYPE_I64) {
            VAR_NODE_I64* as_i64 = (VAR_NODE_I64*)base;
            if (as_i64->nameLength == namelen && strneql(as_i64->name, name, namelen)) {
                return as_i64;
            }
        }
        base = base + VAR_SIZEOF_NODE(base);
        if ((char*)base>=(char*)end-1)
            break;
    }

    return NULL;
}

VAR_NODE_F32* VAR_GET_NODE_F32(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size)
{
    VAR_NODE_BASE* base = (VAR_NODE_BASE*)structure_buffer;
    const void* end = structure_buffer_size + structure_buffer;
    int namelen = strlen(name);
    while (base)
    {
        if (base->type == VAR_NODE_TYPE_F32) {
            VAR_NODE_F32* as_f32 = (VAR_NODE_F32*)base;
            if (as_f32->nameLength == namelen && strneql(as_f32->name, name, namelen)) {
                return as_f32;
            }
        }
        base = base + VAR_SIZEOF_NODE(base);
        if ((char*)base>=(char*)end-1)
            break;
    }

    return NULL;
}

VAR_NODE_F64* VAR_GET_NODE_F64(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size)
{
    VAR_NODE_BASE* base = (VAR_NODE_BASE*)structure_buffer;
    const void* end = structure_buffer_size + structure_buffer;
    int namelen = strlen(name);
    while (base)
    {
        if (base->type == VAR_NODE_TYPE_F64) {
            VAR_NODE_F64* as_f64 = (VAR_NODE_F64*)base;
            if (as_f64->nameLength == namelen && strneql(as_f64->name, name, namelen)) {
                return as_f64;
            }
        }
        base = base + VAR_SIZEOF_NODE(base);
        if ((char*)base>=(char*)end-1)
            break;
    }
    
    return NULL;
}

VAR_NODE_STRING* VAR_GET_NODE_STRING(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size)
{
    VAR_NODE_BASE* base = (VAR_NODE_BASE*)structure_buffer;
    const void* end = structure_buffer_size + structure_buffer;
    int namelen = strlen(name);
    while (base)
    {
        if (base->type == VAR_NODE_TYPE_STRING) {
            VAR_NODE_STRING* as_str = (VAR_NODE_STRING*)base;
            if (as_str->nameLength == namelen && strneql(as_str->name, name, namelen)) {
                return as_str;
            }
        }
        base = base + VAR_SIZEOF_NODE(base);
        if ((char*)base>=(char*)end-1)
            break;
    }

    return NULL;
}
