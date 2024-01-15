#include <libusb-1.0/libusb.h>
#include "reader.h"

int main(){
    libusb_context *context = NULL;
    libusb_device_handle *handle = NULL;
    struct libusb_endpoint_descriptor in_ep; 
    struct libusb_endpoint_descriptor out_ep;

    init_usb_reader(&context, &handle, &in_ep, &out_ep);
    int size = 0x40; 
    uint8_t *report_buffer = (uint8_t*) calloc(size, 1);

    while (1){
        read_hid(context, handle, in_ep, &report_buffer, &size);
        
        //display_buffer_hex(report_buffer, size);
        for (int i=0; i<size;i++){
            printf("%02x ", report_buffer[i]);
        }

        printf("\n");
    }
    
    printf("in endpoint address: %d\n", in_ep.bEndpointAddress);
    printf("out endpoint address: %d\n", out_ep.bEndpointAddress);
    libusb_close(handle);
    libusb_exit(context);

}