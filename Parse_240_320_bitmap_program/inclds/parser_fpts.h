#ifndef BITMAP_PARSER_FUNCTION_PROTOTYPE_S_H
#define BITMAP_PARSER_FUNCTION_PROTOTYPE_S_H

#include <stdbool.h>
#include <stdint.h>
#include "parser_structs.h"

void reset_variables(char *ray1, char *ray2);
bool run_again(void);
void close_file(file *input, bool rw);
void open_file(file *input, file *output, bool rw, ili_pix_pos *arr, char *ria, char *rns);
void write_full_file(file *input, ili_pix_pos *inpi);
void write_line(file *input, ili_pix_pos *inpi, int cur_in);
// void create_write_array(file *input, char set_string[80], uint16_t set);
void create_new_line(file *input);
int get_size_of_positions(file *input, file *output, char *ray1);
void parse_read_input(file *input, char *ray1, char *ray2);
void remove_input_spaces(char *file_input, char *rns);
bool parse_partial(file *input, uint16_t parse_size, ili_pix_pos *arr);
ili_pix_pos set_pixel_position(uint16_t X, uint16_t Y);
void read_file(file *input, char *ria);
bool set_file_ptr(file *input, bool rw);
void set_file_location(file *input);
void get_file_location(file *input);
bool get_file_placement(file *input);
char get_file_configuration(file *input);


#endif