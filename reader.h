#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define VENDORID  0x054c
#define PRODUCTID 0x09cc
#define HID_INTRF 3

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

