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
  string path;

  for (int photo=1; photo<4; photo++) {

	  stringstream ss;
	  ss << "Data/pasillo" << photo << ".pgm";
	  path = ss.str();

	  /// Load an image
	  src = imread( path );

	  //Create candidates matrix
	  Mat horizont = Mat::zeros(1,src.cols, CV_32F);

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

	  /* Metodo directo. Se comprueban todos los puntos de la imagen y si su modulo es mayor
	   * que un umbral, se crea su recta con su orientacion para intersectar con la linea del
	   * horizonte. */
	  int umbral = 100;
	  //threshold(module, module, umbral, 255, CV_THRESH_BINARY);

	  float err = 0.1;
	  for(int i=0; i<module.rows; i++){
		  for(int j=0; j<module.cols; j++){
			  float mod = module.at<float>(Point(j,i));
			  float orient = orientation.at<float>(Point(j,i));
			  if(mod > umbral){
				  //Filtro para lineas verticales y horizontales
				  /*if(!(orient<err) && !(orient>2*M_PI-err) && !(orient<M_PI/2+err && orient>M_PI/2-err)
						  && !(orient<M_PI*3/2+err && orient>M_PI*3/2-err)
						  && !(orient<M_PI+err && orient>M_PI-err)){*/
				  if ( (orient>err && orient<M_PI/2-err) || (orient>M_PI/2+err && orient<M_PI-err)
						  || (orient>M_PI+err && orient<3*M_PI/2-err) || (orient>3*M_PI/2+err && orient<2*M_PI-err) ) {

					  for (double rad=-0.2; rad<0.3; rad=rad+0.1){
						  cout << rad << " ";
						  double pendiente = tan(orient+rad);
						  //Se construyen las ecuaciones de las dos rectas
						  //y=m(x-x0)+y0
						  //y=src.rows/2
						  //m(x-x0)+y0 = src.rows/2
						  //mx-mx0+y0 = src.rows/2
						  //mx = src.rows/2 + mx0 - y0
						  //x = src.rows/2 + mx0 - y0
						  int votado = src.rows/2 + pendiente*i - j;
						  if(votado>=0 && votado<=src.cols){
							  horizont.at<uchar>(votado) = horizont.at<uchar>(votado) + 1;
							  line(src, Point(j,i), Point(votado,src.rows/2), 240);
						  }
					  }
					  cout << endl;
				  }else{
					  module.at<float>(i,j) = 0.0;
				  }
			  }
		  }
	  }

	  //Se mira a ver cual ha sido el punto del horizonte mas votado
	  int masVotado = 2;
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


	  //imshow( "Original", src );
	  imshow( "Gradiente_X", grad_x_impr );
	  imshow( "Gradiente_Y", grad_y_impr );
	  imshow( "Modulo", module_impr);
	  imshow( "Orientation", orientation_impr);
	  imshow( "Final", final);

	  waitKey(0);
	  cvDestroyAllWindows();
  }

  return 0;
 }
