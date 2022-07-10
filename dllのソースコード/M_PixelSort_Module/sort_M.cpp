#include "sort_M.h"
#include <math.h>
#include <stdlib.h>
#include<omp.h>

void sort_M::Mbreak(Pixel_RGBA* a, int w, int h) {
#pragma omp parallel for
	for (int i = 0; i < w * h; i++) {
		a[i].r = a[i].r + 300;
		a[i].g = a[i].g + 300;
		a[i].b = a[i].b + 300;
		a[i].a = a[i].a + 300;
	}
}

void sort_M::rsize(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh) {
	int index = 0;
	float haba = (float)bw / w;
#pragma omp parallel for
	for (int y = 0; y < bh; y++) {
		index = bw * y;
		for (int x = 0; x < bw; x++) {

			double zx = (x - 0.5) / haba;//-0.5しなければしなくてもおｋ
			double zy = (y - 0.5) / haba;//

			int ix = (int)zx;
			int iy = (int)zy;

			float r[4] = { 0 };
			float g[4] = { 0 };
			float b[4] = { 0 };
			float a[4] = { 0 };

			if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
				r[0] = data[ix + w * iy].r;
				g[0] = data[ix + w * iy].g;
				b[0] = data[ix + w * iy].b;
				a[0] = data[ix + w * iy].a;
			}
			else {
				r[0] = 0;
				g[0] = 0;
				b[0] = 0;
				a[0] = 0;
			}

			if (ix + 1 >= 0 && ix + 1 < w && iy >= 0 && iy < h) {
				r[1] = data[ix + 1 + w * iy].r;
				g[1] = data[ix + 1 + w * iy].g;
				b[1] = data[ix + 1 + w * iy].b;
				a[1] = data[ix + 1 + w * iy].a;
			}
			else {
				r[1] = 0;
				g[1] = 0;
				b[1] = 0;
				a[1] = 0;
			}

			if (ix >= 0 && ix < w && iy + 1 >= 0 && iy + 1 < h) {
				r[2] = data[ix + w * iy + w].r;
				g[2] = data[ix + w * iy + w].g;
				b[2] = data[ix + w * iy + w].b;
				a[2] = data[ix + w * iy + w].a;
			}
			else {
				r[2] = 0;
				g[2] = 0;
				b[2] = 0;
				a[2] = 0;
			}

			if (ix + 1 >= 0 && ix + 1 < w && iy + 1 >= 0 && iy + 1 < h) {
				r[3] = data[ix + 1 + w * iy + w].r;
				g[3] = data[ix + 1 + w * iy + w].g;
				b[3] = data[ix + 1 + w * iy + w].b;
				a[3] = data[ix + 1 + w * iy + w].a;
			}
			else {
				r[3] = 0;
				g[3] = 0;
				b[3] = 0;
				a[3] = 0;
			}

			zx = (zx - ix) / 1;
			zy = (zy - iy) / 1;

			r[0] = r[0] + (r[1] - r[0]) * zx;
			r[1] = r[2] + (r[3] - r[2]) * zx;
			r[0] = r[0] + (r[1] - r[0]) * zy;

			g[0] = g[0] + (g[1] - g[0]) * zx;
			g[1] = g[2] + (g[3] - g[2]) * zx;
			g[0] = g[0] + (g[1] - g[0]) * zy;

			b[0] = b[0] + (b[1] - b[0]) * zx;
			b[1] = b[2] + (b[3] - b[2]) * zx;
			b[0] = b[0] + (b[1] - b[0]) * zy;

			a[0] = a[0] + (a[1] - a[0]) * zx;
			a[1] = a[2] + (a[3] - a[2]) * zx;
			a[0] = a[0] + (a[1] - a[0]) * zy;

			if (r[0] < 0) r[0] = 0; else if (r[0] > 255) r[0] = 255;
			if (g[0] < 0) g[0] = 0; else if (g[0] > 255) g[0] = 255;
			if (b[0] < 0) b[0] = 0; else if (b[0] > 255) b[0] = 255;
			if (a[0] < 0) a[0] = 0; else if (a[0] > 255) a[0] = 255;

			work[index].r = (int)r[0];
			work[index].g = (int)g[0];
			work[index].b = (int)b[0];
			work[index].a = (int)a[0];

			index++;
		}
	}
}

void sort_M::SimpleRotation(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh, double si, Mch* mch, double TX, double TY) {
	double sin_S = sin(si);
	double cos_S = cos(si);

	int index = 0;
	float Pw = bw / (float)2 + TX;
	float Ph = bh / (float)2 + TY;
	float Qw = w / (float)2;
	float Qh = h / (float)2;

#pragma omp parallel for
	for (int y = 0; y < bh; y++) {
		index = bw * y - 1;
		for (int x = 0; x < bw; x++) {
			index++;

			double zx = ((float)x - Pw) * cos_S + ((float)y - Ph) * sin_S + Qw;
			double zy = -((float)x - Pw) * sin_S + ((float)y - Ph) * cos_S + Qh;

			int ix = (int)zx;
			int iy = (int)zy;

			if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
				ix = ix + w * iy;
				work[index].r = data[ix].r;
				work[index].g = data[ix].g;
				work[index].b = data[ix].b;
				if (data[ix].a == 0) {
					work[index].a = 0;
					mch[index].a = false;
				}
				else {
					work[index].a = 255;
					mch[index].a = true;
				}
			}
			else {
				work[index].r = 0;
				work[index].g = 0;
				work[index].b = 0;
				work[index].a = 0;
				mch[index].a = false;
			}
		}
	}
}

void sort_M::rotation(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh, double si, Mch* mch, double TX, double TY) {
	double sin_S = sin(si);
	double cos_S = cos(si);

	int index = 0;
	float Pw = bw / (float)2 + TX;
	float Ph = bh / (float)2 + TY;
	float Qw = w / (float)2;
	float Qh = h / (float)2;

#pragma omp parallel for
	for (int y = 0; y < bh; y++) {
		index = bw * y - 1;
		for (int x = 0; x < bw; x++) {
			index++;

			double zx = ((float)x - Pw) * cos_S + ((float)y - Ph) * sin_S + Qw;
			double zy = -((float)x - Pw) * sin_S + ((float)y - Ph) * cos_S + Qh;

			int ix = (int)zx;
			int iy = (int)zy;

			float r[4] = { 0 };
			float g[4] = { 0 };
			float b[4] = { 0 };
			float a[4] = { 0 };

			if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
				r[0] = data[ix + w * iy].r;
				g[0] = data[ix + w * iy].g;
				b[0] = data[ix + w * iy].b;
				a[0] = data[ix + w * iy].a;
			}
			else {
				r[0] = 0;
				g[0] = 0;
				b[0] = 0;
				a[0] = 0;
			}

			if (ix + 1 >= 0 && ix + 1 < w && iy >= 0 && iy < h) {
				r[1] = data[ix + 1 + w * iy].r;
				g[1] = data[ix + 1 + w * iy].g;
				b[1] = data[ix + 1 + w * iy].b;
				a[1] = data[ix + 1 + w * iy].a;
			}
			else {
				r[1] = 0;
				g[1] = 0;
				b[1] = 0;
				a[1] = 0;
			}

			if (ix >= 0 && ix < w && iy + 1 >= 0 && iy + 1 < h) {
				r[2] = data[ix + w * iy + w].r;
				g[2] = data[ix + w * iy + w].g;
				b[2] = data[ix + w * iy + w].b;
				a[2] = data[ix + w * iy + w].a;
			}
			else {
				r[2] = 0;
				g[2] = 0;
				b[2] = 0;
				a[2] = 0;
			}

			if (ix + 1 >= 0 && ix + 1 < w && iy + 1 >= 0 && iy + 1 < h) {
				r[3] = data[ix + 1 + w * iy + w].r;
				g[3] = data[ix + 1 + w * iy + w].g;
				b[3] = data[ix + 1 + w * iy + w].b;
				a[3] = data[ix + 1 + w * iy + w].a;
			}
			else {
				r[3] = 0;
				g[3] = 0;
				b[3] = 0;
				a[3] = 0;
			}

			zx = (zx - ix) / 1;
			zy = (zy - iy) / 1;

			r[0] = r[0] + (r[1] - r[0]) * zx;
			r[1] = r[2] + (r[3] - r[2]) * zx;
			r[0] = r[0] + (r[1] - r[0]) * zy;

			g[0] = g[0] + (g[1] - g[0]) * zx;
			g[1] = g[2] + (g[3] - g[2]) * zx;
			g[0] = g[0] + (g[1] - g[0]) * zy;

			b[0] = b[0] + (b[1] - b[0]) * zx;
			b[1] = b[2] + (b[3] - b[2]) * zx;
			b[0] = b[0] + (b[1] - b[0]) * zy;

			a[0] = a[0] + (a[1] - a[0]) * zx;
			a[1] = a[2] + (a[3] - a[2]) * zx;
			a[0] = a[0] + (a[1] - a[0]) * zy;

			if (r[0] < 0) r[0] = 0; else if (r[0] > 255) r[0] = 255;
			if (g[0] < 0) g[0] = 0; else if (g[0] > 255) g[0] = 255;
			if (b[0] < 0) b[0] = 0; else if (b[0] > 255) b[0] = 255;
			if (a[0] < 0) a[0] = 0; else if (a[0] > 255) a[0] = 255;

			work[index].r = (int)r[0];
			work[index].g = (int)g[0];
			work[index].b = (int)b[0];
			work[index].a = (int)a[0];
		}
	}
}

void sort_M::arotation(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh, double si, Mch* mch) {
	double sin_S = sin(si);
	double cos_S = cos(si);

	int index = 0;
	float Pw = bw / (float)2;
	float Ph = bh / (float)2;
	float Qw = w / (float)2;
	float Qh = h / (float)2;

#pragma omp parallel for
	for (int y = 0; y < bh; y++) {
		index = bw * y - 1;
		for (int x = 0; x < bw; x++) {
			index++;

			double zx = ((float)x - Pw) * cos_S + ((float)y - Ph) * sin_S + Qw;
			double zy = -((float)x - Pw) * sin_S + ((float)y - Ph) * cos_S + Qh;

			if (zx < w - 1 || zy < h - 1) {
				int ix = (int)zx;
				int iy = (int)zy;

				if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
					ix = ix + w * iy;
					work[index].r = data[ix].r;
					work[index].g = data[ix].g;
					work[index].b = data[ix].b;
					if (data[ix].a == 0) {
						work[index].a = 0;
						mch[index].a = false;
					}
					else {
						work[index].a = 255;
						mch[index].a = true;
					}
				}
				else mch[index].a = false;
			}
			else {
				int ix = (int)zx;
				int iy = (int)zy;

				float r[4] = { 0 };
				float g[4] = { 0 };
				float b[4] = { 0 };
				float a[4] = { 0 };

				if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
					r[0] = data[ix + w * iy].r;
					g[0] = data[ix + w * iy].g;
					b[0] = data[ix + w * iy].b;
					a[0] = data[ix + w * iy].a;
				}
				else {
					r[0] = 0;
					g[0] = 0;
					b[0] = 0;
					a[0] = 0;
				}

				if (ix + 1 >= 0 && ix + 1 < w && iy >= 0 && iy < h) {
					r[1] = data[ix + 1 + w * iy].r;
					g[1] = data[ix + 1 + w * iy].g;
					b[1] = data[ix + 1 + w * iy].b;
					a[1] = data[ix + 1 + w * iy].a;
				}
				else {
					r[1] = 0;
					g[1] = 0;
					b[1] = 0;
					a[1] = 0;
				}

				if (ix >= 0 && ix < w && iy + 1 >= 0 && iy + 1 < h) {
					r[2] = data[ix + w * iy + w].r;
					g[2] = data[ix + w * iy + w].g;
					b[2] = data[ix + w * iy + w].b;
					a[2] = data[ix + w * iy + w].a;
				}
				else {
					r[2] = 0;
					g[2] = 0;
					b[2] = 0;
					a[2] = 0;
				}

				if (ix + 1 >= 0 && ix + 1 < w && iy + 1 >= 0 && iy + 1 < h) {
					r[3] = data[ix + 1 + w * iy + w].r;
					g[3] = data[ix + 1 + w * iy + w].g;
					b[3] = data[ix + 1 + w * iy + w].b;
					a[3] = data[ix + 1 + w * iy + w].a;
				}
				else {
					r[3] = 0;
					g[3] = 0;
					b[3] = 0;
					a[3] = 0;
				}

				zx = (zx - ix) / 1;
				zy = (zy - iy) / 1;

				r[0] = r[0] + (r[1] - r[0]) * zx;
				r[1] = r[2] + (r[3] - r[2]) * zx;
				r[0] = r[0] + (r[1] - r[0]) * zy;

				g[0] = g[0] + (g[1] - g[0]) * zx;
				g[1] = g[2] + (g[3] - g[2]) * zx;
				g[0] = g[0] + (g[1] - g[0]) * zy;

				b[0] = b[0] + (b[1] - b[0]) * zx;
				b[1] = b[2] + (b[3] - b[2]) * zx;
				b[0] = b[0] + (b[1] - b[0]) * zy;

				a[0] = a[0] + (a[1] - a[0]) * zx;
				a[1] = a[2] + (a[3] - a[2]) * zx;
				a[0] = a[0] + (a[1] - a[0]) * zy;

				if (r[0] < 0) r[0] = 0; else if (r[0] > 255) r[0] = 255;
				if (g[0] < 0) g[0] = 0; else if (g[0] > 255) g[0] = 255;
				if (b[0] < 0) b[0] = 0; else if (b[0] > 255) b[0] = 255;
				if (a[0] < 0) a[0] = 0; else if (a[0] > 255) a[0] = 255;

				work[index].r = (int)r[0];
				work[index].g = (int)g[0];
				work[index].b = (int)b[0];
				if (a[0] == 0) {
					work[index].a = 0;
					mch[index].a = false;
				}
				else {
					work[index].a = 255;
					mch[index].a = true;
				}
			}
		}
	}
}

void sort_M::koudo(Pixel_RGBA* data, float* k, int max, Mch* mch, float tk, float dk, double TR, double TG, double TB) {
#pragma omp parallel for
	for (int i = 0; i < max; i++) {
		*(k + i) = TR * data[i].r + TG * data[i].g + TB * data[i].b;//ここを書き換えると、ソートの条件を変えれる
		if (*(k + i) < tk && *(k + i) > dk) mch[i].k = true;
		else mch[i].k = false;
	}
}

void sort_M::qsort(Pixel_RGBA* a, int f, int l, float* k) {
	int i = f, j = l;
	float pivot = *(k + (f + l) / 2);

	while (1) {
		while (*(k + i) < pivot)
			i++;
		while (*(k + j) > pivot)
			j--;
		if (i >= j) break;
		
		float w = *(k + i);//輝度のswap
		*(k + i) = *(k + j);
		*(k + j) = w;

		unsigned char s = a[i].r;//rのswap
		a[i].r = a[j].r;
		a[j].r = s;

		s = a[i].g;//gのswap
		a[i].g = a[j].g;
		a[j].g = s;

		s = a[i].b;//bのswap
		a[i].b = a[j].b;
		a[j].b = s;

		s = a[i].a;//aのswap
		a[i].a = a[j].a;
		a[j].a = s;

		i++;
		j--;
	}

	if (f < i - 1) qsort(a, f, i - 1, k);
	if (j + 1 < l) qsort(a, j + 1, l, k);
}

void sort_M::gqsort(Pixel_RGBA* a, int f, int l, float* k) {
	int i = f, j = l;
	float pivot = *(k + (f + l) / 2);

	while (1) {
		while (*(k + i) > pivot)
			i++;
		while (*(k + j) < pivot)
			j--;
		if (i >= j) break;

		float w = *(k + i);//輝度のswap
		*(k + i) = *(k + j);
		*(k + j) = w;

		unsigned char s = a[i].r;//rのswap
		a[i].r = a[j].r;
		a[j].r = s;

		s = a[i].g;//gのswap
		a[i].g = a[j].g;
		a[j].g = s;

		s = a[i].b;//bのswap
		a[i].b = a[j].b;
		a[j].b = s;

		s = a[i].a;//aのswap
		a[i].a = a[j].a;
		a[j].a = s;

		i++;
		j--;
	}

	if (f < i - 1) gqsort(a, f, i - 1, k);
	if (j + 1 < l) gqsort(a, j + 1, l, k);
}

void sort_M::pixelsort(Pixel_RGBA* a, int w, int h, float* k, float tk, float dk, bool moto, Mch* mch, bool gya) {
#pragma omp parallel for
	for (int y = 0; y < h; y++) {
		int i = 0, j = 0;
		int wy = w * y;

		while (1) {
			i = j + 1;
			while (mch[wy + i].a == false && i < w) {
				if (mch[wy + i].k) break;
				else i++;
			}

			if (i < w - 1) {
				j = i;
				while (mch[wy + j].a && j < w) {
					if (mch[wy + j].k) j++;
					else break;
				}
				if (i != j) {
					if (gya) gqsort(a, wy + i, wy + j - 1, k);
					else qsort(a, wy + i, wy + j - 1, k);
				}
				else
					if (moto == false) {
						//a[wy + i].r = 0;//ここはなくて大丈夫だっけ？
						//a[wy + i].g = 0;//たぶんいらなかったと思うのでコメントアウト
						//a[wy + i].b = 0;
						a[wy + i].a = 0;
					}
			}
			else break;
		}
	}
}

void sort_M::stretch(Pixel_RGBA* a, int w, int h, double str, Mch* mch, int seed, double Thi) {
	int wy = 0;
	float i = 0, j = 0;
	srand(seed);

	str = 1 - str / 100;//ここらへんはseed264さんのものを参考。勉強になる
	//https://github.com/SEED264/PixelSorter_s（参考リンクとやら）

	for (int y = 0; y < h; y++) {
		wy = w * y;
		i = 0, j = w - 1;
		while (mch[wy + (int)i].a == false && i < w - 1) i++;
		//i++;
		while (mch[wy + (int)j].a == false && j > 0) j--;
		if (i < j) {
#pragma omp parallel for
			for (int x = w - 1; x >= i; x--) {//optionsのアンカーを使わない場合はx=jのほうがいい
				float zx = (x - i) * (str + ((double)(rand() % 1000) / 1000 - str) * Thi) + i;
				int ix = (int)zx;
				zx = zx - ix;
				a[wy + x].r = (int)(a[wy + ix].r + (a[wy + ix + 1].r - a[wy + ix].r) * zx);
				a[wy + x].g = (int)(a[wy + ix].g + (a[wy + ix + 1].g - a[wy + ix].g) * zx);
				a[wy + x].b = (int)(a[wy + ix].b + (a[wy + ix + 1].b - a[wy + ix].b) * zx);
				a[wy + x].a = (int)(a[wy + ix].a + (a[wy + ix + 1].a - a[wy + ix].a) * zx);
			}
		}
	}
}

void sort_M::BR(Pixel_RGBA* data, Pixel_RGBA* work, int w, int h, int x, int y, double sin_S, double cos_S, double Pw, double Ph, bool ch) {
	int index = w * y + x;
	double zx = ((float)x - Pw) * cos_S + ((float)y - Ph) * sin_S + Pw;
	double zy = -((float)x - Pw) * sin_S + ((float)y - Ph) * cos_S + Ph;

	if (ch) {
		if (zx < 0) zx = 0;
		else if (zx > w - 1) zx = w - 1;
		if (zy < 0) zy = 0;
		else if (zy > h - 1) zy = h - 1;
	}

	int ix = (int)zx;
	int iy = (int)zy;

	float r[4] = { 0 };
	float g[4] = { 0 };
	float b[4] = { 0 };
	float a[4] = { 0 };

	if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
		r[0] = data[ix + w * iy].r;
		g[0] = data[ix + w * iy].g;
		b[0] = data[ix + w * iy].b;
		a[0] = data[ix + w * iy].a;
	}
	else {
		r[0] = 0;
		g[0] = 0;
		b[0] = 0;
		a[0] = 0;
	}

	if (ix + 1 >= 0 && ix + 1 < w && iy >= 0 && iy < h) {
		r[1] = data[ix + 1 + w * iy].r;
		g[1] = data[ix + 1 + w * iy].g;
		b[1] = data[ix + 1 + w * iy].b;
		a[1] = data[ix + 1 + w * iy].a;
	}
	else {
		r[1] = 0;
		g[1] = 0;
		b[1] = 0;
		a[1] = 0;
	}

	if (ix >= 0 && ix < w && iy + 1 >= 0 && iy + 1 < h) {
		r[2] = data[ix + w * iy + w].r;
		g[2] = data[ix + w * iy + w].g;
		b[2] = data[ix + w * iy + w].b;
		a[2] = data[ix + w * iy + w].a;
	}
	else {
		r[2] = 0;
		g[2] = 0;
		b[2] = 0;
		a[2] = 0;
	}

	if (ix + 1 >= 0 && ix + 1 < w && iy + 1 >= 0 && iy + 1 < h) {
		r[3] = data[ix + 1 + w * iy + w].r;
		g[3] = data[ix + 1 + w * iy + w].g;
		b[3] = data[ix + 1 + w * iy + w].b;
		a[3] = data[ix + 1 + w * iy + w].a;
	}
	else {
		r[3] = 0;
		g[3] = 0;
		b[3] = 0;
		a[3] = 0;
	}

	zx = (zx - ix) / 1;
	zy = (zy - iy) / 1;

	r[0] = r[0] + (r[1] - r[0]) * zx;
	r[1] = r[2] + (r[3] - r[2]) * zx;
	r[0] = r[0] + (r[1] - r[0]) * zy;

	g[0] = g[0] + (g[1] - g[0]) * zx;
	g[1] = g[2] + (g[3] - g[2]) * zx;
	g[0] = g[0] + (g[1] - g[0]) * zy;

	b[0] = b[0] + (b[1] - b[0]) * zx;
	b[1] = b[2] + (b[3] - b[2]) * zx;
	b[0] = b[0] + (b[1] - b[0]) * zy;

	a[0] = a[0] + (a[1] - a[0]) * zx;
	a[1] = a[2] + (a[3] - a[2]) * zx;
	a[0] = a[0] + (a[1] - a[0]) * zy;

	if (r[0] < 0) r[0] = 0; else if (r[0] > 255) r[0] = 255;
	if (g[0] < 0) g[0] = 0; else if (g[0] > 255) g[0] = 255;
	if (b[0] < 0) b[0] = 0; else if (b[0] > 255) b[0] = 255;
	if (a[0] < 0) a[0] = 0; else if (a[0] > 255) a[0] = 255;

	work[index].r = (int)r[0];
	work[index].g = (int)g[0];
	work[index].b = (int)b[0];
	work[index].a = (int)a[0];
}

void sort_M::Medge(Pixel_RGBA* data, float* q, float* q1, float* q2, int w, int h) {
#pragma omp parallel for
	for (int i = 0; i < w * h; i++)
		*(q + i) = (0.299 * data[i].r + 0.587 * data[i].g + 0.114 * data[i].b) * data[i].a / 255;

#pragma omp parallel for
	for (int i = 0; i < w * h - 1; i++) {//初期化
		*(q1 + i) = *(q + i);
		*(q2 + i) = *(q + i);
	}

#pragma omp parallel for
	for (int y = 1; y < h - 1; y++) {//めんどいので、端以外
		int i = w * y;//膨張
		for (int x = 1; x < w - 1; x++) {
			i++;
			if (*(q1 + i) < *(q + i - 1)) *(q1 + i) = *(q + i - 1);
			if (*(q1 + i) < *(q + i + 1)) *(q1 + i) = *(q + i + 1);
			if (*(q1 + i) < *(q + i - w)) *(q1 + i) = *(q + i - w);
			if (*(q1 + i) < *(q + i + w)) *(q1 + i) = *(q + i + w);
		}
	}

#pragma omp parallel for
	for (int y = 1; y < h - 1; y++) {//めんどいので、端以外
		int i = w * y;//縮小
		for (int x = 1; x < w - 1; x++) {
			i++;
			if (*(q2 + i) > *(q + i - 1)) *(q2 + i) = *(q + i - 1);
			if (*(q2 + i) > *(q + i + 1)) *(q2 + i) = *(q + i + 1);
			if (*(q2 + i) > *(q + i - w)) *(q2 + i) = *(q + i - w);
			if (*(q2 + i) > *(q + i + w)) *(q2 + i) = *(q + i + w);
		}
	}

#pragma omp parallel for
	for (int i = 0; i < w * h; i++) {//エッジ抽出
		*(q + i) = *(q1 + i) - *(q2 + i);
	}
}

void sort_M::Bokasi(Pixel_RGBA* work, Pixel_RGBA* data, int w, int h, float* q) {

	float sl[4] = { 0 };
	double r = 0, g = 0, b = 0, a = 0;
	float count = 0;
	float ef = 0;

	for (int y = 0; y < h; y++) {
#pragma omp parallel for
		for (int x = 0; x < w; x++) {
			r = 0, g = 0, b = 0, a = 0;
			int Ix = x + w * y;
			int index = 0;

			//RGBA//////////////////////////////////////////
			count = 0;
			ef = *(q + Ix);

			sl[0] = x - ef;
			if (sl[0] < 0) sl[0] = 0;
			else sl[0] = (int)sl[0];

			sl[1] = x + ef;
			if (sl[1] >= w) sl[1] = w - 1;
			else sl[1] = (int)sl[1];

			sl[2] = y - ef;
			if (sl[2] < 0) sl[2] = 0;
			else sl[2] = (int)sl[2];

			sl[3] = y + ef;
			if (sl[3] >= h) sl[3] = h - 1;
			else sl[3] = (int)sl[3];

			for (int i = sl[2]; i < sl[3] + 1; i++) {//周辺ピクセルの合計を求める
				for (int j = sl[0]; j < sl[1] + 1; j++) {
					index = j + w * i;
					r = r + work[index].r;
					g = g + work[index].g;
					b = b + work[index].b;
					a = a + work[index].a;
					count++;
				}
			}

			//efが小数点の場合の処理
			ef = (float)ef - ((int)ef);
			if (sl[0] != 0 && sl[2] != 0) {//左上
				index = (int)(sl[0] - 1 + w * (sl[2] - 1));
				r = r + work[index].r * ef;
				g = g + work[index].g * ef;
				b = b + work[index].b * ef;
				a = a + work[index].a * ef;
				count = count + ef;
			}
			if (sl[1] != w - 1 && sl[2] != 0) {//右上
				index = (int)(sl[1] + 1 + w * (sl[2] - 1));
				r = r + work[index].r * ef;
				g = g + work[index].g * ef;
				b = b + work[index].b * ef;
				a = a + work[index].a * ef;
				count = count + ef;
			}
			if (sl[0] != 0 && sl[3] != h - 1) {//左下
				index = (int)(sl[0] - 1 + w * (sl[2] + 1));
				r = r + work[index].r * ef;
				g = g + work[index].g * ef;
				b = b + work[index].b * ef;
				a = a + work[index].a * ef;
				count = count + ef;
			}
			if (sl[1] != w - 1 && sl[2] != h - 1) {//右下
				index = (int)(sl[1] + 1 + w * (sl[2] + 1));
				r = r + work[index].r * ef;
				g = g + work[index].g * ef;
				b = b + work[index].b * ef;
				a = a + work[index].a * ef;
				count = count + ef;
			}
			if (sl[2] != 0) {//上
				for (int z = sl[0]; z < sl[1] + 1; z++) {
					index = (int)(z + w * (sl[2] - 1));
					r = r + work[index].r * ef;
					g = g + work[index].g * ef;
					b = b + work[index].b * ef;
					a = a + work[index].a * ef;
					count = count + ef;
				}
			}
			if (sl[3] != h - 1) {//下
				for (int z = sl[0]; z < sl[1] + 1; z++) {
					index = (int)(z + w * (sl[3] + 1));
					r = r + work[index].r * ef;
					g = g + work[index].g * ef;
					b = b + work[index].b * ef;
					a = a + work[index].a * ef;
					count = count + ef;
				}
			}
			if (sl[0] != 0) {//左
				for (int z = sl[2]; z < sl[3] + 1; z++) {
					index = (int)(sl[0] - 1 + w * z);
					r = r + work[index].r * ef;
					g = g + work[index].g * ef;
					b = b + work[index].b * ef;
					a = a + work[index].a * ef;
					count = count + ef;
				}
			}
			if (sl[1] != w - 1) {//右
				for (int z = sl[2]; z < sl[3] + 1; z++) {
					index = (int)(sl[1] + 1 + w * z);
					r = r + work[index].r * ef;
					g = g + work[index].g * ef;
					b = b + work[index].b * ef;
					a = a + work[index].a * ef;
					count = count + ef;
				}
			}

			r = r / count;
			g = g / count;
			b = b / count;
			a = a / count;

			//RGBAをdataに
			if (r < 0) r = 0; else if (r > 255) r = 255;
			if (g < 0) g = 0; else if (g > 255) g = 255;
			if (b < 0) b = 0; else if (b > 255) b = 255;
			if (a < 0) a = 0; else if (a > 255) a = 255;

			data[Ix].r = (int)r;
			data[Ix].g = (int)g;
			data[Ix].b = (int)b;
			data[Ix].a = (int)a;
		}
	}
}