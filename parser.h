#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Button{
    char name[10];
    uint8_t id;
    void (*get_state)(struct Button*, uint8_t data);
    int state;
};

struct State {
    struct Button buttons[13];
    struct Button dpad[4];
    struct Button analogs[2];
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

static void get_trigger(struct Button* analog, uint8_t data){
    analog->state = data;
}

void parse_buttons(uint8_t *report_buffer, struct Button **buttons/*, int size*/);

void parse_dpad(uint8_t* report_buffer, struct Button **dpad);

void parse_state(uint8_t *report_buffer, struct State *state/*, int size*/);