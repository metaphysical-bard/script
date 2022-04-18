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
static int	grd(lua_State* L)
{	//grd(data,w,h,posnum,pos,col,alpha,a,mode)
	//変数
	struct Pixel_RGBA* pixels = lua_touserdata(L, 1);
	int w = lua_tointeger(L, 2);
	int h = lua_tointeger(L, 3);
	int posnum = lua_tointeger(L, 4);
	int i = 0;
	int j = 0;
	float pos[240] = { 0 };
	float col[240] = { 0 };
	float alpha = lua_tonumber(L, 7);
	alpha = alpha / 100;
	float a = lua_tonumber(L, 8);
	int mode = lua_tointeger(L, 9);

	for (i = 0; i < posnum * 3; i++) {
		lua_rawgeti(L, 5, i + 1);
		pos[i] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	for (i = 0; i < posnum * 3; i++) {
		lua_rawgeti(L, 6, i + 1);
		col[i] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	//処理
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			int index = x + w * y;
			float r = pixels[index].r;
			float g = pixels[index].g;
			float b = pixels[index].b;

			float effect[80] = { 0 };
			float souwa = 0;
			float pr = 0;
			float pg = 0;
			float pb = 0;

			for (i = 0; i < posnum; i++) {
				float ip[3] = { x, y, 0 };
				float gi = ip[0] - pos[i * 3];
				float go = ip[1] - pos[i * 3 + 1];
				float gp = ip[2] - pos[i * 3 + 2];
				effect[i] = sqrt(gi * gi + go * go + gp * gp);
				if (mode == 0) {
					if (effect[i] < a) effect[i] = 1.0 - effect[i] / a;
					else effect[i] = 0;
				}
			}

			if (mode == 0) {
				for (i = 0; i < posnum; i++) {
					souwa = souwa + effect[i];
				}
				souwa = souwa + 1;
				for (i = 0; i < posnum; i++) {
					pr = pr + (col[i * 3] - r) * effect[i] / souwa;
					pg = pg + (col[i * 3 + 1] - g) * effect[i] / souwa;
					pb = pb + (col[i * 3 + 2] - b) * effect[i] / souwa;
				}
			}
			else {
				int s = 0;
				for (i = 0; i < posnum - 1; i++) {
					if (effect[s] > effect[i + 1]) s = i + 1;
				}
				float gu = effect[s];
				for (i = 0; i < posnum; i++) {
					effect[i] = gu / effect[i] + a;
					if (effect[i] < 0) effect[i] = 0;
				}
				for (i = 0; i < posnum; i++) {
					souwa = souwa + effect[i];
				}
				for (i = 0; i < posnum; i++) {
					float atai = effect[i] / souwa;
					pr = pr + (col[i * 3] - r) * atai;
					pg = pg + (col[i * 3 + 1] - g) * atai;
					pb = pb + (col[i * 3 + 2] - b) * atai;
				}
			}

			pr = r + pr * alpha;
			pg = g + pg * alpha;
			pb = b + pb * alpha;

			r = pr;
			g = pg;
			b = pb;

			if (r < 0) r = 0; else if (r > 255) r = 255;
			if (g < 0) g = 0; else if (g > 255) g = 255;
			if (b < 0) b = 0; else if (b > 255) b = 255;

			pixels[index].r = (int)r;
			pixels[index].g = (int)g;
			pixels[index].b = (int)b;
		}
	}

	//関数終わり
	return	0;
}

//関数の登録
static const struct luaL_reg functiontable[] = {
	{ "grd", grd },
	{ NULL, NULL }
};

//モジュールの初期化
MODULEDEFINE(M_gradation_Module)

