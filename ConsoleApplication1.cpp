#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <map>
#include "taped.h"
#include "sorttape.h"

//#include <ctime>

//namespace fs = std::filesystem;
//using namespace std::chrono;

//void rangen() {
//    std::srand(std::time(nullptr));
//    std::ofstream test("d:/input.txt");
//
//    std::vector <int> v(1000);
//    std::generate(v.begin(), v.end(), []() {return std::rand(); });
//
//    for (auto& i : v) {
//        test << i << std::endl;
//    }
//    test.close();
//}

//void CreateTempFile(fs::path tmp, unsigned int i) {
//    fs::create_directory(tmp);
//    std::string temp_name = "temp_" + std::to_string(i) + ".dat";
//    std::ofstream temp_file(tmp.string() + "/" + temp_name);
//    temp_file.close();
//}

std::map<std::string, int> LoadConfig();


int main(){
    std::map<std::string, int> settings = LoadConfig();

    std::string in_path = "d:/input.txt";
    std::string out_path = "d:/output.txt";
    //std::getline(std::cin, in_path);
    //std::getline(std::cin, out_path);


    //Taped input(in_path, std::ios::out, settings);
    Taped output(out_path, std::ios::out|std::ios::trunc, settings);
    
    //tape::sort(input, output, settings);



//    input.NextCell();
//    input.NextCell();
//    input.NextCell();
//    input.NextCell();
//    input.NextCell();
//
//    //input.StartCell();
//    input.MoveTo(0);
//    input.MoveTo(1001);
//
//    input.PrevCell();
//    input.PrevCell();
//    input.PrevCell();
//    input.PrevCell();
//    input.PrevCell();
//    input.WriteCell(0);
//    input.PrevCell();
//    input.PrevCell();
}


std::map<std::string, int> LoadConfig() {
    std::string param_name, param_val;
    std::map<std::string, int> settings;

    float value = 0;
    std::ifstream config_file("Settings.conf", std::ios::in);
    if (!config_file.is_open()) {
        //throw _exception
    }
    while (std::getline(config_file, param_name, '='), std::getline(config_file, param_val)) {
        settings[param_name] = std::stoi(param_val);
    }
    return settings;
}