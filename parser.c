#include "parser.h"


void parse_buf(uint8_t *report_buffer, struct Button **state/*, int size*/){
    int n = 0;
    for (int i=0; i<16; ++i){
        if (i<4){
            n = 5;
        } else if (i>=4 && i<12){
            n = 6;

        } else if (i>=12 && i<16){
            n = 5;
        }
        
        uint8_t data = report_buffer[n];

        state[i]->is_pressed(state[i], data);
        printf("Button `%s` with id `%02x` %s\n", state[i]->name, state[i]->id, (state[i]->state) ? "is pressed" : "is not pressed");
    }
}