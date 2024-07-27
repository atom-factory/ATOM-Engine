// Author: Jake Rieger
// Created: 7/27/2024.
//

#pragma once

#include "Platform.h"
#include "Types.h"

namespace Atom {
    class Timer {
    public:
        void Start() {
            QueryPerformanceFrequency(&m_Frequency);
            QueryPerformanceCounter(&m_LastTime);
        }

        double GetDeltaTime() {
            LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);
            const LONGLONG elapsed = currentTime.QuadPart - m_LastTime.QuadPart;
            m_LastTime             = currentTime;
            return CAST<f64>(elapsed) / CAST<f64>(m_Frequency.QuadPart);
        }

    private:
        LARGE_INTEGER m_Frequency = {};
        LARGE_INTEGER m_LastTime  = {};
    };
}  // namespace Atom