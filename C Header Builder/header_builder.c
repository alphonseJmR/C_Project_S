#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

uint8_t std_read;


//  std_ctype is new to us, lets learn it!
enum incl_flags {

    no_flags =  0x00,
    std_int  =  0x01,
    std_lib  =  0x02,
    std_bool =  0x04,
    std_str  =  0x08,
    std_math =  0x10,
    std_time =  0x20,
    std_ctyp =  0x40,

};

//  Enum of ways to call a file.
enum file_open_type {

    write_only = 0,
    append_only = 1,
    write_binary = 2,
    append_binary = 3,

};


//  hfi = Header File Info
//  include_flags will be for standard includes.
typedef struct header_file_info_s {

    FILE *h_ptr;
    //  This is the name used by the ifndef directive.
    char header_name[50];
    char open_type[25];
    //  This is the name used to find the location to put the file.
    char file_location[100];
    uint8_t include_flags;
}hfi;



//  This is our first step when loading a file.  Not the first step of the program however!
void set_header_location(hfi *file){
    //  Still not sure if saving as .h works.
    char *file_ext = ".txt";
    printf("Enter New Header File Location and Name.\nFrom Current Directory.\n\tex- src/new_head\n");
    scanf("%s", file->file_location);
        if( (strlen(file->file_location) + strlen(file_ext)) < 96){
            strcat(file->file_location, file_ext);
            printf("Your Header Location: %s\n", file->file_location);
        }else{
            printf("Location to far away.  (Or long folder name, lame)\n");
        }
}


//  This is our second step when loading a file.  Only the two steps needed I think?
void set_file_open_type(hfi *file){
    printf("How to Write to file?\n\twrite_only\n\tappend_only\n\twrite_binary\n\tappend_write\n");
    scanf("%s", file->open_type);
}


//  This sets the name of the internal include file.
void set_file_name(hfi *file){
    char *per = "_S_H";
    printf("Enter your header file name.\n");
    printf("Max Name Size is 50.\n");
    char buffer[100];

    scanf("%s", file->header_name);
   
    printf("Inputted Header Name: %s\n", file->header_name);

        for(uint8_t a = 0; a < strlen(file->header_name); a++){
         //   printf("Charater: %c\n", file->header_name[a]);
            if( islower(file->header_name[a]) ){
                file->header_name[a] -= 32;
                printf("Charater: %c\n", file->header_name[a]);
            }
        }
        strcat(file->header_name, per);
    printf("Full Header Name: %s\n", file->header_name);
}



//  This is our next step
void set_incl_flags(hfi *file){
        std_read = 0;
        uint8_t num_reads = 0;
        uint8_t flag_rtr = 0;
    printf("Standard Includes:\n\tNo Flags = 0\n\tStdint = 1\n");
    printf("\tStdlib = 2\n\tStdbool = 3\n\tString = 4\n\tStdmath = 5\n");
    printf("\tStdtime = 6\n\tCtype =  7\n\n");

    printf("How many will you include?\n");
   scanf(" %hhu", &num_reads);

    if(num_reads != 0){
        std_read = 0;
        printf("We can setup %i includes!\n", num_reads);
        printf("\nEnter flag values associated with the include.\n\tex 0x02, 0x08, 0x20\n");

        for(uint8_t a = 0; a < num_reads; a++){

                scanf("%x", &std_read);
                    printf("Read %i produced: %i.\n", a, std_read);
                flag_rtr |= std_read;

        }
            printf("\n\tSetting Flag Values as: %i.\n", flag_rtr);
            file->include_flags = flag_rtr;

    }else{
        printf("No includes needed!\n");
    }
}


uint8_t open_type_translate(hfi *file){
        printf("Open Type: %s.\n", file->open_type);
    if(strcmp(file->open_type, "write_only") == 0){
        
        return 0;
    }
    if(strcmp(file->open_type, "append_only") == 0){
        return 1;
    }
    if(strcmp(file->open_type, "write_binary") == 0){
        return 2;
    }
    if(strcmp(file->open_type, "append_binary") == 0){
        return 3;
    }
}


void init_file(hfi *file){
        uint8_t ott = 0;

    set_header_location(file);
    set_file_open_type(file);

    ott = open_type_translate(file);

    switch(ott){
            case 0:
              file->h_ptr = fopen(file->file_location, "w");
            break;

            case 1:
                file->h_ptr = fopen(file->file_location, "a");
            break;

            case 2:
                file->h_ptr = fopen(file->file_location, "wb");
            break;

            case 3:
                file->h_ptr = fopen(file->file_location, "ab");
            break;

            default:
                printf("Open Type Error.  Setting default type (append).\n");
                file->h_ptr = fopen(file->file_location, "a");
        }
}


void init_basics(hfi *file){
    set_file_name(file);
    set_incl_flags(file);
}


void write_new_line(hfi *file){
    fwrite("\n", 1, 1, file->h_ptr);
}


void write_x_new_line(hfi *file, uint8_t num_line){
    for(uint8_t a = 0; a < num_line; a++){
        write_new_line(file);
    }
}



void write_basic_header(hfi *file){

    printf("\n\nNow Writing To File.\n\n");
    char dirr[100];
    char dirdif[100];
    dirr[0] = '#';
    dirr[1] = '\0';
    dirdif[0] = '#';
    dirdif[1] = '\0';

    char *directive_s = "ifndef ";
    char *dir_define_s = "define ";
    strcat(dirr, directive_s);
        printf("First strcat: %s\n", dirr);
    strcat(dirr, file->header_name);
    uint8_t ns_len = strlen(dirr);
        printf("Header IfNDef Length: %i\n%s\n", ns_len, dirr);

    strcat(dirdif, dir_define_s);
    strcat(dirdif, file->header_name);
    uint8_t ns_len1 =  strlen(dirdif);
        printf("Header Define Length: %i\n%s\n", ns_len1, dirdif);

    fwrite(dirr, 1, ns_len, file->h_ptr);
        write_new_line(file);
    fwrite(dirdif, 1, ns_len1, file->h_ptr);
        write_x_new_line(file, 3);

        // Stdio.h is a normal include in most of my files, so.. just add it.
    fwrite("#include <stdio.h>", 1, 19, file->h_ptr);
        write_new_line(file);

    printf("Flags: 0x%02x\n", file->include_flags);

    if(file->include_flags & 0x00){
        printf("No Includes To Write To File.\n");
    }
    if(file->include_flags & 0x01){
            printf("Including STDINT.H\n");
        fwrite("#include <stdint.h>", 1, 20, file->h_ptr);
            write_new_line(file);
    }
    if(file->include_flags & 0x02){
            printf("Including STDLIB.H\n");
        fwrite("#include <stdlib.h>", 1, 20, file->h_ptr);
            write_new_line(file);
    }
    if(file->include_flags & 0x04){
            printf("Including STDBOOL.H\n");
        fwrite("#include <stdbool.h>", 1, 21, file->h_ptr);
            write_new_line(file);
    }
    if(file->include_flags & 0x08){
            printf("Including STRING.H\n");
        fwrite("#include <string.h>", 1, 20, file->h_ptr);
            write_new_line(file);
    }
    if(file->include_flags & 0x10){
            printf("Including STDMATH.H\n");
        fwrite("#include <stdmath.h>", 1, 21, file->h_ptr);
            write_new_line(file);
    }
    if(file->include_flags & 0x20){
            printf("Including TIME.H\n");
        fwrite("#include <time.h>", 1, 18, file->h_ptr);
            write_new_line(file);
    }
    if(file->include_flags & 0x40){
            printf("Including CTYPE.H\n");
        fwrite("#include <ctype.h>", 1, 19, file->h_ptr);
            write_new_line(file);
    }

    write_x_new_line(file, 4);

    fwrite("#endif", 1, 7, file->h_ptr);

    printf("\n\nFinished Writing To File\n\n");
}


void close_file(hfi *file){
    printf("\nClosing File\n");
    fclose(file->h_ptr);
}


int main(void){
    hfi new_header;

    init_file(&new_header);
    init_basics(&new_header);

    write_basic_header(&new_header);

    close_file(&new_header);


}