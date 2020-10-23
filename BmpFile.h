#pragma once
#include "header.h"

/*TODO:
* 10/23
* 中间操作需要中间变量存储的，在操作函数内新建，在操作函数内释放
* 再创建一组用于保存save()图像信息的变量，作为默认的输出数据  与load()读取数据使用的变量相对应 - done
* 新建一个用于初始化上面的save()用的变量的方法，初始化必须初始化的数据，图像尺寸，图像文件大小等信息，再每个方法前调用刷新这个数据 - done
* 10/24
* 写双线性插值
* 写直方图均衡
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
    BmpFile();//构造
    ~BmpFile();//析构

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
