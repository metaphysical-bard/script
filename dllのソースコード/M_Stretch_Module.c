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
static int	s(lua_State* L)
{
	//M_Stretch_Module.s(data,work,w,h,obj.track0,objtrack1,check)

	//変数
	struct Pixel_RGBA* data = lua_touserdata(L, 1);
	struct Pixel_RGBA* work = lua_touserdata(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	float sita = lua_tonumber(L, 5);
	float st = lua_tonumber(L, 6);
	int ch = lua_tointeger(L, 7);

	int i = 0;
	int x = 0, y = 0;

	sita = sita * (M_PI / 180);
	float tsin = sin(sita);
	float tcos = cos(sita);

	//値を代入
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			int index = x + w * y;
			work[index].r = 0;
			work[index].g = 0;
			work[index].b = 0;
			work[index].a = 0;
		}
	}

	//処理
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			int index = x + w * y;
			int r = data[index].r;
			int g = data[index].g;
			int b = data[index].b;

			float v = 0.299 * r + 0.587 * g + 0.114 * b;

			if (ch) v = 255 - v;

			int vv = (int)v;

			float xx = 0, yy = 0;

			float k = st * v / 255;

			for (i = 0; i < k; i++) {
				xx = i * tcos;
				yy = i * tsin;

				xx = x + xx;
				yy = y + yy;

				int xxx = (int)xx;
				int yyy = (int)yy;

				if (xxx >= 0 && xxx < w) {
					if (yyy >= 0 && yyy < h) {
						int ix = xxx + w * yyy;
						if (work[ix].a < v) {
							work[ix].r = r;
							work[ix].g = g;
							work[ix].b = b;
							work[ix].a = vv;
						}
					}
				}
			}
			data[index].r = vv;
		}
	}

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			int index = x + w * y;
			work[index].a = 255;
			int v = data[index].r;

			float xx = 0, yy = 0;

			float k = st * v / 255;

			for (i = 0; i < k; i++) {
				xx = i * tcos;
				yy = i * tsin;

				xx = x + xx;
				yy = y + yy;

				int xxx = (int)xx;
				int yyy = (int)yy;

				if (xxx >= 0 && xxx < w) {
					if (yyy >= 0 && yyy < h) {
						int ix = xxx + w * yyy;
						if (work[ix].a < v) {
							work[ix].a = data[index].a;
						}
					}
				}
			}
		}
	}


	//関数終わり
	return	0;
}

//関数の登録
static const struct luaL_reg functiontable[] = {
	{ "s", s },
	{ NULL, NULL }
};

//モジュールの初期化
MODULEDEFINE(M_Stretch_Module)

