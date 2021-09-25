#!/usr/bin/env python3

import os
import re

thisDir = os.path.dirname(os.path.abspath(__file__))
sourcePath = os.path.join(thisDir, "src")

ASSETS = f"{thisDir}/assets"
IMAGES = f"{ASSETS}/images"
ASSETS_OUT = f"{sourcePath}/assets.h"

print("Started preprocesser.py")
print(f" - Current dir is {thisDir}")
print(f" - Source dir is {sourcePath}")
print(f" - Assets are in {ASSETS}")
print(f" - Images are in {IMAGES}")

def toDefine(filename):
    filename = filename[len(filename) - 1][0].upper().replace(".PNG", "")
    name = ""
    for c in filename:
        if c in "0123456789ABCDEFGHJIKLMNOPQRSTUVWXYZ_-":
            name += c
    return name

f = open(ASSETS_OUT, "w")
f.write("#pragma once\n")
for file in os.walk(IMAGES):
    define = toDefine(file)
    print(f"Defined {define}")
    f.write(f"#define {define} \"{IMAGES}/{file[len(file) - 1][0]}\"\n")


f.close()
