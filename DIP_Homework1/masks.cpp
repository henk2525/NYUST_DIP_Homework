#include "bmp.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "FFT1.c"
#include "FFT2.c"

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

void MedianFiltering(int r[1024][1024], int R[1024][1024], int& width, int& height)
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
	for(int x=0;x < width;x++)
		for (int y = 0;y < height;y++) {
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

	for (int x = 0;x<width;x++)
		for (int y = 0;y < height;y++) {
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

void Hough_Line_Transform(int r[1024][1024], int R[1024][1024], int& width, int& height) {

	int Accumulator[800][181] = { 0 };
	int s,i,j;
	int angle;

	double angle_new;
	for (i = 2; i < width - 2; i++)
		for (j = 2; j < height- 2; j++) {
			if (R[i][j] == 255) {
				for (angle = -90; angle < 91; angle++) {
					angle_new = angle * M_PI / 180;

					s = round(i * cos(angle_new) + j * sin(angle_new));
					Accumulator[s][angle+90]++;
				}
			}
			else
				continue;
		}


	for (s = 0; s < 800; s++) {
		for (angle = -90; angle < 91; angle++) {
			if (Accumulator[s][angle+90] > 80) {
//				printf("%d %d %d\n",Accumulator[s][angle], angle, s);
				angle_new = angle * M_PI / 180;
				for (i = 2; i < width-2; i++) {
					for (j = 2; j < height-2; j++)
						if ( (s == round(i * cos(angle_new) + j * sin(angle_new))) && R[i][j]==255)
							r[i][j] = 255;
				}
			}
			else
				continue;
		}
	}
}


void HistogramEqualization(int r[1024][1024], int R[1024][1024], int& width, int& height) 
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

void ErrorDiffusion(int r[1024][1024], int R[1024][1024], int& width, int& height)
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

void rgb2hsv(int H[1024][1024], float S[1024][1024], float V[1024][1024],
	int R[1024][1024], int G[1024][1024], int B[1024][1024], int& width, int& height) 
{
	float temp, delta; 
	float max, min;
	float r, g, b;
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++) {

			//normalize
			r = (float)R[x][y]/255;
			g = (float)G[x][y]/255;
			b = (float)B[x][y]/255;

			temp = (r >= g) ? r : g;
			max = (temp >= b) ? temp : b;  //max
			temp = (r <= g) ? r : g;
			min = (temp <= b) ? temp : b;  //min
			
			V[x][y] = max;
			delta = max - min;
			if (max != 0)
				S[x][y] = delta / max;
			else
				S[x][y] = 0;

			if (max == min)
				H[x][y] = 0;
			else if (r == max && g >= b)
				H[x][y] = round((g - b) / delta * 60);
			else if (r == max && g <= b)
				H[x][y] = round((g - b) / delta * 60) + 360;
			else if (g == max)
				H[x][y] = round((b - r) / delta * 60) + 120;
			else if (b == max)
				H[x][y] = round((r - g) / delta * 60) + 240;
		}
}

void Butterworth_LF(int r[1024][1024], int R[1024][1024],float D0,int n, int width, int height) {
	int i, j, switch_temp;
	int real[1024][1024] = { 0 };
	int imaginary[1024][1024] = { 0 };
	float D;
	float Data[256 * 256 * 2] = { 0 };
	float Mask[1024][1024] = { 0 };


	//Create Mask
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			D = hypot(abs(i - 128), abs(j - 128));
			Mask[i][j] = 1 / (1 + powf((D / D0), 2*n));
		}


	//2D to 1D
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			Data[j * 256 * 2 + i * 2] = (float)R[i][j];
			Data[j * 256 * 2 + i * 2 + 1] = 0;
		}
	fft2(Data, 256, 1);
	//1-D to 2-D
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			real[i][j] = (int)Data[j * 256 * 2 + i * 2];
			imaginary[i][j] = (int)Data[j * 256 * 2 + i * 2 + 1];
		}

	//switch
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			if (i < width / 2 && j < height / 2) {
				switch_temp = real[i][j];
				real[i][j] = real[i + width / 2][j + height / 2];
				real[i + width / 2][j + height / 2] = switch_temp;
			}
			else if ((i >= width / 2 && j < height / 2)) {
				switch_temp = real[i][j];
				real[i][j] = real[i - width / 2][j + height / 2];
				real[i - width / 2][j + height / 2] = switch_temp;
			}
		}
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			if (i < width / 2 && j < height / 2) {
				switch_temp = imaginary[i][j];
				imaginary[i][j] = imaginary[i + width / 2][j + height / 2];
				imaginary[i + width / 2][j + height / 2] = switch_temp;
			}
			else if ((i >= width / 2 && j < height / 2)) {
				switch_temp = imaginary[i][j];
				imaginary[i][j] = imaginary[i - width / 2][j + height / 2];
				imaginary[i - width / 2][j + height / 2] = switch_temp;
			}
		}


	//multiply
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			real[i][j] = (float)real[i][j] * Mask[i][j];
			imaginary[i][j] = (float)imaginary[i][j] * Mask[i][j];
		}

	//2D to 1D
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			Data[j * 256 * 2 + i * 2] = (float)real[i][j];
			Data[j * 256 * 2 + i * 2 + 1] = (float)imaginary[i][j];
		}
	//fft inverse
	fft2(Data, 256, -1);
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			r[i][j] = abs((int)Data[j * 256 * 2 + i * 2]);
}
void Butterworth_HF(int r[1024][1024], int R[1024][1024], float D0, int n, int width, int height) {
	int i, j, switch_temp;
	int real[1024][1024] = { 0 };
	int imaginary[1024][1024] = { 0 };
	float D;
	float Data[256 * 256 * 2] = { 0 };
	float Mask[1024][1024] = { 0 };


	//Create Mask
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			D = hypot(abs(i - 128), abs(j - 128));
			Mask[i][j] = 1 / (1 + powf((D0 / D), 2 * n));
		}


	//2D to 1D
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			Data[j * 256 * 2 + i * 2] = (float)R[i][j];
			Data[j * 256 * 2 + i * 2 + 1] = 0;
		}
	fft2(Data, 256, 1);
	//1-D to 2-D
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			real[i][j] = (int)Data[j * 256 * 2 + i * 2];
			imaginary[i][j] = (int)Data[j * 256 * 2 + i * 2 + 1];
		}

	//switch
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			if (i < width / 2 && j < height / 2) {
				switch_temp = real[i][j];
				real[i][j] = real[i + width / 2][j + height / 2];
				real[i + width / 2][j + height / 2] = switch_temp;
			}
			else if ((i >= width / 2 && j < height / 2)) {
				switch_temp = real[i][j];
				real[i][j] = real[i - width / 2][j + height / 2];
				real[i - width / 2][j + height / 2] = switch_temp;
			}
		}
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			if (i < width / 2 && j < height / 2) {
				switch_temp = imaginary[i][j];
				imaginary[i][j] = imaginary[i + width / 2][j + height / 2];
				imaginary[i + width / 2][j + height / 2] = switch_temp;
			}
			else if ((i >= width / 2 && j < height / 2)) {
				switch_temp = imaginary[i][j];
				imaginary[i][j] = imaginary[i - width / 2][j + height / 2];
				imaginary[i - width / 2][j + height / 2] = switch_temp;
			}
		}


	//multiply
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			real[i][j] = (float)real[i][j] * Mask[i][j];
			imaginary[i][j] = (float)imaginary[i][j] * Mask[i][j];
		}

	//2D to 1D
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++) {
			Data[j * 256 * 2 + i * 2] = (float)real[i][j];
			Data[j * 256 * 2 + i * 2 + 1] = (float)imaginary[i][j];
		}
	//fft inverse
	fft2(Data, 256, -1);
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			r[i][j] = abs((int)Data[j * 256 * 2 + i * 2]);
}

void dilation(int R[1024][1024], int width, int height) {
	int temp[1024][1024] = { 0 };

	for (int x = 1;x < width - 1;x++)
		for (int y = 1; y < height - 1;y++) {
			if (R[x][y] == 255){
				temp[x - 1][y] = 255;
				temp[x][y] = 255;
				temp[x + 1][y] = 255;
				temp[x][y - 1] = 255;
				temp[x][y + 1] = 255;
			}	
		}
	for (int x = 0;x < width;x++)
		for (int y = 0; y < height;y++)
			R[x][y] = temp[x][y];
}



void erosion(int R[1024][1024], int width, int height) {
	int temp[1024][1024] = { 0 };
	for (int x = 1;x < width - 1;x++)
		for (int y = 1; y < height - 1;y++)
			if (R[x - 1][y] == 255 && R[x][y] == 255 && R[x + 1][y] == 255 && R[x][y - 1] == 255 && R[x][y + 1] == 255)
				temp[x][y] = 255;

	for (int x = 0;x < width;x++)
		for (int y = 0; y < height;y++)
			R[x][y] = temp[x][y];
}

