/*
 *
 * Header file For VDMA initialize and CONFIGURE
 *
 */

//Define Buffer size and Frames size
#define FRAME_HORIZONTAL_LEN  1280*4
#define SUBFRAME_HORIZONTAL_SIZE 1280*4
#define SUBFRAME_VERTICAL_SIZE   720

/*
 *    Function For install HDMI to VDMA
 */

int startVdmaHdmi(int ID_VDMA, XAxiVdma_Config *CfgVdmaHdmi, XAxiVdma VdmaHdmi, XAxiVdma_DmaSetup StpVdmaHdmi, DisplayCtrl vgaCtrl)
{
	int Status,i;

	xil_printf("START VDMA HDMI Configure:\n\r");

	CfgVdmaHdmi = XAxiVdma_LookupConfig(ID_VDMA);
	Status = XAxiVdma_CfgInitialize(&VdmaHdmi, CfgVdmaHdmi, CfgVdmaHdmi->BaseAddress);
	if (Status == XST_SUCCESS){
		Status = XAxiVdma_SetFrmStore(&VdmaHdmi, 3, XAXIVDMA_WRITE);
		if (Status == XST_SUCCESS){
		}
		else
		{
			xil_printf("------Buffer\n");
			return Status;
		}
	}
	else
	{
		xil_printf("Driver\n");
		return Status;
	}

	//Necessary variable for HDMI
	StpVdmaHdmi.VertSizeInput = SUBFRAME_VERTICAL_SIZE;
	StpVdmaHdmi.HoriSizeInput = SUBFRAME_HORIZONTAL_SIZE;
	StpVdmaHdmi.Stride = FRAME_HORIZONTAL_LEN;
	StpVdmaHdmi.FrameDelay = 0;
	StpVdmaHdmi.EnableCircularBuf = 1;
	StpVdmaHdmi.EnableSync = 0;
	StpVdmaHdmi.PointNum = 1;
	StpVdmaHdmi.EnableFrameCounter = 0;

	for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
	{
		StpVdmaHdmi.FrameStoreStartAddr[i] = (u32)(vgaCtrl.framePtr[i]);
		xil_printf("Direction %d frame Store Start HDMI: %x\n\r", i, (u32)(vgaCtrl.framePtr[i]));
	}

	Status = XAxiVdma_DmaConfig(&VdmaHdmi, XAXIVDMA_WRITE, &StpVdmaHdmi);
	Status = XAxiVdma_DmaSetBufferAddr(&VdmaHdmi, XAXIVDMA_WRITE, StpVdmaHdmi.FrameStoreStartAddr);
	xil_printf("Direction FrameStoreStartAddr HDMI --> %x\n\r", StpVdmaHdmi.FrameStoreStartAddr);
	Status = XAxiVdma_DmaStart(&VdmaHdmi, XAXIVDMA_WRITE);
	Status = XAxiVdma_StartParking(&VdmaHdmi, vgaCtrl.curFrame, XAXIVDMA_WRITE);

		if (Status != XST_SUCCESS)
		{
			xil_printf("Unable to put the channel %d\r\n", Status);
			return XST_FAILURE;
		}

		xil_printf("STATUS: %d", Status);

	if (Status == XST_SUCCESS){
		return Status;
	}
	else
	{
		xil_printf("Error Buffer Direction:%d\n",Status);
	}
}


int startVdmaHdmi2(int ID_VDMA, XAxiVdma_Config *CfgVdmaHdmi, XAxiVdma VdmaHdmi, XAxiVdma_DmaSetup StpVdmaHdmi, u32 *puntero[], DisplayCtrl vgaCtrl){
	int Status,i;

	xil_printf("START VDMA HDMI Configure:\n\r");
	CfgVdmaHdmi = XAxiVdma_LookupConfig(ID_VDMA);
	Status = XAxiVdma_CfgInitialize(&VdmaHdmi, CfgVdmaHdmi, CfgVdmaHdmi->BaseAddress);

	if (Status == XST_SUCCESS){
		Status = XAxiVdma_SetFrmStore(&VdmaHdmi, 3, XAXIVDMA_WRITE);
		if (Status == XST_SUCCESS){

		}
		else
		{
			xil_printf("------Buffer\n");
			return Status;
		}
	}else{
		xil_printf("Driver\n");
		return Status;
	}

	//Necessary Variable for HDMI
	StpVdmaHdmi.VertSizeInput = SUBFRAME_VERTICAL_SIZE;
	StpVdmaHdmi.HoriSizeInput = SUBFRAME_HORIZONTAL_SIZE;
	StpVdmaHdmi.Stride = FRAME_HORIZONTAL_LEN;
	StpVdmaHdmi.FrameDelay = 0;
	StpVdmaHdmi.EnableCircularBuf = 1;
	StpVdmaHdmi.EnableSync = 0;
	StpVdmaHdmi.PointNum = 1;
	StpVdmaHdmi.EnableFrameCounter = 0;

	for (i = 0; i < DISPLAY_NUM_FRAMES; i++){
		StpVdmaHdmi.FrameStoreStartAddr[i] = (u32) puntero[i];
		xil_printf("Direction %d frame Store Start HDMI --> %x\n\r", i, (u32) puntero[i]);
	}

	Status = XAxiVdma_DmaConfig(&VdmaHdmi, XAXIVDMA_WRITE, &StpVdmaHdmi);
	Status = XAxiVdma_DmaSetBufferAddr(&VdmaHdmi, XAXIVDMA_WRITE, StpVdmaHdmi.FrameStoreStartAddr);
	xil_printf("Direction  FrameStoreStartAddr  HDMI --> %x\n\r", StpVdmaHdmi.FrameStoreStartAddr);
	Status = XAxiVdma_DmaStart(&VdmaHdmi, XAXIVDMA_WRITE);
	Status = XAxiVdma_StartParking(&VdmaHdmi, vgaCtrl.curFrame, XAXIVDMA_WRITE);

		if (Status != XST_SUCCESS)
		{
			xil_printf("Unable to put the channel %d\r\n", Status);
			return XST_FAILURE;
		}

		xil_printf("STATUS: %d", Status);

	if (Status == XST_SUCCESS){
		return Status;
	}else{
		xil_printf("Error Buffer Direction:%d\n",Status);
	}

}

int startVdmaHls(int ID_VDMA, XAxiVdma_Config *CfgVdmaHls, XAxiVdma VdmaHls, XAxiVdma_DmaSetup StpRead, XAxiVdma_DmaSetup StpWrite, u32 *punteroRead[], u32 *punteroWrite[], DisplayCtrl vgaCtrl)
{
	int Status,i;

	xil_printf("Start HLS Configuration:\n");
	CfgVdmaHls = XAxiVdma_LookupConfig(ID_VDMA);
	Status = XAxiVdma_CfgInitialize(&VdmaHls, CfgVdmaHls, CfgVdmaHls->BaseAddress);
	if (Status == XST_SUCCESS){
		xil_printf("Buffer of Frame Error\n");
		Status = XAxiVdma_SetFrmStore(&VdmaHls, 3, XAXIVDMA_WRITE);
		if (Status == XST_SUCCESS){
			xil_printf("Buffer:\n");
		}else{
			xil_printf("3_Buffer:\n");
			return Status;
		}
		xil_printf("3_buffer:\n");
		Status = XAxiVdma_SetFrmStore(&VdmaHls, 3, XAXIVDMA_READ);
		if (Status == XST_SUCCESS){
		}
		else
		{
			xil_printf("3_buffer:\n");
			return Status;
		}
	}else{
		xil_printf("Driver\n");
		return Status;
	}
	//Necessary Parameters for HLS VDmA
	StpWrite.VertSizeInput = SUBFRAME_VERTICAL_SIZE;
	StpWrite.HoriSizeInput = SUBFRAME_HORIZONTAL_SIZE;
	StpWrite.Stride = FRAME_HORIZONTAL_LEN;
	StpWrite.FrameDelay = 0;
	StpWrite.EnableCircularBuf = 1;
	StpWrite.EnableSync = 0;
	StpWrite.PointNum = 1;
	StpWrite.EnableFrameCounter = 0;
	StpWrite.FixedFrameStoreAddr = vgaCtrl.curFrame;
	for (i = 0; i < DISPLAY_NUM_FRAMES; i++){
		StpWrite.FrameStoreStartAddr[i] = (u32) punteroWrite[i];
		xil_printf("Direction %d frame Store Start HLS --> %x\n\r", i, (u32) punteroWrite[i]);
	}

	Status = XAxiVdma_DmaConfig(&VdmaHls, XAXIVDMA_WRITE, &StpWrite);
	if (Status != XST_SUCCESS){
		xil_printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n\r");
	}

	xil_printf("Direction Frame Store StartAddr HLS --> %x\n\r", StpWrite.FrameStoreStartAddr);
	Status = XAxiVdma_DmaSetBufferAddr(&VdmaHls, XAXIVDMA_WRITE, StpWrite.FrameStoreStartAddr);
	if (Status == XST_SUCCESS){
		xil_printf("DmaSetBufferAddr HLS WRITE TODO correct. Status: %d\n\r",Status);
	}
	else
	{
		xil_printf("DmaSetBufferAddres ESCRITURA VDMA HLS. Status: %d\n\r",Status);
	}

	Status = XAxiVdma_DmaStart(&VdmaHls, XAXIVDMA_WRITE);
	if (Status != XST_SUCCESS){
		xil_printf("%%\n");
	}

	Status = XAxiVdma_StartParking(&VdmaHls, vgaCtrl.curFrame, XAXIVDMA_WRITE);
	if (Status == XST_SUCCESS){
		xil_printf("Configuration VDMA HLS. Status: %d\n\r",Status);
	}else{
		xil_printf("Error VDMA HLS. Status: %d\n\r",Status);
	}


	//Necessary Parameters
	StpRead.VertSizeInput = SUBFRAME_VERTICAL_SIZE;
	StpRead.HoriSizeInput = SUBFRAME_HORIZONTAL_SIZE;
	StpRead.Stride = FRAME_HORIZONTAL_LEN;
	StpRead.FrameDelay = 0;
	StpRead.EnableCircularBuf = 1;
	StpRead.EnableSync = 0;
	StpRead.PointNum = 1;
	StpRead.EnableFrameCounter = 0;
	StpRead.FixedFrameStoreAddr = vgaCtrl.curFrame;

	for (i = 0; i < DISPLAY_NUM_FRAMES; i++)
	{
		StpRead.FrameStoreStartAddr[i] = (u32) punteroRead[i];
		xil_printf("Direction %d frame Store Start HLS --> %x\n", i, (u32) punteroRead[i]);
	}

	Status = XAxiVdma_DmaConfig(&VdmaHls, XAXIVDMA_READ, &StpRead);
	Status = XAxiVdma_DmaSetBufferAddr(&VdmaHls, XAXIVDMA_READ, StpRead.FrameStoreStartAddr);
	xil_printf("Direction Frame Store Start Addr HLS --> %x\n\r", StpRead.FrameStoreStartAddr);

	if (Status == XST_SUCCESS)
	{
		xil_printf("DmaSetBufferAddr HLS READ TODO. Status: %d\n\r",Status);
	}
	else
	{
		xil_printf("Error VDMA HLS. Status: %d\n\r",Status);
		return Status;
	}


	Status = XAxiVdma_DmaStart(&VdmaHls, XAXIVDMA_READ);
	Status = XAxiVdma_StartParking(&VdmaHls, vgaCtrl.curFrame, XAXIVDMA_READ);

		if (Status != XST_SUCCESS)
		{
			xil_printf("Unable to put the read channel %d\r\n", Status);
			return XST_FAILURE;
		}

	if (Status == XST_SUCCESS)
	{
		xil_printf("Configuration VDMA HLS. Status:%d\n",Status);
		return Status;
	}else{
		xil_printf("Error Buffer.Status:%d\n",Status);
		return Status;
	}
}

