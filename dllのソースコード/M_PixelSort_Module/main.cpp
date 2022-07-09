#include <lua.hpp>
#include "sort_M.h"
#include <new>
#include<omp.h>

/*struct Pixel_RGBA {
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};*/

int s(lua_State* L) {
    //s(data,w,h,T0,T1,T2,T3,g,m,ch,sd[1],sd[2],sd[3],sd[4],sd[5],sd[6],sd[7])
    //データのポインタ、縦ピクセル数、横ピクセル数、角度(ラジアン)、基準輝度(0~255)、輝度幅(0~100)、引き伸ばし(0~100)、逆順(bool)、元画像(bool)、軽処理(bool)
    //2.99(R)、5.87(G)、1.14(B)、ランダム引き伸ばし(double)、ベクトルX、ベクトルY、メモリ増量、seed(int)

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA形式データ
    int w = static_cast<int>(lua_tointeger(L, 2));//横ピクセル
    int h = static_cast<int>(lua_tointeger(L, 3));//縦ピクセル
    double si= static_cast<double>(lua_tonumber(L, 4));//角度(ラジアン
    float tk = static_cast<float>(lua_tonumber(L, 5));//基準輝度
    float dk = static_cast<float>(lua_tonumber(L, 6) / 100);//輝度幅

    //tkとdkを確定(0~256)
    if (1) {
        float kk = tk, ha = dk;
        tk = kk + (256 - kk) * ha;
        dk = kk + (0 - kk) * ha;
    }

    double str = static_cast<double>(lua_tonumber(L, 7));//引き伸ばし
    bool gya = static_cast<bool>(lua_tointeger(L, 8));//逆順
    bool moto = static_cast<bool>(lua_tointeger(L, 9));//元画像
    bool ch= static_cast<bool>(lua_tointeger(L, 10));//軽量化

    double TR = static_cast<double>(lua_tonumber(L, 11));//2.99(R)
    double TG = static_cast<double>(lua_tonumber(L, 12));//5.87(G)
    double TB = static_cast<double>(lua_tonumber(L, 13));//1.14(B)

    //TR*255+TG*255+TB*255=255になるように調整
    if (1) {
        double S = TR * 255 + TG * 255 + TB * 255;
        if (S == 0) {
            TR = 0;
            TG = 0;
            TB = 0;
        }
        else {
            TR = TR * 255 / S;
            TG = TG * 255 / S;
            TB = TB * 255 / S;
        }
    }

    double Thi = static_cast<double>(lua_tonumber(L, 14) / 100);//ランダム引き伸ばし
    double TX = static_cast<double>(lua_tonumber(L, 15));//ベクトルX
    double TY = static_cast<double>(lua_tonumber(L, 16));//ベクトルY
    double Memori = static_cast<double>(lua_tonumber(L, 17));//メモリ増量

    if (Memori < 0) Memori = 0;

    int seed = static_cast<int>(lua_tointeger(L, 18));//seed



    sort_M obj;//クラス

   
    int wh = (int)(sqrt(w * w + h * h) + 2 + Memori);//ここの求め方が分からない…。とりあえず対角線の長さで…
    Pixel_RGBA* work = 0;//回転後のピクセルを入れる用の領域
    work = new (std::nothrow)Pixel_RGBA[wh * wh];//メモリ確保
    if (work == NULL) {
        obj.Mbreak(data, w, h);//失敗時にdataを破壊
        return -1;
    }

    float* k = 0;//輝度を入れる用
    k = new (std::nothrow)float[wh * wh];//メモリ確保
    if (k == NULL) {
        obj.Mbreak(data, w, h);//失敗時にdataを破壊
        delete[] work;
        return -1;
    }

    Mch* mch = 0;//チェック用
    mch = new (std::nothrow)Mch[wh * wh];//メモリ確保
    if (mch == NULL) {
        obj.Mbreak(data, w, h);//失敗時にdataを破壊
        delete[] work;
        delete[] k;
        return -1;
    }
   

    if (ch) obj.SimpleRotation(data, w, h, work, wh, wh, -si, mch, 0, 0);//回転
    else obj.arotation(data, w, h, work, wh, wh, -si, mch);

    obj.koudo(work, k, wh * wh, mch, tk, dk, TR, TG, TB);//輝度の計算

    obj.pixelsort(work, wh, wh, k, tk, dk, moto, mch, gya);//ここでソート

    obj.stretch(work, wh, wh, str, mch, seed, Thi);//引き伸ばし

    if (ch) obj.SimpleRotation(work, wh, wh, data, w, h, si, mch, TX, TY);//逆回転
    else obj.rotation(work, wh, wh, data, w, h, si, mch, TX, TY);
    

    delete[] work;//メモリ解放
    delete[] k;
    delete[] mch;

    return 0;
}

int k(lua_State* L) {

    sort_M obj;//クラス

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA形式のデータ
    Pixel_RGBA* work = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 2));//空のデータ
    int w = static_cast<int>(lua_tointeger(L, 3));//縦ピクセル
    int h = static_cast<int>(lua_tointeger(L, 4));//横ピクセル
    double k = static_cast<double>(lua_tonumber(L, 5));//全体回転(度数法)
    k = k * obj.PI / 180;
    double t = static_cast<double>(lua_tonumber(L, 6));//ねじれ(度数法)
    t = t * obj.PI / 180;
    double shi = static_cast<double>(lua_tonumber(L, 7) / 100);//シフト
    double bai = static_cast<double>(lua_tonumber(L, 8));//シフトの倍率
    shi = shi * bai;
    double Pw = static_cast<double>((float)w / 2 + lua_tonumber(L, 9));//中心X
    double Ph = static_cast<double>((float)h / 2 + lua_tonumber(L, 10));//中心Y
    bool ch= static_cast<bool>(lua_tointeger(L, 11));//隙間補間の有無

    double* MK = 0;
    MK = new(std::nothrow)double[w * h];//メモリかくほー
    if (MK == NULL) {
        obj.Mbreak(work, w, h);
        return -1;
    }

    double MMax = sqrt(Pw * Pw + Ph * Ph);//中心点との最大の距離

    for (int y = 0; y < h; y++) {//MKに中心との距離/最大の距離
        int wy = w * y;
#pragma omp parallel for
        for (int x = 0; x < w; x++) {
            float cx = x - Pw, cy = y - Ph;
            *(MK + wy + x) = sqrt(cx * cx + cy * cy) / MMax;
        }
    }

    for (int y = 0; y < h; y++) {//ぐるぐるループ～～～
        int wy = w * y;
#pragma omp parallel for
        for (int x = 0; x < w; x++) {
            double C = (t + (k - t) * *(MK + wy + x)) * shi;//ラジアンを求める
            obj.BR(data, work, w, h, x, y, sin(C), cos(C), Pw, Ph, ch);
        }
    }

    delete[] MK;//メモリかいほー
    return 0;
}

int a(lua_State* L) {

    sort_M obj;//クラス

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA形式のデータ
    Pixel_RGBA* work = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 2));//空のデータ
    int w = static_cast<int>(lua_tointeger(L, 3));//縦ピクセル
    int h = static_cast<int>(lua_tointeger(L, 4));//横ピクセル
    double K = static_cast<double>(lua_tonumber(L, 5) / 100);//強度
    if (K < 0) K = 0;

    float* q = 0;
    q = new(std::nothrow)float[w * h];//メモリかくほー
    if (q == NULL) {
        obj.Mbreak(work, w, h);
        return -1;
    }

    float* q1 = 0;
    q1 = new(std::nothrow)float[w * h];//メモリかくほー
    if (q1 == NULL) {
        obj.Mbreak(work, w, h);
        delete[] q;
        return -1;
    }

    float* q2 = 0;
    q2 = new(std::nothrow)float[w * h];//メモリかくほー
    if (q2 == NULL) {
        obj.Mbreak(work, w, h);
        delete[] q;
        delete[] q1;
        return -1;
    }

    obj.Medge(data, q, q1, q2, w, h);//エッジ抽出

    delete[] q1;//いらないのでポイ
    delete[] q2;

#pragma omp parallel for
    for (int i = 0; i < w * h; i++) {//強度の反映
        *(q + i) = *(q + i) / 255 * K;
    }

    obj.Bokasi(data, work, w, h, q);//ぼかし

    delete[] q;//メモリかいほー
    return 0;
}

int t(lua_State* L) {

    sort_M obj;//クラス

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA形式のデータ
    int w = static_cast<int>(lua_tointeger(L, 2));//縦ピクセル
    int h = static_cast<int>(lua_tointeger(L, 3));//横ピクセル
    double K = static_cast<double>((100 - lua_tonumber(L, 4)) / 100);//劣化

    int bw = static_cast<int>(w * K);
    int bh = static_cast<int>(h * K);

    if (bw == 0) bw = 1;
    if (bh == 0) bh = 1;

    Pixel_RGBA* work = 0;
    work = new (std::nothrow)Pixel_RGBA[bw * bh];//メモリ確保
    if (work == NULL) {
        obj.Mbreak(data, w, h);
        return -1;
    }

    obj.rsize(data, w, h, work, bw, bh);

    obj.rsize(work, bw, bh, data, w, h);
   
    delete[] work;
    return 0;
}

int o(lua_State* L) {

    sort_M obj;//クラス

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA形式のデータ
    int w = static_cast<int>(lua_tointeger(L, 2));//縦ピクセル
    int h = static_cast<int>(lua_tointeger(L, 3));//横ピクセル
    
    float* k = 0;
    k = new (std::nothrow)float[w * h];//メモリ確保
    if (k == NULL) {
        obj.Mbreak(data, w, h);
        return -1;
    }

#pragma omp parallel for
    for (int i = 0; i < w * h; i++)
        *(k + i) = (0.299 * data[i].r + 0.587 * data[i].g + 0.114 * data[i].b) * data[i].a / 255;

    obj.qsort(data, 0, w * h - 1, k);

    delete[] k;
    return 0;
}

static luaL_Reg functions[] = {
    { "s", s },//ピクセルソート
    { "k", k },//回転
    { "a", a },//アンチエイリアス(エッジによってぼかしを変える)
    { "t", t },//画質劣化(最終的に没となったrsize関数の供養。手抜き足抜き
    { "o", o },//ただソートするだけ。クイックソートの素晴らしさを感じるために使う
    { nullptr, nullptr }
};

extern "C" {
    __declspec(dllexport) int luaopen_M_PixelSort_Module(lua_State* L) {
        luaL_register(L, "M_PixelSort_Module", functions);
        return 1;
    }
}