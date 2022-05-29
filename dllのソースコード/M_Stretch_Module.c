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
	//M_Stretch_Module.s(data,work,w,h,obj.track1,obj.track2,obj.track3,v,seed)

	//変数
	struct Pixel_RGBA* data = lua_touserdata(L, 1);
	struct Pixel_RGBA* work = lua_touserdata(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	float sita = lua_tonumber(L, 5);
	float st = lua_tonumber(L, 6);
	float raef = lua_tonumber(L, 7);
	raef = raef / 100;
	float vlua[256] = { 0 };
	int i = 0;

	for (i = 0; i < 256; i++) {
		lua_rawgeti(L, 8, i + 1);
		vlua[i] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	int seed = lua_tointeger(L, 9);
	srand(seed);

	int x = 0, y = 0;
	float ra = 0;

	sita = sita * (M_PI / 180);
	float tsin = sin(sita);
	float tcos = cos(sita);

	//値を代入
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			int index = x + w * y;
			work[index].r = data[index].r;
			work[index].g = data[index].g;
			work[index].b = data[index].b;
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
			ra = (rand() % 256);

			float v = 0.299 * r + 0.587 * g + 0.114 * b;

			int vv = (int)v;
			if (vv < 0) vv = 0; else if (vv > 255) vv = 255;

			vv = (int)(vlua[vv] + (ra - vlua[vv]) * raef);
			float k = st * vv / 255;

			float xx = 0, yy = 0;

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
						if (work[ix].a < vv) {
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
			data[index].b = 0;
			work[index].a = data[index].a;
		}
	}

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			int index = x + w * y;
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
						if (data[ix].b < v) {
							work[ix].a = data[index].a;
							data[ix].b = v;
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

