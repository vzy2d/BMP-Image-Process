#pragma once
#include "header.h"

/*TODO:
* �м������Ҫ�м�����洢�ģ��ڲ����������½����ڲ����������ͷ�
* �ٴ���һ�����ڱ���save()ͼ����Ϣ�ı�������ΪĬ�ϵ��������  ��load()��ȡ����ʹ�õı������Ӧ
* �½�һ�����ڳ�ʼ�������save()�õı����ķ�������ʼ�������ʼ�������ݣ�ͼ��ߴ磬ͼ���ļ���С����Ϣ����ÿ������ǰ����ˢ���������
* 
*/

enum ERRORS {
    OPEN_FILE_ERROR, SAVE_FILE_ERROR, ARGUMENT_ERROR
};

class BmpFile
{
private:
    /* data */
    std::string fileName_IN;
    uint8_t* rgbQuad;	//��ŵ�ɫ������
    uint8_t* R;
    uint8_t* G;	//���λͼ����
    uint8_t* B;
    uint8_t** bitmap_gray;	//���λͼ����
    uint8_t*** bitmap_color;
    //uint8_t* imArray_temp;	//���λͼ����
    bmpfileheader myFileHeader;
    bitmapheader myBitMapHeader;
    uint16_t sizeofrgbQuad;
public:
    BmpFile();//���캯��
    ~BmpFile();//��������

    void load(std::string);
    void save(std::string);
    void load(const char*);
    void save(const char*);

    uint8_t ImArray(uint32_t row, uint32_t col);
    uint8_t ImArray(uint32_t row, uint32_t col, uint8_t chn);

    void ImArray(uint32_t row, uint32_t col, uint16_t dat_in);
    void ImArray(uint32_t row, uint32_t col, uint8_t chn, uint16_t dat_in);

    void printImgInfo();

    bool isGrayImg();
    bool isColorImg();
};