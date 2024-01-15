#include <libusb-1.0/libusb.h>
#include "reader.h"

int main(){
    libusb_context *context = NULL;
    libusb_device_handle *handle = NULL;
    struct libusb_endpoint_descriptor in_ep; 
    struct libusb_endpoint_descriptor out_ep;
    
    init_usb_reader(&context, &handle, &in_ep, &out_ep);
    while (1){
        read_hid(context, handle, in_ep);
    }
    
    printf("in endpoint address: %d\n", in_ep.bEndpointAddress);
    printf("out endpoint address: %d\n", out_ep.bEndpointAddress);
    libusb_close(handle);
    libusb_exit(context);

}