//
// Created by Benjamin Watson on 9/9/23.
//

#include <mach/mach.h>
#include "memory_utils.h"

unsigned char *MemoryUtils::ReadProcessMemory(int pid, mach_vm_address_t address, const mach_msg_type_number_t *size) {

    task_t task;
    kern_return_t ret;

    // Get our own task
    ret = task_for_pid(mach_task_self_, pid, &task);

    if (ret != KERN_SUCCESS) {
        return nullptr;
    }

    auto count = (mach_msg_type_number_t) *size;
    vm_offset_t memory;

    // Read the memory at the target address
    ret = vm_read(task, address, *size, &memory, &count);

    if (ret != KERN_SUCCESS) {
        return nullptr;
    }

    return ((unsigned char *) memory);

}

dyld_all_image_infos *MemoryUtils::GetDyldImageInfo(pid_t pid) {

    struct task_dyld_info dyld_info{};
    mach_msg_type_number_t count = TASK_DYLD_INFO_COUNT;
    kern_return_t ret;

    ret = task_info(mach_task_self_, TASK_DYLD_INFO, reinterpret_cast<task_info_t>(&dyld_info), &count);

    if (ret != KERN_SUCCESS) {
        return nullptr;
    }

    mach_vm_address_t image_infos;
    struct dyld_all_image_infos *all_image_infos;
    mach_msg_type_number_t size = sizeof(struct dyld_all_image_infos);
    image_infos = dyld_info.all_image_info_addr;
    all_image_infos = reinterpret_cast<dyld_all_image_infos *>(image_infos);

    unsigned char *data = ReadProcessMemory(pid,reinterpret_cast<mach_vm_address_t>(all_image_infos->dyldAllImageInfosAddress), &size);

    return reinterpret_cast<dyld_all_image_infos *>(data);
}
