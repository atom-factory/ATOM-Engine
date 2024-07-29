// Author: Jake Rieger
// Created: 4/30/2024.
//

#pragma once

#include "Types.h"

namespace IO {
    Option<str> Read(const Path& filename);
    Option<Vector<u8>> ReadAllBytes(const Path& filename);
    Option<Vector<str>> ReadAllLines(const Path& filename);
    Option<Vector<u8>> ReadBlock(const Path& filename, u32 blockOffset, size_t blockSize);

    bool Write(const Path& filename, const str& content);
    bool WriteAllBytes(const Path& filename, const Vector<u8>& bytes);
    bool WriteAllLines(const Path& filename, const Vector<str>& lines);

    Option<size_t> Size(const Path& filename);
}  // namespace IO