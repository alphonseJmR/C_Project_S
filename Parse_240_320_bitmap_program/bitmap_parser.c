#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define upper(a) (uint8_t)( (a & 0xFF00) >> 8)
#define lower(a) (uint8_t)( (a & 0x00FF))


char read_input[384000];
char read_nul_spaces[312000];


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

}file_l;


typedef struct file_data_s {

    uint8_t input_bitmap[76800];
    uint8_t bitmap_buffer[76800];
    ili_pix_pos inital[76800];

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

file mediy;

file logo_D;
file logo_out;
ili_pix_pos position_arr[8252];


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


bool parse_partial(file *input, uint16_t parse_size){
        printf("\nParsing Now.\n");
    uint32_t total = 0;
    uint32_t cur_pos = 0;
        printf("Parse Size: %i.\n", parse_size);
    
    for(uint16_t a = 0; a < 320; a++){
        for(uint16_t b = 0; b < 240; b++){

            if(input->Data.input_bitmap[total] == '1'){
                printf("At X: %i Y: %i, found a 1, out of %i total\n", a, b, total);
                position_arr[cur_pos] = set_pixel_position(a, b);
                cur_pos++;
            }else{
                //   printf("Bitmap: %i\n", input->Data.input_bitmap[cur_pos]);
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


char get_file_configuration(file *input){
    char get;
    printf("How are you writing to the file?  \n");
    scanf(" %c", &get);
    printf("\n%c\n", get);
    return get;

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


void read_file(file *input){
        printf("Reading From File:\n");
    char ch;
    int pos = 0;
    int nl_s = 0;
    while( (ch = fgetc(input->Setting.file_ptr)) != EOF ){
        if(ch == '\n'){
            nl_s++;
        }
        read_input[pos] = (uint8_t)(ch);
        pos++;
    } 
    input->File_Config.file_size = ((pos - (16 * nl_s)) / 4);
    printf("Total New Lines In Read File: %i.\n", nl_s);
}


void remove_input_spaces(uint8_t *file_input){
        printf("Removing Spaces From Input Text\n");
    int pos = 0;
    int aa = 0;
    for(int a = 0; a < 384000; a++){

        if( (file_input[a]) != 0x20){
           read_nul_spaces[pos] = (char)file_input[a];
            pos++;
        }else{
        }
    }
        printf("Removed Spaces.  Positions left: %i\n", pos);
}


//  sr_ia = Spaces Removed Input Array
void parse_read_input(file *input, uint8_t *sr_ia){
        printf("Parsing Into A Structured Array.\n");
    uint8_t bb = 0; 
    int bmp_pos = 0;
    int new_size = input->File_Config.file_size;
    remove_input_spaces(read_input);
        printf("Parsing New Size: %i.\n", new_size);
        
    for(int a = 0; a < (384000 - new_size); a++){

        if(bb == 3){
            input->Data.input_bitmap[bmp_pos] = (char)sr_ia[a];
            bb = 0;
            bmp_pos++;
            
        }else{
          bb++; 
        }
    }

    printf("\nBMP Positional Size: %i.\n", bmp_pos);
}


int get_size_of_positions(file *input){
    int pos_hits = 0;
//  previous until 78000
    for(int a = 0; a < 384000; a++){
        // input->Data.input_bitmap[a]
        if(read_input[a] == '1'){
            pos_hits++;
        }
    }
    logo_out.File_Config.mem_posi = pos_hits;
    return pos_hits;
}


void create_new_line(file *input){
    fwrite("\n", 1, 1, input->Setting.file_ptr);
}


void create_write_array(file *input, char set_string[80], uint16_t set){

    uint8_t a, b, c, d, e, f, g, h, i, j = 0;




   // sprintf(set_string );

    
    sprintf(set_string, "0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i, 0x0%i,", a, b, c, d, e, f, g, h, i, j);
    //    printf("%s", set_string);
    create_new_line(input);

}


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
void open_file(file *input, bool rw){

    int size_x = 0;
    bool parsed = false;

    if(rw){
    get_file_location(input);
    if( set_file_ptr(input, true) ){
            printf("Input File Ptr Setup.\n\n");
        read_file(input);       // aka logo_D
        logo_out.File_Config.file_size = logo_D.File_Config.file_size;
        parse_read_input(&logo_out, read_nul_spaces);
        size_x = get_size_of_positions(&logo_out);
        parsed = parse_partial(&logo_out, size_x);

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
            write_full_file(input, position_arr);
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


//  Deconstruct an agreeed upon language style font into a language 

int main(void){
    FILE *tested;
    tested = fopen("tester.txt", "a");
    FILE *stest;
    stest = fopen("s_test.txt", "a");

while(1){
printf("Opening First File\n");
    open_file(&logo_D, true);
    close_file(&logo_D, true);
printf("Closed First File\n");

printf("Opening Second File\n");
    open_file(&logo_out, false);
    close_file(&logo_out, false);
printf("Closed Second File\n");


    return 0;
}

}