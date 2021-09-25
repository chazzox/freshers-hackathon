#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <png.h>
#include "mapwalls.h"

struct PNG {
    png_structp	png_ptr;
    png_infop info_ptr;

    png_uint_32 width;
    png_uint_32 height;

    png_bytepp rows;

    int bit_depth;
    int color_type;
    int interlace_method;
    int compression_method;
    int filter_method;
    int bytes_pp;
};

struct mapWalls initMapWalls(char imageName []) {
    // open the file
    FILE *inputFile = fopen(imageName, "rb");

    // read the file as png
    struct PNG png;
    png.png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING,
                                          NULL, NULL, NULL);

    if (png.png_ptr == NULL) {
        fprintf(stderr, "png_ptr is NULL\n");
        exit(13);
    }

    png.info_ptr = png_create_info_struct (png.png_ptr);

    if (png.info_ptr == NULL) {
        fprintf(stderr, "png_info_ptr is NULL\n");
        exit(14);
    }

    png_init_io (png.png_ptr, inputFile);
    png_read_png (png.png_ptr, png.info_ptr, 0, 0);
    png_get_IHDR (png.png_ptr, png.info_ptr, & png.width, & png.height, & png.bit_depth,
                  & png.color_type, & png.interlace_method, & png.compression_method,
                  & png.filter_method);

    png.rows = png_get_rows (png.png_ptr, png.info_ptr);
    png.bytes_pp = png_get_rowbytes (png.png_ptr, png.info_ptr) / png.width;

    // close the file
    fclose(inputFile);

    // create map image as an int[] where each int is in form bgr lmao
    // it is bgr as I am looking for white and the endianness doesn't matter
    int *data = (int *) malloc(sizeof(int) * png.width * png.height);

    for (long y = 0; y < png.height; y++) {
        for (long x = 0; x < png.width; x++) {
            data[x + y * png.width] = 0;

            for (long i = 0; i < png.bytes_pp; i++) {
                data[x + y * png.width] |= png.rows[y][png.bytes_pp * x + i] << 8 * i;
            }
        }
    }

    // create map walls
    struct mapWalls map;
    // init to false
    memset(&map, 0, sizeof(map));

    for (unsigned int x = 0; x < png.width; x++) {
        for (unsigned int y = 0; y < png.height; y++) {
            map.wallArr[x][y] = data[x + y * png.width] != 0;
        }
    }

    // free resources
    free(data);

    //Free png rows
    for (unsigned int y = 0; y < png.height; y++) {
        png_free (png.png_ptr, png.rows[y]);
    }
    png_free (png.png_ptr, png.rows);

    return map;
}
