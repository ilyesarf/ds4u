#include "parser.h"


void parse_buf(uint8_t *report_buffer, struct Button **state/*, int size*/){
    int n = 0;
    for (int i=0; i<17; ++i){
        if (i<4){
            n = 5;
        } else if (i>=4 && i<12){
            n = 6;
        }  else if (i==12){
            n = 7;
        } else if (i>=13 && i<16){
            n = 5;
        } else if (i==17){
            n = 8;
        } else if (i==18){
            n = 9;
        }
        
        uint8_t data = report_buffer[n];

        state[i]->get_state(state[i], data);
        printf("Button `%s` with id `%02x` %s\n", state[i]->name, state[i]->id, (state[i]->state) ? "is pressed" : "is not pressed");
    }
}