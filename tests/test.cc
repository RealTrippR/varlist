#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>


#include <varlist_cc/varlist.hh>
#include "test_common.hh"

int main() 
{


    // parsing may not work correctly if the std::ios::binary is not specified.
    std::ifstream varfile("edge_cases.varlist", std::ios::binary);
    std::string varlist;
    {
        std::stringstream buffer;
        buffer << varfile.rdbuf();
        varlist = buffer.str();
    }


    std::size_t nodes_size;
    vlst::parse((std::int8_t*)varlist.data(), varlist.size(), &nodes_size, nullptr);

    if (nodes_size) {
        std::vector<std::int8_t> nodes(nodes_size);
        vlst::parse((std::int8_t*)varlist.data(), varlist.size(), &nodes_size, nodes.data());


        // by default, all strings in nodes are referencing data in the source file,
        // thus freeing that memory would result in a memory leak.

        // it is optimal to store needed strings in a dedicated buffer instead of
        // the original source file - vlst::storeStrings handles this process.

        std::size_t stringbuf_size;
        vlst::storeStrings(nodes.data(), nodes.size(), nullptr, &stringbuf_size, false);

        std::vector<std::int8_t> stringbuf(stringbuf_size);
        vlst::storeStrings(nodes.data(), nodes.size(), stringbuf.data(), &stringbuf_size, false);

        // varlist can be safely emptied - all strings are now in 
        // dedicated buffer instead of the source file.
        varlist = ""; 

        test::print_nodes(nodes);
        
    } else {
        std::cout << "No nodes in file." << std::endl;
    }

    std::cout << "Press any key to quit the program...." << std::endl;
    getc(stdin);
    

    return 0;
}