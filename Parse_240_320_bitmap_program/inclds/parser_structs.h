#ifndef BITMAP_PARSER_STRUCT_S_H
#define BITMAP_PARSER_STRUCT_S_H

#include <stdint.h>
#include <stdbool.h>

#define upper(a) (uint8_t)( (a & 0xFF00) >> 8)
#define lower(a) (uint8_t)( (a & 0x00FF))
#define g_size(a) (size_t)( (sizeof(a) / sizeof(a[0])) )


typedef struct ili_pixel_mem_addr {
    uint8_t xssu;
    uint8_t xssl;
    uint8_t xesu;
    uint8_t xesl;

    uint8_t yssu;
    uint8_t yssl;
    uint8_t yesu;
    uint8_t yesl;
}ili_pix_pos;


typedef struct file_setting_s {

    FILE *file_ptr;
    // Get the location of the file
    char location[50];
    //  Get the name of the file.
    char file_name[50];
    //  Set string to location + file_name
    char file_x[100];
    uint8_t loc_size;
    //  Used to choose between Left or Right bmp.  both are 320x240.
    bool bmp_side;

}file_l;


typedef struct file_data_s {

    uint8_t input_bitmap[76800];
}file_d;


typedef struct file_config_s {
    //  For input size, or output size of bitmap
    size_t file_size;
    uint16_t mem_posi;
    char open_type;
}file_c;


typedef struct file_s {
    file_l Setting;
    file_d Data;
    file_c File_Config;
}file;


#endif