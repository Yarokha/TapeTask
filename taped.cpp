#include "taped.h"


void Taped::Next() {
    cur_pos = tape.tellg();
    std::getline(tape, temp_input);
    //islast = tape.tellp() == -1;
    islast = tape.eof();
    ++ncell;
    isempty = temp_input.empty();
    if (isempty) {
        return;
    }
    value = std::stoi(temp_input);
}

void Taped::Previous() {
    if (cur_pos == 0) {
        return;
    }
    --ncell;
    //islast = tape.tellg() == -1;
    islast = tape.eof();
    if (islast) {
        tape.clear();
    }

    cur_pos -= 2;
    while (cur_pos != 0)
    {
        tape.seekg(--cur_pos);
        std::getline(tape, temp_input);
        if (temp_input.empty()) {
            --cur_pos;
            break;
        }
    }
    if (cur_pos != 0) {
        cur_pos += 2;
    }

    tape.seekg(cur_pos);
    std::getline(tape, temp_input);
    isempty = temp_input.empty();
    if (isempty) { return; }
    value = std::stoi(temp_input);

}

//public
Taped::Taped(std::string file_path, std::ios_base::openmode openmode, std::map<std::string, int> sett): 
    tape(file_path, openmode), file_path(file_path), settings(sett){

    std::getline(tape, temp_input);

    isempty = temp_input.empty();
    if (!isempty) {
        value = std::stoi(temp_input);
    }
    //islast = tape.tellp() == -1;
    islast = tape.eof();
    ncell = 0;
}


int32_t Taped::ReadCell() const{
    std::this_thread::sleep_for(settings.t_reading);
    return value;
}

void Taped::NextCell() {
    Next();
    std::this_thread::sleep_for(settings.t_shift);
}

void Taped::PrevCell() {
    if (cur_pos == 0) {
        return;
    }
    Previous();
    std::this_thread::sleep_for(settings.t_shift);
}

void Taped::Forward(unsigned long long  n) {
    for (; n > 0 && !islast; --n) {
        NextCell();
        std::this_thread::sleep_for(settings.t_sweep);
    }
}

void Taped::Backward(unsigned long long  n) {
    for (; n > 0 && cur_pos > 0; --n) {
        Previous();
        std::this_thread::sleep_for(settings.t_sweep);
    }
}

//void Taped::StartCell() {
//    while (cur_pos > 0) {
//        Previous();
//        std::this_thread::sleep_for(settings.t_sweep);
//    }
//}

void Taped::MoveTo(unsigned long n) {
    if (n > ncell) {
        Forward(n - ncell);
    }
    else if (n < ncell) {
        Backward(ncell-n);
    }
}

void Taped::WriteCell(int32_t val) {
    //if (tape.tellp() == -1) {
    if (tape.eof()){
        tape.clear();
    }
    tape.seekp(cur_pos);
    tape << val << std::endl;
    tape.seekp(cur_pos);


    std::getline(tape, temp_input);
    isempty = temp_input.empty();
    if (!isempty) {
        value = std::stoi(temp_input);
    }

    //islast = tape.tellp() == -1;
    islast = tape.eof();
    std::this_thread::sleep_for(settings.t_writing);
}

bool Taped::IsLast() const{
    return islast;
}

void Taped::Erase() {
    tape.close();
    tape.open(file_path, std::ios::in | std::ios::out | std::ios::trunc);
    cur_pos = 0;
    islast = true;
    isempty = true;
}

Taped::~Taped() {
    tape.close();
}