#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "inclds/parser_in.h"

char read_input[384000];
char read_nul_spaces[312000];
ili_pix_pos position_arr[8252];
// file mediy;
file logo_D;
file logo_out;

//  Deconstruct an agreeed upon language style font into a language 

int main(void){
bool run = true;

while(1){

    if(run){
        printf("Opening First File\n");
            open_file(&logo_D, &logo_out, true, position_arr, read_input, read_nul_spaces);
            close_file(&logo_D, true);
        printf("Closed First File\n");

        printf("Opening Second File\n");
            open_file(&logo_out, &logo_out, false, position_arr, read_input, read_nul_spaces);
            close_file(&logo_out, false);
        printf("Closed Second File\n");

        run = run_again();
        if(run){
            reset_variables(read_input, read_nul_spaces);
        }
    }else{
        return 0;
    }

    
}

}