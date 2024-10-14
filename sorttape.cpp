#include "sorttape.h"


//private
std::vector<int32_t>::iterator SortTape::LoadInMem(Taped& in) {
    auto Iter = memory.begin();
    for (; Iter != memory.end(); ++Iter) {
        if (in.IsLast())
            return Iter;
        *Iter = in.ReadCell();
        in.NextCell();
    }
    return Iter;
}


template <typename ForwardIter>
void SortTape::Merge(ForwardIter begin, ForwardIter end, Taped& in, Taped& out) {
    auto Iter = begin;
    while (Iter != end || !in.IsLast()) {
        if (Iter == end) {
            out.WriteCell(in.ReadCell());
            out.NextCell();
            in.NextCell();
        }
        else if (in.IsLast()) {
            out.WriteCell(*Iter);
            out.NextCell();
            ++Iter;
        }
        else if (Iter != end && !in.IsLast()) {
            if (in.ReadCell() > *Iter) {
                out.WriteCell(*Iter);
                ++Iter;
            }
            else {
                out.WriteCell(in.ReadCell());
                in.NextCell();
            }
            out.NextCell();
        }
    }
}


//public
SortTape::SortTape(Taped& temp1, Taped& temp2, int memory_size) {
    temp_in = &temp1;
    temp_out = &temp2;
    memory.resize(memory_size);
}

void SortTape::Sort(Taped& input_tape, Taped& output_tape) {
    while (!input_tape.IsLast()) {

        auto memend = LoadInMem(input_tape);
        std::sort(memory.begin(), memend);

        if (input_tape.IsLast())
            temp_out = &output_tape;

        temp_out->Erase();
        Merge(memory.begin(), memend, *temp_in, *temp_out);
        temp_out->MoveTo(0);
        temp_in->MoveTo(0);
        std::swap(temp_in, temp_out);
    }
}


//non-class members
void CreateTempFile(fs::path tmp, unsigned int i) {
    std::string temp_name = "temp_" + std::to_string(i) + ".dat";
    std::ofstream temp_file(tmp.string() + "/" + temp_name, std::ios::out | std::ios::trunc);
    temp_file.close();
}


namespace tape {
    void sort(Taped& input, Taped& output, std::map<std::string,int> settings) {
        fs::path tmp = "./tmp";
        fs::create_directory(tmp);
        CreateTempFile(tmp, 1);
        CreateTempFile(tmp, 2);

        {
            Taped temp1(tmp.string() + "/temp_1.dat", std::ios::in | std::ios::out | std::ios::trunc, settings);
            Taped temp2(tmp.string() + "/temp_2.dat", std::ios::in | std::ios::out | std::ios::trunc, settings);

            SortTape tape(temp1, temp2, settings.at("number_memory"));
            tape.Sort(input, output);
        }
        fs::remove_all(tmp);
    }
}


