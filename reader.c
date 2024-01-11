#include <stdio.h>
#include <libusb-1.0/libusb.h>

#define VENDORID  0x054c
#define PRODUCTID 0x09cc
#define INTRF_N 3 //DS4 has 4 interfaces (0-2 audio, 3 HID)

void check_err(libusb_context *context, libusb_device_handle *handle, int err, char* err_msg){
    if (err != LIBUSB_SUCCESS) {
        fprintf(stderr, "%s error: %d\n", err_msg, err);
        libusb_close(handle);
        libusb_exit(context);
        return 1;
    }
}
void connect_device(libusb_context *context, libusb_device_handle **handle){
    int init_err = libusb_init(&context);
    check_err(context, handle, init_err, "Failed to initiate libusb.");

    *handle = libusb_open_device_with_vid_pid(context, VENDORID, PRODUCTID);
    if (*handle == NULL) {
        fprintf(stderr, "Failed to open USB device. Try running program as superuser.\n");
        libusb_exit(context);
    } 
}


void print_device_info(libusb_context *context, libusb_device_handle *handle){
    struct libusb_device_descriptor device_desc;
    int device_desc_err = libusb_get_device_descriptor(libusb_get_device(handle), &device_desc);

    check_err(context, handle, device_desc_err, "Failed to get device device_desc.");

    printf("Vendor ID: 0x%04x\n", device_desc.idVendor);
    printf("Product ID: 0x%04x\n", device_desc.idProduct);
    printf("Number of Configurations: %d\n", device_desc.bNumConfigurations);
}

void detach_device(libusb_context *context, libusb_device_handle *handle, int intrf_n){
    int detach_err;

    for (int i =0; i <= intrf_n; i++){
        if (libusb_kernel_driver_active(handle, i) == 1){
            detach_err = libusb_detach_kernel_driver(handle, i);
            
            check_err(context, handle, detach_err, "Failed to detach device from kernel driver.");
            printf("%d interf is detached\n", i);
        }
    }

}

void connect_intrf(libusb_context *context, libusb_device_handle *handle, struct libusb_config_descriptor *conf_desc, int intrf){
    int claim_err = libusb_claim_interface(handle, intrf); 

    check_err(context, handle, claim_err, "Failed to claim interface");

    printf("interface number %d claimed\n", intrf);

    struct libusb_config_descriptor *conf_desc;
    //struct libusb_interface_descriptor intrf_desc;
    int conf_err = libusb_get_config_descriptor(libusb_get_device(handle), 0, &conf_desc);
    
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
