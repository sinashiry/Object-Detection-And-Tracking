/*
 *
 *
 *
 *
 *    ABOUT CODE :D:D:D
 *
 *
 *
 *
 */

/*
 * ##########_____Headers_____##########
 */

#include "display_demo.h"
#include "xaxivdma.h"
#include "timer_ps.h"
#include "xparameters.h"
#include "xuartps.h"
#include "Vdma.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "xsdps.h"
#include "xfind.h"
#include "xgpio.h"

/*
 * ##########_____Global Variables_____##########
 */

DisplayCtrl vgaCtrl;
XAxiVdma VdmaHDMI;

//Array for 3 x (1280x720 = 2764800) 1 frame (3#color)

u32 vgaBuf[DISPLAY_NUM_FRAMES][DISPLAYDEMO_MAX_FRAME];
u32 hdmiBuf[DISPLAY_NUM_FRAMES][DISPLAYDEMO_MAX_FRAME];

/*
 *##########_____Definitions_____##########
 */

//VGA out
#define VGA_BASEADDR XPAR_AXI_DISPCTRL_VGA_S_AXI_BASEADDR
//VDMA VGA
#define VGA_VDMA_ID XPAR_AXI_VDMA_VGA_DEVICE_ID
//VDMA HDMI
#define HDMI_VDMA_ID XPAR_AXI_VDMA_HDMI_DEVICE_ID
//VDMA Find Core
#define HLS_VDMA_ID XPAR_AXI_VDMA_HLS_DEVICE_ID
//Internal Clock
#define SCU_TIMER_ID XPAR_SCUTIMER_DEVICE_ID
// Control Serial Port
#define UART_BASEADDR XPAR_PS7_UART_1_BASEADDR
//DELETE
#define SW_BASEADDR XPAR_SWS_4BITS_BASEADDR
//DELETE
#define SW_ID XPAR_SWS_4BITS_DEVICE_ID
//DELETE maybe
#define TIMER_LOAD_VALUE	0x64C4B8
//Declare Find Core (created in HLS)
#define FIND_ID XPAR_FIND_0_DEVICE_ID

//VDMA HDMI Parameters
XAxiVdma_Config *CfgVdmaHdmi;
XAxiVdma VdmaHdmi;
XAxiVdma_DmaSetup StpVdmaHdmi;

//VDMA VGA Parameters
XAxiVdma_Config *CfgVdmaVga;
XAxiVdma VdmaVga;
XAxiVdma_DmaSetup StpVdmaVga;

//VDMA HLS Core Parameters
XAxiVdma_Config *CfgVdmaHls;
XAxiVdma VdmaHls;
XAxiVdma_DmaSetup StpRead;
XAxiVdma_DmaSetup StpWrite;

//SCu Timer
XScuTimer TimerPS7;

/*
 *
 *
 */

int main(void)
{
	//clear Cache that show how much before!
	Xil_DCacheDisable();

	//Console Settings
	//Move Cursor to Top Left of console
	xil_printf("\x1B[H");
	//Clear Terminal
	xil_printf("\x1B[2J");
	//////////////////////////

	//Create a VGA pointer with DISPLAY_NUM_FRAMES size
	u32 *vgaPtr[DISPLAY_NUM_FRAMES];
	//Create a HDMI pointer with DISPLAY_NUM_FRAMES size
	u32 *hdmiPtr[DISPLAY_NUM_FRAMES];

	//variable for Loops and Status of Code.
	int i,Status;

	for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
	{
		//Maybe
		vgaPtr[i] = vgaBuf[i];
		hdmiPtr[i] = hdmiBuf[i];
	}
	while(1){}

	//initialize ZYNQ timer
	TimerInitialize(SCU_TIMER_ID);

	/*
	 * Initialize Find CORE (HLS)
	 */

	XFind *coreFind;
	XFind_Config *coreFindCfg;
	Status = XFind_Initialize(&coreFind, FIND_ID);
	xil_printf("Initialize of Core:%d\n", Status);
	coreFindCfg = XFind_LookupConfig(FIND_ID);
	Status = XFind_CfgInitialize(&coreFind, coreFindCfg);
	xil_printf("Configure Core:%d\n", Status);

	///////////////////////////

	/*
	 * TARGET Color Settings
	 */

	//BLUE:
	XFind_SetBl(&coreFind,0);
	XFind_SetBh(&coreFind,95);
	//GREEN:
	XFind_SetGl(&coreFind,150);
	XFind_SetGh(&coreFind,255);
	//RED:
	XFind_SetRl(&coreFind,0);
	XFind_SetRh(&coreFind,143);
	//2nd FILTER:
	XFind_SetTl(&coreFind,30);
	XFind_SetTh(&coreFind,255);

	//////////////////////////

	// Enable TEST mode:
	XFind_SetEn(&coreFind,1);
	//////////////////////////

	//Set ROW and COLUMN SIZE:
	XFind_SetRows(&coreFind, 720);
	XFind_SetCols(&coreFind, 1280);
	//////////////////////////

	//Start Core:
	XFind_Start(&coreFind);
	XFind_EnableAutoRestart(&coreFind);
	/////////////////////////

	//Initialize VMDA HLS
	Status = startVdmaHls(HLS_VDMA_ID, CfgVdmaHls, VdmaHls, StpRead, StpWrite, hdmiPtr, vgaPtr, vgaCtrl);
	if (Status != XST_SUCCESS)
	{
			xil_printf("VDMA HLS Status:%d\n", Status);
	}

	//Configure VGA out (VDMA)
	Status = DisplayDemoInitialize(&vgaCtrl, VGA_VDMA_ID, SCU_TIMER_ID, VGA_BASEADDR, DISPLAY_NOT_HDMI, vgaPtr);
	if (Status != XST_SUCCESS)
	{
		xil_printf("Configure VGA out status:%d\n", Status);
	}

	//Initialize VMDA HDMI
	Status = startVdmaHdmi2(HDMI_VDMA_ID, CfgVdmaHdmi, VdmaHdmi, StpVdmaHdmi, hdmiPtr, vgaCtrl);
	if (Status != XST_SUCCESS)
	{
		xil_printf("VDMA HDMI Status:%d\n", Status);
	}

	// Main loop for Repeat code for tracking Ball

	while (1)
	{
		long int i=0;
		while(i<1000000)
		{
			i++;
		}
		long int r=XFind_GetReturn(&coreFind);
		long int X=r/1000000;
		long int YY=r%1000000;
		long int Y=YY/1000;
		int Dis=YY%1000;
		float f=1753.68;
		Dis=(7.46*f)/(Dis);
		//int rr=Dis;
		xil_printf("X=%d	Y=%d\nDis=%d\n",X,Y,Dis);
	}
}
