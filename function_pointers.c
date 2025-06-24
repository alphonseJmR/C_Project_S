#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

typedef enum {
    setting_one = 0,
    setting_two,
    setting_three,
    setting_four,
    setting_five,
    setting_six,
    setting_seven
}flags;

typedef struct config_uration_s {
    bool readable;
    uint8_t config;
}config;


typedef bool ((* check_avil)(config *setting));
typedef flags ((* set_flag)(uint8_t input));


typedef struct level_two_s {
    uint16_t value;
    check_avil addable;
    set_flag get_flag;
}l_2;


bool program_can_add(config *set){
    if(set->readable){
        return true;
    }else{
        return false;
    }
}

flags find_flag_value(uint8_t input){
    switch(input){
        case 0:
        return setting_one;

        case 1:
        return setting_two;

        case 2:
        return setting_three;

        case 3:
        return setting_four;

        case 4:
        return setting_five;

        case 5:
        return setting_six;

        case 6:
        return setting_seven;
        
        default:
            printf("Error In Input.\n");
        return 0;
    }
}


int main(){

    uint8_t r_val = 0;

config testi = {

    .readable = true,
    .config = 0x03
};


l_2 test = {

    .value = 0,
    .addable = program_can_add,
    .get_flag = find_flag_value

};

printf("Test Function Pointer: %i\n", test.addable(&testi));

    for(uint8_t a = 0; a < 7; a++){
        r_val = test.get_flag(a);
        printf("Returning Enum: %i\n", r_val);
            if(r_val == 3){
                testi.config &= ~0x03;
            }
    }

    printf("Is Config Set: %i\n", testi.config);

}