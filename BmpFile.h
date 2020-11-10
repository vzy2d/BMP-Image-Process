#pragma once
#include "header.h"

//最临近
#define ZOOM_METHOD_1 1
//双线性插值
#define ZOOM_METHOD_2 2

/* 均值滤波 - 盒滤波器
* |1 1 1|
* |1 1 1|
* |1 1 1|
*/
#define FILTER_TYPE_AVE 1
/*  加权平均滤波器
* |1 2 1|
* |2 4 2|
* |1 2 1|
*/
#define FILTER_TYPE_Gauss 2
/*  领域平均滤波器
* |0 1 0|    |1 1 1|
* |1 0 1| or |1 0 1|
* |0 1 0|    |1 1 1|
*/
#define FILTER_TYPE_Neighbor 3
/* 中值滤波器
*/
#define FILTER_TYPE_MidVal 4

#define FILTER_TYPE_Laplace 5
#define FILTER_TYPE_Laplace2 6

#define FILTER_TYPE_Robet 7
#define FILTER_TYPE_Sobel 8

#define TRANS_TYPE_Log 1
#define TRANS_TYPE_Power 2
#define TRANS_TYPE_Linear 3
#define TRANS_TYPE_Reverse 4

enum ERRORS {
    OPEN_FILE_ERROR, SAVE_FILE_ERROR, ARGUMENT_ERROR
};

class BmpFile
{
private:
    /* data */
    std::string fileName_IN;
    //load()输入图像的Header、调色板、位图数据
    bmpfileheader myFileHeader;
    bitmapheader myBitMapHeader;
    uint8_t* rgbQuad;
    uint8_t** bitmap_gray;
    uint8_t*** bitmap_color;
    //save()输入图像的Header、调色板、位图数据
    bmpfileheader myFileHeader_out;
    bitmapheader myBitMapHeader_out;
    uint8_t** bitmap_gray_out;
    uint8_t*** bitmap_color_out;

    uint16_t sizeofrgbQuad;

    uint32_t last_ptr_bitmap_in_height;
    uint32_t last_ptr_bitmap_out_height;
public: 
    BmpFile();//构造
    ~BmpFile();//析构

    void load(std::string);
    void load(const char*);

    //图像处理功能 img_xx
    void img_ZoomIn(std::string file_path_out, uint8_t ZOOM_METHOD, uint8_t zoom_ratio);
    void img_ZoomOut(std::string file_path_out, uint8_t ZOOM_METHOD, uint8_t zoom_ratio);

    void img_HistogramEqualization(std::string file_path_out);

    //滤波器 img_filter_xx
    void img_filter(std::string file_path_out, uint8_t filter_type, uint8_t kernel_size);

    //灰度映射 img_GrayLevelTransformation
    void img_GrayLevelTransformation(std::string file_path_out, uint8_t trans_type);
    void img_GrayLevelTransformation(std::string file_path_out, uint8_t trans_type, float* trans_param);

    uint8_t ImArray(uint32_t row, uint32_t col);
    uint8_t ImArray(uint32_t row, uint32_t col, uint8_t chn);

    void ImArray(uint32_t row, uint32_t col, uint16_t dat_in);
    void ImArray(uint32_t row, uint32_t col, uint8_t chn, uint16_t dat_in);

    void printImgInfo();
    void printHist();

    void func_test();

    int round(float);
    int round(double);

    //TODO:
    void imgShow(uint8_t** ptr, uint32_t height, uint32_t width);
    void imgShow(uint8_t*** ptr, uint32_t height, uint32_t width);


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


    void img_HistogramEqualization_Gray(std::string file_path_out);
    void img_HistogramEqualization_Color(std::string file_path_out);

    void img_filter_ave(std::string file_path_out, uint8_t kernel_size);
    void img_filter_gauss(std::string file_path_out, uint8_t kernel_size);
    void img_filter_neighbor(std::string file_path_out, uint8_t kernel_size);

    void img_filter_midval(std::string file_path_out, uint8_t kernel_size);

    void img_filter_laplace(std::string file_path_out, uint8_t kernel_size);
    void img_filter_laplace2(std::string file_path_out, uint8_t kernel_size);

    void img_filter_robet(std::string file_path_out, uint8_t kernel_size);
    void img_filter_sobel(std::string file_path_out, uint8_t kernel_size);

    float generate_gauss(uint8_t kernel_size, int8_t kernel_row, int8_t kernel_col, float sigma);
    uint8_t get_midval(uint8_t* src_seq, uint8_t len);

    //>>>>>>>>>>>>>> 灰度映射
    void img_GLT_Reverse(std::string file_path_out);
    void img_GLT_Log(std::string file_path_out, float ratio_c);
    void img_GLT_Power(std::string file_path_out, float ratio_const, float ratio_power);
    void img_GLT_Linear(std::string file_path_out);
    //<<<<<<<<<<<<<< 灰度映射

    void init_ptr_bitmap_in(uint32_t height, uint32_t width); 
    void init_ptr_bitmap_out(uint32_t height, uint32_t width);

    bool isGrayImg();
    bool isColorImg();
    void refreshHeader_out(uint16_t bitsperpixel, uint32_t height, uint32_t width);
    void refreshHeader_out(uint32_t height, uint32_t width);
};
