//呼び出し
#define _USE_MATH_DEFINES
#include <math.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#define float double//doubleのほうがいい？

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
	//mymodule.s(data,work,w,h,obj.track1,v,pos,obj.track2,obj.track3,zk,seed,ch,#p,p,ci,cii,c)

//変数
	struct Pixel_RGBA* data = lua_touserdata(L, 1);
	struct Pixel_RGBA* work = lua_touserdata(L, 2);
	int w = lua_tointeger(L, 3);
	int h = lua_tointeger(L, 4);
	float ef = lua_tonumber(L, 5) / 100;
	int vlua[256] = { 0 };
	int i = 0;

	for (i = 0; i < 256; i++) {
		lua_rawgeti(L, 6, i + 1);
		vlua[i] = lua_tointeger(L, -1);
		lua_pop(L, 1);
	}

	float pos[3] = { 0 };
	for (i = 0; i < 3; i++) {
		lua_rawgeti(L, 7, i + 1);
		pos[i] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	float z = lua_tonumber(L, 8) / 100;
	float ran = lua_tonumber(L, 9) / 100;
	int zk = lua_tointeger(L, 10);
	int seed = lua_tointeger(L, 11);
	srand(seed);

	int ch = lua_tointeger(L, 12);
	int np = lua_tointeger(L, 13);
	if (np > 50) np = 50;

	float p[50] = { 0 };
	for (i = 0; i < np; i++) {
		lua_rawgeti(L, 14, i + 1);
		p[i] = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}

	int ci = lua_tointeger(L, 15);
	int cii = lua_tointeger(L, 16);
	float c = lua_tonumber(L, 17);

	int x = 0, y = 0, sf = 0;
	if (zk > 5) sf = sqrt(w * w + h * h);

	float st = 0;//zkが10と11の時用
	float sx, sy;

	if (zk == 10 || zk == 11) {
		sf = sqrt(p[0] * p[0] + p[1] * p[1]);

		sx = p[0] - w;
		st = sqrt(sx * sx + p[1] * p[1]);
		if (sf < st) sf = st;

		sy = p[1] - h;
		st = sqrt(p[0] * p[0] + sy * sy);
		if (sf < st) sf = st;

		sx = p[0] - w;
		sy = p[1] - h;
		st = sqrt(sx * sx + sy * sy);
		if (sf < st) sf = st;
	}

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
			int a = data[index].a;
			float ra = (rand() % 255) + 1;

			float v = (0.299 * r + 0.587 * g + 0.114 * b) * a / 255;

			int vv = (int)v;
			if (vv < 0) vv = 0; else if (vv > 255) vv = 255;

			vv = vlua[vv];

			//fの値を決める
			/*
			zkについて
			0 index順
			1 逆index順
			2 上
			3 下
			4 右
			5 左
			6 右上
			7 左上
			8 右下
			9 左下
			10 アンカー
			11 逆アンカー
			12 テーブルでループ(最大で50)
			*/
			float ind = 0;

			//zkごとにzを指定
			if (zk == 0) ind = (float)index / (w * h);
			else if (zk == 1) ind = 1 - (float)index / (w * h);
			else if (zk == 2) ind = (float)y / h;
			else if (zk == 3) ind = 1 - (float)y / h;
			else if (zk == 4) ind = (float)x / w;
			else if (zk == 5) ind = 1 - (float)x / w;
			else if (zk == 6) ind = sqrt((float)((w - x) * (w - x) + y * y)) / sf;
			else if (zk == 7) ind = sqrt((float)(x * x + y * y)) / sf;
			else if (zk == 8) ind = sqrt((float)((w - x) * (w - x) + (h - y) * (h - y))) / sf;
			else if (zk == 9) ind = sqrt((float)(x * x + (h - y) * (h - y))) / sf;
			else if (zk == 10) {
				sx = p[0] - x;
				sy = p[1] - y;
				ind = sqrt(sx * sx + sy * sy) / sf;
			}
			else if (zk == 11) {
				sx = p[0] - x;
				sy = p[1] - y;
				ind = 1 - sqrt(sx * sx + sy * sy) / sf;
			}
			else ind = p[index % np];

			float f = ef - (ra / 255) * ran - ind * z;

			if (ci == 0) if (f < 0) f = 0;
			if (cii == 0) if (f > 1) f = 1;

			int xx = 0, yy = 0;

			if (ch) {
				xx = x + pos[0] * f;
				yy = y + pos[1] * f;
			}
			else {
				xx = x + (pos[0] - x) * f;
				yy = y + (pos[1] - y) * f;
			}

			if (xx >= 0 && xx < w) {
				if (yy >= 0 && yy < h) {
					int ix = xx + w * yy;
					if (work[ix].a < vv) {
						work[ix].r = r;
						work[ix].g = g;
						work[ix].b = b;
						work[ix].a = vv;
					}
				}
			}
			data[index].g = (int)ra;
			data[index].r = vv;
		}
	}

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			data[x + w * y].b = 0;
		}
	}

	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			int index = x + w * y;
			int vv = data[index].r;
			float ra = data[index].g;

			float ind = 0;

			if (zk == 0) ind = (float)index / (w * h);
			else if (zk == 1) ind = 1 - (float)index / (w * h);
			else if (zk == 2) ind = (float)y / h;
			else if (zk == 3) ind = 1 - (float)y / h;
			else if (zk == 4) ind = (float)x / w;
			else if (zk == 5) ind = 1 - (float)x / w;
			else if (zk == 6) ind = sqrt((float)((w - x) * (w - x) + y * y)) / sf;
			else if (zk == 7) ind = sqrt((float)(x * x + y * y)) / sf;
			else if (zk == 8) ind = sqrt((float)((w - x) * (w - x) + (h - y) * (h - y))) / sf;
			else if (zk == 9) ind = sqrt((float)(x * x + (h - y) * (h - y))) / sf;
			else if (zk == 10) {
				sx = p[0] - x;
				sy = p[1] - y;
				ind = sqrt(sx * sx + sy * sy) / sf;
			}
			else if (zk == 11) {
				sx = p[0] - x;
				sy = p[1] - y;
				ind = 1 - sqrt(sx * sx + sy * sy) / sf;
			}
			else ind = p[index % np];

			float f = ef - (ra / 255) * ran - ind * z;

			if (ci == 0) if (f < 0) f = 0;
			if (cii == 0) if (f > 1) f = 1;

			int xx = 0, yy = 0;

			if (ch) {
				xx = x + pos[0] * f;
				yy = y + pos[1] * f;
			}
			else {
				xx = x + (pos[0] - x) * f;
				yy = y + (pos[1] - y) * f;
			}

			if (xx >= 0 && xx < w) {
				if (yy >= 0 && yy < h) {
					int ix = xx + w * yy;
					if (data[ix].b < vv) {
						work[ix].a = data[index].a;
						data[ix].b = vv;
					}
				}
			}
		}
	}

	if (c != 0) {//テキトーな補間
		for (y = 0; y < h; y++) {
			for (x = 0; x < w; x++) {
				int index = x + w * y;
				if (work[index].a == 0) {
					float cx = 0;
					int ix = 0;
					int count = 0, count_x = 0, count_y = 0;
					float r[4] = { 0 }, g[4] = { 0 }, b[4] = { 0 }, a[4] = { 0 }, k[4] = { 0 };

					while (1) {//右方向
						cx++;

						if (x + cx >= w)
							break;
						if (cx > c)
							break;

						ix = x + cx + w * y;
						if (work[ix].a != 0) {
							r[count] = work[ix].r;
							g[count] = work[ix].g;
							b[count] = work[ix].b;
							a[count] = work[ix].a;
							k[count] = cx;
							count++;
							count_x++;
							break;
						}
					}

					cx = 0;
					while (1) {//左方向
						cx--;

						if (x + cx < 0)
							break;
						if (-cx > c)
							break;

						ix = x + cx + w * y;
						if (work[ix].a != 0) {
							r[count] = work[ix].r;
							g[count] = work[ix].g;
							b[count] = work[ix].b;
							a[count] = work[ix].a;
							k[count] = -1 * cx;
							count++;
							count_x++;
							break;
						}
					}

					cx = 0;
					while (1) {//上方向
						cx++;

						if (y + cx >= h)
							break;
						if (cx > c)
							break;

						ix = x + w * (y + cx);
						if (work[ix].a != 0) {
							r[count] = work[ix].r;
							g[count] = work[ix].g;
							b[count] = work[ix].b;
							a[count] = work[ix].a;
							k[count] = cx;
							count++;
							count_y++;
							break;
						}
					}

					cx = 0;
					while (1) {//下方向
						cx--;

						if (y + cx < 0)
							break;
						if (-cx > c)
							break;

						ix = x + w * (y + cx);
						if (work[ix].a != 0) {
							r[count] = work[ix].r;
							g[count] = work[ix].g;
							b[count] = work[ix].b;
							a[count] = work[ix].a;
							k[count] = -1 * cx;
							count++;
							count_y++;
							break;
						}
					}

					if (count_x == 2 || count_y == 2) {

						float sg = 0;
						float kr = 0, kg = 0, kb = 0, ka = 0;


						for (i = 0; i < count; i++)
							if (sg < k[i]) sg = k[i];

						for (i = 0; i < count; i++)
							k[i] = sg / k[i];

						sg = 0;
						for (i = 0; i < count; i++)
							sg = sg + k[i];

						for (i = 0; i < count; i++) {
							float atai = k[i] / sg;
							kr = kr + r[i] * atai;
							kg = kg + g[i] * atai;
							kb = kb + b[i] * atai;
							ka = ka + a[i] * atai;
						}

						if (kr < 0) kr = 0; else if (kr > 255) kr = 255;
						if (kg < 0) kg = 0; else if (kg > 255) kg = 255;
						if (kb < 0) kb = 0; else if (kb > 255) kb = 255;
						if (ka < 0) ka = 0; else if (ka > 255) ka = 255;

						work[index].r = (int)kr;
						work[index].g = (int)kg;
						work[index].b = (int)kb;
						work[index].a = (int)ka;
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
MODULEDEFINE(M_Sand_Module)

