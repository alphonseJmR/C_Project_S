#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

const char *morse_letters[27] = {
    "._   ",
    "_... ",
    "_._. ",
    "_..  ",
    ".    ",

    ".._. ",
    "__.  ",
    ".... ",
    "..   ",
    ".___ ",

    "_._  ",
    "._.. ",
    "__   ",
    "_.   ",
    "___  ",

    ".__. ",
    "__._ ",
    "._.  ",
    "...  ",
    "_    ",

    ".._  ",
    "..._ ",
    ".__  ",
    "_.._ ",
    "_.__ ",

    "__.. ",
    " "
};

typedef struct message_s {
    char sentence[100];
    char encoded[400];
    size_t s_len;
}message;


typedef message ((* morse_encoder)(char *string_input, uint8_t len));
typedef message ((* morse_decoder)(char *morse_input, uint8_t len));


typedef struct morse_translator {
    morse_encoder encode;
    morse_decoder decode;
}m_translate;


void encode_letter(message *mess, uint8_t letter){

    strcat(mess->encoded, morse_letters[letter]);
    printf("Encoded as: %s.\n", morse_letters[letter]);
    
}


message encode_to_morse(char *string_input, uint8_t len){

    message buffer;

    for(uint8_t a = 0; a < len; a++){

        switch(string_input[a]){
            case 0x61:
                encode_letter(&buffer, 0);
            break;

            case 0x62:
                encode_letter(&buffer, 1);
            break;

            case 0x63:
                encode_letter(&buffer, 2);
            break;

            case 0x64:
                encode_letter(&buffer, 3);
            break;

            case 0x65:
                encode_letter(&buffer, 4);
            break;

            case 0x66:
                encode_letter(&buffer, 5);
            break;

            case 0x67:
                encode_letter(&buffer, 6);
            break;

            case 0x68:
                encode_letter(&buffer, 7);
            break;

            case 0x69:
                encode_letter(&buffer, 8);
            break;

            case 0x6A:
                encode_letter(&buffer, 9);
            break;

            case 0x6B:
                encode_letter(&buffer, 10);
            break;

            case 0x6C:
                encode_letter(&buffer, 11);
            break;

            case 0x6D:
                encode_letter(&buffer, 12);
            break;

            case 0x6E:
                encode_letter(&buffer, 13);
            break;

            case 0x6F:
                encode_letter(&buffer, 14);
            break;

            case 0x70:
                encode_letter(&buffer, 15);
            break;

            case 0x71:
                encode_letter(&buffer, 16);
            break;

            case 0x72:
                encode_letter(&buffer, 17);
            break;

            case 0x73:
                encode_letter(&buffer, 18);
            break;

            case 0x74:
                encode_letter(&buffer, 19);
            break;

            case 0x75:
                encode_letter(&buffer, 20);
            break;

            case 0x76:
                encode_letter(&buffer, 21);
            break;

            case 0x77:
                encode_letter(&buffer, 22);
            break;

            case 0x78:
                encode_letter(&buffer, 23);
            break;

            case 0x79:
                encode_letter(&buffer, 24);
            break;

            case 0x7A:
                encode_letter(&buffer, 25);
            break;

            case 0x20:
                encode_letter(&buffer, 26);
            break;

            default:
                printf("Invalid character entered.\n");
            break;

        }

    }

    return buffer;

}


message decode_from_morse(char *morse_in, uint8_t len){
    message buffer;

    sprintf(buffer.sentence, "Testing");
    return buffer;
}

size_t get_len(char *input){
    return (sizeof(input) / sizeof(char));
}


message get_string(void){
    message buffer;
    
        fgets(buffer.sentence, sizeof(buffer.sentence), stdin);
        buffer.s_len = strlen(buffer.sentence);
        printf("Inputted string length: %i.\n", buffer.s_len);

    return buffer;
}



int main(){

    FILE *f_ptr;
    f_ptr = fopen("morse_output.txt", "a");

m_translate morse = {
    .encode = encode_to_morse,
    .decode = decode_from_morse
};

message my_message;


my_message = get_string();
    printf("Length Of Sentence: %i.\n", my_message.s_len);

my_message = morse.encode(my_message.sentence, my_message.s_len);


    printf("%s", my_message.encoded);
fprintf(f_ptr, "%s", my_message.encoded);

fclose(f_ptr);

}
