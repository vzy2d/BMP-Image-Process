#include "header.h"
using namespace std;

string dir_imgsrc = "C:\\Users\\zheng\\source\\repos\\BmpImgProcessing\\images_src\\";
string dir_imgdst = "C:\\Users\\zheng\\source\\repos\\BmpImgProcessing\\images_dst\\";

int main()
{
	BmpFile test;
	test.load(dir_imgsrc + "lena_C.bmp");
	test.printImgInfo();
	cout << test.isColorImg() << endl;
	return 0;
}