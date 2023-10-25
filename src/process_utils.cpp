//
// Created by Benjamin Watson on 8/26/23.
//

#include <iostream>
#include <libproc.h>
#include "process_utils.h"
#include <filesystem>


int ProcessUtils::GetProcessList(kinfo_proc **process_list, size_t *process_count) {

    size_t size;
    int mib[3];
    int err;
    int limit = 8;

    // Specify the Management Information Base
    mib[0] = CTL_KERN; // High Kernel Limits
    mib[1] = KERN_PROC; // Return the entire process table
    mib[2] = KERN_PROC_ALL;

    // Set to zero
    *process_count = 0;

    // Countdown
    while(limit-- > 0) {
        size = 0;
        if (sysctl(static_cast<int *>(mib), 3, nullptr, &size, nullptr, 0) == -1) {
            std::cout << "Unable to get the appropriate size" << std::endl;
            return 1;
        }
        std::cout << "Number of processes " << size << std::endl;
        // Allocate memory for process info
        void *ptr = malloc(size);
        if (ptr == nullptr) {
            std::cout << "Unable to allocate memory for process size" << std::endl;
            return 1;
        }
        // Get the kinfo_proc data
        if (sysctl(static_cast<int *>(mib), 3, ptr, &size, nullptr, 0) == -1) {
            free(ptr);
            err = errno;
            if (err != ENOMEM) {
                std::cout << "Unable to get process info" << std::endl;
            }
        } else {
            *process_list = static_cast<kinfo_proc *>(ptr);
            *process_count = size / sizeof(kinfo_proc);
            if (*process_count <= 0) {
                std::cout << "Zero PID(s) found" << std::endl;
                return 1;
            }
            return 0;
        }
    }
    std::cout << "Unable to collect PID(s)" << std::endl;
    return 1;
}

void ProcessUtils::GetExecutablePath(pid_t pid, char *buff) {
    int ret;
    ret = proc_pidpath(pid, buff, PATH_MAX);
    if (ret == 0) {
        std::cout << "Failure" << std::endl;
    }
}

std::string ProcessUtils::FindProcessExecutable(const std::string& target_file_name) {

    size_t process_count;
    kinfo_proc *process_list;

    // Get the process list
    ProcessUtils::GetProcessList(&process_list, &process_count);

    for (int i = 0; i < process_count; i++) {
        pid_t pid = process_list[i].kp_proc.p_pid;
        if (pid == 0) {
            continue;
        }
        char *buff = static_cast<char *>(malloc(PATH_MAX));
        ProcessUtils::GetExecutablePath(pid, buff);
        std::string path = std::string(buff);
        std::string filename = std::filesystem::path(path).filename();
        if (filename == target_file_name) {
            free(buff);
            return path;
        }
        free(buff);
    }
    free(process_list);
    return {};
}


