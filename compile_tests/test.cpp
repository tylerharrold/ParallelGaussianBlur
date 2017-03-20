// #define cimg_display 0
#include <iostream>
#include "CImg.h"
using namespace cimg_library;
int main(){
	CImg<unsigned char> image("lena.bmp");
	image = image.get_blur(2.5);
	CImgDisplay main_disp(image , "blur?");
	while(!main_disp.is_closed()){
		// loop endless
	}
	return 0;
}
