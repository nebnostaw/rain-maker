//
// Created by Benjamin Watson on 9/9/23.
//

#ifndef RAIN_MAKER_DARWIN_MEMORY_UTILS_H
#define RAIN_MAKER_DARWIN_MEMORY_UTILS_H

#include <mach/mach_types.h>
#include <mach-o/dyld_images.h>

class MemoryUtils {
    public:
    static unsigned char* ReadProcessMemory(pid_t pid, mach_vm_address_t address, const mach_msg_type_number_t *size);
    static dyld_all_image_infos * GetDyldImageInfo(pid_t pid);
};

#endif //RAIN_MAKER_DARWIN_MEMORY_UTILS_H
