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
    char*           name;
    var_f64         value;
} VAR_NODE_F64;


typedef var_i8 ENV_PARSE_FLAGS;
typedef enum {
    VAR_PARSE_ASSUME_STRINGS = 0x1,
    VAR_PARSE_DISALLOW_OCTAL = 0x2,
    VAR_PARSE_DISALLOW_HEX   = 0x4
} VAR_PARSE_FLAGS_T;

/*
@param
data - the varlist source to validate.
data_len - the length of the varlist source.
offending_line_buffer - a buffer of integers to hold the offending lines. An element of -1 indicates the end of the offending lines.
offending_line_buffer_size - the size of the offending line buffer. 

@returns
VAR_SUCCESS: If there are no offending lines.
VAR_ERR_INVALID: If there are offending lines.
*/
VAR_RESULT VAR_CHECK_VALIDITY(const var_i8* data, var_size_t data_len, var_i32* offending_line_buffer, var_i32 offending_line_buffer_size);

/*
@param
data - the varlist source to validate.
data_len - the length of the varlist source.
length_used - a pointer to the number of bytes needed for the structure buffer.
structure_buffer - a buffer to hold the structure.

@returns
VAR_SUCCESS: EVERY CALL
*/
VAR_RESULT VAR_PARSE(const var_i8* data, var_size_t data_len, var_size_t* length_used, var_i8* structure_buffer);

/*
@param structure_buffer - a buffer that holds the node structures being targeted.
@param structure_size - size of the structure_buffer
@param string_buffer - a buffer the store the strings
@param string_size - the expected size of the string buffer, written to every call.
@param null_terminate - if true, the strings will be null terminated.

@returns
VAR_SUCCESS: EVERY CALL
*/
VAR_RESULT VAR_STORE_STRINGS(void *structure_buffer, var_size_t structure_size, void *string_buffer, var_i64 *string_size, char null_terminate);

var_i8 VAR_SIZEOF_NODE(const void* node);



/*
Returns the first match of the given variable that matches the name and specified type.
@returns
VAR_NODE_I32: A POINTER TO A NODE IF A MATCH WAS FOUND
VAR_NODE_I32: NULL IF DOES NOT EXIST
*/
VAR_NODE_I32*       VAR_GET_NODE_I32(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size);

/*
Returns the first match of the given variable that matches the name and specified type.

@returns
VAR_NODE_I64: A POINTER TO A NODE IF A MATCH WAS FOUND
VAR_NODE_I64: NULL IF DOES NOT EXIST
*/
VAR_NODE_I64*       VAR_GET_NODE_I64(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size);

/*
Returns the first match of the given variable that matches the name and specified type.

@returns
VAR_NODE_F32: A POINTER TO A NODE IF A MATCH WAS FOUND
VAR_NODE_F32: NULL IF DOES NOT EXIST
*/
VAR_NODE_F32*       VAR_GET_NODE_F32(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size);

/*
Returns the first match of the given variable that matches the name and specified type.

@returns
VAR_NODE_F64: A POINTER TO A NODE IF A MATCH WAS FOUND
VAR_NODE_F64: NULL IF DOES NOT EXIST
*/
VAR_NODE_F64*       VAR_GET_NODE_F64(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size);


/*
Returns the first match of the given variable that matches the name and specified type.

@returns
VAR_NODE_STRING: A POINTER TO A NODE IF A MATCH WAS FOUND
VAR_NODE_STRING: NULL IF DOES NOT EXIST
*/
VAR_NODE_STRING*    VAR_GET_NODE_STRING(const char* name, const var_i8* structure_buffer, const var_size_t structure_buffer_size);

#endif