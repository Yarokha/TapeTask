#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <map>
#include "taped.h"
#include "sorttape.h"

std::map<std::string, int> LoadConfig();

int main(){
    std::map<std::string, int> settings = LoadConfig();

    std::string in_path;
    std::string out_path;
    std::getline(std::cin, in_path);
    std::getline(std::cin, out_path);

    Taped input(in_path, std::ios::out, settings);
    Taped output(out_path, std::ios::out|std::ios::trunc, settings);
    
    tape::sort(input, output, settings);
}


std::map<std::string, int> LoadConfig() {
    std::string param_name, param_val;
    std::map<std::string, int> settings;

    std::ifstream config_file("Settings.conf", std::ios::in);

    while (std::getline(config_file, param_name, '='), std::getline(config_file, param_val)) {
        settings[param_name] = std::stoi(param_val);
    }
    
    return settings;
}