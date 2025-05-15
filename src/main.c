#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <zlib.h>
#include "g_blur.h"

#include <time.h>

#define BLUR_POWER 3
#define POINTLESS_REPETIONS 100

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

void copy_row_chanell(png_bytep dest, png_bytep src, png_uint_32 wight, e_chanell chanell){
    png_uint_32 copy_index = 0;
    dest[copy_index++] = src[0];
    for(png_uint_32 i = 0; i < wight; i++){
        dest[copy_index++] = src[i * 4 + chanell];
    }
    dest[copy_index++] = src[(wight - 1) * 4 + chanell];
}

void copy_chanell_and_expand(png_uint_32 height, png_uint_32 wight, png_bytep dest, png_bytep data, e_chanell chanell){
    png_uint_32 new_wight = wight + 2;
    copy_row_chanell(dest, data, wight, chanell);
    for(png_uint_32 i = 0; i < height; i++){
        copy_row_chanell(dest + ((i + 1) * new_wight), data + (i * wight * 4), wight, chanell);
    }
    copy_row_chanell(dest + (height + 1) * new_wight, data + ((height - 1) * wight * 4), wight, chanell);
}

png_bytep copy_chanell(png_uint_32 height, png_uint_32 wight, png_bytep data, e_chanell chanell){
    png_uint_32 size = height * wight;
    png_bytep res = (png_bytep)malloc(size);
    for(png_uint_32 i = 0; i < size; i++){
        res[i] = data[i * 4 + chanell];
    }
    return res;
}

void recollect(png_uint_32 height, png_uint_32 wight, png_bytep dest, png_bytep red, png_bytep green, png_bytep blue){
    for(png_uint_32 i = 0; i < height * wight; i++){
        dest[i * 4 + chanell_r] = red[i];
        dest[i * 4 + chanell_g] = green[i];
        dest[i * 4 + chanell_b] = blue[i];
    };
}

int main(int argc, const char **argv)
{
    const char *input_name;
    const char *output_name;

    switch(argc){
        case 1:
        input_name = "pictures/cat.png";
        output_name = "pictures/res.png";
        break;

        case 3:
        input_name = argv[1];
        output_name = argv[2];
        break;

        default:
        fprintf(stderr, "usage: %s input-file output-file\n", argv[0]);
        exit(2);
    }

    png_image image;

    memset(&image, 0, (sizeof image));
    image.version = PNG_IMAGE_VERSION;

    if (png_image_begin_read_from_file(&image, input_name) == 0){
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

    r_buffer = (png_bytep)malloc((image.width + 2) * (image.height + 2));
    g_buffer = (png_bytep)malloc((image.width + 2) * (image.height + 2));
    b_buffer = (png_bytep)malloc((image.width + 2) * (image.height + 2));

    r_new_buffer = (png_bytep)malloc(image.width * image.height);
    g_new_buffer = (png_bytep)malloc(image.width * image.height);
    b_new_buffer = (png_bytep)malloc(image.width * image.height);

    double time_spent = 0;
    clock_t start, end;
    

    for(int i = 0; i < BLUR_POWER; i++){
        copy_chanell_and_expand(image.height, image.width, r_buffer, buffer, chanell_r);
        copy_chanell_and_expand(image.height, image.width, g_buffer, buffer, chanell_g);
        copy_chanell_and_expand(image.height, image.width, b_buffer, buffer, chanell_b);


        for(int i = 0; i < POINTLESS_REPETIONS; i++){
            start = clock();
            gaussian_blur(image.height, image.width, r_new_buffer, r_buffer);
            gaussian_blur(image.height, image.width, g_new_buffer, g_buffer);
            gaussian_blur(image.height, image.width, b_new_buffer, b_buffer);
            end = clock();
            time_spent += (double)(end - start) / CLOCKS_PER_SEC;
        }

        recollect(image.height, image.width, buffer, r_new_buffer, g_new_buffer, b_new_buffer);
    }

    printf("Суммарное время, затраченное на выполнение целевой функции: %lf секунд\n", time_spent);
    
    if (png_image_write_to_file(&image, output_name, 0/*convert_to_8bit*/,
        buffer, 0/*row_stride*/, NULL/*colormap*/) != 0)
    {
        /* The image has been written successfully. */
        exit(0);
    }
}