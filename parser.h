#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Button{
    char name[10];
    uint8_t id;
    void (*get_state)(struct Button*, uint8_t data);
    bool state;
};

struct Analog{
    char name[10];
    int trigger;
};

static void btn_is_pressed(struct Button* button, uint8_t data){
    button->state = ((data & button->id) != 0);
}

static void dpad_is_pressed(struct Button* button, uint8_t data){
    if ((data&0xF) == 0x07 && button->id == 0x00){
        button->state = true;
    } else{
        button->state = (((data&0xF) == button->id) || (((data&0xF)-1) == button->id) || (((data&0xF)+1) == button->id)); //&0xF because DPAD data is the last digit
    }
}


void parse_buf(uint8_t *report_buffer, struct Button **state/*, int size*/);