#pragma once
#include <vector>
#include <cstdint>
#include <varlist_cc/varlist.hh>

#include <stdio.h>
#include <inttypes.h>

namespace test {


    void print_node(vlst::Node *n) {
        using namespace vlst;
        NodeType type = n->getType();
        if (type == NodeType::I32) {
            NodeI32 *ni32 = static_cast<NodeI32*>(n);
            printf("\t === NODE: I32 ===\n");
            printf("\t NAME: \"%.*s\"\n", ni32->nameLength, ni32->name);
            printf("\t VALUE: %d\n", ni32->value);
            printf("\n");
        }
        else if (type == NodeType::I64) {
            NodeI64 *ni64 = static_cast<NodeI64*>(n);
            printf("\t === NODE: I64 ===\n");
            printf("\t NAME: \"%.*s\"\n", ni64->nameLength, ni64->name);
            printf("\t VALUE: %" PRId64 "\n", ni64->value);
            printf("\n");
        }
        else if (type == NodeType::F32) {
            NodeF32 *nif32 = static_cast<NodeF32*>(n);
            printf("\t === NODE: F32 ===\n");
            printf("\t NAME: \"%.*s\"\n", nif32->nameLength, nif32->name);
            printf("\t VALUE: %f\n", nif32->value);
            printf("\n");
        }
        else if (type == NodeType::F64) {
            NodeF64 *nif64  = static_cast<NodeF64*>(n);
            printf("\t === NODE: F64 ===\n");
            printf("\t NAME: \"%.*s\"\n", nif64->nameLength, nif64->name);
            printf("\t VALUE: %lf\n", nif64->value);
            printf("\n");
        }
        else if (type == NodeType::String) {
            NodeString *nistr = static_cast<NodeString*>(n);
            printf("\t === NODE: STRING ===\n");
            printf("\t NAME<%d>: \"%.*s\"\n", nistr->nameLength, nistr->nameLength, nistr->name);
            printf("\t STRING<%d>: \"%.*s\"\n", nistr->valueLength, nistr->valueLength, nistr->value);
            printf("\n");
        }
    }
    void print_nodes(std::vector<std::int8_t>& nodes)
    {
        using namespace vlst;
        Node *cur = reinterpret_cast<Node*>(nodes.data());
        Node *end = reinterpret_cast<Node*>(nodes.data() + nodes.size());
        while (cur < end)
        {
            print_node(cur);
            cur += cur->getSize();
        }
        
    }

}