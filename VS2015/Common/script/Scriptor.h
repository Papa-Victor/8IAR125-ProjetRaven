#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <stdexcept>
#include <regex>
#include <algorithm>

class Scriptor {
private:
    std::unordered_map<std::string, std::string> params;

public:
    Scriptor(std::string fileName) {
        std::ifstream file(fileName, std::ios::app);
        if (!file.is_open()) {
            throw std::runtime_error("Parameters' file not open.");
        }

        std::string line;
        while (std::getline(file, line)) {
            const std::string s(line);
            const std::regex param("(\\w+)\\s*=\\s*(?:([\\w\\.]+)|\\\"(.+)\\\")");
            std::smatch match;
            if (std::regex_search(s.begin(), s.end(), match, param)) {
                if (std::string(match[2]).size() > 0) {
                    params[match[1]] = match[2];
                }
                else {
                    params[match[1]] = match[3];
                }
            }
        }

        file.close();
    }

    int GetInt(std::string name) {
        return std::atoi(params[name].c_str());
    }

    float GetFloat(std::string name) {
        return std::stof(params[name].c_str());
    }

    double GetDouble(std::string name) {
        return std::stod(params[name].c_str());
    }

    std::string GetString(std::string name) {
        return params[name];
    }

    bool GetBool(std::string name) {
        std::string t("true");
        return t.compare(params[name]) == 0 || std::atoi(params[name].c_str());
    }
};
