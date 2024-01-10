#include <stdio.h>
#include <libusb-1.0/libusb.h>

#define VENDORID  0x054c
#define PRODUCTID 0x09cc
#define INTRF_N 3 //DS4 has 3 interfaces

void connect_device(libusb_context *context, libusb_device_handle **handle){
    int init_err = libusb_init(&context);
    if (init_err < LIBUSB_SUCCESS) {
        fprintf(stderr, "Failed to initialize libusb\n");
    }

    *handle = libusb_open_device_with_vid_pid(context, VENDORID, PRODUCTID);
    if (*handle == NULL) {
        fprintf(stderr, "Failed to open USB device. Try running program as superuser.\n");
        libusb_exit(context);
    } 
}


void print_device_info(libusb_context *context, libusb_device_handle *handle){
    struct libusb_device_descriptor desc;
    int desc_err = libusb_get_device_descriptor(libusb_get_device(handle), &desc);
    if ( desc_err < LIBUSB_SUCCESS) {
        fprintf(stderr, "Failed to get device desc. error: %d\n", desc_err);
        libusb_close(handle);
        libusb_exit(context);
    }

    printf("Vendor ID: 0x%04x\n", desc.idVendor);
    printf("Product ID: 0x%04x\n", desc.idProduct);
    printf("Number of Products: %d\n", desc.bNumConfigurations);
}

void detach_device(libusb_context *context, libusb_device_handle *handle, int intrf_n){
    int detach_err;

    for (int i =0; i <= intrf_n; i++){
        if (libusb_kernel_driver_active(handle, i) == 1){
            detach_err = libusb_detach_kernel_driver(handle, i);
            if (detach_err < LIBUSB_SUCCESS){
                fprintf(stderr, "Failed to detach device from kernel driver. error: %d\n", detach_err);
                libusb_close(handle);
                libusb_exit(context);
            }
            printf("%d interf is detached\n", i);
        }
    }

}


int main() {
    libusb_context *context = NULL;
    libusb_device_handle *handle = NULL;

    connect_device(context, &handle);
    
    print_device_info(context, handle);

    detach_device(context, handle, INTRF_N);

    libusb_close(handle);
    libusb_exit(context);

    return 0;
}
