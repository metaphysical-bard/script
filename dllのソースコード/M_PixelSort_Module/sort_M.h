#pragma once

struct Pixel_RGBA {//RGBA形式の構造体
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};

struct Mch {//チェック用の構造体
    bool a;//領域内のピクセルかどうか
    bool k;//輝度幅に該当するかどうか
};

class sort_M
{
public://絶対にもっと効率的に書けるよね…似たコードが多すぎる。

    //円周率
    double PI = 3.141592653589793;

    //メモリ確保失敗時にdataを壊すもの
    void Mbreak(Pixel_RGBA* a, int w, int h);

    //リサイズ(線形補間
    void rsize(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh);

    //回転(最近傍補間
    void SimpleRotation(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh, double si, Mch* mch, double TX, double TY);

    //回転(線形補間
    void rotation(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh, double si, Mch* mch, double TX, double TY);

    //回転(線形補間(alpha対策用
    void arotation(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh, double si, Mch* mch);

    //dataをもとに、輝度を求めてkに代入
    void koudo(Pixel_RGBA* data, float* k, int max, Mch* mch, float tk, float dk, double TR, double TG, double TB);

    //クイックソート(ちょい改造
    void qsort(Pixel_RGBA* a, int f, int l, float* k);

    //逆クイックソート
    void gqsort(Pixel_RGBA* a, int f, int l, float* k);

    //クイックソートに処理を投げるための関数
    void pixelsort(Pixel_RGBA* a, int w, int h, float* k, float tk, float dk, bool moto, Mch* mch, bool gya);

    //引き伸ばし
    void stretch(Pixel_RGBA* a, int w, int h, double str, Mch* mch, int seed, double Thi);

    //回転用の関数
    void BR(Pixel_RGBA* data, Pixel_RGBA* work, int w, int h, int x, int y, double sin_S, double cos_S, double Pw, double Ph, bool ch);

    //エッジ抽出
    void Medge(Pixel_RGBA* data, float* q, float* q1, float* q2, int w, int h);

    //ぼかし
    void Bokasi(Pixel_RGBA* work, Pixel_RGBA* data, int w, int h, float* q);

};