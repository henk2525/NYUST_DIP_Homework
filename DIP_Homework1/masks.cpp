#include "bmp.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846264338327

int FindMedium(int data[9])   //找中間值
{
	int i, j;
	int temp;
	for (i = 0;i < 10;i++) {       //氣泡排序法
		for (j = 0;j < 10;j++) {
			if (data[j] < data[i]) {
				temp = data[j];
				data[j] = data[i];
				data[i] = temp;
			}
		}
	}
	return data[4];
}

void convolution(int r[1024][1024], int R[1024][1024], float kernel[9], int& width, int& height){
	for (int i = 1; i < width - 1;i++)
		for (int j = 1;j < height - 1;j++) {
			int count = 0;
			for(int k_i = -1; k_i<2; k_i++)
				for (int k_j = -1; k_j < 2; k_j++) {
					r[i][j] += R[i - k_i][j - k_j] * kernel[count];
					count++;
				}
		}
}

void Gaussian_LPF(int r[1024][1024], int R[1024][1024], int& width, int& height, float sigma) {

	int i, j;
	float Gaussian_kernel[9],total=0;
	int count=0;
	//高斯矩陣
	for (i = 0; i < 3;i++) {
		for (j = 0;j < 3;j++) {
			Gaussian_kernel[count] = exp((-pow(abs(i - 1), 2) - pow(abs(j - 1), 2)) / (2 * sigma * sigma));
			total += Gaussian_kernel[count];
			count++;
		}
	}
	for (count = 0; count < 9;count++) {
			Gaussian_kernel[count] /= total;
	}
	convolution(r, R, Gaussian_kernel, width, height);
}

void canny_edge_detection(int r[1024][1024], int R[1024][1024], int& width, int& height,int tmin, int tmax, float sigma) {

	int after_x[1024][1024], after_y[1024][1024];


	Gaussian_LPF(r, R, width, height, sigma);

	float Gx[] = { -1, 0, 1,
				   -2, 0, 2,
			       -1, 0, 1 };
	convolution(after_x, r, Gx, width, height);

	float Gy[] = { 1, 2, 1,
				   0, 0, 0,
				  -1,-2,-1 };
	convolution(after_y, r, Gy, width, height);

	for(int x=1;x<width;x++)
		for (int y = 1;y < height;y++)
			r[x][y] = hypot(after_x[x][y], after_y[x][y]);




	
}

void HistogramEqualization(int r[1024][1024], int R[1024][1024], int& width, int& height)  //第三題
{
	int count[256][2] = { 0 };
	int i, j;
	for (i = 0;i < 256;i++)
		count[i][0] = i;

	//計數
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			count[R[i][j]][1]++;
	//	for (i = 0; i < 256; i++)
	//		printf("%d : %d\n", count[i][0], count[i][1]);

	//累加計數
	int count_temp = 0;
	int count_total[256][2] = { 0 };
	for (i = 0; i < 256; i++) {
		count_temp += count[i][1];
		count_total[i][0] = i;
		count_total[i][1] = count_temp;
	}
	//	for(i = 0; i < 256; i++)
	//		printf("%d : %d\n", count_total[i][0], count_total[i][1]);

	//找最小累加數
	int min;
	for (i = 0; i < 256;i++) {
		if (count_total[i][1] == 0)
			continue;
		else {
			min = count_total[i][1];
			break;
		}
	}
	//printf("%d\n", min);

	//計算
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			r[i][j] = (int)round((float)(count_total[R[i][j]][1] - min) / (width * height - min) * 255);
}

void ErrorDiffusion(int r[1024][1024], int R[1024][1024], int& width, int& height)  //第一題
{
	int i, j, error;
	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			r[i][j] = (R[i][j] >= 128) ? 255 : 0;   //先做二值化

			error = R[i][j] - r[i][j];  //計算誤差值

			R[i + 1][j] = R[i + 1][j] + (int)round(7 * error / 16);
			if (i == 0)
				continue;
			else
				R[i - 1][j + 1] = R[i - 1][j + 1] + (int)round(3 * error / 16);
			R[i][j + 1] = R[i][j + 1] + (int)round(5 * error / 16);
			R[i + 1][j + 1] = R[i + 1][j + 1] + (int)round(1 * error / 16);
		}
	}

}

void MedianFiltering(int r[1024][1024], int R[1024][1024], int& width, int& height)  //第二題
{
	int data[9];
	int x, y, i, j;

	for (i = 0; i < width; i++)
	{
		for (j = 0; j < height; j++)
		{
			for (x = -1;x < 2;x++)
				for (y = -1;y < 2;y++)
					data[3 * (x + 1) + (y + 1)] = R[i + x][j + y];
			r[i][j] = FindMedium(data);
		}
	}
}

