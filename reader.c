#include "reader.h"

void connect_device(libusb_context *context, libusb_device_handle **handle){
    int init_err = libusb_init(&context);
    check_err(context, *handle, init_err, "Failed to initiate libusb.");

    *handle = libusb_open_device_with_vid_pid(context, VENDORID, PRODUCTID);
    if (*handle == NULL) {
        fprintf(stderr, "Failed to open USB device. Check if device is connected or Try running program as superuser.\n");
        libusb_exit(context);
        exit(1);
    } 
}


void print_device_info(libusb_context *context, libusb_device_handle *handle){
    struct libusb_device_descriptor device_desc;
    int device_desc_err = libusb_get_device_descriptor(libusb_get_device(handle), &device_desc);

    check_err(context, handle, device_desc_err, "Failed to get device descriptor.");

    printf("Vendor ID: 0x%04x\n", device_desc.idVendor);
    printf("Product ID: 0x%04x\n", device_desc.idProduct);
    printf("Number of Configurations: %d\n", device_desc.bNumConfigurations);
}

void detach_device(libusb_context *context, libusb_device_handle *handle, int intrf_n){
    int detach_err;

    for (int i =0; i < intrf_n; i++){
        if (libusb_kernel_driver_active(handle, i) == 1){
            detach_err = libusb_detach_kernel_driver(handle, i);
            
            check_err(context, handle, detach_err, "Failed to detach device from kernel driver.");
            printf("%d interf is detached\n", i);
        }
    }

}

void get_config_info(libusb_context *context, libusb_device_handle *handle, struct libusb_config_descriptor **conf_desc){
    int conf_err = libusb_get_config_descriptor(libusb_get_device(handle), 0, conf_desc);
    check_err(context, handle, conf_err, "Failted to get configuration descriptor.");
    
}

void claim_intrf(libusb_context *context, libusb_device_handle *handle, int intrf_n){
    int claim_err = libusb_claim_interface(handle, intrf_n); 

    check_err(context, handle, claim_err, "Failed to claim interface");
}


void init_usb_reader(libusb_context **context, libusb_device_handle **handle, struct libusb_endpoint_descriptor *in_ep, struct libusb_endpoint_descriptor *out_ep) {

    connect_device(*context, handle);
    printf("device conntected successfully\n");
    
    print_device_info(*context, *handle);

    struct libusb_config_descriptor *conf_desc;

    get_config_info(*context, *handle, &conf_desc);
    int intrf_n = conf_desc->bNumInterfaces;
    printf("device has %d interfaces\n", intrf_n);

    detach_device(*context, *handle, intrf_n);
    printf("all interfaces are detached from kernel drivers\n");

    claim_intrf(*context, *handle, HID_INTRF);
    printf("interface number %d is claimed\n", intrf_n);

    *in_ep = conf_desc->interface[HID_INTRF].altsetting->endpoint[0];
    *out_ep = conf_desc->interface[HID_INTRF].altsetting->endpoint[1];

}

void read_hid(libusb_context *context, libusb_device_handle *handle, struct libusb_endpoint_descriptor in_ep, uint8_t **report_buffer, int *size){
    printf("\nTesting interrupt read using endpoint %02X (data size %d)...\n", in_ep.bEndpointAddress, *size);
	int read_err = libusb_interrupt_transfer(handle, in_ep.bEndpointAddress, *report_buffer, *size, size, 5000);
    check_err(context, handle, read_err, "cant read");	

}
