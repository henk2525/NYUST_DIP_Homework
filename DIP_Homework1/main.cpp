#include <iostream>
#include <stdlib.h>
#include "bmp.h"
#include "masks.h"
using namespace std;

int R[MaxBMPSizeX][MaxBMPSizeY];
int r[MaxBMPSizeX][MaxBMPSizeY];


int main(int argc, char *argv[])
{
	int width, height;

	open_bmp((char*)"picture\\house.bmp", R, R, R, width, height);

	canny_edge_detection(r, R, width, height, 50, 80, 1);

	//save_bmp((char*)"picture\\house_new.bmp", r, r, r);
	printf("Job Finished!\n");
	close_bmp();
	
	system("PAUSE");
	return 0;
}

