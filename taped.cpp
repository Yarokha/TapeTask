#include "taped.h"

void Taped::Next() {
    if (islast) {
        return;
    }
    cur_pos = tape.tellg();
    std::getline(tape, temp_input);
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
    if (isempty) {
        return;
    }
    value = std::stoi(temp_input);

}

void Taped::CreateTempFile(fs::path tmp, unsigned int i) {
    std::string temp_name = "temp_" + std::to_string(i) + ".dat";
    std::ofstream temp_file(tmp.string() + "/" + temp_name, std::ios::out | std::ios::trunc);
    temp_file.close();
}

//public
Taped::Taped(std::string file_path, std::ios_base::openmode openmode, std::map<std::string, int> sett): 
    tape(file_path, openmode), file_path(file_path), settings(sett){

    std::getline(tape, temp_input);

    isempty = temp_input.empty();
    if (!isempty) {
        value = std::stoi(temp_input);
    }
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

void Taped::MoveTo(unsigned long long n) {
    if (n > ncell) {
        Forward(n - ncell);
    }
    else if (n < ncell) {
        Backward(ncell-n);
    }
}

void Taped::WriteCell(int32_t val) {
    if (islast){
        if (tape.eof()) {
            tape.clear();
        }
        tape.seekp(cur_pos);
        tape << val << std::endl;
        value = val;

        islast = tape.eof();
    }
    else {
        fs::path tmp = "./tmp_0/";
        fs::create_directory(tmp);
        CreateTempFile(tmp, 0);
        std::fstream temp_file (tmp.string() + "temp_0.dat", std::ios::in | std::ios::out | std::ios::trunc);
        tape.seekg(0);
        std::string temp;
        while (tape.tellg() != cur_pos) {
            std::getline(tape, temp);
            temp_file << temp<<std::endl;
        }
        temp_file << val << std::endl;
        std::getline(tape, temp);
        while (std::getline(tape, temp)) {
            temp_file << temp << std::endl;
        }
        tape.close();
        tape.open(file_path, std::ios::in | std::ios::out | std::ios::trunc);
        temp_file.clear();
        temp_file.seekg(0);
        while (std::getline(temp_file, temp)) {
            tape << temp << std::endl;;
        }
        temp_file.close();
        fs::remove_all(tmp);
        tape.clear();
        
        tape.seekp(cur_pos);
        tape.seekg(cur_pos);
        std::getline(tape, temp);
    }
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



