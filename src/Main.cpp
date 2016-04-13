#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

/** @function main */
int main( int argc, char** argv )
{

  Mat src, src_gray;
  Mat grad;

  /// Load an image
  src = imread( argv[1] );

  //Create candidates matrix
  Mat horizont = Mat::zeros(1,src.cols, CV_32F);

  if( !src.data )
  { return -1; }

  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

  Mat grad_x, grad_y, grad_x2, grad_y2, module, orientation, orientation2;
  Mat grad_x_impr, grad_y_impr, module_impr, orientation_impr;

  //Gradiente X
  Sobel( src, grad_x, CV_64F, 1, 0, 3 );
  //grad_x2 = grad_x/2 + 128;
  //grad_x2.convertTo(grad_x2, CV_8U);

  //Gradiente Y
  Sobel( src, grad_y, CV_64F, 0, 1, 3 );
  grad_y = - grad_y;
  //grad_y2 = grad_y2/2 + 128;
  //grad_y2.convertTo(grad_y2, CV_8U);

  //Modulo
  magnitude(grad_x, grad_y, module);
  //module.convertTo(module, CV_8U);

  //Orientacion
  phase(grad_x, grad_y, orientation);
  /*if(in_degrees){
	  orientation2 = orientation/180*128;
  }else{
	  orientation2 = orientation/3.1415*128;
  }*/
  //orientation2 = orientation;
  //orientation2.convertTo(orientation2, CV_8U);

  /*int count = 0;
  for (int i=0; i<orientation.rows; i++){
	  for (int j=0; j<orientation.cols; j++){
		  float a = orientation.at<double>(i,j);
		  cout << a << "   ";
	  }
	  cout << endl;
	  count++;
  }
  cout << "NUMFILAS = " << count;*/

  /* Metodo directo. Se comprueban todos los puntos de la imagen y si su modulo es mayor
   * que un umbral, se crea su recta con su orientacion para intersectar con la linea del
   * horizonte. */
  int umbral = 95;
  for(int i=0; i<src.rows; i++){
	  for(int j=0; j<src.cols; j++){
		  double mod = module.at<double>(i,j);
		  if(mod > umbral){
			  double orient = orientation.at<double>(i,j);
			  double pendiente = tan(orient);

			  //Se construyen las ecuaciones de las dos rectas
			  //y=m(x-x0)+y0
			  //y=src.rows/2
			  //m(x-x0)+y0 = src.rows/2
			  //mx-mx0+y0 = src.rows/2
			  //mx = src.rows/2 + mx0 - y0
			  //x = src.rows/2 + mx0 - y0
			  int votado = src.rows/2 + pendiente*i - j;
			  if(votado>=0 && votado<=src.rows){
				  horizont.at<double>(votado) = horizont.at<double>(votado) + 1;
			  }

		  }
	  }
  }

  //Se mira a ver cual ha sido el punto del horizonte mas votado
  int masVotado = 2;
  for (int i=0; i<horizont.cols; i++){
	  if(horizont.at<double>(i) > horizont.at<double>(masVotado)){
		  masVotado = i;
	  }
  }

  //Mat final;
  //src.copyTo(final);
  /*for (int i=0; i<src.rows; i++){
	  for (int j=0; j<src.cols; j++){
		  if(i==src.rows/2){
			  //final.at<int>(final.rows/2, j) = 0;
		  }
	  }
  }*/
  /*for(int i = -10; i<10; i++){
	  final.at<double>(final.rows/2+i, masVotado) = 254;
  }*/


  //Se convierten las matrices para que se puedan mostrar por pantalla
  grad_x2 = grad_x/2 + 128;
  grad_y2 = grad_y/2 + 128;
  orientation2 = orientation/3.1415*128;
  /*grad_x2.convertTo(grad_x_impr, CV_8U);
  grad_y2.convertTo(grad_y_impr, CV_8U);
  module.convertTo(module_impr, CV_8U);
  orientation2.convertTo(orientation_impr, CV_8U);


  imshow( "Original", src );
  imshow( "Gradiente_X", grad_x_impr );
  imshow( "Gradiente_Y", grad_y_impr );
  imshow( "Modulo", module_impr);
  imshow( "Orientation", orientation_impr);*/
  //imshow( "Final", final);

  waitKey(0);

  return 0;
  }




//Para la parte opcional:
//Metodo indirecto: Canny, HoughLines para sacar las rectas que aproximan los contornos y luego que?
