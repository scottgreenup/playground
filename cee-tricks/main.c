
/* 
 * What is the difference?
 */

#include <stdio.h>
#include <string.h>

typedef struct _Foo {
    unsigned char a : 1;
    unsigned char b : 1;
    unsigned char c : 1;
    unsigned char d : 4;
} Foo;

int main() {

    Foo A = {1};

    Foo B;
    memset(&B, sizeof(B), 1);

    unsigned char fromA = A.a;
    unsigned char fromB = B.a;

    printf("%d\n%d\n", fromA, fromB);
}
