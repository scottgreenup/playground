#!/usr/bin/env python

import sys


if __name__ == "__main__":

    stdin = sys.stdin.read()
    splat = stdin.split('\n')

    for h in splat:
        if len(h) == 0:
            continue
        h = h.rstrip().strip()

        if h[0:2] == "0x":
            h = h[2:]

        if len(h) != 6:
            print("A color hash is of form 0xFFFFFF or FFFFFF: {}".format(
                bytes(h, 'utf-8')
            ))
            sys.exit(1)

        format_string="{:.3f},{:.3f},{:.3f}".format(
            (int(h[0:2], 16) / 255.0),
            (int(h[2:4], 16) / 255.0),
            (int(h[4:7], 16) / 255.0),
        )

        print(format_string)
