#include "header.h"

using namespace std;

string dir_imgsrc = "C:\\Users\\zheng\\Desktop\\BMP-Image-Process\\images_src\\";
string dir_imgdst = "C:\\Users\\zheng\\Desktop\\BMP-Image-Process\\images_dst\\";

int main()
{
	BmpFile BmpPic;

	uint8_t work_num = 5;

	switch (work_num)
	{
	case 0:	//测试用
		BmpPic.func_test();
		break;
	case 1:	//实验1 BMP文件的读写
		break;
	case 2:	//实验2 图像缩放
		//2.1	最邻近
		BmpPic.load(dir_imgsrc + "lena.bmp");	//读入图片
		BmpPic.img_ZoomOut(dir_imgdst + "work2\\lenas.bmp", ZOOM_METHOD_1, 2);	//缩小2倍保存
		BmpPic.load(dir_imgdst + "work2\\lenas.bmp");
		BmpPic.img_ZoomIn(dir_imgdst + "work2\\lenasz.bmp", ZOOM_METHOD_1, 2);

		BmpPic.load(dir_imgsrc + "lena.bmp");
		BmpPic.img_ZoomOut(dir_imgdst + "work2\\lenas_4x.bmp", ZOOM_METHOD_1, 4);	//缩小4倍保存
		BmpPic.load(dir_imgdst + "work2\\lenas_4x.bmp");
		BmpPic.img_ZoomIn(dir_imgdst + "work2\\lenasz_4x.bmp", ZOOM_METHOD_1, 4);

		//2.2	双线性
		BmpPic.load(dir_imgsrc + "lena.bmp");
		BmpPic.img_ZoomOut(dir_imgdst + "work2\\lenas1.bmp", ZOOM_METHOD_2, 2);
		BmpPic.load(dir_imgdst + "work2\\lenas1.bmp");
		BmpPic.img_ZoomIn(dir_imgdst + "work2\\lenasz1.bmp", ZOOM_METHOD_2, 2);

		BmpPic.load(dir_imgsrc + "lena.bmp");
		BmpPic.img_ZoomOut(dir_imgdst + "work2\\lenas1_8x.bmp", ZOOM_METHOD_2, 8);	//缩小8倍保存
		BmpPic.load(dir_imgdst + "work2\\lenas1_8x.bmp");
		BmpPic.img_ZoomIn(dir_imgdst + "work2\\lenasz1_8x.bmp", ZOOM_METHOD_2, 8);	//放大8倍保存

		//2.3.1	彩色图像-最邻近
		BmpPic.load(dir_imgsrc + "lena_C.bmp");
		BmpPic.img_ZoomOut(dir_imgdst + "work2\\lenas_C.bmp", ZOOM_METHOD_1, 4);
		BmpPic.load(dir_imgdst + "work2\\lenas_C.bmp");
		BmpPic.img_ZoomIn(dir_imgdst + "work2\\lenasz_C.bmp", ZOOM_METHOD_1, 4);

		//2.3.2	彩色图像-双线性
		BmpPic.load(dir_imgsrc + "lena_C.bmp");
		BmpPic.img_ZoomOut(dir_imgdst + "work2\\lenas1_C.bmp", ZOOM_METHOD_2, 4);
		BmpPic.load(dir_imgdst + "work2\\lenas1_C.bmp");
		BmpPic.img_ZoomIn(dir_imgdst + "work2\\lenasz1_C.bmp", ZOOM_METHOD_2, 4);

		break;
	case 3:	//实验3 直方图均衡
		BmpPic.load(dir_imgsrc + "image1.bmp");
		BmpPic.img_HistogramEqualization(dir_imgdst + "work3\\image1_HE.bmp");

		BmpPic.load(dir_imgsrc + "image_color.bmp");
		BmpPic.img_HistogramEqualization(dir_imgdst + "work3\\image_color_HE.bmp");

		break;
	case 4:	//实验4 图像滤波
		BmpPic.load(dir_imgsrc + "Lena1.bmp");
		//盒滤波器
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_BOX_3X3.bmp", FILTER_TYPE_AVE, 3);
		//加权平均滤波器
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_Gauss_3X3.bmp", FILTER_TYPE_Gauss, 3);
		//邻域平均滤波器
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_Neighbor_3X3.bmp", FILTER_TYPE_Neighbor, 3);
		//中值滤波器 3x3
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_MidVal_3X3.bmp", FILTER_TYPE_MidVal, 3);
		//中值滤波器 5x5
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_MidVal_5X5.bmp", FILTER_TYPE_MidVal, 5);
		//中值滤波器 7x7
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_MidVal_7X7.bmp", FILTER_TYPE_MidVal, 7);

		BmpPic.load(dir_imgsrc + "Lena2.bmp");
		//盒滤波器
		BmpPic.img_filter(dir_imgdst + "work4\\Lena2_FILTER_BOX_3X3.bmp", FILTER_TYPE_AVE, 3);
		//加权平均滤波器
		BmpPic.img_filter(dir_imgdst + "work4\\Lena2_FILTER_Gauss_3X3.bmp", FILTER_TYPE_Gauss, 3);
		//邻域平均滤波器
		BmpPic.img_filter(dir_imgdst + "work4\\Lena2_FILTER_Neighbor_3X3.bmp", FILTER_TYPE_Neighbor, 3);
		//中值滤波器 3x3
		BmpPic.img_filter(dir_imgdst + "work4\\Lena2_FILTER_MidVal_3X3.bmp", FILTER_TYPE_MidVal, 3);
		//中值滤波器 5x5
		BmpPic.img_filter(dir_imgdst + "work4\\Lena2_FILTER_MidVal_5X5.bmp", FILTER_TYPE_MidVal, 5);
		//中值滤波器 7x7
		BmpPic.img_filter(dir_imgdst + "work4\\Lena2_FILTER_MidVal_7X7.bmp", FILTER_TYPE_MidVal, 7);

		BmpPic.load(dir_imgsrc + "lena.bmp");
		//Laplace算子
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_Laplace_3X3.bmp", FILTER_TYPE_Laplace, 3);
		//复合Laplace算子
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_Laplace2_3X3.bmp", FILTER_TYPE_Laplace2, 3);
		//Robet算子
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_Robet_3X3.bmp", FILTER_TYPE_Robet, 3);
		//Sobel算子
		BmpPic.img_filter(dir_imgdst + "work4\\Lena1_FILTER_Sobel_3X3.bmp", FILTER_TYPE_Sobel, 3);

		break;
	case 5:	//实验5 图像的灰度映射
		float trans_param[2];
		BmpPic.load(dir_imgsrc + "image1.bmp");
		BmpPic.img_GrayLevelTransformation(dir_imgdst + "work5\\image1_GLT_Reverse.bmp", TRANS_TYPE_Reverse);
		trans_param[0] = 30;//常系数
		BmpPic.img_GrayLevelTransformation(dir_imgdst + "work5\\image1_GLT_Log.bmp", TRANS_TYPE_Log, trans_param);
		trans_param[0] = 30;//常系数
		trans_param[1] = 0.5;//幂次
		BmpPic.img_GrayLevelTransformation(dir_imgdst + "work5\\image1_GLT_Power.bmp", TRANS_TYPE_Power, trans_param);
		BmpPic.img_GrayLevelTransformation(dir_imgdst + "work5\\image1_GLT_Linear.bmp", TRANS_TYPE_Linear);

		BmpPic.load(dir_imgsrc + "image2.bmp");
		BmpPic.printHist();
		BmpPic.img_GrayLevelTransformation(dir_imgdst + "work5\\image2_GLT_Reverse.bmp", TRANS_TYPE_Reverse);
		trans_param[0] = 50;
		BmpPic.img_GrayLevelTransformation(dir_imgdst + "work5\\image2_GLT_Log.bmp", TRANS_TYPE_Log, trans_param);
		trans_param[0] = 20;
		trans_param[1] = 0.5;
		BmpPic.img_GrayLevelTransformation(dir_imgdst + "work5\\image2_GLT_Power.bmp", TRANS_TYPE_Power, trans_param);
		BmpPic.img_GrayLevelTransformation(dir_imgdst + "work5\\image2_GLT_Linear.bmp", TRANS_TYPE_Linear);

		BmpPic.img_filter(dir_imgdst + "work5\\image2_FILTER_Laplace2_3X3.bmp", FILTER_TYPE_Laplace2, 3);
		break;
	default:
		cout << "Wrong WorkNum." << endl;
		break;
	}



	return 0;
}