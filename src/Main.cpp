#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include"IndirectWithHorizont.h"
#include"IndirectWithoutHorizont.h"

using namespace cv;
using namespace std;


int main( int argc, char** argv )
{

	indirectWithoutHorizont("Data/pasillo2.pgm");

	return 0;
}



