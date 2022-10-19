
#include<hls_video.h>
#include<ap_axi_sdata.h>
// maximum image size
#define MAX_WIDTH  1280
#define MAX_HEIGHT 720

// Se definen las estructuras.
//typedef hls::stream<ap_axiu<32,1,1,1>>               AXI_STREAM;
typedef hls::Scalar<3, unsigned char>                 RGB_PIXEL;
typedef hls::Mat<MAX_HEIGHT, MAX_WIDTH, HLS_8UC3>     RGB_IMAGE;
typedef ap_axiu<32,1,1,1> uint_32_ch;

unsigned long int find(hls::stream<uint_32_ch> &instream,hls::stream<uint_32_ch> &outstream, int rows, int cols,int TL,int TH,int GL, int GH,int BL,int BH,int RL,int RH,int EN);


