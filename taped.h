#pragma once
#include <cstdint>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <thread>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;

class Taped {
private:
    struct Config {
        std::chrono::nanoseconds t_reading;
        std::chrono::nanoseconds t_writing;
        std::chrono::nanoseconds t_shift;
        std::chrono::nanoseconds t_sweep;
        Config(std::map<std::string, int> sett) {
            t_reading = std::chrono::nanoseconds(sett.at("delay_reading"));
            t_writing = std::chrono::nanoseconds(sett.at("delay_writing"));
            t_shift = std::chrono::nanoseconds(sett.at("delay_shift"));
            t_sweep = std::chrono::nanoseconds(sett.at("delay_sweep"));
        }
    };


    std::fstream tape;
    int32_t value{ 0 };
    unsigned long long cur_pos{ 0 };
    bool isempty;
    bool islast;
    std::string temp_input;
    std::string file_path;
    unsigned long long ncell;
    Config settings;

    void Next();
    void Previous();

    void CreateTempFile(fs::path tmp, unsigned int i);
public:
    Taped(std::string file_path, std::ios_base::openmode openmode, std::map<std::string, int> sett);
    int32_t ReadCell() const;
    void NextCell();
    void PrevCell();
    void Forward(unsigned long long n);
    void Backward(unsigned long long n);
    void MoveTo(unsigned long n);
    void WriteCell(int32_t val);
    bool IsLast() const;
    void Erase();
    ~Taped();
};