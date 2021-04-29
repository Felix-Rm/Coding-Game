#include "helpers.h"

void float_mod(float& v, float m) {
    if (v > 0)
        while (v >= m)
            v -= m;
    if (v < 0)
        while (v <= -m)
            v += m;
}

void rename_directory(std::string from, std::string to) {
    for (auto& entry : std::filesystem::recursive_directory_iterator(from)) {
        if (!entry.is_regular_file()) continue;

        std::string p = entry.path();
        p = p.substr(from.length(), p.length() - from.length());

        std::filesystem::path from_file = from + p;
        std::filesystem::path to_file = to + p;

        std::filesystem::create_directories(to_file.parent_path());
        std::filesystem::rename(from_file, to_file);
    }
}