/*
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{

  Mat src, src_gray;
  Mat grad;

  /// Load an image
  src = imread( argv[1] );

  //Create candidates matrix
  Mat horizont = Mat::zeros(1,src.cols, CV_8U);
  Mat contours = Mat::zeros(src.rows,src.cols, CV_32FC3);

  if( !src.data )
  { return -1; }

  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

  Mat grad_x, grad_y, grad_x2, grad_y2, module, orientation, orientation2;
  Mat grad_x_impr, grad_y_impr, module_impr, orientation_impr;

  //Gradiente X
  Sobel( src, grad_x, CV_32F, 1, 0, 3 );

  //Gradiente Y
  Sobel( src, grad_y2, CV_32F, 0, 1, 3 );
  grad_y = - grad_y2;

  //Modulo
  magnitude(grad_x, grad_y, module);

  //Orientacion
  phase(grad_x, grad_y, orientation);

   Metodo directo. Se comprueban todos los puntos de la imagen y si su modulo es mayor
   * que un umbral, se crea su recta con su orientacion para intersectar con la linea del
   * horizonte.
  int umbral = 95;
  Mat threshol, threshol2;
  threshold(module, threshol, umbral, 255, CV_THRESH_BINARY);
  cvtColor(threshol, threshol2, CV_GRAY2BGR);

  //cout << "INICIO" << endl;
  float err = 0.3;
  for(int i=0; i<threshol.rows; i++){
	  for(int j=0; j<threshol.cols; j++){
		  float mod = threshol.at<float>(i,j);
		  float orient = orientation.at<float>(i,j);

		  //Filtro para el umbral del modulo
		  if(mod > 0){
			  //Filtro para eliminar lineas verticales y horizontales
			  if((!(orient<(M_PI/2)+err) || !(orient>(M_PI/2)-err)) &&      //PI/2
			  	 (!(orient<(3*M_PI/2)+err) || !(orient>(3*M_PI/2)-err)) &&	//3*PI/2
				 (!(orient<(M_PI)+err) || !(orient>(M_PI)-err)) 			//PI
				 															//FALTAN LAS DE 0 Y 2*PI!!!!!!
				 ){
				  double pendiente = tan(orient);
				  //Se construyen las ecuaciones de las dos rectas. Despejando se tiene:
				  //y=m(x-x0)+y0  =>  y=src.rows/2  =>  m(x-x0)+y0 = src.rows/2  =>
				  //mx-mx0+y0 = src.rows/2  =>  mx = src.rows/2 + mx0 - y0  =>  x = src.rows/2 + mx0 - y0

				  int votado = module.rows/2 + pendiente*j - i;
				  if(votado>=0 && votado<=horizont.cols){
					  horizont.at<uchar>(votado) = horizont.at<uchar>(votado) + 1;
					  //contours.at<float>(i,j) = 255;
					  //Vec3b p = new Vect3b {100,0,0};
					  //contours.at<Scalar>(i,j) = Scalar(255,0,0);
				  }
			  }else{
				  threshol.at<float>(i,j) = 0.0;
				  cout << "BORRANDO ";
			  }
		  }
	  }
  }
  cout << "FIN";

  for(int i=0; i<contours.rows; i++){
	  for(int j=0; j<contours.cols; j++){
		  cout << contours.at<float>(i,j) << "  ";
	  }
	  cout << endl;
  }

  //Se mira a ver cual ha sido el punto del horizonte mas votado
  int masVotado = 0;
  for (int i=0; i<horizont.cols; i++){
	  if(horizont.at<uchar>(i) > horizont.at<uchar>(masVotado)){
		  masVotado = i;
	  }
  }

  Mat final;
  src.copyTo(final);
  for(int i = -10; i<10; i++){
	  final.at<float>(final.rows/2+i, masVotado) = 254;
	  final.at<float>(final.rows/2, masVotado+i) = 254;
  }


  //Se convierten las matrices para que se puedan mostrar por pantalla
  grad_x2 = grad_x/2 + 128;
  grad_y2 = grad_y/2 + 128;
  orientation2 = orientation/3.1415*128;
  grad_x2.convertTo(grad_x_impr, CV_8U);
  grad_y2.convertTo(grad_y_impr, CV_8U);
  module.convertTo(module_impr, CV_8U);
  orientation2.convertTo(orientation_impr, CV_8U);
  //contours.convertTo(contours, CV_8U);

  imshow( "Original", src );
  imshow( "Gradiente_X", grad_x_impr );
  imshow( "Gradiente_Y", grad_y_impr );
  imshow( "Modulo", module_impr);
  imshow( "Orientation", orientation_impr);
  imshow( "Final", final);
  //imshow( "Contours", contours);
  imshow( "Threshold", threshol);

  waitKey(0);

  return 0;
  }




//Para la parte opcional:
//Metodo indirecto: Canny, HoughLines para sacar las rectas que aproximan los contornos y luego
//una segunda ronda de transformada de Hough para que las lineas de los contornos voten al punto del horizonte
*/
