#ifndef PRINTER_H
#define PRINTER_H

#ifdef USB_IO
#include <string.h>

#define USB_PRINT_SIZE 4000
#define USB_PRINT_PACKET_SIZE 500
#define USB_PRINT_INITNUM 12

#endif //USB_IO

#ifdef __cplusplus
extern "C" {
#endif

extern int printf(const char * format, ...);

#ifdef USB_IO
extern char usbPrintBuffer[USB_PRINT_SIZE];
extern int usbBufferSize;
#endif //USB_IO

#ifdef __cplusplus
}
#endif

#endif //_IHC_PRINTER_H_
