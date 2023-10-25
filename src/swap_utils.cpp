//
// Created by Benjamin Watson on 10/8/23.
//

#include "swap_utils.h"

uint32_t SwapUtils::SwapUint32(uint32_t val) {
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF );
    return (val << 16) | (val >> 16);
}
