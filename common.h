#pragma once
//�ṹ�壬λͼ�ļ�ͷ������Ϊ14�ֽڡ�
typedef struct {
	char  filetype[2];  //ָ���ļ�����,������ Ox424D�����ַ���"BM"
	unsigned long   filesize;  //ָ���ļ���С������λͼ�ļ�ͷ��14���ֽڡ�
	short  reserved1;
	short  reserved2;
	unsigned long   bitmapoffset;  //���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
}bmpfileheader;

//�ṹ�壬λͼ��Ϣͷ������Ϊ40�ֽڡ�
typedef struct {
	unsigned long   sizeofinfoheader;  //ָ���ýṹ��ĳ��ȣ�40�ֽڡ�
	int32_t   width;   //ָ��ͼ��Ŀ�ȣ���λ�����ء�
	int32_t   height;  //ָ��ͼ��ĸ߶ȣ���λ�����ء�
	unsigned short  planes;  //������1��
	unsigned short  bitsperpixel; //ָ����ʾ��ɫʱҪ�õ���λ��������ֵΪ1���ڰ׶�ɫͼ����4��16ɫͼ����8��256ɫͼ����24�����ɫͼ�������Ĳ���8��
	unsigned int   compression;  //ѹ����ʽ��һ��Ϊ0����ѹ��
	unsigned int   sizeofbitmap;  //ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ�����sizeofbitmap=width'*height��
	unsigned int   horzres;   //ˮƽ�ֱ���
	unsigned int   vertres;   //��ֱ�ֱ���
	unsigned int   colorsused;  //ָ����ͼ��ʵ���õ�����ɫ���������ֵΪ�㣬���õ�����ɫ��Ϊ2��bitsperpixel���ݡ�
	unsigned int   colorsimp;
}bitmapheader;

#define R_chn 0
#define G_chn 1
#define B_chn 2

//����ͼ����ʲ���
/*
	i^
	|
	|
	|
	|
----|----------> j
(0,0)
*/