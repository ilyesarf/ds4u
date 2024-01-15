#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdlib.h>
#include <libusb-1.0/libusb.h>

#define VENDORID  0x054c
#define PRODUCTID 0x09cc
#define HID_INTRF 3

static void display_buffer_hex(unsigned char *buffer, unsigned size) {
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

static void check_err(libusb_context *context, libusb_device_handle *handle, int err, char* err_msg){
    if (err != LIBUSB_SUCCESS) {
        fprintf(stderr, "\n%s error: %s\n", err_msg, libusb_strerror((enum libusb_error)err));   
        libusb_close(handle);
        libusb_exit(context);
        exit(1);
    }
}


void connect_device(libusb_context *context, libusb_device_handle **handle);

void print_device_info(libusb_context *context, libusb_device_handle *handle);

void detach_device(libusb_context *context, libusb_device_handle *handle, int intrf_n);

void get_config_info(libusb_context *context, libusb_device_handle *handle, struct libusb_config_descriptor **conf_desc);

void claim_intrf(libusb_context *context, libusb_device_handle *handle, int intrf_n);

void read_hid(libusb_context *context, libusb_device_handle *handle, struct libusb_endpoint_descriptor in_ep, uint8_t **report_buffer, int* size);

void init_usb_reader(libusb_context **context, libusb_device_handle **handle, struct libusb_endpoint_descriptor *in_ep, struct libusb_endpoint_descriptor *out_ep);

#endif