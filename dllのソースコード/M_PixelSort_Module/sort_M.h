#pragma once

struct Pixel_RGBA {//RGBA�`���̍\����
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};

struct Mch {//�`�F�b�N�p�̍\����
    bool a;//�̈���̃s�N�Z�����ǂ���
    bool k;//�P�x���ɊY�����邩�ǂ���
};

class sort_M
{
public://��΂ɂ����ƌ����I�ɏ������ˁc�����R�[�h����������B

    //�~����
    double PI = 3.141592653589793;

    //�������m�ێ��s����data���󂷂���
    void Mbreak(Pixel_RGBA* a, int w, int h);

    //���T�C�Y(���`���
    void rsize(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh);

    //��](�ŋߖT���
    void SimpleRotation(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh, double si, Mch* mch, double TX, double TY);

    //��](���`���
    void rotation(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh, double si, Mch* mch, double TX, double TY);

    //��](���`���(alpha�΍��p
    void arotation(Pixel_RGBA* data, int w, int h, Pixel_RGBA* work, int bw, int bh, double si, Mch* mch);

    //data�����ƂɁA�P�x�����߂�k�ɑ��
    void koudo(Pixel_RGBA* data, float* k, int max, Mch* mch, float tk, float dk, double TR, double TG, double TB);

    //�N�C�b�N�\�[�g(���傢����
    void qsort(Pixel_RGBA* a, int f, int l, float* k);

    //�t�N�C�b�N�\�[�g
    void gqsort(Pixel_RGBA* a, int f, int l, float* k);

    //�N�C�b�N�\�[�g�ɏ����𓊂��邽�߂̊֐�
    void pixelsort(Pixel_RGBA* a, int w, int h, float* k, float tk, float dk, bool moto, Mch* mch, bool gya);

    //�����L�΂�
    void stretch(Pixel_RGBA* a, int w, int h, double str, Mch* mch, int seed, double Thi);

    //��]�p�̊֐�
    void BR(Pixel_RGBA* data, Pixel_RGBA* work, int w, int h, int x, int y, double sin_S, double cos_S, double Pw, double Ph, bool ch);

    //�G�b�W���o
    void Medge(Pixel_RGBA* data, float* q, float* q1, float* q2, int w, int h);

    //�ڂ���
    void Bokasi(Pixel_RGBA* work, Pixel_RGBA* data, int w, int h, float* q);

};