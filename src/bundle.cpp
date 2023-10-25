//
// Created by Benjamin Watson on 10/1/23.
//

#include "bundle.h"
#include "swap_utils.h"

uint8_t *Bundle::GetSectionData() {
    return this->section_data;
}

bool Bundle::ValidateMagic(const std::vector<u_int8_t>& source, const std::vector<u_int8_t>& target) {
    return source == target;
}

std::vector<uint8_t> Bundle::ReadMagic() {
    std::vector<u_int8_t> magic;
    magic.reserve(MAGIC_SIZE);
    for (int i = 0; i < MAGIC_SIZE; ++i) {
        magic.push_back(*(this->GetSectionData() + i));
    }
    return magic;
}

Bundle::Header* Bundle::ReadHeader() {
    auto* header = static_cast<Header *>(malloc(sizeof(Header)));
    uint8_t *data = this->GetSectionData();
    auto *header_start = (u_int8_t *) ((char *) data + MAGIC_SIZE);
    memcpy(header, header_start, sizeof(Header));
    header->size = SwapUtils::SwapUint32(header->size);
    header->crc = SwapUtils::SwapUint32(header->crc);
    return header;
}

std::vector<uint8_t> Bundle::ReadPayload(Bundle::Header *header) {
    std::vector<uint8_t> payload;
    payload.reserve(header->size);
    auto *payload_start = (u_int8_t *)((char *) Bundle::GetSectionData() + MAGIC_SIZE + sizeof(Bundle::Header));
    for (int i = 0; i < header->size; ++i) {
        payload.push_back(*(payload_start + i));
    }
    return payload;
}

std::string Bundle::DecryptPayload(Bundle::Header *header, std::vector<uint8_t> payload, std::vector<uint8_t> &key) {
    // Stored decrypted material here
    auto *decrypted = new u_int8_t[header->size];

    for (int i = 0; i < header->size; i++) {
        if (i == header->size) {
            decrypted[i] = '\0';
        }
        decrypted[i] = payload[i] ^ key[i % 4];
    }
    return {reinterpret_cast<const char *>(decrypted)};
}

std::string Bundle::GetPayload() {
    std::vector<uint8_t> magicHeaderVector {0x52, 0x41, 0x49, 0x4e};
    if (!Bundle::ValidateMagic(magicHeaderVector, this->ReadMagic())) {
        return "";
    }
    Bundle::Header *header = Bundle::ReadHeader();
    std::vector<u_int8_t> payload = Bundle::ReadPayload(header);
    return Bundle::DecryptPayload(header, payload, magicHeaderVector);
}

