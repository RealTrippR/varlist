#include "../llvl_var.89.h"
#include "../pow_lookup.89.h"
#include <stdint.h>
#ifndef NDEBUG
#include <stdio.h>
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
    var_i8 dt_i = 0;
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
    var_i16 dt_i = 0;
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
        if (*data==':' && state!=0) {
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

__attribute__((ms_abi))
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
