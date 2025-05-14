#include "g_blur.h"

void gaussian_blur(png_uint_32 height, png_uint_32 wight, png_bytep dest, png_bytep src){
    png_uint_32 src_wight = wight + 2;
    for(png_uint_32 i = 0; i < height; i++){
        for(png_uint_32 j = 0; j < wight; j++){
            int dest_index = i * wight + j;
            int src_index = i * src_wight + j;
            int src_next_row = (i + 1) * src_wight + j;
            int src_next_next_row = (i + 2) * src_wight + j;
            int times_one = src[src_index] + src[src_index + 2];
            times_one += src[src_next_next_row] + src[src_next_next_row + 2];
            int times_two = src[src_index + 1];
            times_two += src[src_next_row] + src[src_next_row + 2];
            times_two += src[src_next_next_row + 1];
            int times_four = src[src_next_row + 1];
            dest[dest_index] = (times_one + times_two * 2 + times_four * 4) / 16;
        }
    }
}