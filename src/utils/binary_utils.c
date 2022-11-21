#include <stdint.h>
#include "binary_utils.h"

void uint8_to_uint32(
    uint32_t *result,
    uint8_t first,
    uint8_t second,
    uint8_t third,
    uint8_t fourth
) {
    result[0] = (((uint32_t) first) << 24)
        | (((uint32_t) second) << 16)
        | (((uint32_t) third) << 8)
        | fourth;
}
