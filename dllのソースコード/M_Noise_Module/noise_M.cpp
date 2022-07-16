#include "noise_M.h"
#include <math.h>
#include<omp.h>
#include <cstdlib>

void noise_M::Mbreak(Pixel_RGBA* a, int w, int h) {
#pragma omp parallel for
	for (int i = 0; i < w * h; i++) {
		a[i].r = a[i].r + 300;
		a[i].g = a[i].g + 300;
		a[i].b = a[i].b + 300;
		a[i].a = a[i].a + 300;
	}
}

void noise_M::swap(int* a, int* b) {
	int p = *a;
	*a = *b;
	*b = p;
}

void noise_M::randM(int* r, int seed) {
	srand(seed);//srandのseed値変えてもrand%Nの値が変わらないのはあるあるらしい…
#pragma omp parallel for//0~255を順に代入
	for (int i = 0; i < 256; i++)
		*(r + i) = i;

#pragma omp parallel for//ランダムにシャッフル
		for (int i = 0; i < 256; i++)
			swap(r + i, r + (int)((double)rand() / ((double)RAND_MAX + 1) * 256));//(int)((double)rand() / ((double)RAND_MAX + 1) * N)

#pragma omp parallel for//インデックスが255~511にコピー
	for (int i = 0; i < 256; i++)
		*(r + i + 256) = *(r + i);
}

double noise_M::fade(double* t) {
	return *t * *t * *t * (*t * (*t * 6 - 15) + 10);//イーズ曲線
}

double noise_M::grad(int hash, double x, double y, double z) {
	switch (hash & 0xF)
	{
	case 0x0: return  x + y;
	case 0x1: return -x + y;
	case 0x2: return  x - y;
	case 0x3: return -x - y;
	case 0x4: return  x + z;
	case 0x5: return -x + z;
	case 0x6: return  x - z;
	case 0x7: return -x - z;
	case 0x8: return  y + z;
	case 0x9: return -y + z;
	case 0xA: return  y - z;
	case 0xB: return -y - z;
	case 0xC: return  y + x;
	case 0xD: return -y + z;
	case 0xE: return  y - x;
	case 0xF: return -y - z;
	default: return 0;
	}
}

double noise_M::lerp(double a, double b, double x) {
	return a + x * (b - a);
}

double noise_M::perlin(double x, double y, double z, int* p) {

	int xi = (int)x % 255;//整数部分
	int yi = (int)y % 255;
	int zi = (int)z % 255;

	x = x - (int)x;//小数部分を求める
	y = y - (int)y;
	z = z - (int)z;

	double u = fade(&x);//fade関数で補間
	double v = fade(&y);
	double w = fade(&z);


	int aaa, aba, aab, abb, baa, bba, bab, bbb;//ハッシュ関数
	aaa = p[p[p[xi] + yi] + zi];
	aba = p[p[p[xi] + yi + 1] + zi];
	aab = p[p[p[xi] + yi] + zi + 1];
	abb = p[p[p[xi] + yi + 1] + zi + 1];
	baa = p[p[p[xi + 1] + yi] + zi];
	bba = p[p[p[xi + 1] + yi + 1] + zi];
	bab = p[p[p[xi + 1] + yi] + zi + 1];
	bbb = p[p[p[xi + 1] + yi + 1] + zi + 1];

	double x1, x2, y1, y2;
	x1 = lerp(grad(aaa, x, y, z),
		grad(baa, x - 1, y, z),
		u);
	x2 = lerp(grad(aba, x, y - 1, z),
		grad(bba, x - 1, y - 1, z),
		u);
	y1 = lerp(x1, x2, v);

	x1 = lerp(grad(aab, x, y, z - 1),
		grad(bab, x - 1, y, z - 1),
		u);
	x2 = lerp(grad(abb, x, y - 1, z - 1),
		grad(bbb, x - 1, y - 1, z - 1),
		u);
	y2 = lerp(x1, x2, v);

	return (lerp(y1, y2, w) + 1) / 2;
}

double noise_M::OctavePerlin(double x, double y, double z, int octaves, double persistence, int* r) {
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	double maxValue = 0;
	for (int i = 0; i < octaves; i++) {
		total += perlin(x * frequency, y * frequency, z * frequency, r) * amplitude;

		maxValue += amplitude;

		amplitude *= persistence;
		frequency *= 2;
	}

	return total / maxValue;
}