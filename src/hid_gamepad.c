#include "hid_gamepad.h"

#include "buttons.h"
#include "encoders.h"

typedef struct
{
	USBD_HANDLE_T hUsb;
	uint8_t report[ARCADECONTROLLER_REPORT_SIZE];
	uint8_t tx_busy;
} ArcadeController_Ctrl_T;

static ArcadeController_Ctrl_T g_arcadeController;

extern const uint8_t ArcadeController_ReportDescriptor[];
extern const uint16_t ArcadeController_ReportDescSize;

static void ArcadeController_UpdateReport(void)
{
	HID_ARCADECONTROLLER_CLEAR_REPORT(&g_arcadeController.report[0]);

	for (int i=0; i<18; i++)
		g_arcadeController.report[i] = buttons[i].state;

	g_arcadeController.report[18] = ((float)x.delta / (float)x.resolution) * 127;
	g_arcadeController.report[19] = ((float)y.delta / (float)y.resolution) * 127;
	g_arcadeController.report[20] = ((float)k.delta / (float)k.resolution) * 127;
}

static ErrorCode_t ArcadeController_GetReport(USBD_HANDLE_T hHid,
									  USB_SETUP_PACKET *pSetup,
									  uint8_t **pBuffer,
									  uint16_t *plength)
{
	switch(pSetup->wValue.WB.H)
	{
	case HID_REPORT_INPUT:
		ArcadeController_UpdateReport();
		memcpy(*pBuffer, &g_arcadeController.report[0], ARCADECONTROLLER_REPORT_SIZE);
		*plength = ARCADECONTROLLER_REPORT_SIZE;
		break;
	case HID_REPORT_OUTPUT:
		return ERR_USBD_STALL;
	case HID_REPORT_FEATURE:
		return ERR_USBD_STALL;
	default:
		return ERR_USBD_STALL;
	}

	return LPC_OK;
}

static ErrorCode_t ArcadeController_SetReport(USBD_HANDLE_T hHid,
											  USB_SETUP_PACKET *pSetup,
											  uint8_t **pBuffer,
											  uint16_t length)
{
	if (!length)
		return LPC_OK;

	switch(pSetup->wValue.WB.H)
	{
	case HID_REPORT_OUTPUT:
	case HID_REPORT_INPUT:
	case HID_REPORT_FEATURE:
	default:
		return ERR_USBD_STALL;
	}

	return LPC_OK;
}

static ErrorCode_t ArcadeController_EpIn_Hdlr(USBD_HANDLE_T hUsb, void *data, uint32_t event)
{
	switch(event)
	{
	case USB_EVT_IN:
		g_arcadeController.tx_busy = 0;
		break;
	default:
		break;
	}

	return LPC_OK;
}

ErrorCode_t ArcadeController_init(USBD_HANDLE_T hUsb,
								  USB_INTERFACE_DESCRIPTOR *pIntfDesc,
								  uint32_t *mem_base,
								  uint32_t *mem_size)
{
	USBD_HID_INIT_PARAM_T hid_param;
	USB_HID_REPORT_T reports_data[1];
	ErrorCode_t ret = LPC_OK;

	if ((!pIntfDesc) || pIntfDesc->bInterfaceClass != USB_DEVICE_CLASS_HUMAN_INTERFACE)
		return ERR_FAILED;

	memset((void *) &hid_param, 0, sizeof(USBD_HID_INIT_PARAM_T));
	hid_param.max_reports = 1;
	hid_param.mem_base = *mem_base;
	hid_param.mem_size = *mem_size;
	hid_param.intf_desc = (uint8_t *) pIntfDesc;

	hid_param.HID_GetReport = ArcadeController_GetReport;
	hid_param.HID_SetReport = ArcadeController_SetReport;
	hid_param.HID_EpIn_Hdlr = ArcadeController_EpIn_Hdlr;

	reports_data[0].len = ArcadeController_ReportDescSize;
	reports_data[0].idle_time = 0;
	reports_data[0].desc = (uint8_t *) &ArcadeController_ReportDescriptor[0];
	hid_param.report_data = reports_data;

	ret = USBD_API->hid->init(hUsb, &hid_param);

	*mem_base = hid_param.mem_base;
	*mem_size = hid_param.mem_size;

	g_arcadeController.hUsb = hUsb;

	return ret;
}

void ArcadeController_Tasks(void)
{
	if(USB_IsConfigured(g_arcadeController.hUsb))
	{
		if (!g_arcadeController.tx_busy)
		{
			ArcadeController_UpdateReport();

			g_arcadeController.tx_busy = 1;
			USBD_API->hw->WriteEP(g_arcadeController.hUsb, HID_EP_IN, &g_arcadeController.report[0], ARCADECONTROLLER_REPORT_SIZE);
		}
	}
	else
		g_arcadeController.tx_busy = 0;
}
