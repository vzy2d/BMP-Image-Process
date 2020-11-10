#include "BmpFile.h"

using namespace std;

BmpFile::BmpFile() {
	//ptr
	rgbQuad = NULL;
	bitmap_gray = NULL;
	bitmap_color = NULL;
	bitmap_gray_out = NULL;
	bitmap_color_out = NULL;

	fileName_IN = "NULL";
	sizeofrgbQuad = 0;
	last_ptr_bitmap_in_height = 0;
	last_ptr_bitmap_out_height = 0;

	myFileHeader.filetype[0] = myFileHeader_out.filetype[0] = 'B';
	myFileHeader.filetype[1] = myFileHeader_out.filetype[1] = 'M';
	myFileHeader.filesize = myFileHeader_out.filesize = 0;
	myFileHeader.reserved1 = myFileHeader_out.bitmapoffset = 0;
	myFileHeader.reserved2 = myFileHeader_out.bitmapoffset = 0;
	myFileHeader.bitmapoffset = myFileHeader_out.bitmapoffset = 0;

	myBitMapHeader.sizeofinfoheader = myBitMapHeader_out.sizeofinfoheader = 40;
	myBitMapHeader.width = myBitMapHeader_out.width = 0;
	myBitMapHeader.height = myBitMapHeader_out.height = 0;
	myBitMapHeader.planes = myBitMapHeader_out.planes = 1;
	myBitMapHeader.bitsperpixel = myBitMapHeader_out.bitsperpixel = 0;
	myBitMapHeader.compression = myBitMapHeader_out.compression = 0;
	myBitMapHeader.sizeofbitmap = myBitMapHeader_out.sizeofbitmap = 0;
	myBitMapHeader.horzres = myBitMapHeader_out.horzres = 0;
	myBitMapHeader.vertres = myBitMapHeader_out.vertres = 0;
	myBitMapHeader.colorsused = myBitMapHeader_out.colorsused = 0;
	myBitMapHeader.colorsimp = myBitMapHeader_out.colorsimp = 0;
}

BmpFile::~BmpFile() {
	delete rgbQuad;
	init_ptr_bitmap_in(0, 0);
	init_ptr_bitmap_out(0, 0);
}

bool BmpFile::isGrayImg() {
	return (myBitMapHeader.bitsperpixel == 8) ? 1 : 0;
}

bool BmpFile::isColorImg() {
	return (myBitMapHeader.bitsperpixel == 24) ? 1 : 0;
}

void BmpFile::func_test()
{
	//uint8_t* src_seq;
	//src_seq = new uint8_t[9];
	//uint8_t test_tmp[9] = { 55,2,1,4,3,9,9,7,6 };
	//for (int i = 0; i < 9; i++)
	//{
	//	src_seq[i] = test_tmp[i];
	//}
	//printf("\n%d\n",get_midval(src_seq, 9));
	////delete[]src_seq;
}

/*
* 灰度图显示
*/
void BmpFile::imgShow(uint8_t** ptr, uint32_t height, uint32_t width)
{
	cout << "**ptr" << endl;
}

/*
* 彩色图显示
*/
void BmpFile::imgShow(uint8_t*** ptr, uint32_t height, uint32_t width)
{
	cout << "***ptr" << endl;
}

void BmpFile::save_Gray(std::string file_path)
{
	if (myBitMapHeader_out.bitsperpixel == 24 ||
		myBitMapHeader_out.width == 0 ||
		myBitMapHeader_out.height == 0)exit(ARGUMENT_ERROR);

	char header[54] = {
	  0x42,        // identity : B
	  0x4d,        // identity : M
	  0, 0, 0, 0,  // file size
	  0, 0,        // reserved1
	  0, 0,        // reserved2
	  0x36, 0x4, 0, 0, // RGB data offset
	  40, 0, 0, 0, // struct BITMAPINFOHEADER size
	  0, 0, 0, 0,  // bmp width
	  0, 0, 0, 0,  // bmp height
	  1, 0,        // planes
	  8, 0,       // bit per pixel
	  0, 0, 0, 0,  // compression
	  0, 0, 0, 0,  // data size
	  0, 0, 0, 0,  // h resolution
	  0, 0, 0, 0,  // v resolution 
	  0, 0, 0, 0,  // used colors
	  0, 0, 0, 0   // important colors
	};

	ofstream outfile(file_path, ios::out | ios::binary);

	if (!outfile) {
		cout << " Write image error!!" << endl;
		exit(SAVE_FILE_ERROR);
	}

	//file size
	header[2] = (uint8_t)(myFileHeader_out.filesize & 0x000000ff);
	header[3] = (myFileHeader_out.filesize >> 8) & 0x000000ff;
	header[4] = (myFileHeader_out.filesize >> 16) & 0x000000ff;
	header[5] = (myFileHeader_out.filesize >> 24) & 0x000000ff;

	// width
	header[18] = myBitMapHeader_out.width & 0x000000ff;
	header[19] = (myBitMapHeader_out.width >> 8) & 0x000000ff;
	header[20] = (myBitMapHeader_out.width >> 16) & 0x000000ff;
	header[21] = (myBitMapHeader_out.width >> 24) & 0x000000ff;

	// height
	header[22] = myBitMapHeader_out.height & 0x000000ff;
	header[23] = (myBitMapHeader_out.height >> 8) & 0x000000ff;
	header[24] = (myBitMapHeader_out.height >> 16) & 0x000000ff;
	header[25] = (myBitMapHeader_out.height >> 24) & 0x000000ff;

	//size of bitmap
	header[34] = myBitMapHeader_out.sizeofbitmap & 0x000000ff;
	header[35] = (myBitMapHeader_out.sizeofbitmap >> 8) & 0x000000ff;
	header[36] = (myBitMapHeader_out.sizeofbitmap >> 16) & 0x000000ff;
	header[37] = (myBitMapHeader_out.sizeofbitmap >> 24) & 0x000000ff;

	// write header
	outfile.write(header, 54);

	// write rgbQuad
	outfile.write((char*)rgbQuad, sizeofrgbQuad);

	// write image
	for (int x = 0; x < myBitMapHeader_out.height; x++) {
		for (int y = 0; y < myBitMapHeader_out.width; y++) {
			outfile.write((char*)&(bitmap_gray_out[x][y]), sizeof(uint8_t));
		}
	}

	outfile.close();
}

void BmpFile::save_Color(std::string file_path)
{
	if (myBitMapHeader_out.bitsperpixel == 8 ||
		myBitMapHeader_out.width == 0 ||
		myBitMapHeader_out.height == 0)exit(ARGUMENT_ERROR);

	char header[54] = {
	  0x42,        // identity : B
	  0x4d,        // identity : M
	  0, 0, 0, 0,  // file size
	  0, 0,        // reserved1
	  0, 0,        // reserved2
	  54, 0, 0, 0, // RGB data offset
	  40, 0, 0, 0, // struct BITMAPINFOHEADER size
	  0, 0, 0, 0,  // bmp width
	  0, 0, 0, 0,  // bmp height
	  1, 0,        // planes
	  24, 0,       // bit per pixel
	  0, 0, 0, 0,  // compression
	  0, 0, 0, 0,  // data size
	  0, 0, 0, 0,  // h resolution
	  0, 0, 0, 0,  // v resolution 
	  0, 0, 0, 0,  // used colors
	  0, 0, 0, 0   // important colors
	};

	ofstream outfile(file_path, ios::out | ios::binary);

	if (!outfile) {
		cout << " Write image error!!" << endl;
		exit(SAVE_FILE_ERROR);
	}

	//file size
	header[2] = (uint8_t)(myFileHeader_out.filesize & 0x000000ff);
	header[3] = (myFileHeader_out.filesize >> 8) & 0x000000ff;
	header[4] = (myFileHeader_out.filesize >> 16) & 0x000000ff;
	header[5] = (myFileHeader_out.filesize >> 24) & 0x000000ff;

	// width
	header[18] = myBitMapHeader_out.width & 0x000000ff;
	header[19] = (myBitMapHeader_out.width >> 8) & 0x000000ff;
	header[20] = (myBitMapHeader_out.width >> 16) & 0x000000ff;
	header[21] = (myBitMapHeader_out.width >> 24) & 0x000000ff;

	// height
	header[22] = myBitMapHeader_out.height & 0x000000ff;
	header[23] = (myBitMapHeader_out.height >> 8) & 0x000000ff;
	header[24] = (myBitMapHeader_out.height >> 16) & 0x000000ff;
	header[25] = (myBitMapHeader_out.height >> 24) & 0x000000ff;

	//size of bitmap
	header[34] = myBitMapHeader_out.sizeofbitmap & 0x000000ff;
	header[35] = (myBitMapHeader_out.sizeofbitmap >> 8) & 0x000000ff;
	header[36] = (myBitMapHeader_out.sizeofbitmap >> 16) & 0x000000ff;
	header[37] = (myBitMapHeader_out.sizeofbitmap >> 24) & 0x000000ff;

	// write header
	outfile.write(header, 54);
	int a=0, b=0;
	bitmap_color[0][a][b] = 0;	//Blue
	bitmap_color[1][a][b] = 255;	//Green
	bitmap_color[2][a][b] = 0;
	// write image
	for (int x = 0; x < myBitMapHeader_out.height; x++) {
		for (int y = 0; y < myBitMapHeader_out.width; y++) {
			outfile.write((char*)&(bitmap_color_out[B_chn][x][y]), sizeof(uint8_t));
			outfile.write((char*)&(bitmap_color_out[G_chn][x][y]), sizeof(uint8_t));
			outfile.write((char*)&(bitmap_color_out[R_chn][x][y]), sizeof(uint8_t));
		}
	}
	outfile.close();
}

void BmpFile::zoom_in_Gray_M1(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height * zoom_ratio;
	width_out = myBitMapHeader.width * zoom_ratio;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "zoom_in Gray M1" << endl;
	cout << "zoom_in Pic Height:" << height_out << endl;
	cout << "zoom_in Pic Width:" << width_out << endl;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_gray_out[row][col] = bitmap_gray[row / zoom_ratio][col / zoom_ratio];
		}
	}
	save(file_path_out);
}

void BmpFile::zoom_in_Gray_M2(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height * zoom_ratio;
	width_out = myBitMapHeader.width * zoom_ratio;
	refreshHeader_out(height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "zoom_in Gray M2" << endl;
	cout << "zoom_in Pic Height:" << height_out << endl;
	cout << "zoom_in Pic Width:" << width_out << endl;


	uint32_t coordinate_row[2], coordinate_col[2];	//距放大目标点最近的四个像素点的坐标
	float coordinate_result_row, coordinate_result_col;	//放大目标像素点的坐标
	float t1, t2;	//暂存横向插值的结果，纵向插值的结果直接赋值给目标矩阵
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			//垂直坐标
			coordinate_result_row = (row * ((float)(myBitMapHeader.height - 1) / (float)(height_out)));
			coordinate_row[0] = floor(coordinate_result_row);
			coordinate_row[1] = ceil(coordinate_result_row);
			//水平坐标
			coordinate_result_col = (col * ((float)(myBitMapHeader.width - 1) / (float)(width_out)));
			coordinate_col[0] = floor(coordinate_result_col);
			coordinate_col[1] = ceil(coordinate_result_col);
			//计算横向插值
			t1 = bitmap_gray[coordinate_row[0]][coordinate_col[0]] +
				(bitmap_gray[coordinate_row[0]][coordinate_col[1]] - bitmap_gray[coordinate_row[0]][coordinate_col[0]]) * (coordinate_result_col - coordinate_col[0]);
			t2 = bitmap_gray[coordinate_row[1]][coordinate_col[0]] +
				(bitmap_gray[coordinate_row[1]][coordinate_col[1]] - bitmap_gray[coordinate_row[1]][coordinate_col[0]]) * (coordinate_result_col - coordinate_col[0]);

			bitmap_gray_out[row][col] = t1 + (t2 - t1) * (coordinate_result_row - coordinate_row[0]);
		}
	}
	save(file_path_out);
}

//FIXME:输入图像为 test_gray.bmp 缩小倍数为4时 得到的图像无法打开 其余倍数正常
void BmpFile::zoom_out_Gray_M1(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height / zoom_ratio;
	width_out = myBitMapHeader.width / zoom_ratio;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "zoom_out Gray M1" << endl;
	cout << "zoom_out Pic Height:" << height_out << endl;
	cout << "zoom_out Pic Width:" << width_out << endl;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_gray_out[row][col] = bitmap_gray[row * zoom_ratio][col * zoom_ratio];
		}
	}
	save(file_path_out);
}

//FIXME:
void BmpFile::zoom_out_Gray_M2(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height / zoom_ratio;
	width_out = myBitMapHeader.width / zoom_ratio;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "zoom_out Gray M2" << endl;
	cout << "zoom_out Pic Height:" << height_out << endl;
	cout << "zoom_out Pic Width:" << width_out << endl;

	float val[2][2];
	uint32_t coordinate_x[2], coordinate_y[2];
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			coordinate_x[0] = (uint32_t)(row * ((float)myBitMapHeader.height / (float)height_out));
			coordinate_x[1] = (uint32_t)((row+1) * ((float)myBitMapHeader.height / (float)height_out));
			coordinate_y[0] = (uint32_t)(col * ((float)myBitMapHeader.width / (float)width_out));
			coordinate_y[1] = (uint32_t)((col+1) * ((float)myBitMapHeader.width / (float)width_out));
			if (coordinate_x[1] == myBitMapHeader.height)coordinate_x[1] = myBitMapHeader.height - 1;
			if (coordinate_y[1] == myBitMapHeader.width)coordinate_y[1] = myBitMapHeader.width - 1;
			val[0][0] = bitmap_gray[coordinate_x[0]][coordinate_y[0]];
			val[0][1] = bitmap_gray[coordinate_x[0]][coordinate_y[1]];
			val[1][0] = bitmap_gray[coordinate_x[1]][coordinate_y[0]];
			val[1][1] = bitmap_gray[coordinate_x[1]][coordinate_y[1]];
			bitmap_gray_out[row][col] = (val[0][0] + val[0][1] + val[1][0] + val[1][1]) / 4;
		}
	}
	save(file_path_out);
}

void BmpFile::printImgInfo() {
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
	cout << "<< Img Info of << " << fileName_IN << " >>" << endl;
	cout << "<< File Type:" << myFileHeader.filetype[0] << myFileHeader.filetype[1] << " ( " << ((myBitMapHeader.bitsperpixel==8)?("Gray"):("Color")) << " ) " << endl;
	cout << "<< Height:" << myBitMapHeader.height << endl;
	cout << "<< Width:" << myBitMapHeader.width << endl;
	cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
}

void BmpFile::printHist()
{
	int histgram[256] = { 0 };
	//统计灰度直方图
	for (int row = 0; row < myBitMapHeader.height; row++) {
		for (int col = 0; col < myBitMapHeader.width; col++) {
			histgram[bitmap_gray[row][col]] += 1;
		}
	}
	for (int i = 0; i < 256; i++) {
		std::cout << "GrayVal = " << i << "\t: " << histgram[i] << endl;
	}
}

void BmpFile::load(string file_path) {
	init_ptr_bitmap_in(0, 0);
	init_ptr_bitmap_out(0, 0);

	fileName_IN = file_path;
	ifstream infile(file_path, ios::in | ios::binary);
	if (!infile) {
		std::cout << "Open file failed." << std::endl;
		exit(OPEN_FILE_ERROR);
	}
	// File Header
	infile.seekg(0, ios::beg);
	infile.read((char*)&(myFileHeader.filetype), 2 * sizeof(char));

	infile.seekg(2, ios::beg);
	infile.read((char*)&(myFileHeader.filesize), sizeof(int));

	infile.read((char*)&(myFileHeader.reserved1), sizeof(short));
	infile.read((char*)&(myFileHeader.reserved2), sizeof(short));

	infile.seekg(10, ios::beg);
	infile.read((char*)&(myFileHeader.bitmapoffset), sizeof(int));

	// Info Header
	infile.seekg(14, ios::beg);
	infile.read((char*)&(myBitMapHeader.sizeofinfoheader), sizeof(int));

	infile.seekg(18, ios::beg);
	infile.read((char*)&(myBitMapHeader.width), sizeof(int));
	infile.read((char*)&(myBitMapHeader.height), sizeof(int));

	infile.read((char*)&(myBitMapHeader.planes), sizeof(short));

	infile.read((char*)&(myBitMapHeader.bitsperpixel), sizeof(short));

	infile.read((char*)&(myBitMapHeader.compression), sizeof(int));

	infile.seekg(34, ios::beg);
	infile.read((char*)&(myBitMapHeader.sizeofbitmap), sizeof(int));

	infile.read((char*)&(myBitMapHeader.horzres), sizeof(int));

	infile.read((char*)&(myBitMapHeader.vertres), sizeof(int));

	// rgbQuad
	sizeofrgbQuad = myFileHeader.bitmapoffset - myBitMapHeader.sizeofinfoheader - 14;
	if (isGrayImg()) {
		rgbQuad = new uint8_t[sizeofrgbQuad];
		infile.seekg(myBitMapHeader.sizeofinfoheader + 14, ios::beg);
		infile.read((char*)rgbQuad, sizeofrgbQuad);
	}

	// bitmap
	init_ptr_bitmap_in(myBitMapHeader.height, myBitMapHeader.width);
	uint8_t temp[3];

	if (isGrayImg()) {
		infile.seekg(myFileHeader.bitmapoffset, ios::beg);
		for (int x = 0; x < myBitMapHeader.height; x++) {
			for (int y = 0; y < myBitMapHeader.width; y++) {
				infile.read((char*)temp, 1);
				bitmap_gray[x][y] = temp[0];
			}
		}
	}
	if (isColorImg()) {
		// 读取bitmap
		infile.seekg(myFileHeader.bitmapoffset, ios::beg);
		for (int x = 0; x < myBitMapHeader.height; x++) {
			for (int y = 0; y < myBitMapHeader.width; y++) {
				infile.read((char*)temp, 3);
				bitmap_color[0][x][y] = temp[0];
				bitmap_color[1][x][y] = temp[1];
				bitmap_color[2][x][y] = temp[2];
			}
		}
	}
	infile.close();
}

void BmpFile::load(const char* file_path) {
	string file_p(file_path);
	this->load(file_p);
}

void BmpFile::img_ZoomIn(std::string file_path_out, uint8_t ZOOM_METHOD, uint8_t zoom_ratio)
{
	if (isGrayImg()) {
		switch (ZOOM_METHOD)
		{
		case ZOOM_METHOD_1:	//method 1
			zoom_in_Gray_M1(file_path_out, zoom_ratio);
			break;
		case ZOOM_METHOD_2:
			zoom_in_Gray_M2(file_path_out, zoom_ratio);
			break;
		default:
			break;
		}
	}
	if (isColorImg()) {
		switch (ZOOM_METHOD)
		{
		case ZOOM_METHOD_1:	//method 1
			zoom_in_Color_M1(file_path_out, zoom_ratio);
			break;
		case ZOOM_METHOD_2:
			zoom_in_Color_M2(file_path_out, zoom_ratio);
			break;
		default:
			break;
		}
	}
}

void BmpFile::img_ZoomOut(std::string file_path_out, uint8_t ZOOM_METHOD, uint8_t zoom_ratio)
{
	if(isGrayImg()){
		switch (ZOOM_METHOD)
		{
		case ZOOM_METHOD_1:	//method 1
			zoom_out_Gray_M1(file_path_out, zoom_ratio);
			break;
		case ZOOM_METHOD_2:
			zoom_out_Gray_M2(file_path_out, zoom_ratio);
			break;
		default:
			break;
		}
	}
	if (isColorImg()) {
		switch (ZOOM_METHOD)
		{
		case ZOOM_METHOD_1:	//method 1
			zoom_out_Color_M1(file_path_out, zoom_ratio);
			break;
		case ZOOM_METHOD_2:
			zoom_out_Color_M2(file_path_out, zoom_ratio);
			break;
		default:
			break;
		}
	}
}

//直方图均衡
void BmpFile::img_HistogramEqualization(std::string file_path_out)
{
	if (isGrayImg()) {
		this->img_HistogramEqualization_Gray(file_path_out);
	}
	if (isColorImg()) {
		this->img_HistogramEqualization_Color(file_path_out);
	}
}

void BmpFile::img_filter(std::string file_path_out, uint8_t filter_type, uint8_t kernel_size)
{
	if (isColorImg()) {
		std::cout << "FilterErr: not a gray img!"<< endl;
		return;
	}
	if (kernel_size%2 == 0) {
		std::cout << "FilterErr: wrong kernel size!" << endl;
		return;
	}
	
	switch (filter_type)
	{
	case FILTER_TYPE_AVE:
		img_filter_ave(file_path_out, kernel_size);
		break;
	case FILTER_TYPE_Gauss:
		img_filter_gauss(file_path_out, kernel_size);
		break;
	case FILTER_TYPE_Neighbor:
		img_filter_neighbor(file_path_out, kernel_size);
		break;
	case FILTER_TYPE_MidVal:
		img_filter_midval(file_path_out, kernel_size);
		break;
	case FILTER_TYPE_Laplace:
		img_filter_laplace(file_path_out, kernel_size);
		break;
	case FILTER_TYPE_Laplace2:
		img_filter_laplace2(file_path_out, kernel_size);
		break;
	case FILTER_TYPE_Robet:
		img_filter_robet(file_path_out, kernel_size);
		break;
	case FILTER_TYPE_Sobel:
		img_filter_sobel(file_path_out, kernel_size);
		break;
	default:
		break;
	}
}

void BmpFile::save(string file_path) {
	if (isGrayImg()) {
		this->save_Gray(file_path);
	}
	if (isColorImg()) {
		this->save_Color(file_path);
	}
}


void BmpFile::save(const char* file_path) {
	string file_p(file_path);
	this->save(file_p);
}

void BmpFile::refreshHeader_out(uint16_t bitsperpixel, uint32_t height, uint32_t width)
{
	myFileHeader_out.filetype[0] = 'B';
	myFileHeader_out.filetype[1] = 'M';
	myFileHeader_out.bitmapoffset = 0;
	myFileHeader_out.bitmapoffset = 0;

	myBitMapHeader_out.sizeofinfoheader = 40;
	myBitMapHeader_out.planes = 1;
	myBitMapHeader_out.compression = 0;
	myBitMapHeader_out.horzres = 0;
	myBitMapHeader_out.vertres = 0;
	myBitMapHeader_out.colorsused = 0;
	myBitMapHeader_out.colorsimp = 0;
	if (bitsperpixel == 8) {	//Gray Img
		myBitMapHeader_out.width = width;
		myBitMapHeader_out.height = height;
		myBitMapHeader_out.sizeofbitmap = width * height;
		myFileHeader_out.bitmapoffset = 1078;
		myFileHeader_out.filesize = myFileHeader_out.bitmapoffset + myBitMapHeader_out.sizeofbitmap;

		myBitMapHeader_out.bitsperpixel = bitsperpixel;
	}
	else if (bitsperpixel == 24) {	//color img
		myBitMapHeader_out.width = width;
		myBitMapHeader_out.height = height;
		myBitMapHeader_out.sizeofbitmap = width * height * 3;
		myFileHeader_out.bitmapoffset = 54;
		myFileHeader_out.filesize = myFileHeader_out.bitmapoffset + myBitMapHeader_out.sizeofbitmap;

		myBitMapHeader_out.bitsperpixel = bitsperpixel;
	}
}
void BmpFile::refreshHeader_out(uint32_t height, uint32_t width)
{
	myFileHeader_out.filetype[0] = 'B';
	myFileHeader_out.filetype[1] = 'M';
	myFileHeader_out.bitmapoffset = 0;
	myFileHeader_out.bitmapoffset = 0;

	myBitMapHeader_out.sizeofinfoheader = 40;
	myBitMapHeader_out.planes = 1;
	myBitMapHeader_out.compression = 0;
	myBitMapHeader_out.horzres = 0;
	myBitMapHeader_out.vertres = 0;
	myBitMapHeader_out.colorsused = 0;
	myBitMapHeader_out.colorsimp = 0;
	if (myBitMapHeader.bitsperpixel == 8) {	//Gray Img
		myBitMapHeader_out.width = width;
		myBitMapHeader_out.height = height;
		myBitMapHeader_out.sizeofbitmap = width * height;
		myFileHeader_out.bitmapoffset = 1078;
		myFileHeader_out.filesize = myFileHeader_out.bitmapoffset + myBitMapHeader_out.sizeofbitmap;

		myBitMapHeader_out.bitsperpixel = myBitMapHeader.bitsperpixel;
	}
	else if (myBitMapHeader.bitsperpixel == 24) {	//color img
		myBitMapHeader_out.width = width;
		myBitMapHeader_out.height = height;
		myBitMapHeader_out.sizeofbitmap = width * height * 3;
		myFileHeader_out.bitmapoffset = 54;
		myFileHeader_out.filesize = myFileHeader_out.bitmapoffset + myBitMapHeader_out.sizeofbitmap;

		myBitMapHeader_out.bitsperpixel = myBitMapHeader.bitsperpixel;
	}
}

void BmpFile::img_GrayLevelTransformation(std::string file_path_out, uint8_t trans_type) {
	float trans_param[2] = { 30,1 };
	this->img_GrayLevelTransformation(file_path_out, trans_type, trans_param);
}
void BmpFile::img_GrayLevelTransformation(std::string file_path_out, uint8_t trans_type, float* trans_param)
{
	if (isColorImg()) {
		std::cout << "FilterErr: not a gray img!" << endl;
		return;
	}
	
	switch (trans_type)
	{
	case TRANS_TYPE_Log:
		img_GLT_Log(file_path_out, trans_param[0]);
		break;
	case TRANS_TYPE_Power:
		img_GLT_Power(file_path_out, trans_param[0], trans_param[1]);
		break;
	case TRANS_TYPE_Linear:
		img_GLT_Linear(file_path_out);
		break;
	case TRANS_TYPE_Reverse:
		img_GLT_Reverse(file_path_out);
		break;
	default:
		break;
	}
}

uint8_t BmpFile::ImArray(uint32_t row, uint32_t col)
{
	return uint8_t(0);// this->bitmap_gray[row * this->myBitMapHeader.width + col]);
}

void BmpFile::ImArray(uint32_t row, uint32_t col, uint16_t dat_in)
{
	//this->bitmap_gray[row * this->myBitMapHeader.width + col] = dat_in;
}

uint8_t BmpFile::ImArray(uint32_t row, uint32_t col, uint8_t chn)
{
	return uint8_t(0);
}

void BmpFile::ImArray(uint32_t row, uint32_t col, uint8_t chn, uint16_t dat_in)
{

}

void BmpFile::zoom_in_Color_M1(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height * zoom_ratio;
	width_out = myBitMapHeader.width * zoom_ratio;
	refreshHeader_out(24, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "zoom_in Color M1" << endl;
	cout << "zoom_in Pic Height:" << height_out << endl;
	cout << "zoom_in Pic Width:" << width_out << endl;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_color_out[R_chn][row][col] = bitmap_color[R_chn][row / zoom_ratio][col / zoom_ratio];
			bitmap_color_out[G_chn][row][col] = bitmap_color[G_chn][row / zoom_ratio][col / zoom_ratio];
			bitmap_color_out[B_chn][row][col] = bitmap_color[B_chn][row / zoom_ratio][col / zoom_ratio];
		}
	}

	save(file_path_out);
}

void BmpFile::zoom_in_Color_M2(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height * zoom_ratio;
	width_out = myBitMapHeader.width * zoom_ratio;
	refreshHeader_out(height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "zoom_in Color M2" << endl;
	cout << "zoom_in Pic Height:" << height_out << endl;
	cout << "zoom_in Pic Width:" << width_out << endl;


	uint32_t coordinate_row[2], coordinate_col[2];	//距放大目标点最近的四个像素点的坐标
	float coordinate_result_row, coordinate_result_col;	//放大目标像素点的坐标
	float t1[3], t2[3];	//暂存横向插值的结果，纵向插值的结果直接赋值给目标矩阵
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			//垂直坐标
			coordinate_result_row = (row * ((float)(myBitMapHeader.height - 1) / (float)(height_out)));
			coordinate_row[0] = floor(coordinate_result_row);
			coordinate_row[1] = ceil(coordinate_result_row);
			//水平坐标
			coordinate_result_col = (col * ((float)(myBitMapHeader.width - 1) / (float)(width_out)));
			coordinate_col[0] = floor(coordinate_result_col);
			coordinate_col[1] = ceil(coordinate_result_col);
			//计算横向插值
			//R_chn
			t1[R_chn] = bitmap_color[R_chn][coordinate_row[0]][coordinate_col[0]] +
				(bitmap_color[R_chn][coordinate_row[0]][coordinate_col[1]] - bitmap_color[R_chn][coordinate_row[0]][coordinate_col[0]]) * (coordinate_result_col - coordinate_col[0]);
			t2[R_chn] = bitmap_color[R_chn][coordinate_row[1]][coordinate_col[0]] +
				(bitmap_color[R_chn][coordinate_row[1]][coordinate_col[1]] - bitmap_color[R_chn][coordinate_row[1]][coordinate_col[0]]) * (coordinate_result_col - coordinate_col[0]);
			//G_chn
			t1[G_chn] = bitmap_color[G_chn][coordinate_row[0]][coordinate_col[0]] +
				(bitmap_color[G_chn][coordinate_row[0]][coordinate_col[1]] - bitmap_color[G_chn][coordinate_row[0]][coordinate_col[0]]) * (coordinate_result_col - coordinate_col[0]);
			t2[G_chn] = bitmap_color[G_chn][coordinate_row[1]][coordinate_col[0]] +
				(bitmap_color[G_chn][coordinate_row[1]][coordinate_col[1]] - bitmap_color[G_chn][coordinate_row[1]][coordinate_col[0]]) * (coordinate_result_col - coordinate_col[0]);
			//B_chn
			t1[B_chn] = bitmap_color[B_chn][coordinate_row[0]][coordinate_col[0]] +
				(bitmap_color[B_chn][coordinate_row[0]][coordinate_col[1]] - bitmap_color[B_chn][coordinate_row[0]][coordinate_col[0]]) * (coordinate_result_col - coordinate_col[0]);
			t2[B_chn] = bitmap_color[B_chn][coordinate_row[1]][coordinate_col[0]] +
				(bitmap_color[B_chn][coordinate_row[1]][coordinate_col[1]] - bitmap_color[B_chn][coordinate_row[1]][coordinate_col[0]]) * (coordinate_result_col - coordinate_col[0]);

			bitmap_color_out[R_chn][row][col] = t1[R_chn] + (t2[R_chn] - t1[R_chn]) * (coordinate_result_row - coordinate_row[0]);
			bitmap_color_out[G_chn][row][col] = t1[G_chn] + (t2[G_chn] - t1[G_chn]) * (coordinate_result_row - coordinate_row[0]);
			bitmap_color_out[B_chn][row][col] = t1[B_chn] + (t2[B_chn] - t1[B_chn]) * (coordinate_result_row - coordinate_row[0]);
		}
	}
	save(file_path_out);
}

void BmpFile::zoom_out_Color_M1(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height / zoom_ratio;
	width_out = myBitMapHeader.width / zoom_ratio;
	refreshHeader_out(24, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "zoom_out Color M1" << endl;
	cout << "zoom_out Pic Height:" << height_out << endl;
	cout << "zoom_out Pic Width:" << width_out << endl;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_color_out[R_chn][row][col] = bitmap_color[R_chn][row * zoom_ratio][col * zoom_ratio];
			bitmap_color_out[G_chn][row][col] = bitmap_color[G_chn][row * zoom_ratio][col * zoom_ratio];
			bitmap_color_out[B_chn][row][col] = bitmap_color[B_chn][row * zoom_ratio][col * zoom_ratio];
		}
	}

	save(file_path_out);
}

void BmpFile::zoom_out_Color_M2(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height / zoom_ratio;
	width_out = myBitMapHeader.width / zoom_ratio;
	refreshHeader_out(height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "zoom_out Color M2" << endl;
	cout << "zoom_out Pic Height:" << height_out << endl;
	cout << "zoom_out Pic Width:" << width_out << endl;

	float val[3][2][2];
	uint32_t coordinate_x[2], coordinate_y[2];
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			coordinate_x[0] = (uint32_t)(row * ((float)myBitMapHeader.height / (float)height_out));
			coordinate_x[1] = (uint32_t)((row + 1) * ((float)myBitMapHeader.height / (float)height_out));
			coordinate_y[0] = (uint32_t)(col * ((float)myBitMapHeader.width / (float)width_out));
			coordinate_y[1] = (uint32_t)((col + 1) * ((float)myBitMapHeader.width / (float)width_out));
			if (coordinate_x[1] == myBitMapHeader.height)coordinate_x[1] = myBitMapHeader.height - 1;
			if (coordinate_y[1] == myBitMapHeader.width)coordinate_y[1] = myBitMapHeader.width - 1;
			val[R_chn][0][0] = bitmap_color[R_chn][coordinate_x[0]][coordinate_y[0]];
			val[G_chn][0][0] = bitmap_color[G_chn][coordinate_x[0]][coordinate_y[0]];
			val[B_chn][0][0] = bitmap_color[B_chn][coordinate_x[0]][coordinate_y[0]];
			val[R_chn][0][1] = bitmap_color[R_chn][coordinate_x[0]][coordinate_y[1]];
			val[G_chn][0][1] = bitmap_color[G_chn][coordinate_x[0]][coordinate_y[1]];
			val[B_chn][0][1] = bitmap_color[B_chn][coordinate_x[0]][coordinate_y[1]];
			val[R_chn][1][0] = bitmap_color[R_chn][coordinate_x[1]][coordinate_y[0]];
			val[G_chn][1][0] = bitmap_color[G_chn][coordinate_x[1]][coordinate_y[0]];
			val[B_chn][1][0] = bitmap_color[B_chn][coordinate_x[1]][coordinate_y[0]];
			val[R_chn][1][1] = bitmap_color[R_chn][coordinate_x[1]][coordinate_y[1]];
			val[G_chn][1][1] = bitmap_color[G_chn][coordinate_x[1]][coordinate_y[1]];
			val[B_chn][1][1] = bitmap_color[B_chn][coordinate_x[1]][coordinate_y[1]];
			bitmap_color_out[R_chn][row][col] = (val[R_chn][0][0] + val[R_chn][0][1] + val[R_chn][1][0] + val[R_chn][1][1]) / 4;
			bitmap_color_out[G_chn][row][col] = (val[G_chn][0][0] + val[G_chn][0][1] + val[G_chn][1][0] + val[G_chn][1][1]) / 4;
			bitmap_color_out[B_chn][row][col] = (val[B_chn][0][0] + val[B_chn][0][1] + val[B_chn][1][0] + val[B_chn][1][1]) / 4;
		}
	}

	save(file_path_out);
}

/*
* 灰度图直方图均衡实现
*/
void BmpFile::img_HistogramEqualization_Gray(std::string file_path_out)
{
	//<<< Header & ptr init <<<
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);
	//>>> Header & ptr init >>>
	cout << " Pic Height:" << height_out << endl;
	cout << " Pic Width:" << width_out << endl;


	int histgram[256] = { 0 };
	double hist_prob[256] = { 0 };
	unsigned char hist_T[256] = { 0 };
	double cnt_hist_prob = 0;

	//统计灰度直方图
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			histgram[bitmap_gray[row][col]] += 1;
		}
	}
	//计算灰度映射
	for (int i = 0; i < 256; i++)
	{
		hist_prob[i] = (double)histgram[i] / (double)myBitMapHeader_out.sizeofbitmap;
		// printf("%d: %d \t %f\n",i,histgram[i],hist_prob[i]);
		cnt_hist_prob += hist_prob[i];
		hist_T[i] = 255 * cnt_hist_prob;
	}
	//灰度映射
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_gray_out[row][col] = (uint8_t)hist_T[bitmap_gray[row][col]];
		}
	}

	save(file_path_out);
}

/*
* 彩色图直方图均衡实现
*/
void BmpFile::img_HistogramEqualization_Color(std::string file_path_out)
{
	//<<< Header & ptr init <<<
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);
	//>>> Header & ptr init >>>
	cout << " Pic Height:" << height_out << endl;
	cout << " Pic Width:" << width_out << endl;


	int histgram[3][256] = { 0 };
	double hist_prob[3][256] = { 0 };
	unsigned char hist_T[3][256] = { 0 };
	double cnt_hist_prob[3] = { 0 };

	//统计各通道直方图
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			histgram[R_chn][ bitmap_color[R_chn][row][col] ] += 1;
			histgram[G_chn][ bitmap_color[G_chn][row][col] ] += 1;
			histgram[B_chn][ bitmap_color[B_chn][row][col] ] += 1;
		}
	}
	//计算各通道映射
	for (int i = 0; i < 256; i++)
	{
		hist_prob[R_chn][i] = (double)histgram[R_chn][i] / (double)myBitMapHeader_out.sizeofbitmap;
		hist_prob[G_chn][i] = (double)histgram[G_chn][i] / (double)myBitMapHeader_out.sizeofbitmap;
		hist_prob[B_chn][i] = (double)histgram[B_chn][i] / (double)myBitMapHeader_out.sizeofbitmap;
		cnt_hist_prob[R_chn] += hist_prob[R_chn][i];
		cnt_hist_prob[G_chn] += hist_prob[G_chn][i];
		cnt_hist_prob[B_chn] += hist_prob[B_chn][i];
		hist_T[R_chn][i] = 255 * cnt_hist_prob[R_chn];
		hist_T[G_chn][i] = 255 * cnt_hist_prob[G_chn];
		hist_T[B_chn][i] = 255 * cnt_hist_prob[B_chn];
	}
	//应用各通道映射
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_color_out[R_chn][row][col] = (uint8_t)hist_T[R_chn][bitmap_color[R_chn][row][col]];
			bitmap_color_out[G_chn][row][col] = (uint8_t)hist_T[G_chn][bitmap_color[G_chn][row][col]];
			bitmap_color_out[B_chn][row][col] = (uint8_t)hist_T[B_chn][bitmap_color[B_chn][row][col]];
		}
	}

	save(file_path_out);
}

void BmpFile::img_filter_ave(std::string file_path_out, uint8_t kernel_size)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_filter_ave" << endl;
	cout << "img_filter_ave Pic Height:" << height_out << endl;
	cout << "img_filter_ave Pic Width:" << width_out << endl;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			//>>>>>>>>>>>>>>>>>>>>>>>>> convr
			int16_t conv_sum = 0;
			for (int kernel_row = -(kernel_size/2); kernel_row <= (kernel_size / 2); kernel_row++) {
				for (int kernel_col = -(kernel_size / 2); kernel_col <= (kernel_size / 2); kernel_col++) {
					int16_t res_pos_row, res_pos_col;	//目标坐标
					res_pos_row = row + kernel_row;
					res_pos_col = col + kernel_col;
					if (res_pos_row<0 || res_pos_row>=height_out
						|| res_pos_col<0 || res_pos_col>=width_out) {
						continue;
					}
					else {
						conv_sum += bitmap_gray[res_pos_row][res_pos_col];	//step1.sum
					}
				}
			}
			bitmap_gray_out[row][col] = conv_sum / (kernel_size * kernel_size);	//step2.ave
			//<<<<<<<<<<<<<<<<<<<<<<<<< conv
		}
	}
	save(file_path_out);
}

void BmpFile::img_filter_gauss(std::string file_path_out, uint8_t kernel_size)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_filter_gauss" << endl;
	cout << "img_filter_gauss Pic Height:" << height_out << endl;
	cout << "img_filter_gauss Pic Width:" << width_out << endl;

	//>>>>>>>>>>>>>>>>>>>>>>>>> init kernel
	kernel_size = 3;
	float kernel[3][3] = { {1,2,1}, {2,4,2}, {1,2,1} };
	//<<<<<<<<<<<<<<<<<<<<<<<<< init kernel

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			//>>>>>>>>>>>>>>>>>>>>>>>>> convr
			float conv_sum = 0;
			for (int kernel_row = -(kernel_size / 2); kernel_row <= (kernel_size / 2); kernel_row++) {
				for (int kernel_col = -(kernel_size / 2); kernel_col <= (kernel_size / 2); kernel_col++) {
					int16_t res_pos_row, res_pos_col;	//目标坐标
					res_pos_row = row + kernel_row;
					res_pos_col = col + kernel_col;
					if (res_pos_row < 0 || res_pos_row >= height_out
						|| res_pos_col < 0 || res_pos_col >= width_out) {
						continue;
					}
					else {
						conv_sum += kernel[kernel_row + kernel_size / 2][kernel_col + kernel_size / 2] * (float)(bitmap_gray[res_pos_row][res_pos_col]);	//step1.sum
					}
				}
			}
			bitmap_gray_out[row][col] = conv_sum / 16.0f;	//step2.ave
			//<<<<<<<<<<<<<<<<<<<<<<<<< conv
		}
	}
	save(file_path_out);

}

void BmpFile::img_filter_neighbor(std::string file_path_out, uint8_t kernel_size)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_filter_neighbor" << endl;
	cout << "img_filter_neighbor Pic Height:" << height_out << endl;
	cout << "img_filter_neighbor Pic Width:" << width_out << endl;

	//>>>>>>>>>>>>>>>>>>>>>>>>> init kernel
	kernel_size = 3;
	float kernel[3][3] = { {1,1,1}, {1,0,1}, {1,1,1} };
	//<<<<<<<<<<<<<<<<<<<<<<<<< init kernel

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			//>>>>>>>>>>>>>>>>>>>>>>>>> convr
			float conv_sum = 0;
			for (int kernel_row = -(kernel_size / 2); kernel_row <= (kernel_size / 2); kernel_row++) {
				for (int kernel_col = -(kernel_size / 2); kernel_col <= (kernel_size / 2); kernel_col++) {
					int16_t res_pos_row, res_pos_col;	//目标坐标
					res_pos_row = row + kernel_row;
					res_pos_col = col + kernel_col;
					if (res_pos_row < 0 || res_pos_row >= height_out
						|| res_pos_col < 0 || res_pos_col >= width_out) {
						continue;
					}
					else {
						conv_sum += kernel[kernel_row + kernel_size / 2][kernel_col + kernel_size / 2] * (float)(bitmap_gray[res_pos_row][res_pos_col]);	//step1.sum
					}
				}
			}
			bitmap_gray_out[row][col] = conv_sum / 8.0f;	//step2.ave
			//<<<<<<<<<<<<<<<<<<<<<<<<< conv
		}
	}
	save(file_path_out);
}

void BmpFile::img_filter_midval(std::string file_path_out, uint8_t kernel_size)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_filter_midval" << endl;
	cout << "img_filter_midval Pic Height:" << height_out << endl;
	cout << "img_filter_midval Pic Width:" << width_out << endl;
	uint8_t kernel_size_2 = kernel_size * kernel_size;
	uint8_t* src_seq;
	src_seq = new uint8_t[kernel_size_2];
	

	float conv_sum = 0;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			uint8_t src_seq_cnt = 0;
			//>>>>>>>>>>>>>>>>>>>>>>>>> conv
			for (int kernel_row = -(kernel_size / 2); kernel_row <= (kernel_size / 2); kernel_row++) {
				for (int kernel_col = -(kernel_size / 2); kernel_col <= (kernel_size / 2); kernel_col++) {
					int16_t res_pos_row, res_pos_col;	//目标坐标
					res_pos_row = row + kernel_row;
					res_pos_col = col + kernel_col;
					if (res_pos_row < 0 || res_pos_row >= height_out
						|| res_pos_col < 0 || res_pos_col >= width_out) {
						src_seq[src_seq_cnt] = 0;
					}
					else {
						src_seq[src_seq_cnt] = bitmap_gray[res_pos_row][res_pos_col];
					}
					src_seq_cnt += 1;
				}
			}
			//std::cout << row << "  "<<col<<  "  :";
			//for (int i = 0; i < kernel_size * kernel_size; i++) {
			//	printf(" %d ", src_seq[i]);
			//}
			bitmap_gray_out[row][col] = get_midval(src_seq, kernel_size_2);
			//printf("  result: %d \n", bitmap_gray_out[row][col]);
			//<<<<<<<<<<<<<<<<<<<<<<<<< conv
		}
	}
	save(file_path_out);

	delete[] src_seq;
}

void BmpFile::img_filter_laplace(std::string file_path_out, uint8_t kernel_size)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_filter_laplace" << endl;
	cout << "img_filter_laplace Pic Height:" << height_out << endl;
	cout << "img_filter_laplace Pic Width:" << width_out << endl;

	//>>>>>>>>>>>>>>>>>>>>>>>>> init kernel
	kernel_size = 3;
	//float kernel[3][3] = { {0,1,0}, {1,-4,1}, {0,1,0} };
	//float kernel[3][3] = { {0,-1,0}, {-1,4,-1}, {0,-1,0} };
	float kernel[3][3] = { {1,1,1}, {1,-8,1}, {1,1,1} };
	//<<<<<<<<<<<<<<<<<<<<<<<<< init kernel

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			//>>>>>>>>>>>>>>>>>>>>>>>>> convr
			float conv_sum = 0;
			for (int kernel_row = -(kernel_size / 2); kernel_row <= (kernel_size / 2); kernel_row++) {
				for (int kernel_col = -(kernel_size / 2); kernel_col <= (kernel_size / 2); kernel_col++) {
					int16_t res_pos_row, res_pos_col;	//目标坐标
					res_pos_row = row + kernel_row;
					res_pos_col = col + kernel_col;
					if (res_pos_row < 0 || res_pos_row >= height_out
						|| res_pos_col < 0 || res_pos_col >= width_out) {
						continue;
					}
					else {
						conv_sum += kernel[kernel_row + kernel_size / 2][kernel_col + kernel_size / 2] * (float)(bitmap_gray[res_pos_row][res_pos_col]);
					}
				}
			}
			bitmap_gray_out[row][col] = (abs(conv_sum) > 255) ? 255 : abs(conv_sum);// (kernel[kernel_size / 2][kernel_size / 2] >= 0) ? (bitmap_gray[row][col] + conv_sum) : (bitmap_gray[row][col] - conv_sum);
			//<<<<<<<<<<<<<<<<<<<<<<<<< conv
		}
	}
	save(file_path_out);
}

void BmpFile::img_filter_laplace2(std::string file_path_out, uint8_t kernel_size)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_filter_laplace" << endl;
	cout << "img_filter_laplace Pic Height:" << height_out << endl;
	cout << "img_filter_laplace Pic Width:" << width_out << endl;

	//>>>>>>>>>>>>>>>>>>>>>>>>> init kernel
	kernel_size = 3;
	float kernel[3][3] = { {0,1,0}, {1,-5,1}, {0,1,0} };
	//float kernel[3][3] = { {1,1,1}, {1,-9,1}, {1,1,1} };
	//<<<<<<<<<<<<<<<<<<<<<<<<< init kernel

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			//>>>>>>>>>>>>>>>>>>>>>>>>> convr
			float conv_sum = 0;
			for (int kernel_row = -(kernel_size / 2); kernel_row <= (kernel_size / 2); kernel_row++) {
				for (int kernel_col = -(kernel_size / 2); kernel_col <= (kernel_size / 2); kernel_col++) {
					int16_t res_pos_row, res_pos_col;	//目标坐标
					res_pos_row = row + kernel_row;
					res_pos_col = col + kernel_col;
					if (res_pos_row < 0 || res_pos_row >= height_out
						|| res_pos_col < 0 || res_pos_col >= width_out) {
						continue;
					}
					else {
						conv_sum += kernel[kernel_row + kernel_size / 2][kernel_col + kernel_size / 2] * (float)(bitmap_gray[res_pos_row][res_pos_col]);
					}
				}
			}
			
			bitmap_gray_out[row][col] = (abs(conv_sum)>255)?255:abs(conv_sum);// (kernel[kernel_size / 2][kernel_size / 2] >= 0) ? (bitmap_gray[row][col] + conv_sum) : (bitmap_gray[row][col] - conv_sum);
			//<<<<<<<<<<<<<<<<<<<<<<<<< conv
		}
	}
	save(file_path_out);
}

void BmpFile::img_filter_robet(std::string file_path_out, uint8_t kernel_size)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_filter_robet" << endl;
	cout << "img_filter_robet Pic Height:" << height_out << endl;
	cout << "img_filter_robet Pic Width:" << width_out << endl;

	//>>>>>>>>>>>>>>>>>>>>>>>>> init kernel
	kernel_size = 3;
	float kernel_x[3][3] = { {0,0,0}, {0,-1,0}, {0,0,1} };
	float kernel_y[3][3] = { {0,0,0}, {0,0,-1}, {0,1,0} };
	//<<<<<<<<<<<<<<<<<<<<<<<<< init kernel

	float tmp;
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			//>>>>>>>>>>>>>>>>>>>>>>>>> convr
			float conv_sum_x = 0;
			float conv_sum_y = 0;
			for (int kernel_row = -(kernel_size / 2); kernel_row <= (kernel_size / 2); kernel_row++) {
				for (int kernel_col = -(kernel_size / 2); kernel_col <= (kernel_size / 2); kernel_col++) {
					int16_t res_pos_row, res_pos_col;	//目标坐标
					res_pos_row = row + kernel_row;
					res_pos_col = col + kernel_col;
					if (res_pos_row < 0 || res_pos_row >= height_out
						|| res_pos_col < 0 || res_pos_col >= width_out) {
						continue;
					}
					else {
						conv_sum_x += kernel_x[kernel_row + kernel_size / 2][kernel_col + kernel_size / 2] * (float)(bitmap_gray[res_pos_row][res_pos_col]);
						conv_sum_y += kernel_y[kernel_row + kernel_size / 2][kernel_col + kernel_size / 2] * (float)(bitmap_gray[res_pos_row][res_pos_col]);
					}
				}
			}
			tmp = 1.0f * (abs(conv_sum_x) + abs(conv_sum_y));
			bitmap_gray_out[row][col] = (tmp > 255) ? 255 : tmp;
			//<<<<<<<<<<<<<<<<<<<<<<<<< conv
		}
	}
	save(file_path_out);
}

void BmpFile::img_filter_sobel(std::string file_path_out, uint8_t kernel_size)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_filter_sobel" << endl;
	cout << "img_filter_sobel Pic Height:" << height_out << endl;
	cout << "img_filter_sobel Pic Width:" << width_out << endl;

	//>>>>>>>>>>>>>>>>>>>>>>>>> init kernel
	kernel_size = 3;
	float kernel_x[3][3] = { {-1,-2,-1}, {0,0,0}, {1,2,1} };
	float kernel_y[3][3] = { {-1,0,1}, {-2,0,2}, {-1,0,1} };
	//<<<<<<<<<<<<<<<<<<<<<<<<< init kernel

	float tmp;
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			//>>>>>>>>>>>>>>>>>>>>>>>>> convr
			float conv_sum_x = 0;
			float conv_sum_y = 0;
			for (int kernel_row = -(kernel_size / 2); kernel_row <= (kernel_size / 2); kernel_row++) {
				for (int kernel_col = -(kernel_size / 2); kernel_col <= (kernel_size / 2); kernel_col++) {
					int16_t res_pos_row, res_pos_col;	//目标坐标
					res_pos_row = row + kernel_row;
					res_pos_col = col + kernel_col;
					if (res_pos_row < 0 || res_pos_row >= height_out
						|| res_pos_col < 0 || res_pos_col >= width_out) {
						continue;
					}
					else {
						conv_sum_x += kernel_x[kernel_row + kernel_size / 2][kernel_col + kernel_size / 2] * (float)(bitmap_gray[res_pos_row][res_pos_col]);
						conv_sum_y += kernel_y[kernel_row + kernel_size / 2][kernel_col + kernel_size / 2] * (float)(bitmap_gray[res_pos_row][res_pos_col]);
					}
				}
			}
			tmp = 0.5f * (abs(conv_sum_x) + abs(conv_sum_y));
			bitmap_gray_out[row][col] = (tmp>255)?255:tmp;
			//<<<<<<<<<<<<<<<<<<<<<<<<< conv
		}
	}
	save(file_path_out);
}

float BmpFile::generate_gauss(uint8_t kernel_size, int8_t kernel_row, int8_t kernel_col, float sigma)
{
	//int i;

	//// 数组的中心点
	//int nCenter;

	//// 数组的某一点到中心点的距离
	//double  dDis;

	//double PI = 3.14159;
	//// 中间变量
	//double  dValue;
	//double  dSum;
	//dSum = 0;

	//// 数组长度，根据概率论的知识，选取[-3*sigma, 3*sigma]以内的数据。
	//// 这些数据会覆盖绝大部分的滤波系数
	//// ceil 的功能是返回不小于其参数的最小整数
	//// 例如 ceil(6.4) 的值为 7;
	//*pnWindowSize = 1 + 2 * ceil(3 * sigma);

	//// 中心
	//nCenter = (*pnWindowSize) / 2;

	//// 分配内存
	//*pdKernel = new double[*pnWindowSize];

	//// g(x)=exp( -x^2/(2 sigma^2)

	//for (i = 0; i < (*pnWindowSize); i++)
	//{
	//	dDis = (double)(i - nCenter);
	//	dValue = exp(-(1 / 2) * dDis * dDis / (sigma * sigma)) / (sqrt(2 * PI) * sigma);
	//	(*pdKernel)[i] = dValue;
	//	dSum += dValue;
	//}

	//// 归一化
	//for (i = 0; i < (*pnWindowSize); i++)
	//{
	//	(*pdKernel)[i] /= dSum;
	//}
	return 0.0f;
}

uint8_t BmpFile::get_midval(uint8_t* src_seq, uint8_t len)
{
	uint8_t tmp;
	for (int i = 0; i < len - 2; i++) {
		for (int j = i; j < len - 2; j++) {
			if (src_seq[j] > src_seq[j + 1]) {
				tmp = src_seq[j];
				src_seq[j] = src_seq[j + 1];
				src_seq[j + 1] = tmp;
			}
		}
	}
	return uint8_t(src_seq[(len/2)]);
}

void BmpFile::img_GLT_Reverse(std::string file_path_out)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_GLT_Reverse" << endl;
	cout << "img_GLT_Reverse Pic Height:" << height_out << endl;
	cout << "img_GLT_Reverse Pic Width:" << width_out << endl;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_gray_out[row][col] = 255 - bitmap_gray[row][col];
		}
	}
	save(file_path_out);
}

void BmpFile::img_GLT_Log(std::string file_path_out, float ratio_c)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_GLT_Log" << endl;
	cout << "img_GLT_Log Pic Height:" << height_out << endl;
	cout << "img_GLT_Log Pic Width:" << width_out << endl;

	float tmp;
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			tmp = ratio_c * log(1 + bitmap_gray[row][col]);	//输出限幅
			bitmap_gray_out[row][col] = (tmp>255)?255:tmp;
		}
	}
	save(file_path_out);
}

void BmpFile::img_GLT_Power(std::string file_path_out, float ratio_const, float ratio_power)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_GLT_Reverse" << endl;
	cout << "img_GLT_Reverse Pic Height:" << height_out << endl;
	cout << "img_GLT_Reverse Pic Width:" << width_out << endl;

	float tmp;
	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			tmp = ratio_const * pow(bitmap_gray[row][col], ratio_power);	//输出限幅
			bitmap_gray_out[row][col] = (tmp > 255) ? 255 : tmp;
		}
	}
	save(file_path_out);
}

void BmpFile::img_GLT_Linear(std::string file_path_out)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "img_GLT_Linear" << endl;
	cout << "img_GLT_Linear Pic Height:" << height_out << endl;
	cout << "img_GLT_Linear Pic Width:" << width_out << endl;

	uint8_t cut_1 = 50;
	uint8_t cut_2 = 200;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			if (bitmap_gray[row][col] >= 0 && bitmap_gray[row][col] < cut_1) {
				bitmap_gray_out[row][col] = 1.6f * bitmap_gray[row][col];
			}
			else if (bitmap_gray[row][col] >= cut_1 && bitmap_gray[row][col] < cut_2) {
				bitmap_gray_out[row][col] = 1.0f * bitmap_gray[row][col];
			}
			else if (bitmap_gray[row][col] >= cut_2 && bitmap_gray[row][col] < 256) {
				bitmap_gray_out[row][col] = 0.8f * bitmap_gray[row][col] + 255.0f * 0.2f;
			}
		}
	}
	save(file_path_out);
}

/*	内存管理
* ( height !=0 && width != 0 ) => init bitmap_gray/bitmap_color
* ( height ==0 && width == 0 ) => delete bitmap_gray/bitmap_color
*/
void BmpFile::init_ptr_bitmap_in(uint32_t height, uint32_t width)
{
	if (isGrayImg()) {
		if (height == 0 && width == 0) {
			for (int i = 0; i < last_ptr_bitmap_in_height; i++)
			{
				delete[] bitmap_gray[i];
			}
			delete[] bitmap_gray;
			bitmap_gray = NULL;
			last_ptr_bitmap_in_height = height;
		}
		else {
			//若非NULL 先置NULL
			if (bitmap_gray != NULL) {
				for (int i = 0; i < last_ptr_bitmap_in_height; i++)
				{
					delete[] bitmap_gray[i];
				}
				delete[] bitmap_gray;
				bitmap_gray = NULL;
			}
			//分配内存
			bitmap_gray = new uint8_t * [height];
			for (int i = 0; i < height; i++)
			{
				bitmap_gray[i] = new uint8_t[width];
			}
			//记录分配空间大小 用于delete
			last_ptr_bitmap_in_height = height;
		}
	}
	if (isColorImg()) {
		if (height == 0 && width == 0) {
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < last_ptr_bitmap_in_height; j++)
				{
					delete[] bitmap_color[i][j];
				}
				delete[] bitmap_color[i];
			}
			delete[] bitmap_color;
			bitmap_color = NULL;

			last_ptr_bitmap_in_height = height;
		}
		else {
			if (bitmap_color != NULL) {
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < last_ptr_bitmap_in_height; j++)
					{
						delete[] bitmap_color[i][j];
					}
					delete[] bitmap_color[i];
				}
				delete[] bitmap_color;
				bitmap_color = NULL;
			}

			bitmap_color = new uint8_t * *[3];
			for (int i = 0; i < 3; i++)
			{
				bitmap_color[i] = new uint8_t * [height];
				for (int j = 0; j < height; j++)
				{
					bitmap_color[i][j] = new uint8_t[width];
				}
			}

			last_ptr_bitmap_in_height = height;
		}
	}
}


/*	内存管理
* ( height !=0 && width != 0 ) => init bitmap_gray/bitmap_color _out
* ( height ==0 && width == 0 ) => delete bitmap_gray/bitmap_color _out
*/
void BmpFile::init_ptr_bitmap_out(uint32_t height, uint32_t width)
{
	if (isGrayImg()) {
		if (height == 0 && width == 0) {
			if (bitmap_gray_out != NULL) {
				for (int i = 0; i < last_ptr_bitmap_out_height; i++)
				{
					delete[] bitmap_gray_out[i];
				}
				delete[] bitmap_gray_out;
				bitmap_gray_out = NULL;
			}
			last_ptr_bitmap_out_height = height;
		}
		else {
			//若非NULL 先置NULL
			if (bitmap_gray_out != NULL) {
				for (int i = 0; i < last_ptr_bitmap_out_height; i++)
				{
					delete[] bitmap_gray_out[i];
				}
				delete[] bitmap_gray_out;
				bitmap_gray_out = NULL;
			}
			//分配内存
			bitmap_gray_out = new uint8_t * [height];
			for (int i = 0; i < height; i++)
			{
				bitmap_gray_out[i] = new uint8_t[width];
			}
			//记录分配空间大小 用于delete
			last_ptr_bitmap_out_height = height;
		}
	}
	if (isColorImg()) {
		if (height == 0 && width == 0) {
			if (bitmap_color_out != NULL) {
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < last_ptr_bitmap_out_height; j++)
					{
						delete[] bitmap_color_out[i][j];
					}
					delete[] bitmap_color_out[i];
				}
				delete[] bitmap_color_out;
				bitmap_color_out = NULL;
			}
			last_ptr_bitmap_out_height = height;
		}
		else {
			if (bitmap_color_out != NULL) {
				for (int i = 0; i < 3; i++)
				{
					for (int j = 0; j < last_ptr_bitmap_out_height; j++)
					{
						delete[] bitmap_color_out[i][j];
					}
					delete[] bitmap_color_out[i];
				}
				delete[] bitmap_color_out;
				bitmap_color_out = NULL;
			}

			bitmap_color_out = new uint8_t * *[3];
			for (int i = 0; i < 3; i++)
			{
				bitmap_color_out[i] = new uint8_t * [height];
				for (int j = 0; j < height; j++)
				{
					bitmap_color_out[i][j] = new uint8_t[width];
				}
			}

			last_ptr_bitmap_out_height = height;
		}
	}
}

//取整
int BmpFile::round(float dat_in)
{
	if (dat_in > 0) {
		return	(int)(dat_in * 2 + 1) / 2;
	}
	else {
		return	(int)(dat_in * 2 - 1) / 2;
	}
}

//取整
int BmpFile::round(double dat_in)
{
	if (dat_in > 0) {
		return	(int)(dat_in * 2 + 1) / 2;
	}
	else {
		return	(int)(dat_in * 2 - 1) / 2;
	}
}


/*
* template
*/
/*
void BmpFile::xxx(std::string file_path_out)
{
	//<<< Header & ptr init <<<
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height;
	width_out = myBitMapHeader.width;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);
	//>>> Header & ptr init >>>

	cout << " Pic Height:" << height_out << endl;
	cout << " Pic Width:" << width_out << endl;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_gray_out[row][col] = bitmap_gray[row][col];
		}
	}

	save(file_path_out);
}
*/