#include"core.h"
#include<hls_video.h>
hls::Mat<720,1280, HLS_8UC3> imagenHLS();
unsigned long int find(hls::stream<uint_32_ch> &instream,hls::stream<uint_32_ch> &outstream, int rows, int cols,int TL,int TH,int GL, int GH,int BL,int BH,int RL,int RH, int EN)
{


	RGB_IMAGE img_0(cols,rows);
	RGB_IMAGE img_1(cols,rows);
	//RGB_IMAGE img_2(rows,cols);
	RGB_PIXEL pix;
	int X=0;
	unsigned long int Pos=0;
	//Variable for Position in Pixels
	int row=1;
	int col=1;
	//*******************************
	//Variable for min/MAX of Row and Column
	int TRL=1;
	int TRH=1;
	int TCL=1;
	int TCH=1;
	int DR=0;
	int DC=0;
	int ENS=0;
	//*******************************
	hls::AXIvideo2Mat(instream, img_0);
	for (int i=0;i<(rows*cols);i++)
		{
		//Create Row and Column counter
		if (col==1280)
		{
			col=0;
			row++;
		}
		col++;
		//*************************
			pix=img_0.read();
			unsigned char R,G,B;
			B=pix.val[0];
			G=pix.val[1];
			R=pix.val[2];
			X=G-(B/2)-(R/2);
			if (G>=GL && G<=GH && B>=BL && B<=BH && R>=RL && R<=RH)
			{
				if (X>=TL && X<=TH)
				{
				//Find min/MAX of Row and Column
				if (TCL==1)
					TCL=col;
				if (TRL==1)
				{
					TRL=row;
					//Draw square if eqp
					DR=row;
					DC=col;
					ENS=1;
				}
				if (TRL>row)
					TRL=row;
				if (TRH<row)
					TRH=row;
				if (TCL>col)
					TCL=col;
				if (TCH<col)
					TCH=col;
				}
				if (EN==1)
				{
					pix.val[0]=0;
					pix.val[1]=0;
					pix.val[2]=0;
				}

			}
			if (EN==0 && ENS==1 && row>=(DR+2) && row<DR+22 && col>=(DC-10) && col<=(DC+10))
			{
				pix.val[0]=0;
				pix.val[1]=255;
				pix.val[2]=255;
			}
			img_1.write(pix);
	}
	long int Trow=(TRH-TRL)/2;
	unsigned long int Tcol=(TCH-TCL)/2;
	Tcol=Tcol+(TCL-1);
	Trow=Trow+(TRL-1);
	int Dis=TCH-TCL;
	Trow=Trow*1000;
	Tcol=Tcol*1000000;
	Pos=Tcol+Trow+Dis;
	hls::Mat2AXIvideo(img_1,outstream);
	return Pos;
}
