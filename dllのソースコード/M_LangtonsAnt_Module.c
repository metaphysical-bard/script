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
	//mymodule.s(data,pw,ph,lim,posnum,pos,#rl,ct,rl,lag,l,mode,r,g,b,result)

	//変数
	int i = 0, j = 0;

	struct Pixel_RGBA* data = lua_touserdata(L, 1);
	int w = lua_tointeger(L, 2);
	int h = lua_tointeger(L, 3);
	double lim = lua_tonumber(L, 4);
	int posnum = lua_tointeger(L, 5);//posの数

	int p[30] = { 0 };//pos
	for (i = 0; i < posnum * 3; i++) {
		lua_rawgeti(L, 6, i + 1);
		p[i] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	int colnum = lua_tointeger(L, 7);

	int c[900] = { 0 };//posnumごとのcolorを取得
	for (i = 0; i < colnum * 3 * posnum; i++) {
		lua_rawgeti(L, 8, i + 1);
		c[i] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	int rl[30] = { 0 };//右左の順番
	for (i = 0; i < colnum; i++) {
		lua_rawgeti(L, 9, i + 1);
		rl[i] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	double lag[10] = { 0 };//lag（stepの差）
	for (i = 0; i < posnum; i++) {
		lua_rawgeti(L, 10, i + 1);
		lag[i] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	int l[10] = { 0 };//初期の方向
	for (i = 0; i < posnum; i++) {
		lua_rawgeti(L, 11, i + 1);
		l[i] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	int mode = lua_tointeger(L, 12);//mode
	if (mode < 1 || mode > 4) mode = 1;

	int ar = lua_tointeger(L, 13);//アリの色
	int ag = lua_tointeger(L, 14);
	int ab = lua_tointeger(L, 15);

	double count = 0;
	int x = 0, y = 0;
	int la = 0, index = 0, a = 0;

	//処理
	/*
	めも
	0で右に、1で左に
	laから次のx,yを決定
	laはひとつ前の方向　1,上　2,下　3,左　4,右

	mode
	1 通常のモード。表示用のdataを用意
	2 引継ぎ元を作るモード。引継ぎ用のdataを用意
	3 引継ぎ用のdataをもとに計算するモード。表示用のdataにする
	4 引継ぎ用のdataをもとに計算するモード。さらに引継ぎ用のdataを用意

	*/

	if (mode == 1 || mode == 2) {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				index = x + w * y;
				data[index].a = 0;
			}
		}
	}

	while (1) {
		count++;
		if (count > lim) break;

		for (i = 0; i < posnum; i++) {
			if (count > lag[i]) {
				x = p[i * 3];
				y = p[i * 3 + 1];
				la = l[i];

				index = x + w * y;
				a = data[index].a;

				for (j = 0; j < colnum; j++) {
					if (a == j) {
						int sn = i * colnum * 3;
						data[index].r = c[j * 3 + sn];
						data[index].g = c[j * 3 + 1 + sn];
						data[index].b = c[j * 3 + 2 + sn];
						a++;
						if (a > colnum - 1) a = a - colnum;
						data[index].a = a;
						if (rl[j]) {
							if (la == 1) {
								x++; la = 4;
							}
							else if (la == 2) {
								x--; la = 3;
							}
							else if (la == 3) {
								y++; la = 1;
							}
							else {
								y--; la = 2;
							}
						}
						else {
							if (la == 1) {
								x--; la = 3;
							}
							else if (la == 2) {
								x++; la = 4;
							}
							else if (la == 3) {
								y--; la = 2;
							}
							else {
								y++; la = 1;
							}
						}
						break;
					}
				}

				if (x < 0 || x >= w || y < 0 || y >= h) {
					if (x < 0) x = w - 1; else if (x > w - 1) x = 0;
					if (y < 0) y = h - 1; else if (y > h - 1) y = 0;
				}

				p[i * 3] = x;
				p[i * 3 + 1] = y;
				l[i] = la;
			}
		}
	}

	if (ar != 300) {
		for (i = 0; i < posnum; i++) {
			index = p[i * 3] + w * p[i * 3 + 1];
			data[index].r = ar;
			data[index].g = ag;
			data[index].b = ab;
		}
	}

	if (mode == 1 || mode == 3) {
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				index = x + w * y;
				data[index].a = 255;
			}
		}
	}


	//resultに最終位置と最終方向を詰め込み
	for (i = 0; i < posnum * 3; i++) {
		lua_pushnumber(L, p[i]);
		lua_rawseti(L, 16, i + 1);
	}

	for (i = 0; i < posnum; i++) {
		lua_pushinteger(L, l[i]);
		lua_rawseti(L, 16, i + 1 + posnum * 3);
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
MODULEDEFINE(M_LangtonsAnt_Module)

