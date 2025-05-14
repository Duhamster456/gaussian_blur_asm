#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <png.h>
#include <zlib.h>

typedef enum e_chanell{
    chanell_r = 0,
    chanell_g = 1,
    chanell_b = 2,
    chanell_a = 3
} e_chanell;

png_bytep r_chanell;
png_bytep g_chanell;
png_bytep b_chanell;
png_bytep alpha_chanell;

png_bytep copy_chanell_and_expand(int height, int wight, png_bytep data, e_chanell chanell){
    png_bytep res = (png_bytep)malloc(height * wight + height * 2 + wight * 2 + 4);
    int copy_index = 0;
    for(int i = 0; i < wight; i++){
        res[copy_index++] = data[i * 4 + chanell];
    }
    for(int i = 0; i < height; i++){
        png_bytep current_row = data + i * wight;
        res[copy_index++] = current_row[chanell];
        for(int j = 0; j < wight; j++){
            res[copy_index++] = current_row[j * 4 + chanell];
        }
        res[copy_index++] = current_row[(wight - 1) * 4 + chanell];
    }
    int last_row_index = (height - 1) * wight;
    for(int i = last_row_index; i < last_row_index + wight; i++){
        res[copy_index++] = data[i * 4 + chanell];
    }
    return res;
}

png_bytep recollect(int height, int wight, png_bytep red, png_bytep green, png_bytep blue, png_bytep alpha){
    png_bytep res = (png_bytep)malloc(height * wight * 4);
    for(int i = 0; i < height * wight; i++){
        res[i * 4 + chanell_r] = red[wight + 1 + i];
        res[i * 4 + chanell_g] = green[wight + 1 + i];
        res[i * 4 + chanell_b] = blue[wight + 1 + i];
        res[i * 4 + chanell_a] = alpha[wight + 1 + i];
    }
    return res;
}

png_bytep gauss_copy(int height, int wight, png_bytep data){

}

void do_the_thing(int height, int wight, png_bytep data){

}

void gauss_process(int height, int wight, png_bytep data){
    gauss_copy(height, wight, data);

}

int main(int argc, const char **argv)
{
    if(argc != 3){
        fprintf(stderr, "pngtopng: usage: pngtopng input-file output-file\n");
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

    srand(4400);

    for(int i = 0; i < image.height; i++){
        char offset = 0;
        png_bytep current_ptr = buffer + i * image.width * 4;
        for(int j = 0; j < image.width; j++){
            current_ptr[j * 4] += rand();
            current_ptr[j * 4 + 1] += offset;
            current_ptr[j * 4 + 2] += offset;
            offset++;
        }
    }
    if (png_image_write_to_file(&image, argv[2], 0/*convert_to_8bit*/,
        buffer, 0/*row_stride*/, NULL/*colormap*/) != 0)
    {
        /* The image has been written successfully. */
        exit(0);
    }
}