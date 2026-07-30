#pragma once
#include <vector>
#include <cstdint>
#include <varlist_cc/varlist.hh>

#include <stdio.h>
#include <inttypes.h>
#include <utility>
namespace test {


    void print_node(const vlst::Node *n) {
        using namespace vlst;
        NodeType type = n->getType();
        if (type == NodeType::I32) {
            const NodeI32 *ni32 = static_cast<const NodeI32*>(n);
            printf("\t === NODE: I32 ===\n");
            printf("\t NAME: \"%.*s\"\n", ni32->nameLength, ni32->name);
            printf("\t VALUE: %d\n", ni32->value);
            printf("\n");
        }
        else if (type == NodeType::I64) {
            const NodeI64 *ni64 = static_cast<const NodeI64*>(n);
            printf("\t === NODE: I64 ===\n");
            printf("\t NAME: \"%.*s\"\n", ni64->nameLength, ni64->name);
            printf("\t VALUE: %" PRId64 "\n", ni64->value);
            printf("\n");
        }
        else if (type == NodeType::F32) {
            const NodeF32 *nif32 = static_cast<const NodeF32*>(n);
            printf("\t === NODE: F32 ===\n");
            printf("\t NAME: \"%.*s\"\n", nif32->nameLength, nif32->name);
            printf("\t VALUE: %f\n", nif32->value);
            printf("\n");
        }
        else if (type == NodeType::F64) {
            const NodeF64 *nif64  = static_cast<const NodeF64*>(n);
            printf("\t === NODE: F64 ===\n");
            printf("\t NAME: \"%.*s\"\n", nif64->nameLength, nif64->name);
            printf("\t VALUE: %lf\n", nif64->value);
            printf("\n");
        }
        else if (type == NodeType::String) {
            const NodeString *nistr = static_cast<const NodeString*>(n);
            printf("\t === NODE: STRING ===\n");
            printf("\t NAME<%d>: \"%.*s\"\n", nistr->nameLength, nistr->nameLength, nistr->name);
            printf("\t STRING<%d>: \"%.*s\"\n", nistr->valueLength, nistr->valueLength, nistr->value);
            printf("\n");
        }
    }
    void print_nodes(const std::pair<std::int8_t*,size_t> nodes)
    {
        using namespace vlst;
        const Node *cur = reinterpret_cast<const Node*>(nodes.first);
        const Node *end = reinterpret_cast<const Node*>(nodes.first + nodes.second);
        while (cur < end)
        {
            print_node(cur);
            cur += cur->getSize();
        }
        
    }

}