#ifndef MELON_TIME_H
#define MELON_TIME_H

#include "types.h"

namespace Time {
    u64 stamp();
    u64 stamp_freq();
    u64 tick_ms();
    f32 delta();
    void update();

    inline static u64 previous_time = 0;
    inline static float delta_time = 0.0f;
}


#endif // MELON_TIME_H