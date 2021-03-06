cmake_minimum_required(VERSION 3.12)

# set the project name
project(FreshersTD_Hackathon)
set(REPO_URL "https://github.com/chazzox/freshers-hackathon")

# Link Homebrew Dir if platform macos
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	link_directories("/opt/homebrew/lib")
	include_directories("/opt/homebrew/include")
endif()

# compiler flags
set(CMAKE_C_FLAGS_DEBUG "-DALLEGRO_UNSTABLE -DTOUCH=0 -g -O0 -Wall -pthread -Wextra -Wno-unused-parameter -DDEBUG=1 -DRES_X=1920 -DRES_Y=1080 -DCOMP_SOC_COIN_WIDTH=248")
# -fsanitize=address,undefined ???
set(CMAKE_C_FLAGS_RELEASE "-DALLEGRO_UNSTABLE -DTOUCH=1 -O2 -Wall -pthread -Wno-unused-parameter -DDEBUG=0 -DRES_X=3840 -DRES_Y=2160 -DCOMP_SOC_COIN_WIDTH=248")

# run the preprocessing python file
set(PRE_PY "${CMAKE_SOURCE_DIR}/preprocessing.py")
add_custom_target(prepy ${PRE_PY})

# src
set(MAIN
    src/main2.c
    )
set(SRC_FILES
    src/utils.c
    src/entity.c
    src/mapwalls.c
    src/gamestate.c
    )

add_executable(FreshersTD_Hackathon ${SRC_FILES} ${MAIN})
target_link_libraries(FreshersTD_Hackathon
    png z pthread m
    allegro allegro_main allegro_image allegro_primitives
    allegro_color allegro_font allegro_ttf allegro_audio)
add_dependencies(FreshersTD_Hackathon prepy)
