#!/usr/bin/env python3

import os
import re

thisDir = os.path.dirname(os.path.abspath(__file__))
sourcePath = os.path.join(thisDir, "src")

ASSETS = f"{thisDir}/assets"
IMAGES = f"{ASSETS}/images"
FONTS  = f"{ASSETS}/fonts"
ASSETS_OUT = f"{sourcePath}/assets.h"

print("Started preprocesser.py")
print(f" - Current dir is {thisDir}")
print(f" - Source dir is {sourcePath}")
print(f" - Assets are in {ASSETS}")
print(f" - Images are in {IMAGES}")

def toDefine(filename):
    filename = filename.upper().replace(".PNG", "").replace(".TTF", "")
    name = ""
    for c in filename:
        if c in "0123456789ABCDEFGHJIKLMNOPQRSTUVWXYZ_-":
            name += c
    return name

assetsFile = open(ASSETS_OUT, "w")
assetsFile.write("#pragma once\n")
assetsFile.write("#include <allegro5/allegro.h>\n")
assetsFile.write("#include <allegro5/allegro_image.h>\n")
assetsFile.write("#include <allegro5/allegro_ttf.h>\n")


externInit = ""
initAssets = ""
freeAssets = ""
for file in os.listdir(IMAGES):
    define = toDefine(file)
    print(f"Defined {define}")
    assetsFile.write(f"extern ALLEGRO_BITMAP *__{define};\n")
    externInit += f"ALLEGRO_BITMAP *__{define} = NULL;\n"
    initAssets += f"__{define} = al_load_bitmap(\"{IMAGES}/{file}\");\n"
    freeAssets += f"al_destroy_bitmap({define});\n"

    assetsFile.write(f"#define {define} __{define}\n")
    assetsFile.write(f"#define IMG_{define} \"{IMAGES}/{file}\"\n")

for file in os.listdir(FONTS):
    define = toDefine(file)
    print(f"Defined {define}")
    assetsFile.write(f"extern ALLEGRO_FONT *__{define};\n")
    externInit += f"ALLEGRO_FONT *__{define} = NULL;\n"
    initAssets += f"__{define} = al_load_ttf_font(\"{FONTS}/{file}\", COMP_SOC_COIN_WIDTH, ALLEGRO_TTF_NO_KERNING);\n"
    freeAssets += f"al_destroy_font({define});\n"

    assetsFile.write(f"#define {define} __{define}\n")
    assetsFile.write(f"#define FONT_{define} \"{FONTS}/{file}\"\n")    

assetsFile.close()

mainFile = open(f"{sourcePath}/main.c", "r")
mainSRC = mainFile.read().replace("__INIT__ASSETS__()", initAssets).replace("__FREE__ASSETS__()", freeAssets).replace("__EXTERN__ASSET__DEFS__()", externInit)
mainFile.close()

mainFile = open(f"{sourcePath}/main2.c", "w")
mainFile.write(mainSRC)
mainFile.close()
