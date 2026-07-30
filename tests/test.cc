#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#define TEST_VLQK 1

#ifdef TEST_VLQK
#include <varlist_cc/vlqk.hh>
#else
#include <varlist_cc/varlist.hh>
#endif

#include "test_common.hh"

int main() 
{

#ifdef TEST_VLQK
    vlst::Result res;
    vlqk::Varlist varlist;
    std::cout << "Loading varlist...";
    if ((res = varlist.load("edge_cases.varlist")) != vlst::Result::Success) {
        std::cout << "FAIL\n";
    }
    std::cout << "OK\n";

    test::print_nodes(varlist.getNodes());



#else

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
        vlst::Result res;

        std::vector<std::int8_t> nodes(nodes_size);
        std::cout << "Parsing varlist file... ";
        if ((res = vlst::parse((std::int8_t*)varlist.data(), varlist.size(), &nodes_size, nodes.data()))
            !=vlst::Result::Success) 
        {
            std::cout << "FAIL\n";
            return -1;
        }
        std::cout << "OK\n";


        // by default, all strings in nodes are referencing data in the source file,
        // thus freeing that memory would result in a memory leak.

        // it is optimal to store needed strings in a dedicated buffer instead of
        // the original source file - vlst::storeStrings handles this process.

        std::size_t stringbuf_size = 0;
        std::cout << "Evaluating string buffer size... ";
        if ((res = vlst::storeStrings(nodes.data(), nodes.size(), nullptr, &stringbuf_size, false))
            !=vlst::Result::Success) 
        {
            std::cout << "FAIL\n";
            return -1;
        }
        std::cout << "OK\n";

        std::vector<std::int8_t> stringbuf(stringbuf_size);
        std::cout << "Storing strings in a dedicated buffer... ";
        if ((res = vlst::storeStrings(nodes.data(), nodes.size(), stringbuf.data(), &stringbuf_size, false))
            != vlst::Result::Success)
        {
            std::cout << "FAIL\n";
            return -1;
        }
        std::cout << "OK\n";

        // varlist can be safely emptied - all strings are now in 
        // dedicated buffer instead of the source file.
        varlist = ""; 

        test::print_nodes(nodes);
        
    } else {
        std::cout << "No nodes in file." << std::endl;
    }
#endif // TEST_VLQK

    std::cout << "Press any key to quit the program...." << std::endl;
    (void)getc(stdin);
    

    return 0;
}