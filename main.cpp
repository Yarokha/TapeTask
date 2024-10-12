#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "taped.h"
#include "sorttape.h"


std::map<std::string, int> LoadConfig();

int main() {
    std::map<std::string, int> settings = LoadConfig();

    std::string path;

    std::getline(std::cin, path);
    Taped input(path, std::ios::in, settings);

    std::getline(std::cin, path);
    Taped output(path, std::ios::out | std::ios::trunc, settings);

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