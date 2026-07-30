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

#pragma once
#include <cstdint>

namespace vlst {


        
    using i8  = std::int8_t;
    using i16 = std::int16_t;
    using u16 = std::uint16_t;
    using i32 = std::int32_t;
    using i64 = std::int64_t;
    using f32 = float;
    using f64 = double;

    namespace vc {
        extern "C" i8 VAR_SIZEOF_NODE(const void* node);
    }

    enum class Result {
        Success    = 0,
        ErrFile    = -1,
        ErrInvalid = -2
    };


    enum class NodeType : i8 {
        I32 =     1,
        I64 =     2,
        F32 =     3,
        F64 =     4,
        String =  5
    };


    class Node {
    protected:
        NodeType type;
        Node(NodeType t) : type(t) {}
    public:
        inline NodeType getType() {
            return type;
        }
        inline i8 getSize() {
            return vc::VAR_SIZEOF_NODE(this);
        }
    };

    class NodeI32 : public Node {
    public:
        i16             nameLength;
        i32             value;
        char*           name;
    public:
        NodeI32() : Node(NodeType::I32) {}
    };

    class NodeI64 : public Node {
    public:
        i16             nameLength;
        char*           name;
        i64             value;
    public:
        NodeI64() : Node(NodeType::I64) {}
    };

    class NodeF32 : public Node {
    public:
        i16             nameLength;
        f32             value;
        char*           name;
    public:
        NodeF32() : Node(NodeType::F32) {}
    };

    class NodeF64 : public Node {
    public:
        i16             nameLength;
        char*           name;
        f64             value;
    public:
        NodeF64() : Node(NodeType::F64) {}
    };

    class NodeString : public Node {
    public:
        i16             nameLength;
        char*           name;
        i64             valueLength;
        char*           value;
    public:
        NodeString() : Node(NodeType::String) {}
    };


    namespace vc {  
        extern "C" Result VAR_CHECK_VALIDITY(const i8* data, size_t data_len, i32* offending_line_buffer, i32 offending_line_buffer_size);
        extern "C" Result VAR_PARSE(const i8* data, size_t data_len, size_t* length_used, i8* structure_buffer);
        extern "C" Result VAR_STORE_STRINGS(void *structure_buffer, size_t structure_size, void *string_buffer, size_t *string_size, char null_terminate);
        extern "C" NodeI32*       VAR_GET_NODE_I32(const char* name, const i8* structure_buffer, const size_t structure_buffer_size);
        extern "C" NodeI64*       VAR_GET_NODE_I64(const char* name, const i8* structure_buffer, const size_t structure_buffer_size);
        extern "C" NodeF32*       VAR_GET_NODE_F32(const char* name, const i8* structure_buffer, const size_t structure_buffer_size);
        extern "C"  NodeF64*       VAR_GET_NODE_F64(const char* name, const i8* structure_buffer, const size_t structure_buffer_size);
        extern "C" NodeString*    VAR_GET_NODE_STRING(const char* name, const i8* structure_buffer, const size_t structure_buffer_size);
    }








    
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
    inline Result checkValidity(const i8* data, size_t data_len, i32* offending_line_buffer, 
        i32 offending_line_buffer_size
    ) {
        return vc::VAR_CHECK_VALIDITY(data,data_len,offending_line_buffer,offending_line_buffer_size);
    }

    /*
    @param
    data - the varlist source to validate.
    data_len - the length of the varlist source.
    length_used - a pointer to the number of bytes needed for the structure buffer.
    structure_buffer - a buffer to hold the structure.

    @returns
    VAR_SUCCESS: EVERY CALL
    */
    inline Result parse(const i8* data, size_t data_len, size_t* length_used, i8* structure_buffer
    ) {
        return vc::VAR_PARSE(data, data_len, length_used, structure_buffer);
    }

    /*
    @param structure_buffer - a buffer that holds the node structures being targeted.
    @param structure_size - size of the structure_buffer
    @param string_buffer - a buffer the store the strings
    @param string_size - the expected size of the string buffer, written to every call.
    @param null_terminate - if true, the strings will be null terminated.

    @returns
    VAR_SUCCESS: EVERY CALL
    */
    inline Result storeStrings(void *structure_buffer, size_t structure_size, void *string_buffer,
         size_t *string_size, bool null_terminate
    ) {
        return vc::VAR_STORE_STRINGS(
            structure_buffer, structure_size, string_buffer, string_size, (char)null_terminate);
    }


    /*
    Returns the first match of the given variable that matches the name and specified type.
    @returns
    VAR_NODE_I32: A POINTER TO A NODE IF A MATCH WAS FOUND
    VAR_NODE_I32: NULL IF DOES NOT EXIST
    */
    inline NodeI32*       getNodeI32(const char* name, const i8* structure_buffer, 
        const size_t structure_buffer_size
    ) {
        return vc::VAR_GET_NODE_I32(name,structure_buffer,structure_buffer_size);
    }

    /*
    Returns the first match of the given variable that matches the name and specified type.

    @returns
    VAR_NODE_I64: A POINTER TO A NODE IF A MATCH WAS FOUND
    VAR_NODE_I64: NULL IF DOES NOT EXIST
    */
    inline NodeI64*       getNodeI64(const char* name, const i8* structure_buffer, 
        const size_t structure_buffer_size
    ) {
        return vc::VAR_GET_NODE_I64(name,structure_buffer,structure_buffer_size);
    }

    /*
    Returns the first match of the given variable that matches the name and specified type.

    @returns
    VAR_NODE_F32: A POINTER TO A NODE IF A MATCH WAS FOUND
    VAR_NODE_F32: NULL IF DOES NOT EXIST
    */
    inline NodeF32*       getNodeF32(const char* name, const i8* structure_buffer, 
        const size_t structure_buffer_size
    ) {
        return vc::VAR_GET_NODE_F32(name,structure_buffer,structure_buffer_size);
    }

    /*
    Returns the first match of the given variable that matches the name and specified type.

    @returns
    VAR_NODE_F64: A POINTER TO A NODE IF A MATCH WAS FOUND
    VAR_NODE_F64: NULL IF DOES NOT EXIST
    */
    inline NodeF64*       getNodeF64(const char* name, const i8* structure_buffer, 
        const size_t structure_buffer_size
    ) {
        return vc::VAR_GET_NODE_F64(name,structure_buffer,structure_buffer_size);
    }


    /*
    Returns the first match of the given variable that matches the name and specified type.

    @returns
    VAR_NODE_STRING: A POINTER TO A NODE IF A MATCH WAS FOUND
    VAR_NODE_STRING: NULL IF DOES NOT EXIST
    */
    inline NodeString*    getNodeString(const char* name, const i8* structure_buffer,
         const size_t structure_buffer_size
        ) {
        return vc::VAR_GET_NODE_STRING(name,structure_buffer,structure_buffer_size);
    }
}
