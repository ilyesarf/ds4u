#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Button{
    char name[10];
    uint8_t id;
    void (*is_pressed)(struct Button*, uint8_t data);
    bool state;
};

static void btn_is_pressed(struct Button* button, uint8_t data){
    button->state = ((data & button->id) != 0);
}

static void dpad_is_pressed(struct Button* button, uint8_t data){
    button->state = (data == 0);
}


void parse_buf(uint8_t *report_buffer, struct Button **state/*, int size*/);