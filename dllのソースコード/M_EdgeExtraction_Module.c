//呼び出し
#define _USE_MATH_DEFINES
#include <math.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>

//マクロ
#define	MODULEDEFINE(name)	\
	__declspec(dllexport) int luaopen_##name (lua_State* L) { \
		luaL_register(L, #name, functiontable); \
		return	1;\
	}

//構造体の準備
struct Pixel_RGBA {
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
};

//関数
static int	i(lua_State* L)
{	//i(data,work,w,h,obj.track0,obj.track1,obj.track2,obj.track3,r,g,b,a,z)
	//変数
	struct Pixel_RGBA* data = lua_touserdata(L, 1);
	struct Pixel_RGBA* work = lua_touserdata(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	float sir = lua_tonumber(L, 5);
	float sig = lua_tonumber(L, 6);
	float sib = lua_tonumber(L, 7);
	float sia = lua_tonumber(L, 8);
	int r = lua_tointeger(L, 9);
	int g = lua_tointeger(L, 10);
	int b = lua_tointeger(L, 11);
	int a = lua_tointeger(L, 12);
	int z = lua_tointeger(L, 13);

	int index = 0;
	float he = 0;
	for (int y = 1; y < h - 1; y++) {//端以外(端はめんどくさいのでパス)
		index = w * y;
		for (int x = 1; x < w - 1; x++) {
			index++;

			//r
			he = data[index - 1 - w].r + data[index - w].r + data[index + 1 - w].r + data[index - 1].r + data[index + 1].r + data[index - 1 + w].r + data[index + w].r + data[index + 1 + w].r - 8 * data[index].r;
			if (z) {
				if (he > sir || -1 * he > sir) work[index].r = 1;
				else work[index].r = 0;
			}
			else {
				if (he > sir) work[index].r = 1;
				else work[index].r = 0;
			}

			//g
			he = data[index - 1 - w].g + data[index - w].g + data[index + 1 - w].g + data[index - 1].g + data[index + 1].g + data[index - 1 + w].g + data[index + w].g + data[index + 1 + w].g - 8 * data[index].g;
			if (z) {
				if (he > sig || -1 * he > sig) work[index].g = 1;
				else work[index].g = 0;
			}
			else {
				if (he > sig) work[index].g = 1;
				else work[index].g = 0;
			}

			//b
			he = data[index - 1 - w].b + data[index - w].b + data[index + 1 - w].b + data[index - 1].b + data[index + 1].b + data[index - 1 + w].b + data[index + w].b + data[index + 1 + w].b - 8 * data[index].b;
			if (z) {
				if (he > sib || -1 * he > sib) work[index].b = 1;
				else work[index].b = 0;
			}
			else {
				if (he > sib) work[index].b = 1;
				else work[index].b = 0;
			}

			//a
			he = data[index - 1 - w].a + data[index - w].a + data[index + 1 - w].a + data[index - 1].a + data[index + 1].a + data[index - 1 + w].a + data[index + w].a + data[index + 1 + w].a - 8 * data[index].a;
			if (z) {
				if (he > sia || -1 * he > sia) work[index].a = 1;
				else work[index].a = 0;
			}
			else {
				if (he > sia) work[index].a = 1;
				else work[index].a = 0;
			}
		}
	}

	for (int i = 0; i < w * h - 1; i++) {
		if (work[i].r || work[i].g || work[i].b || work[i].a) {
			data[i].r = r;
			data[i].g = g;
			data[i].b = b;
			data[i].a = 255;
		}
		else {
			data[i].r = 0;
			data[i].g = 0;
			data[i].b = 0;
			if (a) data[i].a = 0;
			else data[i].a = 255;
		}
	}

	//関数終わり
	return	0;
}

//関数
static int	d(lua_State* L)
{	//d(data,w,h,si,r,g,b,a,z,siki)

	struct Pixel_RGBA* data = lua_touserdata(L, 1);
	int w = lua_tointeger(L, 2);
	int h = lua_tointeger(L, 3);
	float si = lua_tonumber(L, 4);
	int r = lua_tointeger(L, 5);
	int g = lua_tointeger(L, 6);
	int b = lua_tointeger(L, 7);
	int a = lua_tointeger(L, 8);
	int z = lua_tointeger(L, 9);
	float siki = lua_tonumber(L, 10);

	float* k;
	k = malloc(w * h * 4);
	if (k == NULL) return -1;

	float* q;
	q = malloc(w * h * 4);
	if (q == NULL) return -1;

	for (int i = 0; i < w * h; i++) {
		*(q + i) = (0.299 * data[i].r + 0.587 * data[i].g + 0.114 * data[i].b) * data[i].a / 255;
	}

	int index = 0;
	float he = 0;
	for (int y = 1; y < h - 1; y++) {//端以外(端はめんどくさいのでパス)
		index = w * y;
		for (int x = 1; x < w - 1; x++) {
			index++;

			he = *(q + index - 1 - w) + *(q + index - w) + *(q + index + 1 - w) + *(q + index - 1) + *(q + index + 1) + *(q + index - 1 + w) + *(q + index + w) + *(q + index + 1 + w) - 8 * *(q + index);
			if (z)
				if (he < 0) *(k + index) = -1 * he;
				else *(k + index) = he;
			else *(k + index) = he;

			//強度を加算
			*(k + index) += si;
		}
	}

	for (int i = 0; i < w * h - 1; i++) {
		if (*(k + i) < siki) *(k + i) = 0;
		else if (*(k + i) > 100) *(k + i) = 1;
		else *(k + i) = *(k + i) / 100;

		data[i].r = (int)(*(k + i) * r);
		data[i].g = (int)(*(k + i) * g);
		data[i].b = (int)(*(k + i) * b);
		if (a) data[i].a = (int)(*(k + i) * 255);
		else data[i].a = 255;
	}

	free(k);
	free(q);

	return 0;
}

//関数
static int	s(lua_State* L)
{	//s(data,w,h,obj.track0,obj.track1,obj.track2,obj.track3,t,s)

	struct Pixel_RGBA* data = lua_touserdata(L, 1);
	int w = lua_tointeger(L, 2);
	int h = lua_tointeger(L, 3);
	int r = lua_tointeger(L, 4);
	int g = lua_tointeger(L, 5);
	int b = lua_tointeger(L, 6);
	int a = lua_tointeger(L, 7);
	int t = lua_tointeger(L, 8);
	float s = lua_tonumber(L, 9);

	int i = 0;

	struct Pixel_RGBA* q1;
	struct Pixel_RGBA* q2;

	q1 = malloc(w * h * 4);
	if (q1 == NULL) return -1;

	q2 = malloc(w * h * 4);
	if (q2 == NULL) return -1;

	for (i = 0; i < w * h - 1; i++) {//初期化
		q1[i].r = data[i].r;
		q1[i].g = data[i].g;
		q1[i].b = data[i].b;
		q1[i].a = data[i].a;
		q2[i].r = data[i].r;
		q2[i].g = data[i].g;
		q2[i].b = data[i].b;
		q2[i].a = data[i].a;
	}

	for (int y = 1; y < h - 1; y++) {//めんどいので、端以外
		i = w * y;//膨張
		for (int x = 1; x < w - 1; x++) {
			i++;

			if (q1[i].r < data[i - 1].r) q1[i].r = data[i - 1].r;
			if (q1[i].r < data[i + 1].r) q1[i].r = data[i + 1].r;
			if (q1[i].r < data[i - w].r) q1[i].r = data[i - w].r;
			if (q1[i].r < data[i + w].r) q1[i].r = data[i + w].r;

			if (q1[i].g < data[i - 1].g) q1[i].g = data[i - 1].g;
			if (q1[i].g < data[i + 1].g) q1[i].g = data[i + 1].g;
			if (q1[i].g < data[i - w].g) q1[i].g = data[i - w].g;
			if (q1[i].g < data[i + w].g) q1[i].g = data[i + w].g;

			if (q1[i].b < data[i - 1].b) q1[i].b = data[i - 1].b;
			if (q1[i].b < data[i + 1].b) q1[i].b = data[i + 1].b;
			if (q1[i].b < data[i - w].b) q1[i].b = data[i - w].b;
			if (q1[i].b < data[i + w].b) q1[i].b = data[i + w].b;
		}
	}

	for (int y = 1; y < h - 1; y++) {//めんどいので、端以外
		i = w * y;//縮小
		for (int x = 1; x < w - 1; x++) {
			i++;

			if (q2[i].r > data[i - 1].r) q2[i].r = data[i - 1].r;
			if (q2[i].r > data[i + 1].r) q2[i].r = data[i + 1].r;
			if (q2[i].r > data[i - w].r) q2[i].r = data[i - w].r;
			if (q2[i].r > data[i + w].r) q2[i].r = data[i + w].r;

			if (q2[i].g > data[i - 1].g) q2[i].g = data[i - 1].g;
			if (q2[i].g > data[i + 1].g) q2[i].g = data[i + 1].g;
			if (q2[i].g > data[i - w].g) q2[i].g = data[i - w].g;
			if (q2[i].g > data[i + w].g) q2[i].g = data[i + w].g;

			if (q2[i].b > data[i - 1].b) q2[i].b = data[i - 1].b;
			if (q2[i].b > data[i + 1].b) q2[i].b = data[i + 1].b;
			if (q2[i].b > data[i - w].b) q2[i].b = data[i - w].b;
			if (q2[i].b > data[i + w].b) q2[i].b = data[i + w].b;
		}
	}

	float pr, pg, pb, pa;
	for (i = 0; i < w * h - 1; i++) {
		if (t) {
			pr = (q1[i].r - q2[i].r) * r;
			pg = (q1[i].g - q2[i].g) * g;
			pb = (q1[i].b - q2[i].b) * b;
			pa = q1[i].a - q2[i].a + a;
		}
		else {
			pr = (q1[i].r - q2[i].r) + r;
			pg = (q1[i].g - q2[i].g) + g;
			pb = (q1[i].b - q2[i].b) + b;
			pa = (q1[i].a - q2[i].a) + a;
		}

		if (pr < s) pr = 0; else if (pr > 255) pr = 255;
		if (pg < s) pg = 0; else if (pg > 255) pg = 255;
		if (pb < s) pb = 0; else if (pb > 255) pb = 255;
		if (pa < s) pa = 0; else if (pa > 255) pa = 255;

		data[i].r = (int)pr;
		data[i].g = (int)pg;
		data[i].b = (int)pb;
		data[i].a = (int)pa;
	}

	free(q1);
	free(q2);

	return 0;
}

//関数
static int	r(lua_State* L)
{	//r(data,w,h,obj.track0,obj.track1,r,g,b,a,i)

	struct Pixel_RGBA* data = lua_touserdata(L, 1);
	int w = lua_tointeger(L, 2);
	int h = lua_tointeger(L, 3);
	float k = lua_tonumber(L, 4) / 100;
	float si = lua_tonumber(L, 5) / 100;
	int r = lua_tointeger(L, 6);
	int g = lua_tointeger(L, 7);
	int b = lua_tointeger(L, 8);
	int a = lua_tointeger(L, 9);
	int iro = lua_tointeger(L, 10);

	int i = 0;

	float* q;
	float* q1;
	float* q2;

	q = malloc(w * h * 4);
	if (q == NULL) return -1;

	q1 = malloc(w * h * 4);
	if (q1 == NULL) return -1;

	q2 = malloc(w * h * 4);
	if (q2 == NULL) return -1;

	for (int i = 0; i < w * h; i++)
		*(q + i) = (0.299 * data[i].r + 0.587 * data[i].g + 0.114 * data[i].b) * data[i].a / 255;

	for (i = 0; i < w * h - 1; i++) {//初期化
		*(q1 + i) = *(q + i);
		*(q2 + i) = *(q + i);
	}

	for (int y = 1; y < h - 1; y++) {//めんどいので、端以外
		i = w * y;//膨張
		for (int x = 1; x < w - 1; x++) {
			i++;
			if (*(q1 + i) < *(q + i - 1)) *(q1 + i) = *(q + i - 1);
			if (*(q1 + i) < *(q + i + 1)) *(q1 + i) = *(q + i + 1);
			if (*(q1 + i) < *(q + i - w)) *(q1 + i) = *(q + i - w);
			if (*(q1 + i) < *(q + i + w)) *(q1 + i) = *(q + i + w);
		}
	}

	for (int y = 1; y < h - 1; y++) {//めんどいので、端以外
		i = w * y;//縮小
		for (int x = 1; x < w - 1; x++) {
			i++;
			if (*(q2 + i) > *(q + i - 1)) *(q2 + i) = *(q + i - 1);
			if (*(q2 + i) > *(q + i + 1)) *(q2 + i) = *(q + i + 1);
			if (*(q2 + i) > *(q + i - w)) *(q2 + i) = *(q + i - w);
			if (*(q2 + i) > *(q + i + w)) *(q2 + i) = *(q + i + w);
		}
	}

	float pr, pg, pb, pa;
	for (i = 0; i < w * h - 1; i++) {
		*(q1 + i) = *(q1 + i) - *(q2 + i);
		*(q1 + i) = *(q1 + i) * k / 255;
		
		if (*(q1 + i) > 1) *(q1 + i) = 1;

		if (*(q1 + i) < si) *(q1 + i) = 0;
		else *(q1 + i) = (*(q1 + i) - si) / (1 - si);

		if (iro) {
			pr = data[i].r * *(q1 + i);
			pg = data[i].g * *(q1 + i);
			pb = data[i].b * *(q1 + i);
			if (a) pa = data[i].a * *(q1 + i);
			else pa = data[i].a;
		}
		else {
			pr = r * *(q1 + i);
			pg = g * *(q1 + i);
			pb = b * *(q1 + i);
			if (a) pa = 255 * *(q1 + i);
			else pa = 255;
		}

		if (pr < 0) pr = 0; else if (pr > 255) pr = 255;
		if (pg < 0) pg = 0; else if (pg > 255) pg = 255;
		if (pb < 0) pb = 0; else if (pb > 255) pb = 255;
		if (pa < 0) pa = 0; else if (pa > 255) pa = 255;

		data[i].r = (int)pr;
		data[i].g = (int)pg;
		data[i].b = (int)pb;
		data[i].a = (int)pa;
	}

	free(q);
	free(q1);
	free(q2);

	return 0;
}

//関数の登録
static const struct luaL_reg functiontable[] = {
	{ "i", i },//ラプシアンフィルタ(RGBA)
	{ "d", d },//ラプシアンフィルタ(輝度)
	{ "s", s },//膨張と縮小の差分
	{ "r", r },//グラジエント(輝度)
	{ NULL, NULL }
};

//モジュールの初期化
MODULEDEFINE(M_EdgeExtraction_Module)

