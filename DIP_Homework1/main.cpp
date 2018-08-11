#include <iostream>
#include <stdlib.h>
#include "bmp.h"
#include "masks.h"

using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int G[MaxBMPSizeX][MaxBMPSizeY];
int B[MaxBMPSizeX][MaxBMPSizeY];

int r[MaxBMPSizeX][MaxBMPSizeY];
int g[MaxBMPSizeX][MaxBMPSizeY];
int b[MaxBMPSizeX][MaxBMPSizeY];

int H[MaxBMPSizeX][MaxBMPSizeY];
float S[MaxBMPSizeX][MaxBMPSizeY];
float V[MaxBMPSizeX][MaxBMPSizeY];

int main(int argc, char *argv[])
{
	int width, height;
	int x, y;

	//第一題 (1)
	//open_bmp((char*)"picture\\skin_det_1.bmp", R, G, B, width, height);
	//rgb2hsv(H, S, V, R, G, B, width, height);
	//for (x = 0;x < width;x++) {
	//	for (y = 0;y < height;y++) {
	//		if (H[x][y] >= 6 && H[x][y] <= 50 && S[x][y] >= 0.11 && S[x][y] <= 0.8 && V[x][y] >= 0.65) {
	//			r[x][y] = 255;
	//			g[x][y] = 255;
	//			b[x][y] = 255;
	//		}
	//	}
	//}
	//save_bmp((char*)"out\\skin_det_1_initial.bmp", r, g, b);


	//第一題 (2)
	//open_bmp((char*)"picture\\skin_det_2.bmp", R, G, B, width, height);
	//rgb2hsv(H, S, V, R, G, B, width, height);
	//for (x = 0;x < width;x++) {
	//	for (y = 0;y < height;y++) {
	//		if (H[x][y] >= 6 && H[x][y] <= 50 && S[x][y] >= 0.1 && S[x][y] <= 0.8 && V[x][y] >= 0.6) {
	//			r[x][y] = 255;
	//			g[x][y] = 255;
	//			b[x][y] = 255;
	//		}
	//	}
	//}
	//save_bmp((char*)"out\\skin_det_2_new.bmp", r, g, b);
	

	//第二題_lowpass
	//int i, j;
	//int n=2;
	//float D0 = 10;
	//open_bmp((char *)"picture\\cameraman.bmp", R, R, R, width, height);
	//Butterworth_LF(r, R, D0, n, width, height);
	//save_bmp((char *)"out//cam_low_n=2_d0=10.bmp", r, r, r);


	//第二題_highpass
	//int i, j;
	//int n = 2;
	//float D0 = 50;
	//open_bmp((char *)"picture\\lena.bmp", R, R, R, width, height);
	//Butterworth_HF(r, R, D0, n, width, height);
	//save_bmp((char *)"out//lena_high_n=2_d0=50.bmp", r, r, r);
	


	//第三題
	//open_bmp((char*)"picture\\noise_remov_morph_gray.bmp", R, R, R, width, height);
    //
	//erosion(R, width, height);
	//save_bmp((char*)"out\\noise_remov_morph_gray_1.bmp", R, R, R);
	//erosion(R, width, height);
	//save_bmp((char*)"out\\noise_remov_morph_gray_2.bmp", R, R, R);
	//dilation(R, width, height);
	//save_bmp((char*)"out\\noise_remov_morph_gray_3.bmp", R, R, R);
	//dilation(R, width, height);
	//save_bmp((char*)"out\\noise_remov_morph_gray_4.bmp", R, R, R);
	//dilation(R, width, height);
	//save_bmp((char*)"out\\noise_remov_morph_gray_new.bmp", R, R, R);
	


	printf("Job Finished!\n");
	close_bmp();
	
	system("PAUSE");
	return 0;
}

