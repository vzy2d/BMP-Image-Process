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

void BmpFile::save_test(std::string file_path)
{
	this->refreshHeader_out(8, myBitMapHeader.height, myBitMapHeader.width);
	save(file_path);
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
			outfile.write((char*)&(bitmap_color_out[R_chn][x][y]), sizeof(uint8_t));
			outfile.write((char*)&(bitmap_color_out[G_chn][x][y]), sizeof(uint8_t));
			outfile.write((char*)&(bitmap_color_out[B_chn][x][y]), sizeof(uint8_t));
		}
	}
	//outfile.write((char*)bitmap_color,sizeof(uint8_t)*myBitMapHeader_out.sizeofbitmap);

	outfile.close();
}

void BmpFile::zoom_in_Gray_M1(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height * zoom_ratio;
	width_out = myBitMapHeader.width * zoom_ratio;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

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
	return; //未编写
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height * zoom_ratio;
	width_out = myBitMapHeader.width * zoom_ratio;
	refreshHeader_out(8, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

	cout << "zoom_in Pic Height:" << height_out << endl;
	cout << "zoom_in Pic Width:" << width_out << endl;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_gray_out[row][col] = bitmap_gray[row * zoom_ratio][col * zoom_ratio];
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

	cout << "zoom_out Pic Height:" << height_out << endl;
	cout << "zoom_out Pic Width:" << width_out << endl;

	for (int row = 0; row < height_out; row++) {
		for (int col = 0; col < width_out; col++) {
			bitmap_gray_out[row][col] = bitmap_gray[row * zoom_ratio][col * zoom_ratio];
		}
	}

	save(file_path_out);
}

void BmpFile::zoom_out_Gray_M2(std::string file_path_out, uint8_t zoom_ratio)
{
}

void BmpFile::printImgInfo() {
	cout << "Img Info of <" << fileName_IN << ">" << endl;
	cout << "File Type:" << myFileHeader.filetype[0] << myFileHeader.filetype[1] << endl;
	cout << "File Type:" << myFileHeader.filesize << endl;
	cout << "Height of Img:" << myBitMapHeader.height << endl;
	cout << "Width of Img:" << myBitMapHeader.width << endl;
}

void BmpFile::load(string file_path) {
	this->~BmpFile();
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

void BmpFile::zoom_in(std::string file_path_out, bool ZOOM_METHOD, uint8_t zoom_ratio)
{
	if (isGrayImg()) {
		switch (ZOOM_METHOD)
		{
		case 0:	//method 1
			zoom_in_Gray_M1(file_path_out, zoom_ratio);
		case 1:
			zoom_in_Gray_M2(file_path_out, zoom_ratio);
		default:
			break;
		}
	}
	if (isColorImg()) {
		switch (ZOOM_METHOD)
		{
		case 0:	//method 1
			zoom_in_Color_M1(file_path_out, zoom_ratio);
		case 1:
			zoom_in_Color_M2(file_path_out, zoom_ratio);
		default:
			break;
		}
	}
}

void BmpFile::zoom_out(std::string file_path_out, bool ZOOM_METHOD, uint8_t zoom_ratio)
{
	if(isGrayImg()){
		switch (ZOOM_METHOD)
		{
		case 0:	//method 1
			zoom_out_Gray_M1(file_path_out, zoom_ratio);
		case 1:
			zoom_out_Gray_M2(file_path_out, zoom_ratio);
		default:
			break;
		}
	}
	if (isColorImg()) {
		switch (ZOOM_METHOD)
		{
		case 0:	//method 1
			zoom_out_Color_M1(file_path_out, zoom_ratio);
		case 1:
			zoom_out_Color_M2(file_path_out, zoom_ratio);
		default:
			break;
		}
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
}

void BmpFile::zoom_out_Color_M1(std::string file_path_out, uint8_t zoom_ratio)
{
	uint32_t height_out, width_out;
	height_out = myBitMapHeader.height / zoom_ratio;
	width_out = myBitMapHeader.width / zoom_ratio;
	refreshHeader_out(24, height_out, width_out);
	init_ptr_bitmap_out(height_out, width_out);

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
			for (int i = 0; i < last_ptr_bitmap_out_height; i++)
			{
				delete[] bitmap_gray_out[i];
			}
			delete[] bitmap_gray_out;
			bitmap_gray_out = NULL;
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