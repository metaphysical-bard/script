#pragma once

struct Pixel_RGBA {//RGBA形式の構造体
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};

class noise_M
{

public:/*
       理論はなんとなく理解できたのですが、いざコード実装しようと思っても
       「は？」となったので、noise_cppのコードは以下のリンク先のものそのままです。
       （変数名などはほんの一部変更しました）
       https://postd.cc/understanding-perlin-noise/
       */

    //データ破壊用
    void Mbreak(Pixel_RGBA* a, int w, int h);

    //swap
    void swap(int* a, int* b);

    //乱数の発生
    void randM(int* r, int seed);

    //fade
    double fade(double* t);

    //グラディエント関数
    double grad(int hash, double x, double y, double z);

    //線形補間
    double lerp(double a, double b, double x);

    //パリーンノイズの関数
    double perlin(double x, double y, double z, int* p);

    //複数オクターブ重ねる関数
    double OctavePerlin(double x, double y, double z, int octaves, double persistence, int* r);

};