#pragma once
#include "header.h"

/*TODO:
* 10/23
* �м������Ҫ�м�����洢�ģ��ڲ����������½����ڲ����������ͷ�
* �ٴ���һ�����ڱ���save()ͼ����Ϣ�ı�������ΪĬ�ϵ��������  ��load()��ȡ����ʹ�õı������Ӧ - done
* �½�һ�����ڳ�ʼ�������save()�õı����ķ�������ʼ�������ʼ�������ݣ�ͼ��ߴ磬ͼ���ļ���С����Ϣ����ÿ������ǰ����ˢ��������� - done
* 10/24
* д˫���Բ�ֵ
* дֱ��ͼ����
*/

enum ERRORS {
    OPEN_FILE_ERROR, SAVE_FILE_ERROR, ARGUMENT_ERROR
};

class BmpFile
{
private:
    /* data */
    std::string fileName_IN;
    uint8_t* rgbQuad;
    uint8_t** bitmap_gray;
    uint8_t*** bitmap_color;
    uint8_t** bitmap_gray_out;
    uint8_t*** bitmap_color_out;
    bmpfileheader myFileHeader;
    bitmapheader myBitMapHeader;
    bmpfileheader myFileHeader_out;
    bitmapheader myBitMapHeader_out;
    uint16_t sizeofrgbQuad;

    uint32_t last_ptr_bitmap_in_height;
    uint32_t last_ptr_bitmap_out_height;
public: 
    BmpFile();//����
    ~BmpFile();//����

    void load(std::string);
    void load(const char*);

    void zoom_in(std::string file_path_out, bool ZOOM_METHOD, uint8_t zoom_ratio);
    void zoom_out(std::string file_path_out, bool ZOOM_METHOD, uint8_t zoom_ratio);

    void refreshHeader_out(uint16_t bitsperpixel, uint32_t height, uint32_t width);

    uint8_t ImArray(uint32_t row, uint32_t col);
    uint8_t ImArray(uint32_t row, uint32_t col, uint8_t chn);

    void ImArray(uint32_t row, uint32_t col, uint16_t dat_in);
    void ImArray(uint32_t row, uint32_t col, uint8_t chn, uint16_t dat_in);

    void printImgInfo();

    bool isGrayImg();
    bool isColorImg();

    void save_test(std::string);
protected:
    void save(const char*);
    void save(std::string);
    void save_Gray(std::string);
    void save_Color(std::string);

    void zoom_in_Gray_M1(std::string file_path_out, uint8_t zoom_ratio);
    void zoom_in_Gray_M2(std::string file_path_out, uint8_t zoom_ratio);
    void zoom_out_Gray_M1(std::string file_path_out, uint8_t zoom_ratio);
    void zoom_out_Gray_M2(std::string file_path_out, uint8_t zoom_ratio);

    void zoom_in_Color_M1(std::string file_path_out, uint8_t zoom_ratio);
    void zoom_in_Color_M2(std::string file_path_out, uint8_t zoom_ratio);
    void zoom_out_Color_M1(std::string file_path_out, uint8_t zoom_ratio);
    void zoom_out_Color_M2(std::string file_path_out, uint8_t zoom_ratio);

    void init_ptr_bitmap_in(uint32_t height, uint32_t width); 
    void init_ptr_bitmap_out(uint32_t height, uint32_t width);

};
