#include<opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include"IndirectWithoutHorizont.h"

using namespace cv;
using namespace std;

int indirectWithoutHorizont () {

	VideoCapture capture;
	capture.open(0); //0 es la webcam integrada y 1 es la webcam USB

	capture.set(CV_CAP_PROP_FRAME_WIDTH,600);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,400);

	Mat cameraFeed;

	Mat src, grad_x, grad_y, module, orientation;//, votes;
	Mat grad_y2, grad_x2, orientation2, final, colorfinal;
	Mat grad_x_impr, grad_y_impr, module_impr, orientation_impr;
	bool end = false;
	char key = 0;
	int frame = 0;

	while(!end){

		capture.read(src);
		src.copyTo(final);

		Mat votes = Mat::zeros(src.rows,src.cols, CV_32S);

		GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

		//Gradiente X
		Sobel( src, grad_x, CV_64F, 1, 0, 3 );

		//Gradiente Y
		Sobel( src, grad_y2, CV_64F, 0, 1, 3 );
		grad_y = grad_y2;

		//Modulo
		magnitude(grad_x, grad_y, module);

		//Orientacion
		phase(grad_x, grad_y, orientation);


		/*Metodo indirecto. Canny, HoughLines y se intersecta cada linea con la
		 * linea del horizonte.
		 */
		Canny(src, final, 50, 200, 3);
		src.copyTo(colorfinal);
		vector<Vec4i> lines;
		HoughLinesP(final, lines, 1, CV_PI/180, 50, 50, 10 );
		for( size_t i = 0; i < lines.size(); i++ ){
			Vec4i l = lines[i];
			line( colorfinal, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 1, CV_AA);
			//						x1,    y1			x2,	  y2

			if(l[3]!=l[1]){
				//Filtro para eliminar lineas verticales y horizontales
				double err = 0.1;
				double a = abs(l[2]-l[0]);
				double b = abs(l[3]-l[1]);
				double orient = atan(a/b);

				if((!(orient<(M_PI/2)+err) || !(orient>(M_PI/2)-err)) &&      		//PI/2
						(!(orient<(3*M_PI/2)+err) || !(orient>(3*M_PI/2)-err)) &&	//3*PI/2
						(!(orient<(M_PI)+err) || !(orient>(M_PI)-err)) &&			//PI
						(orient>0+err)	&& (orient<(2*M_PI)-err)		 			//0 y 2*PI (por separado)
				){

					for (int v=0; v<votes.rows; v++) {
						if((l[3]-l[1]) != 0){
							double vote = l[0] + ((v-l[1])*(l[2]-l[0]))/(l[3]-l[1]);
							if(vote>0 && vote<votes.cols){
								votes.at<unsigned>(v,vote)++;
								circle(colorfinal, Point(vote,v), 1, Scalar(255,255,0));

							}
						}
					}
				}
			}
		}

		double min, max;
		Point min_loc, max_loc, min_loc_viejo, max_loc_viejo;
		minMaxLoc(votes, &min, &max, &min_loc, &max_loc);

		//Se pinta una cruz en la imagen para indicar el punto mas votado
		line(colorfinal, Point(max_loc.x-10, max_loc.y-10), Point(max_loc.x+10, max_loc.y+10), Scalar(255,0,0), 4);
		line(colorfinal, Point(max_loc.x+10, max_loc.y-10), Point(max_loc.x-10, max_loc.y+10), Scalar(255,0,0), 4);

		imshow( "Original", src );
		imshow( "Final color", colorfinal);

		key = cv::waitKey(1);
		if(key==27){
			end = true;
		}

		if(frame==20){
			frame=0;
		}
		frame++;
	}

	return 0;

}
