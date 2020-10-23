#pragma once
//�ṹ�壬λͼ�ļ�ͷ������Ϊ14�ֽڡ�
typedef struct {
	char  filetype[2];  //ָ���ļ�����,������ Ox424D�����ַ���"BM"
	uint32_t   filesize;  //ָ���ļ���С������λͼ�ļ�ͷ��14���ֽڡ�
	uint16_t  reserved1;
	uint16_t  reserved2;
	uint32_t   bitmapoffset;  //���ļ�ͷ��ʵ��λͼ���ݵ�ƫ���ֽ���
}bmpfileheader;

//�ṹ�壬λͼ��Ϣͷ������Ϊ40�ֽڡ�
typedef struct {
	uint32_t   sizeofinfoheader;  //ָ���ýṹ��ĳ��ȣ�40�ֽڡ�
	uint32_t   width;   //ָ��ͼ��Ŀ�ȣ���λ�����ء�
	uint32_t   height;  //ָ��ͼ��ĸ߶ȣ���λ�����ء�
	uint16_t  planes;  //������1��
	uint16_t  bitsperpixel; //ָ����ʾ��ɫʱҪ�õ���λ��������ֵΪ1���ڰ׶�ɫͼ����4��16ɫͼ����8��256ɫͼ����24�����ɫͼ�������Ĳ���8��
	uint32_t   compression;  //ѹ����ʽ��һ��Ϊ0����ѹ��
	uint32_t   sizeofbitmap;  //ָ��ʵ�ʵ�λͼ����ռ�õ��ֽ�����sizeofbitmap=width'*height��
	uint32_t   horzres;   //ˮƽ�ֱ���
	uint32_t   vertres;   //��ֱ�ֱ���
	uint32_t   colorsused;  //ָ����ͼ��ʵ���õ�����ɫ���������ֵΪ�㣬���õ�����ɫ��Ϊ2��bitsperpixel���ݡ�
	uint32_t   colorsimp;
}bitmapheader;

#define R_chn 2
#define G_chn 1
#define B_chn 0

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