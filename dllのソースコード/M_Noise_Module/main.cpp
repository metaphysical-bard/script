#include <lua.hpp>
#include "noise_M.h"
#include <new>
#include<omp.h>

int s(lua_State* L) {

    noise_M obj;//クラス

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA形式のデータ
    int w = static_cast<int>(lua_tointeger(L, 2));//縦ピクセル
    int h = static_cast<int>(lua_tointeger(L, 3));//横ピクセル
    double ef = static_cast<double>(lua_tonumber(L, 4) / 100);
    int octaves = static_cast<int>(lua_tointeger(L, 5));
    double SubSize = static_cast<double>(lua_tonumber(L, 6));
    int seed = static_cast<int>(lua_tointeger(L, 7));//seed値

    int* r = 0;//縦用の乱数テーブル
    r = new (std::nothrow)int[512];//メモリ確保
    if (r == NULL) {
        obj.Mbreak(data, w, h);//失敗時にdataを破壊
        return -1;
    }

    obj.randM(r, seed);
    double d = 0;

#pragma omp parallel for
    for (int y = 0; y < h; y++) {//ぐるぐるループ～～～
        int wy = w * y;
        for (int x = 0; x < w; x++) {
            d = obj.OctavePerlin((double)x * ef, (double)y * ef, (double)0, octaves, SubSize, r);
            if (d < 0) d = 0;
            else if (d > 1) d = 255;
            else d = 255 * d;
            data[wy + x].r = static_cast<unsigned char>(d);
            data[wy + x].g = static_cast<unsigned char>(d);
            data[wy + x].b = static_cast<unsigned char>(d);
        }
    }

    delete[] r;

    return 0;
}

static luaL_Reg functions[] = {
    { "s", s },
    { nullptr, nullptr }
};

extern "C" {
    __declspec(dllexport) int luaopen_M_Noise_Module(lua_State* L) {
        luaL_register(L, "M_Noise_Module", functions);
        return 1;
    }
}