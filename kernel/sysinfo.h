// sysinfo.h
#ifndef _SYSINFO_H_
#define _SYSINFO_H_

#include <stdint.h>

struct sysinfo {
    uint64_t freemem;   // Số lượng bộ nhớ trống (tính bằng byte)
    uint64_t nproc;     // Số lượng tiến trình đang hoạt động
};

#endif // _SYSINFO_H_