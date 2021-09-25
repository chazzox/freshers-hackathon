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
    filename = filename.upper().replace(".PNG", "")
    name = ""
    for c in filename:
        if c in "0123456789ABCDEFGHJIKLMNOPQRSTUVWXYZ_-":
            name += c
    return name

assetsFile = open(ASSETS_OUT, "w")
assetsFile.write("#pragma once\n")
assetsFile.write("#include <allegro5/allegro.h>\n")
assetsFile.write("#include <allegro5/allegro_image.h>\n")

bitmapInit = ""
bitmapFree = ""
for file in os.listdir(IMAGES):
    define = toDefine(file)
    print(f"Defined {define}")
    assetsFile.write(f"ALLEGRO_BITMAP *__{define};\n")
    bitmapInit += f"__{define}= al_load_bitmap(\"{IMAGES}/{file}\");\n"
    bitmapFree += f"al_destroy_bitmap({define});"

    assetsFile.write(f"#define {define} __{define}\n")
    assetsFile.write(f"#define IMG_{define} \"{IMAGES}/{file}\"\n")

assetsFile.close()

mainFile = open(f"{sourcePath}/main.c", "r")
mainSRC = mainFile.read().replace("// __INIT__BITMAPS__", bitmapInit).replace("// __FREE__BITMAPS__", bitmapFree)
mainFile.close()

mainFile = open(f"{sourcePath}/main2.c", "w")
mainFile.write(mainSRC)
mainFile.close()
