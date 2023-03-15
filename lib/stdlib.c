#include "stdlib.h"

void* memset(void* dst, int value, size_t size) {
    unsigned char* ptr = dst;
    while (size-- > 0) {
        *ptr++ = (unsigned char)value;
    }
    return dst;
}
