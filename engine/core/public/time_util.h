#ifndef MELON_TIME_H
#define MELON_TIME_H

#include "types.h"

namespace TimeUtil {

    struct SysClock {
        s32 hour = 0;
        s32 minute = 0;
        s32 second = 0;
        s32 day = 0;
        s32 month = 0;
        s32 year = 0;
    };

    u64 stamp();
    u64 stamp_freq();
    u64 tick_ms();
    f32 delta();
    void update();
    void get_system_clock(SysClock& time);

    inline static u64 previous_time = 0;
    inline static float delta_time = 0.0f;
}

#endif // MELON_TIME_H
