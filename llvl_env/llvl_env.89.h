#ifndef LLVL_ENV_H
#define LLVL_ENV_H

typedef char                env_i8;
typedef short               env_i16;
typedef unsigned short      env_u16;
typedef int                 env_i32;
typedef int                 env_i64;
typedef float               env_f32;
typedef double              env_f64;
typedef unsigned long long  env_size_t;

typedef enum {
    ENV_SUCCESS     = 0,
    ENV_ERR_FILE    = -1,
    ENV_ERR_INVALID = -2
} ENV_RESULT;

typedef env_i8 ENV_NODE_TYPE;
typedef enum {
    ENV_NODE_TYPE_I32 =     1,
    ENV_NODE_TYPE_I64 =     2,
    ENV_NODE_TYPE_F32 =     3,
    ENV_NODE_TYPE_F64 =     4,
    ENV_NODE_TYPE_STRING =  5
} ENV_NODE_TYPE_ENUM;

typedef struct {
    ENV_NODE_TYPE   type;
} ENV_NODE_BASE;



typedef struct {
    ENV_NODE_TYPE   type;
    env_i16         nameLength;
    char*           name;
    env_i64         valueLength;
    char*           value;
} ENV_NODE_STRING;

typedef struct {
    ENV_NODE_TYPE   type;
    env_i16         nameLength;
    env_i32         value;
    char*           name;
} ENV_NODE_I32;

typedef struct {
    ENV_NODE_TYPE   type;
    env_i16         nameLength;
    char*           name;
    env_i64         value;
} ENV_NODE_I64;

typedef struct {
    ENV_NODE_TYPE   type;
    env_i16         nameLength;
    env_f32         value;
    char*           name;
} ENV_NODE_F32;

typedef struct {
    ENV_NODE_TYPE   type;
    env_i16         nameLength;
    env_f64         value;
    char*           name;
} ENV_NODE_F64;


typedef env_i8 ENV_PARSE_FLAGS;
typedef enum {
    ENV_PARSE_ASSUME_STRINGS = 0x1,
    ENV_PARSE_DISALLOW_OCTAL = 0x2,
    ENV_PARSE_DISALLOW_HEX   = 0x4
} ENV_PARSE_FLAGS_T;

ENV_RESULT ENV_CHECK_VALIDITY(const env_i8* data, env_size_t data_len, env_i32* offending_line_buffer, env_i32 offending_line_buffer_size);

ENV_RESULT ENV_PARSE(const env_i8* data, env_size_t data_len, env_size_t* length_used, env_i8* node_buffer);

env_i8 ENV_SIZEOF_NODE(const void* node);
#endif