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


class ITaped {
public:
    virtual long long int CellNum() const = 0;
    virtual int32_t ReadCell() const = 0;
    virtual void NextCell() = 0;
    virtual void PrevCell() = 0;
    virtual void Forward(unsigned long long) = 0;
    virtual void Backward(unsigned long long) = 0;
    virtual void MoveTo(unsigned long long) = 0;
    virtual void WriteCell(int32_t) = 0;
    virtual bool IsLast() const = 0;
    ITaped& operator=(const ITaped) = delete;
};


class Taped : public ITaped{
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
    int32_t ReadCell() const override;
    long long int CellNum() const;
    void NextCell()override;
    void PrevCell()override;
    void Forward(unsigned long long n)override;
    void Backward(unsigned long long n)override;
    void MoveTo(unsigned long long n)override;
    void WriteCell(int32_t val)override;
    bool IsLast() const override;
    void Erase();
    ~Taped();
};