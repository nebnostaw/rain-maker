//
// Created by Benjamin Watson on 9/9/23.
//

#ifndef RAIN_MAKER_DARWIN_BUNDLE_READER_H
#define RAIN_MAKER_DARWIN_BUNDLE_READER_H

#include <unistd.h>
#include <mach-o/loader.h>
#include "bundle.h"

class BundleReader {
public:
    static Bundle ReadBundle(pid_t pid);
private:
    static bool FindAndLoadSection(mach_header_64 *header, uint8_t **section_data, uint64_t *section_data_size);
};


#endif //RAIN_MAKER_DARWIN_BUNDLE_READER_H
