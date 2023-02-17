// math.c

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

int32_t abs(int32_t x) {
    return (x < 0) ? -x : x;
}

int32_t log10(int32_t x) {
    if (x < 0) {
        x = -x;
    }
    int32_t count = 0;
    while (x > 0) {
        x /= 10;
        count++;
    }
    return count;
}
