// Author: Jake Rieger
// Created: 4/30/2024.
//

#include "IO.h"

#include <fstream>

namespace IO {
    Option<str> Read(const Path& filename) {
        if (!exists(filename) || is_directory(filename)) {
            return std::nullopt;
        }

        std::ifstream file(filename);
        if (!file.is_open()) {
            return std::nullopt;
        }

        str content((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());
        file.close();

        return content;
    }

    Option<Vector<u8>> ReadAllBytes(const Path& filename) {
        if (!exists(filename) || is_directory(filename)) {
            return std::nullopt;
        }

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            return std::nullopt;
        }

        Vector<u8> bytes(std::istreambuf_iterator(file), {});
        file.close();

        return bytes;
    }

    Option<Vector<str>> ReadAllLines(const Path& filename) {
        if (!exists(filename) || is_directory(filename)) {
            return std::nullopt;
        }

        std::ifstream file(filename);
        if (!file.is_open()) {
            return std::nullopt;
        }

        Vector<str> lines;
        str line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }

        file.close();

        return lines;
    }

    Option<Vector<u8>>
    ReadBlock(const Path& filename, const u32 blockOffset, const size_t blockSize) {
        if (!exists(filename) || is_directory(filename)) {
            return std::nullopt;
        }

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            return std::nullopt;
        }

        file.seekg(blockOffset, std::ios::beg);
        if (!file.good()) {
            return std::nullopt;
        }

        Vector<u8> buffer(blockSize);
        file.read(reinterpret_cast<char*>(buffer.data()), static_cast<std::streamsize>(blockSize));

        if (file.gcount() != static_cast<std::streamsize>(blockSize)) {
            return std::nullopt;
        }

        return buffer;
    }

    bool Write(const Path& filename, const str& content) {
        std::ofstream outfile(filename);
        if (!outfile.is_open()) {
            return false;
        }

        outfile.write(content.c_str(), static_cast<std::streamsize>(content.length()));
        outfile.close();

        return true;
    }

    bool WriteAllBytes(const Path& filename, const Vector<u8>& bytes) {
        std::ofstream outfile(filename, std::ios::binary);
        if (!outfile.is_open()) {
            return false;
        }

        outfile.write(reinterpret_cast<const char*>(bytes.data()),
                      static_cast<std::streamsize>(bytes.size()));
        outfile.close();

        return true;
    }

    bool WriteAllLines(const Path& filename, const Vector<str>& lines) {
        std::ofstream outfile(filename);
        if (!outfile.is_open()) {
            return false;
        }

        for (const auto& line : lines) {
            outfile << line << std::endl;
        }

        outfile.close();

        return true;
    }

    Option<size_t> Size(const Path& filename) {
        if (!exists(filename) || is_directory(filename)) {
            return std::nullopt;
        }

        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            return std::nullopt;
        }

        file.seekg(0, std::ios::end);
        const size_t size = file.tellg();
        file.seekg(0, std::ios::beg);

        return size;
    }
}  // namespace IO