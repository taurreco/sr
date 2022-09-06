
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "sr.h"

/**
 * sr_tga.c
 * --------
 * loads tga images into memory
 * supports data types 1, 2, 9, & 10
 * and pixel / color depths of 16, 24, & 32
 * 
 */

struct tga_header {
   uint8_t id_len;
   uint8_t color_map_type;
   uint8_t data_type_code;
   int16_t color_map_origin;
   int16_t color_map_len;
   uint8_t color_map_depth;
   int16_t x_origin;
   int16_t y_origin;
   int16_t width;
   int16_t height;
   uint8_t bits_per_pixel;
   uint8_t image_descriptor;
};

/**********
 * unpack *
 **********/

/* 
 * takes in a varying representation of color in 'data'
 * and converts to the standard color struct
 */
static uint32_t
unpack(uint8_t* bytes, int n_bytes)
{
   uint8_t a = 0;
   uint8_t r = 0; 
   uint8_t b = 0; 
   uint8_t g = 0;

   switch (n_bytes) {

      case 2:    /* ARRRRRGG GGGBBBBB XXXXXXXX XXXXXXXX */

         /* grab 5-bit representation of each color */
         r = (bytes[1] >> 2) & 0x1F;
         g = ((bytes[1] << 3) & 0x18) | ((bytes[0] >> 5) & 0x07);
         b = (bytes[0]) & 0x1F;

         /* 
          * scale channels by 8.2258 by copying & appending 
          * top three bit to the bottom 
          */
         r = (r << 3) | (r >> 2);
         g = (g << 3) | (g >> 2);
         b = (b << 3) | (b >> 2);

         /* attribute channel */
         a = 255 * ((bytes[0] & 0x80) >> 7);
        break;

      case 3:    /* RRRRRRRR GGGGGGGG BBBBBBBB XXXXXXXX */
         a = 255;
         r = bytes[2];
         g = bytes[1];
         b = bytes[0];
         break;

       case 4:    /* AAAAAAAA RRRRRRRR GGGGGGGG BBBBBBBB */
         a = bytes[3];
         r = bytes[2];
         g = bytes[1];
         b = bytes[0];
         break;
   }

   return a << 24 | r << 16 | g << 8 | b;
}

/********
 * read *
 ********/

/* 
 * reads tga data into RAM for quick parsing,
 * allocates image and color buffers on the heap
 */
static void
read(struct tga_header* header, uint8_t** color_map_p, 
            uint8_t** image_data_p, FILE* fp)
{
   /* fill header */
   fread(&header->id_len, 1, 1, fp);
   fread(&header->color_map_type, 1, 1, fp);
   fread(&header->data_type_code, 1, 1, fp);
   fread(&header->color_map_origin, 1, 2, fp);
   fread(&header->color_map_len, 1, 2, fp);
   fread(&header->color_map_depth, 1, 1, fp);
   fread(&header->x_origin, 1, 2, fp);
   fread(&header->y_origin, 1, 2, fp);
   fread(&header->width, 1, 2, fp);
   fread(&header->height, 1, 2, fp);
   fread(&header->bits_per_pixel, 1, 1, fp);
   fread(&header->image_descriptor, 1, 1, fp);

   int pixel_depth = header->bits_per_pixel / 8;
   int color_depth = header->color_map_depth / 8;
   
   uint8_t* color_map = calloc(header->color_map_len * color_depth, 1);
   uint8_t* image_data = calloc(header->width * 
                                header->height * 
                                pixel_depth, 1);

   fseek(fp, header->id_len, SEEK_CUR);
   fread(color_map, 1, header->color_map_len * color_depth, fp);
   fread(image_data, 1, header->width * header->height * pixel_depth, fp);

   *color_map_p = color_map;
   *image_data_p = image_data;
}

/***************
 * sr_load_tga *
 ***************/

/*
 * allocates a texture buffer, fills with color contents,
 * and sets it to the given pointer
 * 1 on success, 0 on failure
 */
extern int
sr_load_tga(uint32_t** texture_p, int* width_p,
            int* height_p, char* file)
{

   struct tga_header header;
   uint8_t* color_map;
   uint8_t* image_data;

   /* open file */
   FILE* fp = fopen(file, "rb");
   if (!fp)
      return 0;

   read(&header, &color_map, &image_data, fp);
   fclose(fp);

   int pixel_depth = header.bits_per_pixel / 8;
   int color_depth = header.color_map_depth / 8;
   uint8_t* color_tga;    /* raw color bytes read from tga */
   uint8_t* packet;

   uint32_t* texture = calloc(header.width * header.height, 4);
   

   switch (header.data_type_code) {
      case 1:    /* uncompressed color mapped */

         for (int i = 0; i < header.width * header.height; i++) {
            color_tga = color_map + image_data[i] * color_depth;
            texture[i] = unpack(color_tga, color_depth);
         }
         break;

      case 2:    /* uncompressed RGB */

         color_tga = image_data;
         for (int i = 0; i < header.width * header.height; i++) {
            texture[i] = unpack(color_tga, pixel_depth);
            color_tga += pixel_depth;
         }
         break;

      case 9:    /* run length encoded color mapped */

         packet = image_data;
         
         for (int i = 0; i < header.width * header.height; i++) {

            int len = (*packet & 0x7F) + 1;

            if (*packet & 0x80) {     /* run length packet */

               uint32_t color = unpack(color_map + packet[1], color_depth);
               for (int j = 0; j < len; j++) {
                  texture[i + j] = color;
               }

               /* next packet */
               packet += pixel_depth + 1;  
            } else {                  /* raw packet */
               color_tga = color_map + packet[1];
               for (int j = 0; j < len; j++) {
                  texture[i + j] = unpack(color_tga, color_depth);
                  color_tga += pixel_depth;
               }

                /* next packet */
               packet += len * pixel_depth + 1;   
            }

            i += len - 1;
         }
         break;

      case 10:   /* run length encoded RGB */

         packet = image_data;

         for (int i = 0; i < header.width * header.height; i++) {

            int len = (*packet & 0x7F) + 1;

            if (*packet & 0x80) {    /* run length packet */

               uint32_t color = unpack(packet + 1, pixel_depth);
               for (int j = 0; j < len; j++) {
                  texture[i + j] = color;
               }

               /* next packet */
               packet += pixel_depth + 1;  
            } else {                /* raw packet */

               color_tga = packet + 1;
               for (int j = 0; j < len; j++) {
                  texture[i + j] = unpack(color_tga, pixel_depth);
                  color_tga += pixel_depth;
               }

               /* next packet */
               packet += len * pixel_depth + 1;  
            }

            i += len - 1;
         }
         break;

      default:
         printf("unsupported tga data type code");
         return 0;
   }

   *width_p = header.width;
   *height_p = header.height;
   *texture_p = texture;
   free(color_map);
   free(image_data);

   return 1;
}