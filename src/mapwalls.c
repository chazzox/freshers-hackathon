#include <stdlib.h>
#include <stdio.h>
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

struct mapWalls *initMapWalls(char imageName []) {
    // open the file
    FILE *inputFile = fopen(imageName, "rb");

    // read the file as png
    struct PNG png;
    png.png_ptr = png_create_read_struct (PNG_LIBPNG_VER_STRING,
                                          NULL, NULL, NULL);

    if (png.png_ptr == NULL)
        return NULL;

    png.info_ptr = png_create_info_struct (png.png_ptr);

    if (png.info_ptr == NULL)
        return NULL;

    png_init_io (png.png_ptr, inputFile);
    png_read_png (png.png_ptr, png.info_ptr, 0, 0);
    png_get_IHDR (png.png_ptr, png.info_ptr, & png.width, & png.height, & png.bit_depth,
                  & png.color_type, & png.interlace_method, & png.compression_method,
                  & png.filter_method);

    png.rows = png_get_rows (png.png_ptr, png.info_ptr);
    png.bytes_pp = png_get_rowbytes (png.png_ptr, png.info_ptr) / png.width;

    // close the file
    fclose(inputFile);

    // create map image

}
