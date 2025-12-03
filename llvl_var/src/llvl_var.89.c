#include "../llvl_var.89.h"
#include "../pow_lookup.89.h"
#include <stdint.h>
#include "../deps/streql/streqlasm.h"
#ifndef NDEBUG
#include <stdio.h>
#include <string.h>
#endif


extern char _var_is_space_c_entry(char);


var_i32 _var_atoi32(char* a, var_i64 digits) 
{
    char m = 0;
    char neg = 0;
    if (*a=='-') {
        a++;
        digits--;
        neg = 1;
    }
    else if (digits>1&&*(a+1) == 'o') {
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
    else if (digits>1&&*(a+1) == 'o') {
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
        } else if (*data==':' && state!=0) {
            state = 1;
            if (offending_line_buffer_max_count && last_offending_line != line && offending_line_count <= offending_line_buffer_max_count-1) {
                offending_line_buffer[offending_line_count] = line;
                offending_line_count++;
                last_offending_line=line;
            }
        }  else {
            if (*data == '\n' || data == data_end-1) {
                line++;
                valueLength = 0;
                state = 0;
            }
            else if (state == 0) {
                if (*data==':') {
                    state = 1;
                } 
                else if (!_var_is_space_c_entry(*data)) {
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
                if (!_var_is_space_c_entry(*data)) {
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

VAR_RESULT VAR_PARSE(data, data_len, length_used, node_buffer)
    const var_i8*   data; 
    var_size_t      data_len; 
    var_size_t*     length_used;
    var_i8*         node_buffer;
{
    VAR_RESULT res = _var_parse_arm64(data, data_len, length_used, node_buffer);
    return res;
}
