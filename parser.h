#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Button{
    char name[10];
    uint8_t id;
    void (*is_pressed)(struct Button*, uint8_t data);
    int state;
};

static void is_pressed(struct Button* button, uint8_t data){
    button->state = ((data & button->id) != 0)  ? 1 : 0;
}


void parse_buf(uint8_t *report_buffer, struct Button **state/*, int size*/);