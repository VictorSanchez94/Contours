#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

using namespace cv;

/** @function main */
int main( int argc, char** argv )
{

  Mat src, src_gray;
  Mat grad;

  //char* window_name = "Sobel Demo - Simple Edge Detector";
  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;

  //int c;

  /// Load an image
  src = imread( argv[1] );

  //Create candidates matrix
  Mat horizont = Mat::zeros(1,src.cols, CV_8U);

  if( !src.data )
  { return -1; }

  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

  Mat grad_x, grad_y, grad_x2, grad_y2, module, orientation, orientation2;

  //Gradiente X
  Sobel( src, grad_x, CV_32F, 1, 0, 3 );
  grad_x2 = grad_x/2 + 128;
  grad_x2.convertTo(grad_x2, CV_8U);


  //Gradiente Y
  Sobel( src, grad_y, CV_32F, 0, 1, 3 );
  grad_y2 = - grad_y;
  grad_y2 = grad_y2/2 + 128;
  grad_y2.convertTo(grad_y2, CV_8U);

  //Modulo
  magnitude(grad_x, grad_y, module);
  module.convertTo(module, CV_8U);

  //Orientacion
  bool in_degrees = true;
  phase(grad_x, -grad_y, orientation, in_degrees);
  if(in_degrees){
	  orientation2 = orientation/180*128;
  }else{
	  orientation2 = orientation/3.1415*128;
  }
  orientation2.convertTo(orientation2, CV_8U);


  /* Metodo directo. Se comprueban todos los puntos de la imagen y si su modulo es mayor
   * que un umbral, se crea su recta con su orientacion para intersectar con la linea del
   * horizonte. */
  int umbral = 200;
  for(int i=0; i<src.rows; i++){
	  for(int j=0; j<src.cols; j++){
		  double mod = module.at<int>(i,j);
		  if(mod > umbral){
			  double orient = orientation2.at<int>(i,j);
			  //Se construyen las ecuaciones de las dos rectas
			  double pendiente;
			  if(orientation2.at<int>(i,j) == 0){
				  pendiente = 0;
			  }else{
				  pendiente = tan(orientation2.at<int>(i,j));
			  }

			  //y=m(x-x0)+y0
			  //y=src.rows/2
			  //m(x-x0)+y0 = src.rows/2
			  //mx-mx0+y0 = src.rows/2
			  //mx = src.rows/2 + mx0 - y0
			  //x = src.rows/2 + mx0 - y0
			  int votado = src.rows/2 + pendiente*i - j;
			  if(votado>=0 && votado<=src.rows){
				  horizont.at<uchar>(votado) = horizont.at<uchar>(votado) + 1;
			  }

		  }
	  }
  }

  //Se mira a ver cual ha sido el punto del horizonte mas votado
  int masVotado = 0;
  for (int i=0; i<horizont.cols; i++){
	  //int a = horizont.at<uchar>(i);
	  if(horizont.at<uchar>(i) > horizont.at<uchar>(masVotado)){
		  masVotado = i;
	  }
  }

  Mat final = src.clone();
  for (int i=0; i<src.rows; i++){
	  for (int j=0; j<src.cols; j++){
		  if(i==src.rows/2){
			  final.at<int>(final.rows/2, j) = 0;
		  }
	  }
  }
  final.at<uchar>(final.rows/2, masVotado) = 254;

  imshow( "Original", src );
  imshow( "Gradiente_X", grad_x2 );
  imshow( "Gradiente_Y", grad_y2 );
  imshow( "Modulo", module);
  imshow( "Orientation", orientation2);
  imshow( "Final", final);

  waitKey(0);

  return 0;
  }




//Para la parte opcional:
//Metodo indirecto: Canny, HoughLines para sacar las rectas que aproximan los contornos y luego que?
