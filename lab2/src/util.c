#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#include "util.h"

int imnorm(float *img, int width, int height) {

	int k;
	float max = 0;
	for (k=0; k<width*height; k++) {
		if (max < img[k]) max = img[k];
	}
	for (k=0; k<width*height; k++) {
		img[k] = img[k]/max;
	}
	return 0;
}

int im2byte(unsigned char *dst, float *src, int width, int height) {

	imnorm(src, width, height);
	int k;
	for (k=0; k<width*height; k++) {
		dst[k] = (unsigned char)round(256*fabs(src[k]));
	}
	return 0;
}

int imread(float **img, int *width, int *height, char *fname) {

	FILE* fin = fopen(fname, "rb");
	if (!fin) {
		printf("cannot open input file: %s\n", fname);
		return -1;
	}

	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	WORD fileType;
	int i;
	int j;

	fread(&fileType, 1, sizeof(WORD), fin);
	if (fileType != 0x4d42) {
		printf("%s is not a bmp file\n", fname);
		return 1;
	}

	fread(&bitHead, 1, sizeof(BITMAPFILEHEADER), fin);
	fread(&bitInfoHead, 1, sizeof(BITMAPINFOHEADER), fin);
	fseek(fin, 4*bitInfoHead.biClrUsed, SEEK_CUR);

	*width	= bitInfoHead.biWidth;
	*height = bitInfoHead.biHeight;

	int w = bitInfoHead.biWidth;
	int h = bitInfoHead.biHeight; 

	float* limg = (float*) calloc(w*h, sizeof(float));

	if (bitInfoHead.biBitCount == 8) {
		for (i=h-1; i>=0; i--) {
			for (j=0; j<w; j++) {
				unsigned char imbyte = 0;
				fread(&imbyte, 1, sizeof(char), fin);
				limg[j*h+i] = 1.0*imbyte;
			}
			if (w%4 != 0) {
				int p;
				for (p=0; p<4-(w%4); p++) {
					unsigned char k = 0;
					fread(&k, 1, sizeof(char), fin);
				}
			}
		}	
	}
	else if (bitInfoHead.biBitCount == 24) {
		for (i=h; i>=0; i--) {
			for (j=0; j<w; j++) {
				unsigned char r = 0;
				unsigned char g = 0;
				unsigned char b = 0;

				fread(&r, 1, sizeof(char), fin);
				fread(&g, 1, sizeof(char), fin);
				fread(&b, 1, sizeof(char), fin);
				limg[j*h+i] = (1.0*r+1.0*g+1.0*b)/3.0;
				fread(&r, 1, sizeof(char), fin);
			}
			if (w%4 != 0) {
				int p;
				for (p=0; p<4-(w%4); p++) {
					unsigned int k = 0;
					fread(&k, 1, sizeof(unsigned int), fin);
				}
			}
		}
	}
	imnorm(limg, w, h);

	*img = limg;

	fclose(fin);
	return 0;
}
int imwrite(float *img, int width, int height, char *fname) {

	unsigned char* imgbyte = (unsigned char*) calloc(height*width, sizeof(unsigned char));
	im2byte(imgbyte, img, width, height);

	FILE* fout = fopen(fname, "wb");
	if (!fout) {
		printf("cannot open output file: %s\n", fname);
		return -1;
	}

	BITMAPFILEHEADER bitHead;
	BITMAPINFOHEADER bitInfoHead;
	WORD fileType;
	int i;
	int j;

	fileType			= 0x4d42;
	bitHead.bfSize		= width*height+256*4+54;
	if (width % 4 != 0) bitHead.bfSize += (4 - width % 4)*height;
	bitHead.bfReserved1	= 0;
	bitHead.bfReserved2	= 0;
	bitHead.bfOffBits	= 0x436;

	bitInfoHead.biSize			= 0x28;
	bitInfoHead.biWidth			= width;
	bitInfoHead.biHeight		= height;
	bitInfoHead.biPlanes		= 1;
	bitInfoHead.biBitCount		= 8;
	bitInfoHead.biCompression	= 0;
	bitInfoHead.biSizeImage		= 0;
	bitInfoHead.biXPelPerMeter	= 0;
	bitInfoHead.biYPelPerMeter 	= 0;
	bitInfoHead.biClrUsed		= 0x100;
	bitInfoHead.biClrImportant	= 0;

	fwrite(&fileType, 1, sizeof(WORD), fout);
	fwrite(&bitHead.bfSize, 1, sizeof(int), fout);
	fwrite(&bitHead.bfReserved1, 1, sizeof(WORD), fout);
	fwrite(&bitHead.bfReserved2, 1, sizeof(WORD), fout);
	fwrite(&bitHead.bfOffBits, 1, sizeof(int), fout);

	fwrite(&bitInfoHead.biSize, 1, sizeof(int), fout);
	fwrite(&bitInfoHead.biWidth, 1, sizeof(int), fout);
	fwrite(&bitInfoHead.biHeight, 1, sizeof(int), fout);
	fwrite(&bitInfoHead.biPlanes, 1, sizeof(WORD), fout);
	fwrite(&bitInfoHead.biBitCount, 1, sizeof(WORD), fout);
	fwrite(&bitInfoHead.biCompression, 1, sizeof(int), fout);
	fwrite(&bitInfoHead.biSizeImage, 1, sizeof(int), fout);
	fwrite(&bitInfoHead.biXPelPerMeter, 1, sizeof(int), fout);
	fwrite(&bitInfoHead.biYPelPerMeter, 1, sizeof(int), fout);
	fwrite(&bitInfoHead.biClrUsed, 1, sizeof(int), fout);
	fwrite(&bitInfoHead.biClrImportant, 1, sizeof(int), fout);

	//write color map
	for (i=0; i<256; i++) {
		unsigned char k = i;
		fwrite(&k, 1, sizeof(char), fout);
		fwrite(&k, 1, sizeof(char), fout);
		fwrite(&k, 1, sizeof(char), fout);
		k = 0;
		fwrite(&k, 1, sizeof(char), fout);
	}

	for (i=height-1; i>=0; i--) {
		for (j=0; j<width; j++) {
			fwrite(&imgbyte[j*height+i], 1, sizeof(char), fout);
		}
		if (width%4 != 0) {
			int p;
			for (p=0; p<4-(width%4); p++) {
				unsigned char k = 0;
				fwrite(&k, 1, sizeof(char), fout);
			}
		}
	}

	fclose(fout);
	free(imgbyte);
	return 0;
}
