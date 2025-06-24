#ifndef BITMAP_PARSER_FUNCTION_S_H
#define BITMAP_PARSER_FUNCTION_S_H

#include <stdio.h>

#include "parser_fpts.h"

//  File Functions
char get_file_configuration(file *input){
    char get;
    printf("How are you writing to the file?  \n");
    scanf(" %c", &get);
    printf("\n%c\n", get);
    return get;

}

//  Return 1 for left, 2 for right, 0 for input error.
bool get_file_placement(file *input){

    char in;
    printf("Left or Right BMP?\n\tEnter l or r.\n");
    scanf(" %c", &in);

  if( (in == 'l') || (in == 'r')){
    if(in == 'l'){
        printf("Left BMP Selected.\n");
        return 1;
    }else{
        printf("Right BMP Selected.\n");
        return 2;
    }
  }else{
    printf("Not a chosen letter.");
    return 0;
  }

}

//  User Input of file location.
void get_file_location(file *input){

    printf("Enter Bitmap Location: .../\n");
    scanf("%s", input->Setting.location);

    printf("Enter Bitmap File Name: .txt\n");
    scanf("%s", input->Setting.file_name);

    strcat(input->Setting.file_x, input->Setting.location);
    strcat(input->Setting.file_x, input->Setting.file_name);

}


void set_file_location(file *input){

    printf("Enter New File Location: .../\n");
    scanf("%s", input->Setting.location);

    printf("Enter New File Name: /...txt\n");
    scanf("%s", input->Setting.file_name);

    strcat(input->Setting.file_x, input->Setting.location);
    strcat(input->Setting.file_x, input->Setting.file_name);

}


//  Helper Function to open_file()
bool set_file_ptr(file *input, bool rw){

    if(rw){
        input->Setting.file_ptr = fopen(input->Setting.file_x, "r");
        if(input->Setting.file_ptr == NULL){
            return false;
        }else {
            return true;
        }
    }else{
        if(input->File_Config.open_type == 'w'){
                printf("Writing to new file\n");
            input->Setting.file_ptr = fopen(input->Setting.file_x, "w");
            if(input->Setting.file_ptr == NULL){
                return false;
            }else {
                return true;
            }
        }else if(input->File_Config.open_type == 'a'){
                printf("Appending to file\n");
            input->Setting.file_ptr = fopen(input->Setting.file_x, "a");
            if(input->Setting.file_ptr == NULL){
                return false;
            }else {
                return true;
            }
        }else{
            printf("Error.  The File Pointer was set incorrectly\n");
        }
    }
}


void read_file(file *input, char *ria){
        printf("Reading From File:\n");
    char ch;
    int pos = 0;
    int nl_s = 0;
    while( (ch = fgetc(input->Setting.file_ptr)) != EOF ){
        if(ch == '\n'){
            nl_s++;
        }
        ria[pos] = (uint8_t)(ch);
        pos++;
    } 
    printf("Total Size of Input file read: %i.\n", pos);
    input->File_Config.file_size = ((pos - (16 * nl_s)) / 4);
    printf("Total New Lines In Read File: %i.\n", nl_s);
}



ili_pix_pos set_pixel_position(uint16_t X, uint16_t Y){
    ili_pix_pos buffer;

    uint8_t a, b;
    a = upper(X);
    b = lower(X);
    buffer.xssu = a;
    buffer.xssl = b;
    buffer.xesu = a;
    buffer.xesl = b;
    a = upper(Y);
    b = lower(Y);
    buffer.yssu = a;
    buffer.yssl = b;
    buffer.yesu = a;
    buffer.yesl = b;

    return buffer;
}


bool parse_partial(file *input, uint16_t parse_size, ili_pix_pos *arr){
        printf("\nParsing Now.\n");
    uint32_t total = 0;
    uint32_t cur_pos = 0;
    uint8_t addr = 0;
        printf("Parse Size: %i.\n", parse_size);
    addr = (input->Setting.bmp_side) ? 0 : 240;

    for(uint16_t a = 0; a < 320; a++){
        for(uint16_t b = (0 + addr); b < (240 + addr); b++){

            if(input->Data.input_bitmap[total] == '1'){
                arr[cur_pos] = set_pixel_position(a, b);
                cur_pos++;
            }else{

            }
            total++;
            
        }
    }
 
    if(cur_pos == parse_size){
         return true;
    }else{
        return false;
    }
}


void remove_input_spaces(char *file_input, char *rns){
        printf("Removing Spaces From Input Text\n");
    int pos = 0;
    int aa = 0;
    for(int a = 0; a < 384000; a++){

        if( (file_input[a]) != 0x20){
           rns[pos] = file_input[a];
            pos++;
        }else{
        }
    }
        printf("Removed Spaces.  Positions left: %i\n", pos);
}


//  ray1 = Read Input, ray2 = spaces removed array
void parse_read_input(file *input, char *ray1, char *ray2){
        printf("Parsing Into A Structured Array.\n");
    uint8_t bb = 0; 
    int bmp_pos = 0;
    int new_size = input->File_Config.file_size;
    remove_input_spaces(ray1, ray2);
        printf("Parsing New Size: %i.\n", new_size);
        
    for(int a = 0; a < (384000 - new_size); a++){

        if(bb == 3){
            input->Data.input_bitmap[bmp_pos] = (char)ray2[a];
            bb = 0;
            bmp_pos++;
            
        }else{
          bb++; 
        }
    }

    printf("\nBMP Positional Size: %i.\n", bmp_pos);
}


int get_size_of_positions(file *input, file *output, char *ray1){
    int pos_hits = 0;
//  previous until 78000
    size_t r1 = g_size(ray1);
    printf("\tSize of position check array: %i.\n", r1);

    for(int a = 0; a < r1; a++){
        if(ray1[a] == '1'){
            pos_hits++;
        }
    }
    output->File_Config.mem_posi = pos_hits;
    printf("Number of Hits for Input File: %i.\n", pos_hits);
    return pos_hits;
}


void create_new_line(file *input){
    fwrite("\n", 1, 1, input->Setting.file_ptr);
}


/*
void create_write_array(file *input, char set_string[80], uint16_t set){

    uint8_t a, b, c, d, e, f, g, h, i, j = 0;

    sprintf(set_string, "0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i,", a, b, c, d, e, f, g, h, i, j);
    //    printf("%s", set_string);
    create_new_line(input);

}
*/


void write_line(file *input, ili_pix_pos *inpi, int cur_in){
  if(cur_in < (input->File_Config.mem_posi - 1)){
    fprintf(input->Setting.file_ptr, "strucx%i = {\n\t.xssu = %i,\n\t.xssl = %i,\n\t.xseu = %i,\n\t.xsel = %i,\n\t.yssu = %i,\n\t.yssl = %i,\n\t.yseu = %i,\n\t.ysel = %i\n},\n ", cur_in, inpi[cur_in].xssu, inpi[cur_in].xssl, inpi[cur_in].xesu, inpi[cur_in].xesl, inpi[cur_in].yssu, inpi[cur_in].yssl, inpi[cur_in].yesu, inpi[cur_in].yesl);
  }else{
    fprintf(input->Setting.file_ptr, "strucx%i = {\n\t.xssu = %i,\n\t.xssl = %i,\n\t.xseu = %i,\n\t.xsel = %i,\n\t.yssu = %i,\n\t.yssl = %i,\n\t.yseu = %i,\n\t.ysel = %i\n}\n ", cur_in, inpi[cur_in].xssu, inpi[cur_in].xssl, inpi[cur_in].xesu, inpi[cur_in].xesl, inpi[cur_in].yssu, inpi[cur_in].yssl, inpi[cur_in].yesu, inpi[cur_in].yesl);
  }
}


void write_full_file(file *input, ili_pix_pos *inpi){
   fwrite("ili_pix_pos parsed[8252] = {\n", 1, 30, input->Setting.file_ptr);
        for(uint16_t a = 0; a < 8252; a++){
            write_line(input, inpi, a);
        }
    fwrite("\n};", 1, 3, input->Setting.file_ptr);
}


//  Open a file.  Read data or write data.
void open_file(file *input, file *output, bool rw, ili_pix_pos *arr, char *ria, char *rns){

    int size_x = 0;
    bool parsed = false;

    if(rw){
    get_file_location(input);
    if( set_file_ptr(input, true) ){
            printf("Input File Ptr Setup.\n\n");
        read_file(input, ria);       // aka logo_D
        input->Setting.bmp_side = get_file_placement(input);
        output->File_Config.file_size = input->File_Config.file_size;
        parse_read_input(output, ria, rns);
        size_x = get_size_of_positions(input, output, ria);
        parsed = parse_partial(output, size_x, arr);

        if(parsed){
            printf("\nThere are %i memory locations.\n", size_x);
                if(!input->File_Config.file_size){
                    printf("\nInput File Error.\n File Has No Size\n");
                }else{
                    printf("Size of input file: %lli.\n", input->File_Config.file_size);
                }
                printf("Displaying New-ly read file.\n");
        }else{
            printf("Error in parsing.\n");
        }

    }else{
        printf("Input File Not Found.\n");
    }

    }else
    {
      set_file_location(input);
      input->File_Config.open_type = get_file_configuration(input);
        if( set_file_ptr(input, false) ){
            
            printf("Output File Ptr Setup.\n\n");
            //write_to_file(input);
            write_full_file(input, arr);
        }else{
            printf("Output File Not Found.\n\n");
        }
    } 
}


void close_file(file *input, bool rw){
    if(rw){
        printf("\n\nClosing Read File Now.\n");
    }else{
       printf("\n\nClosing Write File Now.\n"); 
    }
    fclose(input->Setting.file_ptr);
}


//  Return 1 for yes, 0 for no or incorrect.
bool run_again(void){
    char q;
    printf("Run Program Again?\n\tEnter y/n.\n");
    scanf(" %c", &q);

    if( (q != 'y') && (q != 'n')){
        printf("No Correct Input.\n\tDefault Setting Applied ->no<-\n");
        return 0;
    }

    if(q == 'y'){
        return 1;
    }

    if(q == 'n'){
        return 0;
    }
}


void reset_variables(char *ray1, char *ray2){

    size_t r1, r2 = 0;
    r1 = g_size(ray1);
        printf("Size of Array 1: %i.\n", r1);
    r2 = g_size(ray2);
        printf("Size of Array 2: %i.\n", r2);

    printf("\nResetting Variable Arrays Now.\n");

    for(int a = 0; a < r1; a++){
        ray1[a] = ' ';
    }

    for(int b = 0; b < r2; b++){
        ray2[b] = ' ';
    }

    printf("Reset Finished.\n\n");

}



#endif