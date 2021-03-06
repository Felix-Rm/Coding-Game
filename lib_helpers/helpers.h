#pragma once

#include <filesystem>
#include <string>

template <typename T>
T min(T a, T b) {
    return a > b ? b : a;
}

template <typename T>
T abs(T a) {
    return a > 0 ? a : -a;
}

void float_mod(float &v, float m);

void rename_directory(std::string from, std::string to);