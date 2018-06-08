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
	//save_bmp((char*)"out\\skin_det_1_new.bmp", r, g, b);



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
	

	//第三題
	open_bmp((char*)"picture\\skin_det_2.bmp", R, R, R, width, height);
	int str[25];
	for (int i = 0;i < 25;i++)
		str[i] = i;
	erosion(r,R,str,width,height,5,5);




	printf("Job Finished!\n");
	close_bmp();
	
	system("PAUSE");
	return 0;
}

