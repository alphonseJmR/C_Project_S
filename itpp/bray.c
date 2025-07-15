#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "image.c"
#include "first_parse.c"
#include "second_parse.c"

#define value_to_R(a)  (uint32_t)((a & 0xFF) << 16)
#define value_to_G(a)  (uint32_t)((a & 0xFF) << 8)
#define value_to_B(a)  (uint32_t)((a & 0xFF))

typedef struct pixel_data_s {

  uint32_t pixel_color;
  uint16_t length;

}pixel;




void export_first_parse(uint32_t *import_1, uint32_t built_size){
    FILE *export_ptr;
    export_ptr = fopen("first_parse.txt", "w");

    fprintf(export_ptr, "uint32_t fp_image[%i] = {\n", built_size);

    for(uint32_t b = 0; b < built_size; b++){
  //      printf("Writing iteration %i to file...\n", b);
      if(b % 16 == 0){
        fprintf(export_ptr, "\n");
      }
        fprintf(export_ptr, "0x%06x, ", import_1[b]);
    }

    fprintf(export_ptr, "};");
    fclose(export_ptr);
}




void second_parse(pixel_cord *import_2, uint32_t px_cnt){

      FILE *export_ptr;
    export_ptr = fopen("second_parse.txt", "w");

    fprintf(export_ptr, "pixel_cord pixel_image[%i] = {\n", px_cnt);
    for(uint32_t b = 0; b < px_cnt; b++){
  //      printf("Writing iteration %i to file...\n", b);
          fprintf(export_ptr, "{");
          fprintf(export_ptr, "  .x_cord = 0x%04x, ", import_2[b].x_cord);
          fprintf(export_ptr, "  .y_cord = 0x%04x ", import_2[b].y_cord);
          fprintf(export_ptr, "}, \n");
    }

    fprintf(export_ptr, "};");
    fclose(export_ptr);
}


//  Image size correlates to width * height (or vice versa)
// our example width is 470 and height of 220
void image_compression(uint8_t *raw_array, uint32_t raw_size, uint16_t width, uint16_t height){

    uint32_t cor_size = (raw_size / 3);
    uint32_t crementer = 0x00000000;
    uint32_t pixel_buffer = 0x00000000;
    uint32_t pre_buff = 0x00000000;
    uint32_t full_pixel[ cor_size ];

    for(uint32_t a = 0; a < raw_size; a += 3){

      pixel_buffer = 0x00000000;
      pixel_buffer |= value_to_R(raw_array[a]);
      pixel_buffer |= value_to_G(raw_array[a+1]);
      pixel_buffer |= value_to_B(raw_array[a+2]);
        full_pixel[crementer] = pixel_buffer;
        crementer++;
        pre_buff = pixel_buffer;
    }

      export_first_parse(full_pixel, cor_size);
}


void pixel_finder(uint16_t height, uint16_t width, uint32_t *input){

  uint32_t max_size = (height * width);
  //  Create an array of the max size of the image.
  pixel_cord data[max_size];
  uint32_t incrementer = 0x0000;
  uint32_t pixel_count = 0x0000;
  FILE *export_ptr;
  export_ptr = fopen("all_coords.txt", "w");
  fprintf(export_ptr, "const pixel_cord coords = {\n");

  for(uint16_t a = 0; a < height; a++){
    for(uint16_t b = 0; b < width; b++){

        if(input[incrementer] <= 0x0f0f0f){
          data[incrementer].x_cord = b;
          data[incrementer].y_cord = a;
          fprintf(export_ptr, "{");
          fprintf(export_ptr, "  .x_cord = 0x%04x, ", b);
          fprintf(export_ptr, "  .y_cord = 0x%04x ", a);
          fprintf(export_ptr, "}, \n");
          data[incrementer].stored_data = true;
        }else{
          data[incrementer].stored_data = false;
        }
        incrementer++;
        
    }
  }
  printf("\nPixels Found.\n");

  for(uint32_t c = 0; c < max_size; c++){
    if(data[c].stored_data == true){
      pixel_count++;
    }
  }

  printf("New Pixel Count: %i.\n", pixel_count);

  uint32_t new_count = 0x0000;

  pixel_cord full[pixel_count];
  for(uint32_t d = 0; d < max_size; d++){
    if(data[d].stored_data == true){
      full[new_count].x_cord = data[d].x_cord;
      full[new_count].y_cord = data[d].y_cord;
      new_count++;
    }
  }

 second_parse(full, pixel_count);

fprintf(export_ptr, "coords [%i] = {\n", pixel_count);
  fprintf(export_ptr, "};");
    fclose(export_ptr);

}



// ysl = Y start location
uint16_t get_size_of_window_pane(const pixel_cord *input, uint32_t nmpx, uint16_t ysl, uint16_t height){

  printf("Getting window pane size now...\n");


  uint16_t buff[nmpx];
  uint32_t inc = 0x0000;

  for(uint32_t b = nmpx; b > 0; b--){
    buff[inc] = input[b].y_cord;
    inc++;
  }

  printf("\nInput Size: %i.\n", nmpx);
    int32_t buffer = 0x0000;
    for(uint32_t a = 0; a < nmpx; a++){

      if(buff[a] >= ysl && buff[a] <= ysl + height){
          buffer++;
      }else{
   //     printf("Current Y: %i @ pixel: %i.\n", input[a].y_cord, a);
      }

  }
  if(buffer > nmpx){
    printf("Error.  Buffer is %i greater than size of input. Return 0\n\n", (buffer - nmpx));
    return 0;
  }else{
    printf("Window Pane Size: %i.\n\n", buffer);
    return buffer;
  }
}


void parse_partial_window(const pixel_cord *image, uint32_t nmpx, uint16_t ys, uint16_t height){

  uint16_t wp_size = get_size_of_window_pane(image, nmpx, ys, height);
  printf("WP_SIZE: %i.\n", wp_size);
  
  FILE *export_ptr_1;
  export_ptr_1 = fopen("parse_three.txt", "w");
  fprintf(export_ptr_1, "const pixel_cord window[%i] = {\n", wp_size);
  pixel_cord export[wp_size];
  uint32_t inc = 0x0000;

  for(uint32_t a = 0; a < nmpx; a++){

    if( image[a].y_cord >= (ys - 50)){
      if( image[a].y_cord < ((ys + height) - 50 )){
        export[inc].x_cord = image[a].x_cord;
        export[inc].y_cord = image[a].y_cord;
        inc++;
      }
    }
  }
  printf("Incrementer: %i.\n", inc);

  for(uint16_t b = 0; b < inc; b++){
      fprintf(export_ptr_1, "{");
      fprintf(export_ptr_1, "  .x_cord = 0x%04x, ", export[b].x_cord);
      fprintf(export_ptr_1, "  .y_cord = 0x%04x ",  export[b].y_cord);
      fprintf(export_ptr_1, "}, \n");
  }

    fprintf(export_ptr_1, "};");
    fclose(export_ptr_1);

}

int main(){

    printf("Beginning Image to Pixel Array Now.\n\n");

    uint32_t arr_size = (sizeof(interior) / sizeof(interior[0]));

  size_t input_array = (sizeof(interior) / 3);
    image_compression(interior, arr_size, 470, 220);
  //  second_parse(fp_image, 94000);
  pixel_finder(200, 470, fp_image);

  printf("\nFull Image Size Array: %i.\n", arr_size);

  parse_partial_window(pixel_image, 32542, 150, 30);

    printf("Pixel Array Compress Complete.  Exit Program.\n\n");


}