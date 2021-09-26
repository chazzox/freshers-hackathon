# FRESHERS HACKATHON 2021

## A Tower Defense Game for the big touch screen in the Shilling Building

### Technology

- [Cmake](https://cmake.org/)
- [Allegro](https://github.com/liballeg/allegro5)
- Naked ANSI C

### Running this project

We have a modded manjaro ISO that launches the program on launch. This works really well.
Install community/allegro _(manjaro package)_ then run the executable.

```bash
sudo pacman -Sy; sudo pacman -S allegro; ./rts
```

You can also run the `iso_run.sh` file, this will install build tools, compile and run the game

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
