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
{	//i(data,work,w,h,pos[1],pos[2],obj.track0,obj.track1,obj.track2,obj.track3,t,b,u,n)
	//変数
	struct Pixel_RGBA* data = lua_touserdata(L, 1);
	struct Pixel_RGBA* work = lua_touserdata(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	float px = lua_tonumber(L, 5) + (float)w / 2;
	float py = lua_tonumber(L, 6) + (float)h / 2;
	float ha = lua_tonumber(L, 7) / 100 / 3;
	float Rha = lua_tonumber(L, 8) / 100;
	float Gha = lua_tonumber(L, 9) / 100;
	float Bha = lua_tonumber(L, 10) / 100;
	float type = lua_tonumber(L, 11);
	float bef = lua_tonumber(L, 12);
	float hef = lua_tonumber(L, 13) / 100;
	float N = lua_tonumber(L, 14);

	float haba[3] = { 0 };

	/*typeはずれ幅の大きい順
		1赤緑
		2緑赤
		3赤青
		4青赤
		5緑青
		6青緑*/

	if (type == 1) {
		haba[0] = 1 + 2 * ha;
		haba[1] = 1 + ha;
		haba[2] = 1;
	}
	else if (type == 2) {
		haba[0] = 1 + ha;
		haba[1] = 1 + 2 * ha;
		haba[2] = 1;
	}
	else if (type == 3) {
		haba[0] = 1 + 2 * ha;
		haba[1] = 1;
		haba[2] = 1 + ha;
	}
	else if (type == 4) {
		haba[0] = 1 + ha;
		haba[1] = 1;
		haba[2] = 1 + 2 * ha;
	}
	else if (type == 5) {
		haba[0] = 1;
		haba[1] = 1 + 2 * ha;
		haba[2] = 1 + ha;
	}
	else {
		haba[0] = 1;
		haba[1] = 1 + ha;
		haba[2] = 1 + 2 * ha;
	}

	haba[0] = haba[0] + Rha;
	haba[1] = haba[1] + Gha;
	haba[2] = haba[2] + Bha;


	//処理
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int index = x + w * y;
			float a[4] = { 0 };
			int ia[3] = { 0 };

			if (1) {//rの処理
				float zx = (x - px) / haba[0] + px;
				float zy = (y - py) / haba[0] + py;

				int ix = (int)zx;
				int iy = (int)zy;

				float r[4] = { 0 };

				if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
					r[0] = data[ix + w * iy].r;
					a[0] = data[ix + w * iy].a;
				}
				else {
					r[0] = 0;
					a[0] = 0;
				}

				if (ix + 1 >= 0 && ix + 1 < w && iy >= 0 && iy < h) {
					r[1] = data[ix + 1 + w * iy].r;
					a[1] = data[ix + 1 + w * iy].a;
				}
				else {
					r[1] = 0;
					a[1] = 0;
				}

				if (ix >= 0 && ix < w && iy + 1 >= 0 && iy + 1 < h) {
					r[2] = data[ix + w * iy + w].r;
					a[2] = data[ix + w * iy + w].a;
				}
				else {
					r[2] = 0;
					a[2] = 0;
				}

				if (ix + 1 >= 0 && ix + 1 < w && iy + 1 >= 0 && iy + 1 < h) {
					r[3] = data[ix + 1 + w * iy + w].r;
					a[3] = data[ix + 1 + w * iy + w].a;
				}
				else {
					r[3] = 0;
					a[3] = 0;
				}

				r[0] = r[0] + (r[1] - r[0]) * ((zx - ix) / 1);
				r[1] = r[2] + (r[3] - r[2]) * ((zx - ix) / 1);
				r[0] = r[0] + (r[1] - r[0]) * ((zy - iy) / 1);

				a[0] = a[0] + (a[1] - a[0]) * ((zx - ix) / 1);
				a[1] = a[2] + (a[3] - a[2]) * ((zx - ix) / 1);
				a[0] = a[0] + (a[1] - a[0]) * ((zy - iy) / 1);

				if (r[0] < 0) r[0] = 0; else if (r[0] > 255) r[0] = 255;
				if (a[0] < 0) a[0] = 0; else if (a[0] > 255) a[0] = 255;
				work[index].r = (int)r[0];
				ia[0] = a[0];
			}
			if (1) {//gの処理
				float zx = (x - px) / haba[1] + px;
				float zy = (y - py) / haba[1] + py;

				int ix = (int)zx;
				int iy = (int)zy;

				float g[4] = { 0 };

				if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
					g[0] = data[ix + w * iy].g;
					a[0] = data[ix + w * iy].a;
				}
				else {
					g[0] = 0;
					a[0] = 0;
				}

				if (ix + 1 >= 0 && ix + 1 < w && iy >= 0 && iy < h) {
					g[1] = data[ix + 1 + w * iy].g;
					a[1] = data[ix + 1 + w * iy].a;
				}
				else {
					g[1] = 0;
					a[1] = 0;
				}

				if (ix >= 0 && ix < w && iy + 1 >= 0 && iy + 1 < h) {
					g[2] = data[ix + w * iy + w].g;
					a[2] = data[ix + w * iy + w].a;
				}
				else {
					g[2] = 0;
					a[2] = 0;
				}

				if (ix + 1 >= 0 && ix + 1 < w && iy + 1 >= 0 && iy + 1 < h) {
					g[3] = data[ix + 1 + w * iy + w].g;
					a[3] = data[ix + 1 + w * iy + w].a;
				}
				else {
					g[3] = 0;
					a[3] = 0;
				}

				g[0] = g[0] + (g[1] - g[0]) * ((zx - ix) / 1);
				g[1] = g[2] + (g[3] - g[2]) * ((zx - ix) / 1);
				g[0] = g[0] + (g[1] - g[0]) * ((zy - iy) / 1);

				a[0] = a[0] + (a[1] - a[0]) * ((zx - ix) / 1);
				a[1] = a[2] + (a[3] - a[2]) * ((zx - ix) / 1);
				a[0] = a[0] + (a[1] - a[0]) * ((zy - iy) / 1);

				if (g[0] < 0) g[0] = 0; else if (g[0] > 255) g[0] = 255;
				if (a[0] < 0) a[0] = 0; else if (a[0] > 255) a[0] = 255;
				work[index].g = (int)g[0];
				ia[1] = a[0];
			}
			if (1) {//bの処理
				float zx = (x - px) / haba[2] + px;
				float zy = (y - py) / haba[2] + py;

				int ix = (int)zx;
				int iy = (int)zy;

				float b[4] = { 0 };

				if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
					b[0] = data[ix + w * iy].b;
					a[0] = data[ix + w * iy].a;
				}
				else {
					b[0] = 0;
					a[0] = 0;
				}

				if (ix + 1 >= 0 && ix + 1 < w && iy >= 0 && iy < h) {
					b[1] = data[ix + 1 + w * iy].b;
					a[1] = data[ix + 1 + w * iy].a;
				}
				else {
					b[1] = 0;
					a[1] = 0;
				}

				if (ix >= 0 && ix < w && iy + 1 >= 0 && iy + 1 < h) {
					b[2] = data[ix + w * iy + w].b;
					a[2] = data[ix + w * iy + w].a;
				}
				else {
					b[2] = 0;
					a[2] = 0;
				}

				if (ix + 1 >= 0 && ix + 1 < w && iy + 1 >= 0 && iy + 1 < h) {
					b[3] = data[ix + 1 + w * iy + w].b;
					a[3] = data[ix + 1 + w * iy + w].a;
				}
				else {
					b[3] = 0;
					a[3] = 0;
				}

				b[0] = b[0] + (b[1] - b[0]) * ((zx - ix) / 1);
				b[1] = b[2] + (b[3] - b[2]) * ((zx - ix) / 1);
				b[0] = b[0] + (b[1] - b[0]) * ((zy - iy) / 1);

				a[0] = a[0] + (a[1] - a[0]) * ((zx - ix) / 1);
				a[1] = a[2] + (a[3] - a[2]) * ((zx - ix) / 1);
				a[0] = a[0] + (a[1] - a[0]) * ((zy - iy) / 1);

				if (b[0] < 0) b[0] = 0; else if (b[0] > 255) b[0] = 255;
				if (a[0] < 0) a[0] = 0; else if (a[0] > 255) a[0] = 255;
				work[index].b = (int)b[0];
				ia[2] = a[0];
			}
			work[index].a = (int)((ia[0] + ia[1] + ia[2]) / 3);
		}
	}


	//ぼかし
	if (bef < 0) bef = 0;

	if (bef != 0) {
		float sl[4] = { 0 };
		double r = 0, g = 0, b = 0, a = 0;
		float count = 0;
		float ef = 0;

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				r = 0, g = 0, b = 0;

				//R//////////////////////////////////////////
				count = 0;
				ef = haba[0] - 1;
				if (ef <= 0) ef = -1 * bef * ef;
				else ef = bef * ef;

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

				for (int i = sl[2]; i < sl[3] + 1; i++) {
					for (int j = sl[0]; j < sl[1] + 1; j++) {
						int index = j + w * i;
						r = r + work[index].r;
						count++;
					}
				}

				//efが小数点の場合の処理
				ef = (float)ef - ((int)ef);
				if (sl[0] != 0 && sl[2] != 0) {//左上
					r = r + work[(int)(sl[0] - 1 + w * (sl[2] - 1))].r * ef;
					count = count + ef;
				}
				if (sl[1] != w - 1 && sl[2] != 0) {//右上
					r = r + work[(int)(sl[1] + 1 + w * (sl[2] - 1))].r * ef;
					count = count + ef;
				}
				if (sl[0] != 0 && sl[3] != h - 1) {//左下
					r = r + work[(int)(sl[0] - 1 + w * (sl[2] + 1))].r * ef;
					count = count + ef;
				}
				if (sl[1] != w - 1 && sl[2] != h - 1) {//右下
					r = r + work[(int)(sl[1] + 1 + w * (sl[2] + 1))].r * ef;
					count = count + ef;
				}
				if (sl[2] != 0) {//上
					for (int z = sl[0]; z < sl[1] + 1; z++) {
						r = r + work[(int)(z + w * (sl[2] - 1))].r * ef;
						count = count + ef;
					}
				}
				if (sl[3] != h - 1) {//下
					for (int z = sl[0]; z < sl[1] + 1; z++) {
						r = r + work[(int)(z + w * (sl[3] + 1))].r * ef;
						count = count + ef;
					}
				}
				if (sl[0] != 0) {//左
					for (int z = sl[2]; z < sl[3] + 1; z++) {
						r = r + work[(int)(sl[0] - 1 + w * z)].r * ef;
						count = count + ef;
					}
				}
				if (sl[1] != w - 1) {//右
					for (int z = sl[2]; z < sl[3] + 1; z++) {
						r = r + work[(int)(sl[1] + 1 + w * z)].r * ef;
						count = count + ef;
					}
				}

				r = r / count;

				//G//////////////////////////////////////////
				count = 0;
				ef = haba[1] - 1;
				if (ef <= 0) ef = -1 * bef * ef;
				else ef = bef * ef;

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

				for (int i = sl[2]; i < sl[3] + 1; i++) {
					for (int j = sl[0]; j < sl[1] + 1; j++) {
						int index = j + w * i;
						g = g + work[index].g;
						count++;
					}
				}

				//efが小数点の場合の処理
				ef = (float)ef - ((int)ef);
				if (sl[0] != 0 && sl[2] != 0) {//左上
					g = g + work[(int)(sl[0] - 1 + w * (sl[2] - 1))].g * ef;
					count = count + ef;
				}
				if (sl[1] != w - 1 && sl[2] != 0) {//右上
					g = g + work[(int)(sl[1] + 1 + w * (sl[2] - 1))].g * ef;
					count = count + ef;
				}
				if (sl[0] != 0 && sl[3] != h - 1) {//左下
					g = g + work[(int)(sl[0] - 1 + w * (sl[2] + 1))].g * ef;
					count = count + ef;
				}
				if (sl[1] != w - 1 && sl[2] != h - 1) {//右下
					g = g + work[(int)(sl[1] + 1 + w * (sl[2] + 1))].g * ef;
					count = count + ef;
				}
				if (sl[2] != 0) {//上
					for (int z = sl[0]; z < sl[1] + 1; z++) {
						g = g + work[(int)(z + w * (sl[2] - 1))].g * ef;
						count = count + ef;
					}
				}
				if (sl[3] != h - 1) {//下
					for (int z = sl[0]; z < sl[1] + 1; z++) {
						g = g + work[(int)(z + w * (sl[3] + 1))].g * ef;
						count = count + ef;
					}
				}
				if (sl[0] != 0) {//左
					for (int z = sl[2]; z < sl[3] + 1; z++) {
						g = g + work[(int)(sl[0] - 1 + w * z)].g * ef;
						count = count + ef;
					}
				}
				if (sl[1] != w - 1) {//右
					for (int z = sl[2]; z < sl[3] + 1; z++) {
						g = g + work[(int)(sl[1] + 1 + w * z)].g * ef;
						count = count + ef;
					}
				}

				g = g / count;

				//B//////////////////////////////////////////
				count = 0;
				ef = haba[1] - 1;
				if (ef <= 0) ef = -1 * bef * ef;
				else ef = bef * ef;

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

				for (int i = sl[2]; i < sl[3] + 1; i++) {
					for (int j = sl[0]; j < sl[1] + 1; j++) {
						int index = j + w * i;
						b = b + work[index].b;
						count++;
					}
				}

				//efが小数点の場合の処理
				ef = (float)ef - ((int)ef);
				if (sl[0] != 0 && sl[2] != 0) {//左上
					b = b + work[(int)(sl[0] - 1 + w * (sl[2] - 1))].b * ef;
					count = count + ef;
				}
				if (sl[1] != w - 1 && sl[2] != 0) {//右上
					b = b + work[(int)(sl[1] + 1 + w * (sl[2] - 1))].b * ef;
					count = count + ef;
				}
				if (sl[0] != 0 && sl[3] != h - 1) {//左下
					b = b + work[(int)(sl[0] - 1 + w * (sl[2] + 1))].b * ef;
					count = count + ef;
				}
				if (sl[1] != w - 1 && sl[2] != h - 1) {//右下
					b = b + work[(int)(sl[1] + 1 + w * (sl[2] + 1))].b * ef;
					count = count + ef;
				}
				if (sl[2] != 0) {//上
					for (int z = sl[0]; z < sl[1] + 1; z++) {
						b = b + work[(int)(z + w * (sl[2] - 1))].b * ef;
						count = count + ef;
					}
				}
				if (sl[3] != h - 1) {//下
					for (int z = sl[0]; z < sl[1] + 1; z++) {
						b = b + work[(int)(z + w * (sl[3] + 1))].b * ef;
						count = count + ef;
					}
				}
				if (sl[0] != 0) {//左
					for (int z = sl[2]; z < sl[3] + 1; z++) {
						b = b + work[(int)(sl[0] - 1 + w * z)].b * ef;
						count = count + ef;
					}
				}
				if (sl[1] != w - 1) {//右
					for (int z = sl[2]; z < sl[3] + 1; z++) {
						b = b + work[(int)(sl[1] + 1 + w * z)].b * ef;
						count = count + ef;
					}
				}

				b = b / count;

				//RGBAをdataに
				if (r < 0) r = 0; else if (r > 255) r = 255;
				if (g < 0) g = 0; else if (g > 255) g = 255;
				if (b < 0) b = 0; else if (b > 255) b = 255;

				int index = x + w * y;
				data[index].r = (int)r;
				data[index].g = (int)g;
				data[index].b = (int)b;
				data[index].a = work[index].a;

			}
		}
	}
	else {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				data[index].r = work[index].r;
				data[index].g = work[index].g;
				data[index].b = work[index].b;
				data[index].a = work[index].a;
			}
		}
	}


	//放射ブラー
	if (N < 0) N = 0;
	else N = (int)N;

	if (N != 0 || hef != 0) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				double sr = 0, sg = 0, sb = 0, sa = 0;

				for (int i = 1; i <= N; i++) {

					float zx = (x - px) / (1 + hef * i / N) + px;
					float zy = (y - py) / (1 + hef * i / N) + py;

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

					sr = sr + r[0];
					sg = sg + g[0];
					sb = sb + b[0];
					sa = sa + a[0];
				}

				sr = sr / N;
				sg = sg / N;
				sb = sb / N;
				sa = sa / N;

				if (sr < 0) sr = 0; else if (sr > 255) sr = 255;
				if (sg < 0) sg = 0; else if (sg > 255) sg = 255;
				if (sb < 0) sb = 0; else if (sb > 255) sb = 255;
				if (sa < 0) sa = 0; else if (sa > 255) sa = 255;

				work[index].r = (int)sr;
				work[index].g = (int)sg;
				work[index].b = (int)sb;
				work[index].a = (int)sa;
			}
		}
	}
	else {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				work[index].r = data[index].r;
				work[index].g = data[index].g;
				work[index].b = data[index].b;
				work[index].a = data[index].a;
			}
		}
	}


	//関数終わり
	return	0;
}

//関数の登録
static const struct luaL_reg functiontable[] = {
	{ "i", i },
	{ NULL, NULL }
};

//モジュールの初期化
MODULEDEFINE(M_ColorShift_Module)

