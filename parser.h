#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Button{
    char name[7];
    uint8_t id;
    int is_pressed;
}; 

void parse_buttons(uint8_t data);

void parse_buf(uint8_t *report_buffer/*, int size*/);