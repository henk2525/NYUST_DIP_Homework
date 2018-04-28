void MedianFiltering(int r[1024][1024], int R[1024][1024], int& width, int& height);

void Gaussian_LPF(int r[1024][1024], int R[1024][1024], int & width, int & height, float sigma);

void canny_edge_detection(int r[1024][1024], int R[1024][1024], int& width, int& height, int tmin, int tmax, float sigma);

void HistogramEqualization(int r[1024][1024], int R[1024][1024], int& width, int& height);

void ErrorDiffusion(int r[1024][1024], int R[1024][1024], int& width, int& height);
