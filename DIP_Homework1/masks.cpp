#include "bmp.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846264338327
#define MAX_BRIGHTNESS 255


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
					r[i][j] +=(int) R[i - k_i][j - k_j] * kernel[count];
					count++;
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

void Rotate(int r[1024][1024], int R[1024][1024], int& width, int& height, float angle) {

	int i, j;
	double x, y;			
	int p1, p2;
	int half_width = width / 2;
	int half_height = height/ 2;

	int after_i, after_j;
	for (i = 0; i <= width; i++)
		for (j = 0; j <= height; j++) {

			after_i = i - half_width;
			after_j = j - half_height;
			x = after_i * cos(angle*M_PI / 180) + after_j * sin(angle*M_PI / 180) + half_width;
			y = after_i * sin(angle*M_PI / 180)*(-1) + after_j * cos(angle*M_PI / 180) + half_height;


			if (x>0 && x<width && y>0 && y<height) {		//判斷是否對應到之原圖有像素點

				p1 = R[int(x)][int(y)] * abs((int) y - int(y + 1)) +
					R[int(x)][int(y + 1)] * abs((int) y - int(y));

				p2 = R[int(x + 1)][int(y)] * abs((int) y - int(y + 1)) +
					R[int(x + 1)][int(y + 1)] * abs((int) y - int(y));

				r[i][j] = p1 * abs((int) x - int(x + 1)) + p2 * abs((int) x - int(x));
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

void canny_edge_detection(int r[1024][1024], int R[1024][1024], int& width, int& height, int tmin, int tmax, float sigma) {

	int after_x[1024][1024] = { 0 }, after_y[1024][1024] = { 0 };
	int nms[1024][1024] = { 0 };


	Gaussian_LPF(r, R, width, height, sigma);
	save_bmp((char*)"picture\\house_step1.bmp", r, r, r);

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
	save_bmp((char*)"picture\\house_step2.bmp", r, r, r);


	// Non-maximum suppression
	for(int x=1;x < width-1;x++)
		for (int y = 1;y < height-1;y++) {
			float dir = (float)(fmod(atan2(after_y[x][y], after_x[x][y]) + M_PI,M_PI) / M_PI) * 8;	//atan2() return -pi~pi ,so add pi
			if (((dir <= 1 || dir > 7) && r[x][y] > r[x][y + 1] && r[x][y] > r[x][y - 1]) ||			// 0 deg
				((dir > 1 && dir <= 3) && r[x][y] > r[x - 1][y + 1] && r[x][y] > r[x + 1][y - 1]) ||	// 45 deg		nw	nn  ne
				((dir > 3 && dir <= 5) && r[x][y] > r[x + 1][y] && r[x][y] > r[x - 1][y]) ||			// 90 deg		ww	**	ee
				((dir > 5 && dir <= 7) && r[x][y] > r[x + 1][y + 1] && r[x][y] > r[x - 1][y - 1]))		// 135 deg		sw	sa	se
				nms[x][y] = r[x][y];
			else
				nms[x][y] = 0;
		}
	save_bmp((char*)"picture\\house_step3.bmp", nms, nms, nms);

	//歸零
	for (int x = 0;x < width;x++)
		for (int y = 0;y < height;y++)
			r[x][y] = 0;


	int edges[1024 * 1024] = { 0 };
	int c = 1;

	for (int x = 1;x<width-1;x++)
		for (int y = 1;y < height-1;y++) {
			if (nms[x][y] >= tmax && r[x][y] == 0) { // trace edges
				r[x][y] = MAX_BRIGHTNESS;
				int nedges = 1;
				edges[0] = c;

				do {
					nedges--;
					int t = edges[nedges];

					int nbs[8];  //// neighbours
					nbs[0] = t - width;  // nn
					nbs[1] = t + width;  // ss
					nbs[2] = t + 1;      // ww
					nbs[3] = t - 1;      // ee
					nbs[4] = nbs[0] + 1; // nw
					nbs[5] = nbs[0] - 1; // ne
					nbs[6] = nbs[1] + 1; // sw
					nbs[7] = nbs[1] - 1; // se

					for (int i = -1; i < 2; i++)
						for (int j = -1; j < 2; j++)
							if (i == 0 && j == 0)
								continue;
							else
								if (nms[x + i][y + j] >= tmin && r[x + i][y + j] == 0) {
									r[x + i][y + j] = MAX_BRIGHTNESS;
									edges[nedges] = nbs[3*(i+1)+(j+1)];
									nedges++;
								}
				} while (nedges > 0);
			}
			c++;
		}

	save_bmp((char*)"picture\\house_step4.bmp", r, r, r);
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
