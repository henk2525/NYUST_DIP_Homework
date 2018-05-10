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


int main(int argc, char *argv[])
{
	int width, height;



	int angle = 225;
	open_bmp((char*)"picture\\framed_lena_color_256.bmp", R, G, B, width, height);
	Rotate(r, R, width, height, angle);
	Rotate(g, G, width, height, angle);
	Rotate(b, B, width, height, angle);
	save_bmp((char*)"picture\\framed_lena_color_256_new.bmp", r, g, b);



	/*
	open_bmp((char*)"picture\\house.bmp", R, R, R, width, height);
	canny_edge_detection(r, R, width, height, 50, 80, 1);
	*/


	//save_bmp((char*)"picture\\house_new.bmp", r, r, r);
	printf("Job Finished!\n");
	close_bmp();
	
	system("PAUSE");
	return 0;
}

