#include "parser.h"


void parse_buf(uint8_t *report_buffer, struct Button **state/*, int size*/){
    uint8_t data = report_buffer[5];
    for (int i=0; i<8; ++i){
        state[i]->is_pressed(state[i], data);
        printf("Button `%s` with id `%02x` %s\n", state[i]->name, state[i]->id, (state[i]->state) ? "is pressed" : "is not pressed");
    }
}