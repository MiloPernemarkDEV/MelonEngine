#include "time.h"

#ifdef _WIN32
#include <windows.h>

namespace Time {
    u64 stamp() {
        LARGE_INTEGER val;
        const BOOL ret = QueryPerformanceCounter(&val);
        if (!ret){ return {}; };

        return val.QuadPart;
    }

    u64 stamp_freq() {
        LARGE_INTEGER val;
        const BOOL ret = QueryPerformanceFrequency(&val);
        if (!ret){ return {}; };
        return val.QuadPart;
    }

    u64 tick_ms() {
        return GetTickCount64();
    }

    void update() {
        u64 current_time = stamp();

        if (previous_time == 0)
        {
            previous_time = current_time;
            return;
        }

        delta_time = static_cast<float>(current_time - previous_time) /
                static_cast<float>(stamp_freq());

        previous_time = current_time;
    }

    float delta()
    {
        return delta_time;
    }
}

#endif // _WIN32


