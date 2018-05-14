

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>

void die(const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);

    if (fmt[0] && fmt[strlen(fmt)-1] == ':') {
        fputc(' ', stderr);
        perror(NULL);
    }

    exit(1);
}

int main(void) {
    Display* display = XOpenDisplay(NULL);

    if (!display) {
        die("Could not open display\n");
    }

    int screen = DefaultScreen(display);
    int sw = DisplayWidth(display, screen);
    int sh = DisplayHeight(display, screen);

    printf("screen        : %d\n", screen);
    printf("screen height : %d\n", sw);
    printf("screen width  : %d\n", sh);

    Window root = RootWindow(display, screen);

    return 0;
}
