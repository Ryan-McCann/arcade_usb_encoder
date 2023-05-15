#include "lpc_types.h"
#include "error.h"
#include "usbd_rom_api.h"

#ifndef __APP_USBD_CFG_H
#define __APP_USBD_CFG_H

// HID Endpoint Addresses
#define HID_EP_IN		0x81
#define HID_EP_OUT		0x01

#define HID_ARCADE_REPORT_INTERVAL_MS	10
#define HID_ARCADE_REPORT_INTERVAL		1

// Constants to define memory area used by USBD_LIB stack
#define USB_STACK_MEM_BASE		0x20004000
#define USB_STACK_MEM_SIZE		0x0800

// Constants used by USBD LIB stack. Do not change
#define USB_MAX_IF_NUM			8
#define USB_MAX_EP_NUM			5
#define USB_MAX_PACKET0			64
#define USB_FS_MAX_BULK_PACKET	64
#define USB_HS_MAX_BULK_PACKET	512
#define USB_DFU_XFER_SIZE		2048

extern const uint8_t USB_DeviceDescriptor[];
extern uint8_t USB_HsConfigDescriptor[];
extern uint8_t USB_FsConfigDescriptor[];
extern const uint8_t USB_StringDescriptor[];
extern const uint8_t USB_DeviceQualifier[];

// USB descriptor arrays defined in *_desc.c file

/**
 * @brief	Find the address of interface descriptor for given class type.
 * @param 	pDesc		: Pointer to configuration descriptor in which the desired
 * 			class interface descriptor can be found.
 * @param	intfClass	: Interface class type to be searched.
 * @return	If found returns the address of requested interface else returns NULL.
 */

extern USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass);

#endif
