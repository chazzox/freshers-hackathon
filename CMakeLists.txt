cmake_minimum_required(VERSION 3.12)

# set the project name
project(CursedRTS_Hackathon)
set(REPO_URL "https://github.com/chazzox/freshers-hackathon")

# compiler flags
set(CMAKE_C_FLAGS_DEBUG "-g -O0 -Wall -pthread -Wextra -Wno-unused-parameter -Wno-writable-strings -DDEBUG=1")
# -fsanitize=address,undefined ???
set(CMAKE_C_FLAGS_RELEASE "-O2 -Wall -pthread -Wno-unused-parameter -Wno-writable-strings -DDEBUG=0")

# run the preprocessing python file
set(PRE_PY "${CMAKE_SOURCE_DIR}/preprocessing.py")
add_custom_target(prepy ${PRE_PY})

# src
set(MAIN
    src/main.c
    )
set(SRC_FILES
    src/utils.c
    )

add_executable(CursedRTS_Hackathon ${SRC_FILES} ${MAIN})
target_link_libraries(CursedRTS_Hackathon png z pthread allegro allegro_main allegro_image allegro_primitives allegro_color allegro_font allegro_ttf allegro_audio)
add_dependencies(CursedRTS_Hackathon prepy)
