#ifndef HID_GAMEPAD_H
#define HID_GAMEPAD_H

#include <string.h>
#include "app_usbd_cfg.h"

#define ARCADECONTROLLER_REPORT_SIZE				21
#define HID_ARCADECONTROLLER_CLEAR_REPORT(x)		memset(x, 0, ARCADECONTROLLER_REPORT_SIZE);

extern ErrorCode_t ArcadeController_init(USBD_HANDLE_T hUsb,
										 USB_INTERFACE_DESCRIPTOR *pIntfDesc,
										 uint32_t *mem_base,
										 uint32_t *mem_size);

extern void ArcadeController_Tasks(void);

#endif
