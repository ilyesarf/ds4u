#include "parser.h"

void parse_battery(uint8_t *report_buffer, int *battery){
    *battery = report_buffer[30]%16;
    printf("battery is at %d/15\n\n", *battery);
}

void parse_buttons(uint8_t *report_buffer, struct Button **buttons/*, int size*/){
    int n = 0;
    for (int i=0; i<14; ++i){
        if (i<4){
            n = 5;
        } else if (i>=4 && i<12){
            n = 6;
        }  else if (i==12 || i==13){
            n = 7;
        } 
        
        uint8_t data = report_buffer[n];
        buttons[i]->get_state(buttons[i], data);
        printf("Button `%s` with id `%02x` %s\n", buttons[i]->name, buttons[i]->id, (buttons[i]->state!=0) ? "is pressed" : "is not pressed");
    }
}

void parse_dpad(uint8_t* report_buffer, struct Button **dpad){
    int n = 5;
    for (int i=0; i<4; ++i){
        uint8_t data = report_buffer[n];

        dpad[i]->get_state(dpad[i], data);
        printf("Button `%s` with id `%02x` %s\n", dpad[i]->name, dpad[i]->id, (dpad[i]->state!=0) ? "is pressed" : "is not pressed");
    }
}

void parse_analogs(uint8_t* report_buffer, struct Button **analogs){
    for (int i=0; i<6;++i){
        uint8_t data = report_buffer[analogs[i]->id];

        analogs[i]->get_state(analogs[i], data);
        printf("Button `%s` with id `%02x` is at %d\n", analogs[i]->name, analogs[i]->id, analogs[i]->state);

    }
}

void parse_state(uint8_t* report_buffer, struct State* state){
    parse_battery(report_buffer, &(state->battery));
    parse_buttons(report_buffer, &(state->buttons));
    parse_dpad(report_buffer, &(state->dpad));
    parse_analogs(report_buffer, &(state->analogs));
}