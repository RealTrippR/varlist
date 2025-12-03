#ifndef LLVL_VAR_H
#define LLVL_VAR_H

typedef char                var_i8;
typedef short               var_i16;
typedef unsigned short      var_u16;
typedef int                 var_i32;
typedef long long           var_i64;
typedef float               var_f32;
typedef double              var_f64;
typedef unsigned long long  var_size_t;

typedef enum {
    VAR_SUCCESS     = 0,
    VAR_ERR_FILE    = -1,
    VAR_ERR_INVALID = -2
} VAR_RESULT;

typedef var_i8 VAR_NODE_TYPE;
typedef enum {
    VAR_NODE_TYPE_I32 =     1,
    VAR_NODE_TYPE_I64 =     2,
    VAR_NODE_TYPE_F32 =     3,
    VAR_NODE_TYPE_F64 =     4,
    VAR_NODE_TYPE_STRING =  5
} VAR_NODE_TYPE_ENUM;

typedef struct {
    VAR_NODE_TYPE   type;
} VAR_NODE_BASE;



typedef struct {
    VAR_NODE_TYPE   type;
    var_i16         nameLength;
    char*           name;
    var_i64         valueLength;
    char*           value;
} VAR_NODE_STRING;

typedef struct {
    VAR_NODE_TYPE   type;
    var_i16         nameLength;
    var_i32         value;
    char*           name;
} VAR_NODE_I32;

typedef struct {
    VAR_NODE_TYPE   type;
    var_i16         nameLength;
    char*           name;
    var_i64         value;
} VAR_NODE_I64;

typedef struct {
    VAR_NODE_TYPE   type;
    var_i16         nameLength;
    var_f32         value;
    char*           name;
} VAR_NODE_F32;

typedef struct {
    VAR_NODE_TYPE   type;
    var_i16         nameLength;
    var_f64         value;
    char*           name;
} VAR_NODE_F64;


typedef var_i8 ENV_PARSE_FLAGS;
typedef enum {
    VAR_PARSE_ASSUME_STRINGS = 0x1,
    VAR_PARSE_DISALLOW_OCTAL = 0x2,
    VAR_PARSE_DISALLOW_HEX   = 0x4
} VAR_PARSE_FLAGS_T;

VAR_RESULT VAR_CHECK_VALIDITY(const var_i8* data, var_size_t data_len, var_i32* offending_line_buffer, var_i32 offending_line_buffer_size);

VAR_RESULT VAR_PARSE(const var_i8* data, var_size_t data_len, var_size_t* length_used, var_i8* node_buffer);

var_i8 VAR_SIZEOF_NODE(const void* node);
#endif