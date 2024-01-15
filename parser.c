#include "parser.h"

void parse_buttons(uint8_t data){
    printf("data: %02x\n", data);
    int n = 4; //number of buttons
    struct Button buttons[n];
    char* names[7] = {"square", "cross", "circle", "triangle"};
    for (int i=0; i<n; i++){
        //buttons[i].name = names[i];
        strcpy(buttons[i].name, names[i]);
         
        buttons[i].id = pow(2, (i+4));

        buttons[i].is_pressed = ((data&buttons[i].id) != 0)  ? 1 : 0;
        printf("Button `%s` with id `%02x` %s\n", buttons[i].name, buttons[i].id, (buttons[i].is_pressed == 1) ? "is pressed" : "is not pressd");
    }

}

void parse_buf(uint8_t *report_buffer/*, int size*/){
    parse_buttons(report_buffer[5]);
}