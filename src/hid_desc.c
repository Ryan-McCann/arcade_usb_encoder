#include "app_usbd_cfg.h"

const uint8_t ArcadeController_ReportDescriptor[] = {
	HID_UsagePage(HID_USAGE_PAGE_GENERIC),
		HID_Usage(HID_USAGE_GENERIC_GAMEPAD),
		HID_Collection(HID_Application),
		HID_Usage(HID_USAGE_GENERIC_POINTER),
			HID_Collection(HID_Physical),
				HID_UsagePage(HID_USAGE_PAGE_BUTTON),
					HID_UsageMin(1),
					HID_UsageMax(18),
					HID_LogicalMin(0),
					HID_LogicalMax(1),
					HID_ReportCount(18),
					HID_ReportSize(1),
					HID_Input(HID_Data | HID_Variable | HID_Absolute),

				HID_UsagePage(HID_USAGE_PAGE_GENERIC),
					HID_Usage(HID_USAGE_GENERIC_X),
					HID_Usage(HID_USAGE_GENERIC_Y),
					HID_Usage(HID_USAGE_GENERIC_Z),
					HID_LogicalMin( (uint8_t) -127),
					HID_LogicalMax(127),
					HID_ReportSize(8),
					HID_ReportCount(3),
					HID_Input(HID_Data | HID_Variable | HID_Relative),
			HID_EndCollection,
		HID_EndCollection
};
const uint16_t ArcadeController_ReportDescSize = sizeof(ArcadeController_ReportDescriptor);

/**
 * USB Standard Device Descriptor
 */
ALIGNED(4) const uint8_t USB_DeviceDescriptor[] = {
	USB_DEVICE_DESC_SIZE,			/* bLength */
	USB_DEVICE_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0200),					/* bcdUSB = 2.00*/
	0x00,							/* bDeviceClass */
	0x00,							/* bDeviceSubClass */
	0x00,							/* bDeviceProtocol */
	USB_MAX_PACKET0,				/* bMaxPacketSize0 */
	WBVAL(0x1FC9),					/* idVendor */
	WBVAL(0x0086),					/* idProduct */
	WBVAL(0x0100),					/* bcdDevice */
	0x01,							/* iManufacturer */
	0x02,							/* iProduct */
	0x03,							/* iSerialNumber */
	0x01							/* bNumConfigurations */
};

/**
 * USB FSConfiguration Descriptor
 * All Descriptors (Configuration, Interface, Endpoint, Class, Vendor)
 */
ALIGNED(4) uint8_t USB_FsConfigDescriptor[] = {
	/* Configuration 1 */
	USB_CONFIGURATION_DESC_SIZE,			/* bLength */
	USB_CONFIGURATION_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(									/* wTotalLength */
		USB_CONFIGURATION_DESC_SIZE   +
		USB_INTERFACE_DESC_SIZE     +
		HID_DESC_SIZE               +
		USB_ENDPOINT_DESC_SIZE
		),
	0x01,							/* bNumInterfaces */
	0x01,							/* bConfigurationValue */
	0x00,							/* iConfiguration */
	USB_CONFIG_SELF_POWERED,		/* bmAttributes */
	USB_CONFIG_POWER_MA(100),			/* bMaxPower */

	/* Interface 0, Alternate Setting 0, HID Class */
	USB_INTERFACE_DESC_SIZE,		/* bLength */
	USB_INTERFACE_DESCRIPTOR_TYPE,	/* bDescriptorType */
	0x00,							/* bInterfaceNumber */
	0x00,							/* bAlternateSetting */
	0x01,							/* bNumEndpoints */
	USB_DEVICE_CLASS_HUMAN_INTERFACE,	/* bInterfaceClass */
	HID_SUBCLASS_NONE,				/* bInterfaceSubClass */
	HID_PROTOCOL_NONE,			/* bInterfaceProtocol */
	0x04,							/* iInterface */
	/* HID Class Descriptor */
	/* HID_DESC_OFFSET = 0x0012 */
	HID_DESC_SIZE,					/* bLength */
	HID_HID_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0111),					/* bcdHID : 1.11*/
	0x00,							/* bCountryCode */
	0x01,							/* bNumDescriptors */
	HID_REPORT_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(sizeof(ArcadeController_ReportDescriptor)),	/* wDescriptorLength */
	/* Endpoint, HID Interrupt In */
	USB_ENDPOINT_DESC_SIZE,			/* bLength */
	USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType */
	HID_EP_IN,						/* bEndpointAddress */
	USB_ENDPOINT_TYPE_INTERRUPT,	/* bmAttributes */
	WBVAL(0x0008),					/* wMaxPacketSize */
	HID_ARCADE_REPORT_INTERVAL,		/* bInterval */
	/* Terminator */
	0								/* bLength */
};

/**
 * USB String Descriptor (optional)
 */
const uint8_t USB_StringDescriptor[] = {
	/* Index 0x00: LANGID Codes */
	0x04,							/* bLength */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	WBVAL(0x0409),					/* wLANGID  0x0409 = US English*/
	/* Index 0x01: Manufacturer */
	(18 * 2 + 2),					/* bLength (18 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'N', 0,
	'X', 0,
	'P', 0,
	' ', 0,
	'S', 0,
	'e', 0,
	'm', 0,
	'i', 0,
	'c', 0,
	'o', 0,
	'n', 0,
	'd', 0,
	'u', 0,
	'c', 0,
	't', 0,
	'o', 0,
	'r', 0,
	's', 0,
	/* Index 0x02: Product */
	(16 * 2 + 2),					/* bLength (16 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'L', 0,
	'P', 0,
	'C', 0,
	'1', 0,
	'1', 0,
	'U', 0,
	'x', 0,
	' ', 0,
	'K', 0,
	'E', 0,
	'Y', 0,
	'B', 0,
	'O', 0,
	'A', 0,
	'R', 0,
	'D', 0,
	/* Index 0x03: Serial Number */
	(13 * 2 + 2),					/* bLength (13 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'A', 0,
	'B', 0,
	'C', 0,
	'D', 0,
	'1', 0,
	'2', 0,
	'3', 0,
	'4', 0,
	'5', 0,
	'6', 0,
	'7', 0,
	'8', 0,
	'9', 0,
	/* Index 0x04: Interface 0, Alternate Setting 0 */
	(12 * 2 + 2),					/* bLength (12 Char + Type + lenght) */
	USB_STRING_DESCRIPTOR_TYPE,		/* bDescriptorType */
	'H', 0,
	'I', 0,
	'D', 0,
	' ', 0,
	'K', 0,
	'E', 0,
	'Y', 0,
	'B', 0,
	'O', 0,
	'A', 0,
	'R', 0,
	'D', 0,
};
