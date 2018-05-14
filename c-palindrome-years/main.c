
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>

bool binary_palindrome(int32_t value) {
    int h = 31;
    while (!(value & 1 << h)) {
        --h;
    }

    for (int i = 0; i < h; ++i) {
        bool a = value & 1 << i;
        bool b = value & 1 << (h-i);

        if (a != b) {
            return false;
        }
    }

    return true;
}

int main() {
    time_t curr = time(NULL);
    int32_t year = localtime(&curr)->tm_year + 1900;

    for (int y = year; y < (year+200); ++y) {
        if (binary_palindrome(y)) {
            printf("%d is a palindrome.\n", y);
        }
    }

    return 0;
}
