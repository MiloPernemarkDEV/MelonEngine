#include "time_util.h"
#include "logger.h"

#ifdef _WIN32
#include <windows.h>

namespace TimeUtil {
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
    void get_system_clock(SysClock& time){
        SYSTEMTIME systime{};
        GetSystemTime(&systime);
        time.hour = static_cast<s32>(systime.wHour);
        time.minute = static_cast<s32>(systime.wMinute);
        time.second = static_cast<s32>(systime.wSecond);
        time.day = static_cast<s32>(systime.wDay);
        time.month = static_cast<s32>(systime.wMonth);
        time.year = static_cast<s32>(systime.wYear);
    }
}

#endif // _WIN32
