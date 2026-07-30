#pragma once
#ifndef VLQK_HH
#define VLQK_HH

/* VARLIST QUICK 
* A heavily simplified version of the varlist
* system, designed to make it amazing simple
* to use.
*/
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

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <optional>
#include <istream>
#include <fstream>
#include <filesystem>

#include <varlist_cc/varlist.hh>


namespace vlqk {
    using namespace vlst;

    class Varlist {
    public:


        inline Result load(const std::filesystem::path path) {
            std::ifstream f;
            f.open(path, std::ios::binary);

            return load(f);
        }

        inline Result load(std::istream& stream)
        {
            Result r;
            std::vector<vlst::i8> src = std::vector<vlst::i8>(
                std::istreambuf_iterator<char>(stream),
                std::istreambuf_iterator<char>()
            );



            size_t size = 0;
            r = vlst::parse(src.data(), src.size(),  &size, nullptr);
            if (r != Result::Success) {
                return r;
            }
            if (size == 0) {
                return Result::Success;
            }
            data.resize(size);
            vlst::parse(src.data(), src.size(), &size, data.data());


            nsize = data.size();
            size_t ssize = 0;
            vlst::storeStrings(data.data(), nsize, nullptr, &ssize, true);

            if (1) {
                data.resize(nsize + ssize); 

                vlst::storeStrings(data.data(), nsize, data.data()+nsize, &ssize, true);

                // add nodes to node lookup table
               
                Node* cur = reinterpret_cast<Node*>(data.data());
                Node* end = reinterpret_cast<Node*>(data.data() + nsize);

                while (cur < end)
                {
                    std::pair<const char*, i16> nmp = cur->getName();
                    nodeLookup[nmp.first] = cur;

                    cur = reinterpret_cast<Node*>(
                        reinterpret_cast<i8*>(cur) + cur->getSize());
                }

                return Result::Success;
            }
            else {
                return Result::ErrInvalid;
            }
        }

        inline std::pair<const i8*,size_t> getNodes() const {
            return { data.data(),nsize};
        }

        inline std::pair<i8*, size_t> getNodes() {
            return { data.data(),nsize };
        }

        inline std::string_view get_string(std::string_view name)  {
            auto it = nodeLookup.find(name);
            if (it == nodeLookup.end()) return {};
            Node* n = it->second;
            if (n && n->getType() == NodeType::String) {
                return (static_cast<NodeString*>(n)->value);
            }
            return {};
        }

        inline i32* get_i32(std::string_view name) {
            auto it = nodeLookup.find(name);
            if (it == nodeLookup.end()) return nullptr;
            Node* n = it->second;
            if (n && n->getType() == NodeType::I32) {
                return &(static_cast<NodeI32*>(n)->value);
            }
            return NULL;
        }

        inline i64* get_i64(std::string_view name) {
            auto it = nodeLookup.find(name);
            if (it == nodeLookup.end()) return nullptr;
            Node* n = it->second;
            if (n && n->getType() == NodeType::I64) {
                return &(static_cast<NodeI64*>(n)->value);
            }
            return NULL;
        }

        inline f32* get_f32(std::string_view name) {
            Node* n = nodeLookup[name];
            if (n && n->getType() == NodeType::F32) {
                return &(static_cast<NodeF32*>(n)->value);
            }
            return NULL;
        }

        inline f64* get_f64(std::string_view name) {
            auto it = nodeLookup.find(name);
            if (it == nodeLookup.end()) return nullptr;
            Node* n = it->second;

            if (n && n->getType() == NodeType::F64) {
                return &(static_cast<NodeF64*>(n)->value);
            }
            return NULL;
        }

    public:
        std::unordered_map<std::string_view, Node*> nodeLookup;
        std::vector<i8> data;
        size_t nsize = 0;
    };
}
#endif // !VLQK_HH