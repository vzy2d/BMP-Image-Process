#pragma once
#include "header.h"

/*TODO:
* 中间操作需要中间变量存储的，在操作函数内新建，在操作函数内释放
* 再创建一组用于保存save()图像信息的变量，作为默认的输出数据  与load()读取数据使用的变量相对应
* 新建一个用于初始化上面的save()用的变量的方法，初始化必须初始化的数据，图像尺寸，图像文件大小等信息，再每个方法前调用刷新这个数据
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
    uint8_t* rgbQuad;	//存放调色板数据
    uint8_t* R;
    uint8_t* G;	//存放位图数据
    uint8_t* B;
    uint8_t** bitmap_gray;	//存放位图数据
    uint8_t*** bitmap_color;
    //uint8_t* imArray_temp;	//存放位图数据
    bmpfileheader myFileHeader;
    bitmapheader myBitMapHeader;
    uint16_t sizeofrgbQuad;
public:
    BmpFile();//构造函数
    ~BmpFile();//析构函数

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