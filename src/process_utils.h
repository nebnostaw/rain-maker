//
// Created by Benjamin Watson on 8/26/23.
//

#ifndef RAIN_MAKER_DARWIN_PROCESS_UTILS_H
#define RAIN_MAKER_DARWIN_PROCESS_UTILS_H

#include <sys/sysctl.h>


class ProcessUtils {
public:
    static int GetProcessList(kinfo_proc **process_list, size_t *process_count);
    static void GetExecutablePath(pid_t pid, char *buff);
    static std::string FindProcessExecutable(const std::string &target_file_name);
};
#endif //RAIN_MAKER_DARWIN_PROCESS_UTILS_H
