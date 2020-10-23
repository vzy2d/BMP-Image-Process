#pragma once
//结构体，位图文件头，长度为14字节。
typedef struct {
	char  filetype[2];  //指定文件类型,必须是 Ox424D，即字符串"BM"
	uint32_t   filesize;  //指定文件大小，包括位图文件头的14个字节。
	uint16_t  reserved1;
	uint16_t  reserved2;
	uint32_t   bitmapoffset;  //从文件头到实际位图数据的偏移字节数
}bmpfileheader;

//结构体，位图信息头，长度为40字节。
typedef struct {
	uint32_t   sizeofinfoheader;  //指定该结构体的长度，40字节。
	uint32_t   width;   //指定图像的宽度，单位是像素。
	uint32_t   height;  //指定图像的高度，单位是像素。
	uint16_t  planes;  //必须是1。
	uint16_t  bitsperpixel; //指定表示颜色时要用到的位数，常用值为1（黑白二色图）、4（16色图）、8（256色图）、24（真彩色图）；本文采用8。
	uint32_t   compression;  //压缩方式，一般为0即不压缩
	uint32_t   sizeofbitmap;  //指定实际的位图数据占用的字节数，sizeofbitmap=width'*height。
	uint32_t   horzres;   //水平分辨率
	uint32_t   vertres;   //垂直分辨率
	uint32_t   colorsused;  //指定本图像实际用到的颜色数，如果该值为零，则用到的颜色数为2的bitsperpixel次幂。
	uint32_t   colorsimp;
}bitmapheader;

#define R_chn 2
#define G_chn 1
#define B_chn 0

//定义图像访问操作
/*
	i^
	|
	|
	|
	|
----|----------> j
(0,0)
*/