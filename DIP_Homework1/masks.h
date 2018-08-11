void MedianFiltering(int r[1024][1024], int R[1024][1024], int& width, int& height);

void Gaussian_LPF(int r[1024][1024], int R[1024][1024], int & width, int & height, float sigma);

void Rotate(int r[1024][1024], int R[1024][1024], int& width, int& height, float angle);

void canny_edge_detection(int r[1024][1024], int R[1024][1024], int& width, int& height, int tmin, int tmax, float sigma);

void Hough_Line_Transform(int r[1024][1024], int R[1024][1024], int& width, int& height);

void HistogramEqualization(int r[1024][1024], int R[1024][1024], int& width, int& height);

void ErrorDiffusion(int r[1024][1024], int R[1024][1024], int& width, int& height);

void rgb2hsv(int H[1024][1024], float S[1024][1024], float V[1024][1024],
	int R[1024][1024], int G[1024][1024], int B[1024][1024], int& width, int& height);

void Butterworth_LF(int r[1024][1024], int R[1024][1024], float D0, int n, int width, int height);

void Butterworth_HF(int r[1024][1024], int R[1024][1024], float D0, int n, int width, int height);

void dilation(int R[1024][1024], int width, int height); //ÂX±i

void erosion(int R[1024][1024], int width, int height);  //«I»k