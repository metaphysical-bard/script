//これ作るためにvsを導入した僕。えらい！

//呼び出し
#define _USE_MATH_DEFINES
#include <math.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

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
static int	henka(lua_State* L)
{
	//変数
	struct Pixel_RGBA* pixels = lua_touserdata(L, 1);
	int w = lua_tointeger(L, 2);
	int h = lua_tointeger(L, 3);
	int mode = lua_tointeger(L, 4);
	int i;
	float meido[256] = { 0 };
	int Pdata[256] = { 0 };
	int Rdata[256] = { 0 };
	int Gdata[256] = { 0 };
	int Bdata[256] = { 0 };
	int Adata[256] = { 0 };

	if (mode >= 0 && mode <= 17) {
		for (i = 0; i < 256; i++) {
			lua_rawgeti(L, 5, i + 1);
			meido[i] = lua_tointeger(L, -1);
			lua_pop(L, 1);
		}
	}

	//modeごとに処理
	//modeがへんな場合、意図的に画像を崩壊させる
	if (mode == 0) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				int r = pixels[index].r;
				int g = pixels[index].g;
				int b = pixels[index].b;

				int a = (r + g + b) / 3 + (r + g + b) % 3;

				if (a > meido[a]) {
					float e = a - meido[a];
					r = r - e * r / a;
					g = g - e * g / a;
					b = b - e * b / a;
				}
				else if(a < meido[a]) {
					float e = meido[a] - a;
					r = r + e * (255 - r) / (255 - a);
					g = g + e * (255 - g) / (255 - a);
					b = b + e * (255 - b) / (255 - a);
				}

				pixels[index].r = r;
				pixels[index].g = g;
				pixels[index].b = b;
			}
		}
	}
	else if (mode == 1) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				pixels[index].r = meido[pixels[index].r];
				pixels[index].b = meido[pixels[index].b];
				pixels[index].g = meido[pixels[index].g];
			}
		}
	}
	else if (mode == 2) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				pixels[index].r = meido[pixels[index].r];
			}
		}
	}
	else if (mode == 3) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				pixels[index].g = meido[pixels[index].g];
			}
		}
	}
	else if (mode == 4) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				pixels[index].b = meido[pixels[index].b];
			}
		}
	}
	else if (mode == 5) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				pixels[index].g = meido[pixels[index].g];
				pixels[index].b = meido[pixels[index].b];
			}
		}
	}
	else if (mode == 6) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				pixels[index].r = meido[pixels[index].r];
				pixels[index].b = meido[pixels[index].b];
			}
		}
	}
	else if (mode == 7) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				pixels[index].r = meido[pixels[index].r];
				pixels[index].g = meido[pixels[index].g];
			}
		}
	}
	else if (mode == 8) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				pixels[index].a = meido[pixels[index].a];
			}
		}
	}
	else if (mode == 9) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				int r = pixels[index].r;
				int g= pixels[index].g;
				int b = pixels[index].b;

				int mx = r;
				if (mx < g) mx = g;
				if (mx < b) mx = b;

				int mm = meido[mx];

				if (mx == r) {
					r = mm;
					g = (float)g / r * mm;
					b = (float)b / r * mm;
				}
				else if (mx == g) {
					r = (float)r / g * mm;
					g = mm;
					b = (float)b / g * mm;
				}
				else if (mx == b) {
					r = (float)r / b * mm;
					g = (float)g / b * mm;
					b = mm;
				}

				int rr = r / 1;
				int gg = g / 1;
				int bb = b / 1;

				if (rr < 0) rr = 0; else if (rr > 255) rr = 255;
				if (gg < 0) gg = 0; else if (gg > 255) gg = 255;
				if (bb < 0) bb = 0; else if (bb > 255) bb = 255;

				pixels[index].r = rr;
				pixels[index].g = gg;
				pixels[index].b = bb;
			}
		}
	}
	else if (mode == 10) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				float r = pixels[index].r;
				float g = pixels[index].g;
				float b = pixels[index].b;

				float y = 0.299 * r + 0.587 * g + 0.114 * b;
				float cb = 0.564 * (b - y);
				float u = cb + 128;
				float cr = 0.713 * (r - y);
				float v = cr + 128;

				int yy = y / 1;
				y = meido[yy];

				cb = u - 128;
				cr = v - 128;
				r = 1.000 * y + 1.402 * cr;
				g = 1.000 * y - 0.344 * cb - 0.714 * cr;
				b = 1.000 * y + 1.772 * cb;

				if (r < 0) r = 0; else if (r > 255) r = 255;
				if (g < 0) g = 0; else if (g > 255) g = 255;
				if (b < 0) b = 0; else if (b > 255) b = 255;

				pixels[index].r = r;
				pixels[index].g = g;
				pixels[index].b = b;
			}
		}
	}
	else if (mode == 11) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				float r = pixels[index].r;
				float g = pixels[index].g;
				float b = pixels[index].b;

				float y = 0.299 * r + 0.587 * g + 0.114 * b;
				float cb = 0.564 * (b - y);
				float u = cb + 128;
				float cr = 0.713 * (r - y);
				float v = cr + 128;

				int uu = u / 1;
				u = meido[uu];

				cb = u - 128;
				cr = v - 128;
				r = 1.000 * y + 1.402 * cr;
				g = 1.000 * y - 0.344 * cb - 0.714 * cr;
				b = 1.000 * y + 1.772 * cb;

				if (r < 0) r = 0; else if (r > 255) r = 255;
				if (g < 0) g = 0; else if (g > 255) g = 255;
				if (b < 0) b = 0; else if (b > 255) b = 255;

				pixels[index].r = r;
				pixels[index].g = g;
				pixels[index].b = b;
			}
		}
	}
	else if (mode == 12) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				float r = pixels[index].r;
				float g = pixels[index].g;
				float b = pixels[index].b;

				float y = 0.299 * r + 0.587 * g + 0.114 * b;
				float cb = 0.564 * (b - y);
				float u = cb + 128;
				float cr = 0.713 * (r - y);
				float v = cr + 128;

				int vv = v/ 1;
				v = meido[vv];

				cb = u - 128;
				cr = v - 128;
				r = 1.000 * y + 1.402 * cr;
				g = 1.000 * y - 0.344 * cb - 0.714 * cr;
				b = 1.000 * y + 1.772 * cb;

				if (r < 0) r = 0; else if (r > 255) r = 255;
				if (g < 0) g = 0; else if (g > 255) g = 255;
				if (b < 0) b = 0; else if (b > 255) b = 255;

				pixels[index].r = r;
				pixels[index].g = g;
				pixels[index].b = b;
			}
		}
	}
	else if (mode == 13) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				int r = pixels[index].r;
				int g = pixels[index].g;
				int b = pixels[index].b;

				float y = 0.299 * r + 0.587 * g + 0.114 * b;
				float cb = 0.564 * (b - y);
				float u = cb + 128;
				float cr = 0.713 * (r - y);
				float v = cr + 128;

				int yy = y / 1;
				float gy = meido[yy];

				gy = 1 + (gy - yy) / 100;
				if (gy < 0)
					gy = 0;
				else if (gy > 255)
					gy = 255;

				y = (float)255 * pow(y / (float)255, (float)1 / gy);

				cb = u - 128;
				cr = v - 128;
				r = 1.000 * y + 1.402 * cr;
				g = 1.000 * y - 0.344 * cb - 0.714 * cr;
				b = 1.000 * y + 1.772 * cb;

				if (r < 0) r = 0; else if (r > 255) r = 255;
				if (g < 0) g = 0; else if (g > 255) g = 255;
				if (b < 0) b = 0; else if (b > 255) b = 255;

				pixels[index].r = r;
				pixels[index].g = g;
				pixels[index].b = b;
			}
		}
	}
	else if (mode == 14) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				int r = pixels[index].r;
				int g = pixels[index].g;
				int b = pixels[index].b;
				int a = pixels[index].a;

				int he = (r + g + b) / 3 + (r + g + b) % 3;
				a = meido[he];

				if (a < 0) a = 0; else if (a > 255) a = 255;
				pixels[index].a = a;
			}
		}
	}
	else if (mode == 15) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				int r = pixels[index].r;
				int g = pixels[index].g;
				int b = pixels[index].b;
				int a = pixels[index].a;

				int mx = r;
				if (mx < g) mx = g;
				else if (mx < b) mx = b;

				pixels[index].a = meido[mx];
			}
		}
	}
	else if (mode == 16) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				int r = pixels[index].r;
				int g = pixels[index].g;
				int b = pixels[index].b;
				int a = pixels[index].a;

				float y = 0.299 * r + 0.587 * g + 0.114 * b;
				int he = y / 1;
				pixels[index].a = meido[he];
			}
		}
	}
	else {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				int index = x + w * y;
				float r = (float)pixels[index].r / 255;
				float g = (float)pixels[index].g / 255;
				float b = (float)pixels[index].b / 255;

				float mx = r;
				if (mx < g) mx = g;
				if (mx < b) mx = b;

				float mn = r;
				if (mn > g) mn = g;
				if (mn > b) mn = b;

				float a = 5000;
				float px = mn + a / 100 * mx;
				float pn = mx - a / 100 * mx;
				int ix = px * 255;
				int in = pn * 255;

				if (mx == r)
					pixels[index].r = ix;
				else if (mx == g)
					pixels[index].g = ix;
				else
					pixels[index].b = ix;

				if (mn == r)
					pixels[index].r = in;
				else if (mn == g)
					pixels[index].g = in;
				else
					pixels[index].b = in;
			}
		}
	}

	//ヒストグラム用のテーブル
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int index = x + w * y;
			int r = pixels[index].r;
			int g = pixels[index].g;
			int b = pixels[index].b;
			int a = pixels[index].a;
			int p = (r + g + b) / 3 + (r + g + b) % 3;
			Pdata[p] = Pdata[p] + 1;
			Rdata[r] = Rdata[r] + 1;
			Gdata[g] = Gdata[g] + 1;
			Bdata[b] = Bdata[b] + 1;
			Adata[a] = Adata[a] + 1;
		}
	}
	for (i = 0; i < 256; i++) {
		lua_pushnumber(L, Pdata[i]);
		lua_rawseti(L, 6, i + 1);
		lua_pushnumber(L, Rdata[i]);
		lua_rawseti(L, 7, i + 1);
		lua_pushnumber(L, Gdata[i]);
		lua_rawseti(L, 8, i + 1);
		lua_pushnumber(L, Bdata[i]);
		lua_rawseti(L, 9, i + 1);
		lua_pushnumber(L, Adata[i]);
		lua_rawseti(L, 10, i + 1);
	}

	//関数終わり
	return	0;
}

//関数の登録
static const struct luaL_reg functiontable[] = {
	{ "henka", henka },
	{ NULL, NULL }
};

//モジュールの初期化
MODULEDEFINE(M_ToneCurve_Module)

