#include <math.h>
#include <string.h>

#include "reader.h"
#include "parser.h"


void init_buttons(struct Button *buttons[]){
    char* names[13] = {"square", "cross", "circle", "triangle", "l1", "r1", "l2", "r2", "share", "opt", "l3", "r3", "psbtn"};

    for (int i=0; i<13; i++){
        buttons[i] = malloc(sizeof(struct Button));
        strcpy(buttons[i]->name, names[i]);
        if (i<4){
            buttons[i]->id = pow(2, (i+4));
        } else if (i>=4 && i<12){
            buttons[i]->id = pow(2, (i-4));
        } else {
            buttons[i]->id = 1;
        }

        buttons[i]->get_state = btn_is_pressed;
    }

}

void init_dpad(struct Button *dpad[]){
    char* names[4] = {"up", "right", "down", "left"};
    for (int i=0; i<4; i++){ 
        dpad[i] = malloc(sizeof(struct Button));
        strcpy(dpad[i]->name, names[i]);
         
        dpad[i]->id = i*2;
        dpad[i]->get_state = dpad_is_pressed;
    }

}

void init_analogs(struct Button *analogs[]){
    char* names[6] = {"l-h", "l-v", "r-h", "r-v", "l2", "r2"};
    uint8_t ids[6] = {1, 2, 3, 4, 8, 9};
    for (int i=0; i<6; i++){ 
        analogs[i] = malloc(sizeof(struct Button));
        strcpy(analogs[i]->name, names[i]);
         
        analogs[i]->id = ids[i];
        analogs[i]->get_state = get_trigger;
    }
}

void init_state(struct State *state){
    init_buttons(state->buttons);
    init_dpad(state->dpad);
    init_analogs(state->analogs);
    
}

int main(){
    libusb_context *context = NULL;
    libusb_device_handle *handle = NULL;
    struct libusb_endpoint_descriptor in_ep; 
    struct libusb_endpoint_descriptor out_ep;

    init_usb_reader(&context, &handle, &in_ep, &out_ep);

    struct State state;

    init_state(&state);

    int size = 0x40; 
    uint8_t *report_buffer = (uint8_t*) calloc(size, 1);
    
    while (1){
        read_hid(context, handle, in_ep, &report_buffer, &size);

        //display_buffer_hex(report_buffer, size);
        
        for (int i=0; i<size;i++){
            printf("%02x ", report_buffer[i]);
        }

        printf("\n");

        parse_state(report_buffer, &state);

    }
    
    printf("in endpoint address: %d\n", in_ep.bEndpointAddress);
    printf("out endpoint address: %d\n", out_ep.bEndpointAddress);
    libusb_close(handle);
    libusb_exit(context);

}