
#include <stdio.h>

typedef struct _MyStruct {
    int Doku;
    int Nani;
    int Naze;
} MyStruct; 

typedef MyStruct Foo[1];

int main(void) {
    Foo x;

    x[0].Doku = 12;
    x[1].Doku = 20;
    x[2].Doku = 24;

    printf("%d\n", x[0].Doku);
    printf("%d\n", x[1].Doku);
    printf("%d\n", x[2].Doku);

    return 0;
}
