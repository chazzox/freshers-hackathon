#!/usr/bin/env python3

import os
import re

thisDir = os.path.dirname(os.path.abspath(__file__))
sourcePath = os.path.join(thisDir, "src")

ASSETS = "{thisDir}/assets"
IMAGES = f"{ASSETS}/images"
DEF_RE = re.compile("[a-zA-Z0-9_-]", re.I)

ASSETS_OUT = f"{sourcePath}/assets.h"

def toDefine(filename):
    name = []
    [name.append(g) for g in DEF_RE.search(filename)]
    return "".join(name).upper()

f = open(ASSETS_OUT, "w")
f.write("#pragma once\n")
for file in os.walk(IMAGES):
    define = toDefine(file)
    print(f"Defined {define}")
    f.write(f"#define \"{define}\"\n")


f.close()
