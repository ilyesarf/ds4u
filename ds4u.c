#include <math.h>
#include <string.h>

#include "reader.h"
#include "parser.h"


void init_buttons(int* n, struct Button *state[]){
    char* names[13] = {"square", "cross", "circle", "triangle", "l1", "r1", "l2", "r2", "share", "opt", "l3", "r3", "psbtn"};

    for (int i=0; i<13; i++){
        strcpy(state[*n]->name, names[i]);
        if (i<4){
            state[*n]->id = pow(2, (i+4));
        } else if (i>=4 && i<12){
            state[*n]->id = pow(2, (i-4));
        } else {
            state[*n]->id = 1;
        }

        state[*n]->get_state = btn_is_pressed;
        (*n)++;
    }

}

void init_dpad(int *n, struct Button *state[]){
    char* names[4] = {"up", "right", "down", "left"};
    for (int i=0; i<4; i++){ 
        strcpy(state[*n]->name, names[i]);
         
        state[*n]->id = i*2;
        state[*n]->get_state = dpad_is_pressed;
        (*n)++;
    }

}


void init_state(struct Button *state[]){

    int n = 0;

    init_buttons(&n, state);
    init_dpad(&n, state);
    for (int i=0; i<n; i++){
        printf("Button `%s` with id `%02x` is added to state\n", state[i]->name, state[i]->id);
    }
    
}

int main(){
    libusb_context *context = NULL;
    libusb_device_handle *handle = NULL;
    struct libusb_endpoint_descriptor in_ep; 
    struct libusb_endpoint_descriptor out_ep;

    init_usb_reader(&context, &handle, &in_ep, &out_ep);

    struct Button *state[19];
    for (int i=0; i<19; i++){
        state[i] = malloc(sizeof(struct Button));
    }

    init_state(state);

    int size = 0x40; 
    uint8_t *report_buffer = (uint8_t*) calloc(size, 1);
    
    while (1){
        read_hid(context, handle, in_ep, &report_buffer, &size);

        //display_buffer_hex(report_buffer, size);
        
        for (int i=0; i<size;i++){
            printf("%02x ", report_buffer[i]);
        }

        printf("\n");

        parse_buf(report_buffer, state);

    }
    
    printf("in endpoint address: %d\n", in_ep.bEndpointAddress);
    printf("out endpoint address: %d\n", out_ep.bEndpointAddress);
    libusb_close(handle);
    libusb_exit(context);

}