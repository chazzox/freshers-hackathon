# HACKATHON
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
#### Requirements
 - libpng
 - zlib
 - allegro
 - pthread
#### To compile
Use cmake or run `comp.sh` which has the commands to setup cmake, compile and run.
