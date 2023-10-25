#include "bundle_reader.h"

#include <iostream>

int main() {

    pid_t pid = getpid();
    Bundle bundle = BundleReader::ReadBundle(pid);
    if (bundle.GetSectionData() != nullptr) {
        std::string payload = bundle.GetPayload();
        std::cout << payload << std::endl;
    }
    return 0;
}
