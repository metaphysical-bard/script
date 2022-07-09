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
    //�f�[�^�̃|�C���^�A�c�s�N�Z�����A���s�N�Z�����A�p�x(���W�A��)�A��P�x(0~255)�A�P�x��(0~100)�A�����L�΂�(0~100)�A�t��(bool)�A���摜(bool)�A�y����(bool)
    //2.99(R)�A5.87(G)�A1.14(B)�A�����_�������L�΂�(double)�A�x�N�g��X�A�x�N�g��Y�A���������ʁAseed(int)

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA�`���f�[�^
    int w = static_cast<int>(lua_tointeger(L, 2));//���s�N�Z��
    int h = static_cast<int>(lua_tointeger(L, 3));//�c�s�N�Z��
    double si= static_cast<double>(lua_tonumber(L, 4));//�p�x(���W�A��
    float tk = static_cast<float>(lua_tonumber(L, 5));//��P�x
    float dk = static_cast<float>(lua_tonumber(L, 6) / 100);//�P�x��

    //tk��dk���m��(0~256)
    if (1) {
        float kk = tk, ha = dk;
        tk = kk + (256 - kk) * ha;
        dk = kk + (0 - kk) * ha;
    }

    double str = static_cast<double>(lua_tonumber(L, 7));//�����L�΂�
    bool gya = static_cast<bool>(lua_tointeger(L, 8));//�t��
    bool moto = static_cast<bool>(lua_tointeger(L, 9));//���摜
    bool ch= static_cast<bool>(lua_tointeger(L, 10));//�y�ʉ�

    double TR = static_cast<double>(lua_tonumber(L, 11));//2.99(R)
    double TG = static_cast<double>(lua_tonumber(L, 12));//5.87(G)
    double TB = static_cast<double>(lua_tonumber(L, 13));//1.14(B)

    //TR*255+TG*255+TB*255=255�ɂȂ�悤�ɒ���
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

    double Thi = static_cast<double>(lua_tonumber(L, 14) / 100);//�����_�������L�΂�
    double TX = static_cast<double>(lua_tonumber(L, 15));//�x�N�g��X
    double TY = static_cast<double>(lua_tonumber(L, 16));//�x�N�g��Y
    double Memori = static_cast<double>(lua_tonumber(L, 17));//����������

    if (Memori < 0) Memori = 0;

    int seed = static_cast<int>(lua_tointeger(L, 18));//seed



    sort_M obj;//�N���X

   
    int wh = (int)(sqrt(w * w + h * h) + 2 + Memori);//�����̋��ߕ���������Ȃ��c�B�Ƃ肠�����Ίp���̒����Łc
    Pixel_RGBA* work = 0;//��]��̃s�N�Z��������p�̗̈�
    work = new (std::nothrow)Pixel_RGBA[wh * wh];//�������m��
    if (work == NULL) {
        obj.Mbreak(data, w, h);//���s����data��j��
        return -1;
    }

    float* k = 0;//�P�x������p
    k = new (std::nothrow)float[wh * wh];//�������m��
    if (k == NULL) {
        obj.Mbreak(data, w, h);//���s����data��j��
        delete[] work;
        return -1;
    }

    Mch* mch = 0;//�`�F�b�N�p
    mch = new (std::nothrow)Mch[wh * wh];//�������m��
    if (mch == NULL) {
        obj.Mbreak(data, w, h);//���s����data��j��
        delete[] work;
        delete[] k;
        return -1;
    }
   

    if (ch) obj.SimpleRotation(data, w, h, work, wh, wh, -si, mch, 0, 0);//��]
    else obj.arotation(data, w, h, work, wh, wh, -si, mch);

    obj.koudo(work, k, wh * wh, mch, tk, dk, TR, TG, TB);//�P�x�̌v�Z

    obj.pixelsort(work, wh, wh, k, tk, dk, moto, mch, gya);//�����Ń\�[�g

    obj.stretch(work, wh, wh, str, mch, seed, Thi);//�����L�΂�

    if (ch) obj.SimpleRotation(work, wh, wh, data, w, h, si, mch, TX, TY);//�t��]
    else obj.rotation(work, wh, wh, data, w, h, si, mch, TX, TY);
    

    delete[] work;//���������
    delete[] k;
    delete[] mch;

    return 0;
}

int k(lua_State* L) {

    sort_M obj;//�N���X

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA�`���̃f�[�^
    Pixel_RGBA* work = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 2));//��̃f�[�^
    int w = static_cast<int>(lua_tointeger(L, 3));//�c�s�N�Z��
    int h = static_cast<int>(lua_tointeger(L, 4));//���s�N�Z��
    double k = static_cast<double>(lua_tonumber(L, 5));//�S�̉�](�x���@)
    k = k * obj.PI / 180;
    double t = static_cast<double>(lua_tonumber(L, 6));//�˂���(�x���@)
    t = t * obj.PI / 180;
    double shi = static_cast<double>(lua_tonumber(L, 7) / 100);//�V�t�g
    double bai = static_cast<double>(lua_tonumber(L, 8));//�V�t�g�̔{��
    shi = shi * bai;
    double Pw = static_cast<double>((float)w / 2 + lua_tonumber(L, 9));//���SX
    double Ph = static_cast<double>((float)h / 2 + lua_tonumber(L, 10));//���SY
    bool ch= static_cast<bool>(lua_tointeger(L, 11));//���ԕ�Ԃ̗L��

    double* MK = 0;
    MK = new(std::nothrow)double[w * h];//�����������ف[
    if (MK == NULL) {
        obj.Mbreak(work, w, h);
        return -1;
    }

    double MMax = sqrt(Pw * Pw + Ph * Ph);//���S�_�Ƃ̍ő�̋���

    for (int y = 0; y < h; y++) {//MK�ɒ��S�Ƃ̋���/�ő�̋���
        int wy = w * y;
#pragma omp parallel for
        for (int x = 0; x < w; x++) {
            float cx = x - Pw, cy = y - Ph;
            *(MK + wy + x) = sqrt(cx * cx + cy * cy) / MMax;
        }
    }

    for (int y = 0; y < h; y++) {//���邮�郋�[�v�`�`�`
        int wy = w * y;
#pragma omp parallel for
        for (int x = 0; x < w; x++) {
            double C = (t + (k - t) * *(MK + wy + x)) * shi;//���W�A�������߂�
            obj.BR(data, work, w, h, x, y, sin(C), cos(C), Pw, Ph, ch);
        }
    }

    delete[] MK;//�����������ف[
    return 0;
}

int a(lua_State* L) {

    sort_M obj;//�N���X

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA�`���̃f�[�^
    Pixel_RGBA* work = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 2));//��̃f�[�^
    int w = static_cast<int>(lua_tointeger(L, 3));//�c�s�N�Z��
    int h = static_cast<int>(lua_tointeger(L, 4));//���s�N�Z��
    double K = static_cast<double>(lua_tonumber(L, 5) / 100);//���x
    if (K < 0) K = 0;

    float* q = 0;
    q = new(std::nothrow)float[w * h];//�����������ف[
    if (q == NULL) {
        obj.Mbreak(work, w, h);
        return -1;
    }

    float* q1 = 0;
    q1 = new(std::nothrow)float[w * h];//�����������ف[
    if (q1 == NULL) {
        obj.Mbreak(work, w, h);
        delete[] q;
        return -1;
    }

    float* q2 = 0;
    q2 = new(std::nothrow)float[w * h];//�����������ف[
    if (q2 == NULL) {
        obj.Mbreak(work, w, h);
        delete[] q;
        delete[] q1;
        return -1;
    }

    obj.Medge(data, q, q1, q2, w, h);//�G�b�W���o

    delete[] q1;//����Ȃ��̂Ń|�C
    delete[] q2;

#pragma omp parallel for
    for (int i = 0; i < w * h; i++) {//���x�̔��f
        *(q + i) = *(q + i) / 255 * K;
    }

    obj.Bokasi(data, work, w, h, q);//�ڂ���

    delete[] q;//�����������ف[
    return 0;
}

int t(lua_State* L) {

    sort_M obj;//�N���X

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA�`���̃f�[�^
    int w = static_cast<int>(lua_tointeger(L, 2));//�c�s�N�Z��
    int h = static_cast<int>(lua_tointeger(L, 3));//���s�N�Z��
    double K = static_cast<double>((100 - lua_tonumber(L, 4)) / 100);//��

    int bw = static_cast<int>(w * K);
    int bh = static_cast<int>(h * K);

    if (bw == 0) bw = 1;
    if (bh == 0) bh = 1;

    Pixel_RGBA* work = 0;
    work = new (std::nothrow)Pixel_RGBA[bw * bh];//�������m��
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

    sort_M obj;//�N���X

    Pixel_RGBA* data = reinterpret_cast<Pixel_RGBA*>(lua_touserdata(L, 1));//RGBA�`���̃f�[�^
    int w = static_cast<int>(lua_tointeger(L, 2));//�c�s�N�Z��
    int h = static_cast<int>(lua_tointeger(L, 3));//���s�N�Z��
    
    float* k = 0;
    k = new (std::nothrow)float[w * h];//�������m��
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
    { "s", s },//�s�N�Z���\�[�g
    { "k", k },//��]
    { "a", a },//�A���`�G�C���A�X(�G�b�W�ɂ���Ăڂ�����ς���)
    { "t", t },//�掿��(�ŏI�I�ɖv�ƂȂ���rsize�֐��̋��{�B�蔲��������
    { "o", o },//�����\�[�g���邾���B�N�C�b�N�\�[�g�̑f���炵���������邽�߂Ɏg��
    { nullptr, nullptr }
};

extern "C" {
    __declspec(dllexport) int luaopen_M_PixelSort_Module(lua_State* L) {
        luaL_register(L, "M_PixelSort_Module", functions);
        return 1;
    }
}