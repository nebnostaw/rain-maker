//
// Created by Benjamin Watson on 10/1/23.
//

#ifndef RAIN_MAKER_DARWIN_BUNDLE_H
#define RAIN_MAKER_DARWIN_BUNDLE_H


#include <unistd.h>
#include <cstdint>
#include <vector>
#include <string>

#define MAGIC_SIZE 4

class Bundle {
public:
    Bundle(uint8_t *sectionData, uint64_t sectionDataSize)
            : section_data(sectionData),
            section_data_size(sectionDataSize) {

    }

    uint8_t * GetSectionData();
    std::string GetPayload();
    struct Header {
        uint32_t size;
        uint32_t crc;
    };
private:
    uint8_t *section_data;
    uint64_t section_data_size;
    std::vector<uint8_t> ReadMagic();
    std::vector<uint8_t> ReadPayload(Bundle::Header *header);
    Header * ReadHeader();
    static bool ValidateMagic(const std::vector<u_int8_t>& source, const std::vector<u_int8_t>& target);
    static std::string DecryptPayload(Header *header, std::vector<uint8_t> payload, std::vector<uint8_t> &key);
};


#endif //RAIN_MAKER_DARWIN_BUNDLE_H
