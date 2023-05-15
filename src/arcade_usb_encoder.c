/*
===============================================================================
 Name        : arcade_usb_encoder.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#include "chip.h"

#include "app_usbd_cfg.h"
#include "hid_gamepad.h"

#include "buttons.h"
#include "encoders.h"
#include "timers.h"

#include <cr_section_macros.h>

#define TICKSPERMICROSECOND 48

// Pin defines
#define BTN0PORT	1
#define BTN0PIN		13
#define BTN1PORT	0
#define BTN1PIN		14
#define BTN2PORT	0
#define BTN2PIN		13
#define BTN3PORT	0
#define BTN3PIN		12
#define BTN4PORT	0
#define BTN4PIN		11
#define BTN5PORT	1
#define BTN5PIN		29
#define BTN6PORT	0
#define BTN6PIN		22
#define BTN7PORT	0
#define BTN7PIN		10
#define BTN8PORT	0
#define BTN8PIN		9
#define BTN9PORT	0
#define BTN9PIN		8
#define BTN10PORT	1
#define BTN10PIN	25
#define BTN11PORT	1
#define BTN11PIN	19
#define BTN12PORT	0
#define BTN12PIN	0
#define BTN13PORT	0
#define BTN13PIN	1
#define BTN14PORT	0
#define BTN14PIN	20
#define BTN15PORT	0
#define BTN15PIN	2
#define BTN16PORT	1
#define BTN16PIN	26
#define BTN17PORT	1
#define BTN17PIN	27

#define K1PORT	1
#define K1PIN	22
#define K2PORT	1
#define K2PIN	14
#define X1PORT	0
#define X1PIN	17
#define X2PORT	0
#define X2PIN	18
#define Y1PORT 	0
#define Y1PIN	19
#define Y2PORT	1
#define Y2PIN	16

// Interrupt handler defines
#define k1InterruptHandler FLEX_INT0_IRQHandler
#define k2InterruptHandler FLEX_INT1_IRQHandler
#define x1InterruptHandler FLEX_INT2_IRQHandler
#define x2InterruptHandler FLEX_INT3_IRQHandler
#define y1InterruptHandler FLEX_INT4_IRQHandler
#define y2InterruptHandler FLEX_INT5_IRQHandler

#define group0InterruptHandler GINT0_IRQHandler
#define group1InterruptHandler GINT1_IRQHandler

void k1InterruptHandler(void)
{
	setEncoderState(&k, Chip_GPIO_GetPinState(LPC_GPIO, K1PORT, K1PIN), Chip_GPIO_GetPinState(LPC_GPIO, K2PORT, K2PIN));
}

void k2InterruptHandler(void)
{
	setEncoderState(&k, Chip_GPIO_GetPinState(LPC_GPIO, K1PORT, K1PIN), Chip_GPIO_GetPinState(LPC_GPIO, K2PORT, K2PIN));
}

void x1InterruptHandler(void)
{
	setEncoderState(&x, Chip_GPIO_GetPinState(LPC_GPIO, X1PORT, X1PIN), Chip_GPIO_GetPinState(LPC_GPIO, X2PORT, X2PIN));
}

void x2InterruptHandler(void)
{
	setEncoderState(&x, Chip_GPIO_GetPinState(LPC_GPIO, X1PORT, X1PIN), Chip_GPIO_GetPinState(LPC_GPIO, X2PORT, X2PIN));
}

void y1InterruptHandler(void)
{
	setEncoderState(&y, Chip_GPIO_GetPinState(LPC_GPIO, Y1PORT, Y1PIN), Chip_GPIO_GetPinState(LPC_GPIO, Y2PORT, Y2PIN));
}

void y2InterruptHandler(void)
{
	setEncoderState(&y, Chip_GPIO_GetPinState(LPC_GPIO, Y1PORT, Y1PIN), Chip_GPIO_GetPinState(LPC_GPIO, Y2PORT, Y2PIN));
}

void group0InterruptHandler(void)
{
	setButtonState(1, Chip_GPIO_GetPinState(LPC_GPIO, BTN1PORT, BTN1PIN));
	setButtonState(2, Chip_GPIO_GetPinState(LPC_GPIO, BTN2PORT, BTN2PIN));
	setButtonState(3, Chip_GPIO_GetPinState(LPC_GPIO, BTN3PORT, BTN3PIN));
	setButtonState(4, Chip_GPIO_GetPinState(LPC_GPIO, BTN4PORT, BTN4PIN));
	setButtonState(6, Chip_GPIO_GetPinState(LPC_GPIO, BTN6PORT, BTN6PIN));
	setButtonState(7, Chip_GPIO_GetPinState(LPC_GPIO, BTN7PORT, BTN7PIN));
	setButtonState(8, Chip_GPIO_GetPinState(LPC_GPIO, BTN8PORT, BTN8PIN));
	setButtonState(9, Chip_GPIO_GetPinState(LPC_GPIO, BTN9PORT, BTN9PIN));
	setButtonState(12, Chip_GPIO_GetPinState(LPC_GPIO, BTN12PORT, BTN12PIN));
	setButtonState(13, Chip_GPIO_GetPinState(LPC_GPIO, BTN13PORT, BTN13PIN));
	setButtonState(14, Chip_GPIO_GetPinState(LPC_GPIO, BTN14PORT, BTN14PIN));
	setButtonState(15, Chip_GPIO_GetPinState(LPC_GPIO, BTN15PORT, BTN15PIN));
}

void group1InterruptHandler(void)
{
	setButtonState(0, Chip_GPIO_GetPinState(LPC_GPIO, BTN0PORT, BTN0PIN));
	setButtonState(5, Chip_GPIO_GetPinState(LPC_GPIO, BTN5PORT, BTN5PIN));
	setButtonState(10, Chip_GPIO_GetPinState(LPC_GPIO, BTN10PORT, BTN10PIN));
	setButtonState(11, Chip_GPIO_GetPinState(LPC_GPIO, BTN11PORT, BTN11PIN));
	setButtonState(16, Chip_GPIO_GetPinState(LPC_GPIO, BTN16PORT, BTN16PIN));
	setButtonState(17, Chip_GPIO_GetPinState(LPC_GPIO, BTN17PORT, BTN17PIN));
}

void setupPins(void)
{
	// Set all pins used for buttons to PIOX_Y function, no pullup/pulldown, inverted
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN0PORT, BTN0PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 0
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN0PORT, BTN0PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN1PORT, BTN1PIN, IOCON_FUNC1|IOCON_MODE_INACT|IOCON_INV_EN); // btn 1
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN1PORT, BTN1PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN2PORT, BTN2PIN, IOCON_FUNC1|IOCON_MODE_INACT|IOCON_INV_EN); // btn 2
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN2PORT, BTN2PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN3PORT, BTN3PIN, IOCON_FUNC1|IOCON_MODE_INACT|IOCON_INV_EN); // btn 3
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN3PORT, BTN3PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN4PORT, BTN4PIN, IOCON_FUNC1|IOCON_MODE_INACT|IOCON_INV_EN); // btn 4
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN4PORT, BTN4PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN5PORT, BTN5PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 5
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN5PORT, BTN5PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN6PORT, BTN6PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 6
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN6PORT, BTN6PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN7PORT, BTN7PIN, IOCON_FUNC1|IOCON_MODE_INACT|IOCON_INV_EN); // btn 7
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN7PORT, BTN7PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN8PORT,  BTN8PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 8
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN8PORT, BTN8PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN9PORT, BTN9PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 9
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN9PORT, BTN9PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN10PORT, BTN10PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 10
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN10PORT, BTN10PORT);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN11PORT, BTN11PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 11
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN11PORT, BTN11PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN12PORT, BTN12PIN, IOCON_FUNC1|IOCON_MODE_INACT|IOCON_INV_EN); // btn 12
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN12PORT, BTN12PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN13PORT, BTN13PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 13
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN13PORT, BTN13PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN14PORT, BTN14PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 14
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN14PORT, BTN14PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN15PORT, BTN15PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 15
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN15PORT, BTN15PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN16PORT, BTN16PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 16
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN16PORT, BTN16PIN);
	Chip_IOCON_PinMuxSet(LPC_IOCON, BTN17PORT, BTN17PIN, IOCON_FUNC0|IOCON_MODE_INACT|IOCON_INV_EN); // btn 17
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, BTN17PORT, BTN17PIN);

	uint32_t group1Mask = 1 << BTN1PIN | 1 << BTN2PIN | 1 << BTN3PIN | 1 << BTN4PIN | 1 << BTN6PIN | 1 << BTN7PIN |
			1 << BTN8PIN | 1 << BTN9PIN | 1 << BTN12PIN | 1 << BTN13PIN | 1 << BTN14PIN | 1 << BTN15PIN;
	uint32_t group2Mask = 1 << BTN0PIN | 1 << BTN5PIN | 1 << BTN10PIN | 1 << BTN11PIN | 1 << BTN16PIN | 1 << BTN17PIN;


	// Set group interrupts for pins on ports 0 and 1
	Chip_GPIOGP_EnableGroupPins(LPC_GPIOGROUP, 0, 0, group1Mask);
	Chip_GPIOGP_SelectOrMode(LPC_GPIOGROUP, 0);
	Chip_GPIOGP_SelectEdgeMode(LPC_GPIOGROUP, 0);
	Chip_GPIOGP_SelectLowLevel(LPC_GPIOGROUP, 0, 0, group1Mask);
	Chip_GPIOGP_SelectHighLevel(LPC_GPIOGROUP, 0, 0, group1Mask);

	Chip_GPIOGP_EnableGroupPins(LPC_GPIOGROUP, 1, 1, group2Mask);
	Chip_GPIOGP_SelectOrMode(LPC_GPIOGROUP, 1);
	Chip_GPIOGP_SelectEdgeMode(LPC_GPIOGROUP, 1);
	Chip_GPIOGP_SelectLowLevel(LPC_GPIOGROUP, 1, 1, group2Mask);
	Chip_GPIOGP_SelectHighLevel(LPC_GPIOGROUP, 1, 1, group2Mask);

	// Set all pins for encoder inputs to PIOX_Y, pulled down and set interrupts
	Chip_IOCON_PinMuxSet(LPC_IOCON, K1PORT, K1PIN, IOCON_FUNC0|IOCON_MODE_PULLDOWN); // k1
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, K1PORT, K1PIN);
	Chip_SYSCTL_SetPinInterrupt(0, K1PORT, K1PIN);
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(0));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(0));
	Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(0));

	Chip_IOCON_PinMuxSet(LPC_IOCON, K2PORT, K2PIN, IOCON_FUNC0|IOCON_MODE_PULLDOWN); // k2
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, K2PORT, K2PIN);
	Chip_SYSCTL_SetPinInterrupt(1, K2PORT, K2PIN);
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(1));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(1));
	Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(1));

	Chip_IOCON_PinMuxSet(LPC_IOCON, X1PORT, X1PIN, IOCON_FUNC0|IOCON_MODE_PULLDOWN); // x1
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, X1PORT, X1PIN);
	Chip_SYSCTL_SetPinInterrupt(2, X1PORT, X2PIN);
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(2));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(2));
	Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(2));

	Chip_IOCON_PinMuxSet(LPC_IOCON, X2PORT, X2PIN, IOCON_FUNC0|IOCON_MODE_PULLDOWN); // x2
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, X2PORT, X2PIN);
	Chip_SYSCTL_SetPinInterrupt(3, X2PORT, X2PIN);
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(3));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(3));
	Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(3));

	Chip_IOCON_PinMuxSet(LPC_IOCON, Y1PORT, Y1PIN, IOCON_FUNC0|IOCON_MODE_PULLDOWN); // y1
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, Y1PORT, Y1PIN);
	Chip_SYSCTL_SetPinInterrupt(4, Y1PORT, Y1PIN);
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(4));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(4));
	Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(4));

	Chip_IOCON_PinMuxSet(LPC_IOCON, Y2PIN, Y2PORT, IOCON_FUNC0|IOCON_MODE_PULLDOWN); // y2
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, Y2PORT, Y2PIN);
	Chip_SYSCTL_SetPinInterrupt(5, Y2PORT, Y2PIN);
	Chip_PININT_SetPinModeEdge(LPC_PININT, PININTCH(5));
	Chip_PININT_EnableIntLow(LPC_PININT, PININTCH(5));
	Chip_PININT_EnableIntHigh(LPC_PININT, PININTCH(5));

	NVIC_EnableIRQ(PIN_INT0_IRQn);
	NVIC_EnableIRQ(PIN_INT1_IRQn);
	NVIC_EnableIRQ(PIN_INT2_IRQn);
	NVIC_EnableIRQ(PIN_INT3_IRQn);
	NVIC_EnableIRQ(PIN_INT4_IRQn);
	NVIC_EnableIRQ(PIN_INT5_IRQn);

	NVIC_EnableIRQ(GINT0_IRQn);
	NVIC_EnableIRQ(GINT1_IRQn);
}

void setupChip(void)
{
	//SysTick_Config(SystemCoreClock / TICKSPERMICROSECOND); // set systick interrupt to 1us

	//Chip_GPIO_Init(LPC_GPIO);
	//Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_PINT);
	//Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_GROUP0INT);
	//Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_GROUP1INT);

	// initialize usb clocks
	Chip_Clock_SetUSBClockSource(SYSCTL_USBCLKSRC_PLLOUT, 1);
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_USB);
	Chip_Clock_EnablePeriphClock(SYSCTL_CLOCK_USBRAM);

	Chip_SYSCTL_PowerUp(SYSCTL_POWERDOWN_USBPAD_PD);
}

static USBD_HANDLE_T g_hUsb;
extern const USBD_HW_API_T hw_api;
extern const USBD_CORE_API_T core_api;
extern const USBD_HID_API_T hid_api;

static const USBD_API_T g_usbApi =
{
	&hw_api,
	&core_api,
	0,
	0,
	&hid_api,
	0,
	0,
	0x02221101
};

const USBD_API_T *g_pUsbApi = &g_usbApi;

void USB_IRQHandler(void)
{
	USBD_API->hw->ISR(g_hUsb);
}

USB_INTERFACE_DESCRIPTOR *find_IntfDesc(const uint8_t *pDesc, uint32_t intfClass)
{
	USB_COMMON_DESCRIPTOR *pD;
	USB_INTERFACE_DESCRIPTOR *pIntfDesc = 0;
	uint32_t next_desc_adr;

	pD = (USB_COMMON_DESCRIPTOR *) pDesc;
	next_desc_adr = (uint32_t) pDesc;

	while (pD->bLength)
	{
		if(pD->bDescriptorType == USB_INTERFACE_DESCRIPTOR_TYPE)
		{
			pIntfDesc = (USB_INTERFACE_DESCRIPTOR *) pD;
			if (pIntfDesc->bInterfaceClass == intfClass)
				break;
		}
	}
	pIntfDesc = 0;
	next_desc_adr = (uint32_t) pD + pD->bLength;
	pD = (USB_COMMON_DESCRIPTOR *) next_desc_adr;

	return pIntfDesc;
}

int main(void)
{
	USBD_API_INIT_PARAM_T usb_param;
	USB_CORE_DESCS_T desc;
	ErrorCode_t ret = LPC_OK;

	// Initialize buttons and encoder axes
    for(int i=0; i<18; i++)
	{
		buttons[i].state = false;
		buttons[i].timestamp = 0;
	}

	k.prevA = FALSE;
	k.A = FALSE;
	k.delta = 0;

	x.prevA = FALSE;
	x.A = false;
	x.delta = 0;

	y.prevA = FALSE;
	y.A = false;
	y.delta = 0;

	// Call setup
	setupChip();
    //setupPins();

    // initialize usb call backs
    memset((void*) &usb_param, 0, sizeof(USBD_API_INIT_PARAM_T));
    usb_param.usb_reg_base = LPC_USB0_BASE;
    usb_param.max_num_ep = 2;
    usb_param.mem_base = USB_STACK_MEM_BASE;
    usb_param.mem_size = USB_STACK_MEM_SIZE;

    // Set the USB descriptors
    desc.device_desc = (uint8_t *) USB_DeviceDescriptor;
    desc.string_desc = (uint8_t *) USB_StringDescriptor;

    desc.high_speed_desc = USB_FsConfigDescriptor;
    desc.full_speed_desc = USB_FsConfigDescriptor;
    desc.device_qualifier = 0;

    ret = USBD_API->hw->Init(&g_hUsb, &desc, &usb_param);

    if (ret == LPC_OK)
    {
    	USBD_API->hw->ForceFullSpeed(g_hUsb, 1);
    	ret = ArcadeController_init(g_hUsb,
    							    (USB_INTERFACE_DESCRIPTOR *) &USB_FsConfigDescriptor[sizeof(USB_CONFIGURATION_DESCRIPTOR)],
    								&usb_param.mem_base, &usb_param.mem_size);

    	if(ret == LPC_OK)
    	{
    		NVIC_EnableIRQ(USB0_IRQn);
    		USBD_API->hw->Connect(g_hUsb, 1);
    	}
    }

    while(1)
    {
    	ArcadeController_Tasks();
    	// Sleep until next IRQ
    	__WFI();
    }
    return 0 ;
}
