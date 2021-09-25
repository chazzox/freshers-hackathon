# FRESHERS HACKATHON 2021
## RTS for the big touch screen in Shilling
### Technology
 - Cmake
 - (Allegro)[https://github.com/liballeg/allegro5]
 - Naked ANSI C

### Running this project
We have a modded manjaro ISO that launches the program on launch. This works really well.
Install community/allegro *(manjaro package)* then run the executable.

```bash
sudo pacman -Sy; sudo pacman -S allegro; ./rts
```

### Compiling
**Compile with DEBUG for HD and RELEASE for 4k the resolution of the big tablet.**

#### Requirements
 - libpng
 - zlib
 - allegro
 - pthread

#### To compile
Use cmake or run `comp.sh` which has the commands to setup cmake, compile and run.

## Assets
### BACKGROUND
This is the background image and background_collisions is a map of the hitboxes. Non-black pixels are walls.
