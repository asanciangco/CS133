#ifndef UTIL_H
#define UTIL_H

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned int LONG;

typedef struct tagBITMAPFILEHEADER{

	DWORD   bfSize;
	WORD    bfReserved1;
	WORD    bfReserved2;
	DWORD   bfOffBits;
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER{

	DWORD   biSize;
	LONG    biWidth;
	LONG    biHeight;
	WORD    biPlanes;
	WORD    biBitCount;
	DWORD   biCompression;
	DWORD   biSizeImage;
	LONG    biXPelPerMeter;
	LONG    biYPelPerMeter;
	DWORD   biClrUsed;
	DWORD   biClrImportant;
}BITMAPINFOHEADER;

typedef struct tagRGBQUAD{

	BYTE    rgbBlue;
	BYTE    rgbGreen;
	BYTE    rgbRed;
	BYTE    rgbReserved;
}RGBQUAD;

int imnorm(float *img, int width, int height);
int im2byte(unsigned char *dst, float *src, int width, int height);
int imread(float **img, int *width, int *height, char *fname);
int imwrite(float *img, int width, int height, char *fname);

#endif
