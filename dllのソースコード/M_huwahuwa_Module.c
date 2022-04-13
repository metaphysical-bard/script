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
static int	hu(lua_State* L)
{	//hu(data,w,h,posnum,pos,col,r,a,alpha)
	//変数
	struct Pixel_RGBA* pixels = lua_touserdata(L, 1);
	int w = lua_tointeger(L, 2);
	int h = lua_tointeger(L, 3);
	int posnum = lua_tointeger(L, 4);
	int i;
	float pos[240] = {0};
	float col[240] = {0};
	float rrr = lua_tonumber(L, 7);
	float a = lua_tonumber(L, 8);
	float alpha = lua_tonumber(L, 9);
	alpha = alpha / 100;

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

			float effect[80] = {0};

			for (i = 0; i < posnum; i++) {
				float ip[3] = { x, y, 0 };
				float gi = ip[0] - pos[i * 3];
				float go = ip[1] - pos[i * 3 + 1];
				float gp = ip[2] - pos[i * 3 + 2];
				effect[i] = sqrt(gi * gi + go * go + gp * gp);
				if (effect[i] <= rrr) effect[i] = 1;
				else if (effect[i] <= rrr + a) effect[i] = 1.0 - ((effect[i] - rrr) / a);
				else effect[i] = 0;
			}

			float pr = r; 
			float pg = g;
			float pb = b;
			for (i = 0; i < posnum; i++) {
				if (effect[i] != 0) {
					float rt = 0, gt = 0, bt = 0;
					rt = (col[i * 3] - r) * effect[i];
					gt = (col[i * 3 + 1] - g) * effect[i];
					bt = (col[i * 3 + 2] - b) * effect[i];
						pr = pr + rt * alpha;
						pg = pg + gt * alpha;
						pb = pb + bt * alpha;
				}
			}

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
	{ "hu", hu },
	{ NULL, NULL }
};

//モジュールの初期化
MODULEDEFINE(M_huwahuwa_Module)

