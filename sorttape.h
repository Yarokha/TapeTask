#pragma once
#include <vector>
#include <algorithm>
#include <filesystem>
#include <map>
#include "taped.h"

namespace fs = std::filesystem;

class SortTape {
private:
    std::vector<int32_t> memory;

    Taped* temp_in;
    Taped* temp_out;

    std::vector<int32_t>::iterator LoadInMem(Taped& in);

    template <typename ForwardIter>
    void Merge(ForwardIter begin, ForwardIter end, Taped& in, Taped& out);

public:
    SortTape(Taped& temp1, Taped& temp2, int memory_size);
    void Sort(Taped& input_tape, Taped& output_tape);
};


namespace tape {
    void sort(Taped& input, Taped& output, std::map<std::string,int> settings);
}