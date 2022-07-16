#pragma once

struct Pixel_RGBA {//RGBA�`���̍\����
    unsigned char b;
    unsigned char g;
    unsigned char r;
    unsigned char a;
};

class noise_M
{

public:/*
       ���_�͂Ȃ�ƂȂ������ł����̂ł����A�����R�[�h�������悤�Ǝv���Ă�
       �u�́H�v�ƂȂ����̂ŁAnoise_cpp�̃R�[�h�͈ȉ��̃����N��̂��̂��̂܂܂ł��B
       �i�ϐ����Ȃǂ͂ق�̈ꕔ�ύX���܂����j
       https://postd.cc/understanding-perlin-noise/
       */

    //�f�[�^�j��p
    void Mbreak(Pixel_RGBA* a, int w, int h);

    //swap
    void swap(int* a, int* b);

    //�����̔���
    void randM(int* r, int seed);

    //fade
    double fade(double* t);

    //�O���f�B�G���g�֐�
    double grad(int hash, double x, double y, double z);

    //���`���
    double lerp(double a, double b, double x);

    //�p���[���m�C�Y�̊֐�
    double perlin(double x, double y, double z, int* p);

    //�����I�N�^�[�u�d�˂�֐�
    double OctavePerlin(double x, double y, double z, int octaves, double persistence, int* r);

};