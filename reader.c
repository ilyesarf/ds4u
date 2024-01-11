#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define VENDORID  0x054c
#define PRODUCTID 0x09cc
#define HID_INTRF 3
#define HID_GET_REPORT 0x01
#define HID_REPORT_TYPE_INPUT 0x01

static void display_buffer_hex(unsigned char *buffer, unsigned size)
{
	unsigned i, j, k;

	for (i=0; i<size; i+=16) {
		printf("\n  %08x  ", i);
		for(j=0,k=0; k<16; j++,k++) {
			if (i+j < size) {
				printf("%02x", buffer[i+j]);
			} else {
				printf("  ");
			}
			printf(" ");
		}
		printf(" ");
		for(j=0,k=0; k<16; j++,k++) {
			if (i+j < size) {
				if ((buffer[i+j] < 32) || (buffer[i+j] > 126)) {
					printf(".");
				} else {
					printf("%c", buffer[i+j]);
				}
			}
		}
	}
	printf("\n" );
}

void check_err(libusb_context *context, libusb_device_handle *handle, int err, char* err_msg){
    if (err != LIBUSB_SUCCESS) {
        fprintf(stderr, "\n%s error: %s\n", err_msg, libusb_strerror((enum libusb_error)err));   
        libusb_close(handle);
        libusb_exit(context);
        exit(1);
    }
}


void connect_device(libusb_context *context, libusb_device_handle **handle){
    int init_err = libusb_init(&context);
    check_err(context, *handle, init_err, "Failed to initiate libusb.");

    *handle = libusb_open_device_with_vid_pid(context, VENDORID, PRODUCTID);
    if (*handle == NULL) {
        fprintf(stderr, "Failed to open USB device. Try running program as superuser.\n");
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

void read_hid(libusb_context *context, libusb_device_handle *handle, struct libusb_endpoint_descriptor in_ep){
    int size = 0x40;
    uint8_t *report_buffer = (uint8_t*) calloc(size, 1);
    printf("\nTesting interrupt read using endpoint %02X...\n", in_ep.bEndpointAddress);
	int read_err = libusb_interrupt_transfer(handle, in_ep.bEndpointAddress, report_buffer, size, &size, 5000);
    check_err(context, handle, read_err, "cant read");

	display_buffer_hex(report_buffer, size);
}

int main() {
    libusb_context *context = NULL;
    libusb_device_handle *handle = NULL;

    connect_device(context, &handle);
    printf("device conntected successfully\n");
    
    print_device_info(context, handle);

    struct libusb_config_descriptor *conf_desc;

    get_config_info(context, handle, &conf_desc);
    int intrf_n = conf_desc->bNumInterfaces;
    printf("device has %d interfaces\n", intrf_n);

    detach_device(context, handle, intrf_n);
    printf("all interfaces are detached from kernel drivers\n");

    claim_intrf(context, handle, HID_INTRF);
    printf("interface number %d is claimed\n", intrf_n);

    struct libusb_endpoint_descriptor in_ep = conf_desc->interface[HID_INTRF].altsetting->endpoint[0];
    struct libusb_endpoint_descriptor out_ep = conf_desc->interface[HID_INTRF].altsetting->endpoint[1];

    printf("in endpoint address: %d\n", in_ep.bEndpointAddress);
    printf("out endpoint address: %d\n", out_ep.bEndpointAddress);

    while (1){
        read_hid(context, handle, in_ep);
    }


    libusb_close(handle);
    libusb_exit(context);

    return 0;
}
