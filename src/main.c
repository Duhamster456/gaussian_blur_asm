#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <zlib.h>
#include "g_blur.h"

typedef enum e_chanell{
    chanell_r = 0,
    chanell_g = 1,
    chanell_b = 2,
    chanell_a = 3
} e_chanell;

png_bytep r_buffer;
png_bytep g_buffer;
png_bytep b_buffer;

png_bytep r_new_buffer;
png_bytep g_new_buffer;
png_bytep b_new_buffer;

void copy_row_chanell(png_bytep dest, png_bytep src, int wight, e_chanell chanell){
    int copy_index = 0;
    dest[copy_index++] = src[0];
    for(int i = 0; i < wight; i++){
        dest[copy_index++] = src[i * 4 + chanell];
    }
    dest[copy_index++] = src[(wight - 1) * 4 + chanell];
}

png_bytep copy_chanell_and_expand(int height, int wight, png_bytep data, e_chanell chanell){
    png_bytep res = (png_bytep)malloc(height * wight + height * 2 + wight * 2 + 4);
    int new_wight = wight + 2;
    copy_row_chanell(res, data, wight, chanell);
    for(int i = 0; i < height; i++){
        copy_row_chanell(res + ((i + 1) * new_wight), data + (i * wight * 4), wight, chanell);
    }
    return res;
}

png_bytep copy_chanell(int height, int wight, png_bytep data, e_chanell chanell){
    int size = height * wight;
    png_bytep res = (png_bytep)malloc(size);
    for(int i = 0; i < size; i++){
        res[i] = data[i * 4 + chanell];
    }
    return res;
}

void recollect(int height, int wight, png_bytep dest, png_bytep red, png_bytep green, png_bytep blue){
    for(int i = 0; i < height * wight; i++){
        dest[i * 4 + chanell_r] = red[i];
        dest[i * 4 + chanell_g] = green[i];
        dest[i * 4 + chanell_b] = blue[i];
    };
}

int main(int argc, const char **argv)
{
    if(argc != 3){
        fprintf(stderr, "usage: %s input-file output-file\n", argv[0]);
        exit(2);
    }
    png_image image;

    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_file(&image, argv[1]) == 0){
        fprintf(stderr, "pngtopng: error: %s\n", image.message);
        exit(1);
    }
    png_bytep buffer;
    image.format = PNG_FORMAT_RGBA;
    buffer = malloc(PNG_IMAGE_SIZE(image));
    printf("Выделено памяти: %d\n", PNG_IMAGE_SIZE(image));

    if (png_image_finish_read(&image, NULL, buffer, 0, NULL) == 0)
    {
        if (buffer == NULL)
            png_image_free(&image);
        else
            free(buffer);
    }

    

    r_buffer = copy_chanell_and_expand(image.height, image.width, buffer, chanell_r);
    g_buffer = copy_chanell_and_expand(image.height, image.width, buffer, chanell_g);
    b_buffer = copy_chanell_and_expand(image.height, image.width, buffer, chanell_b);

    png_image image2 = image;
    image2.format = PNG_FORMAT_GRAY;
    image2.height += 2;
    image2.width += 2;
    png_image_write_to_file(&image2, "debug/red.png", 0, r_buffer, 0, NULL);
    png_image_write_to_file(&image2, "debug/green.png", 0, g_buffer, 0, NULL);
    png_image_write_to_file(&image2, "debug/blue.png", 0, b_buffer, 0, NULL);

    r_new_buffer = (png_bytep)malloc(image.width * image.height);
    g_new_buffer = (png_bytep)malloc(image.width * image.height);
    b_new_buffer = (png_bytep)malloc(image.width * image.height);

    gaussian_blur(image.height, image.width, r_new_buffer, r_buffer);
    gaussian_blur(image.height, image.width, g_new_buffer, g_buffer);
    gaussian_blur(image.height, image.width, b_new_buffer, b_buffer);

    recollect(image.height, image.width, buffer, r_new_buffer, g_new_buffer, b_new_buffer);
    if (png_image_write_to_file(&image, argv[2], 0/*convert_to_8bit*/,
        buffer, 0/*row_stride*/, NULL/*colormap*/) != 0)
    {
        /* The image has been written successfully. */
        exit(0);
    }
}