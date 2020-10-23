#include "header.h"
using namespace std;

string dir_imgsrc = "C:\\Users\\zheng\\Desktop\\BMP-Image-Process\\images_src\\";
string dir_imgdst = "C:\\Users\\zheng\\Desktop\\BMP-Image-Process\\images_dst\\";

int main()
{
	BmpFile test;
	test.load(dir_imgsrc + "test_color.bmp");
	test.printImgInfo();
	cout << test.isColorImg() << endl;
	//test.save_test(dir_imgdst + "savetest.bmp");
	test.zoom_out(dir_imgdst + "zoomout_2x.bmp", 0, 2);
	test.zoom_out(dir_imgdst + "zoomout_4x.bmp", 0, 4);
	test.zoom_in(dir_imgdst + "zoomoin_4x.bmp", 0, 4);
	return 0;
}