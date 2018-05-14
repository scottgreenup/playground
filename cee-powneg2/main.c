
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long optimised(int n) {
    long long x = 2;

    if (n % 2 == 0) {
        for (int i = 1; i < n; i++ && i++) {
            x <<= 2;
        }
        return x;
    } else {
        for (int i = 1; i < n; i++ && i++) {
            x <<= 1;
        }
    }

    //return x * ((-(n & 1) << 1) + 1);

    if ((n & 1) == 0) {
        return x;
    } else {
        return -x;
    }
}

int main(int argc, char* argv[]) {

    if (argc <= 3) {
        printf("Need argument, -o or -n");
        return 2;
    }

    int n = atoi(argv[2]);
    int c = atoi(argv[3]);

    long long x = 0;
    long double y = 0.0;

    for (int i = 0; i < c; i++) {
        if (strcmp(argv[1], "-o") == 0) {
            x = optimised(n);
        } else {
            y = powl(-2.0, n);
        }
    }

    printf(" = %ld\n", x);
    printf(" = %llf\n", y);
    return 0;
}

