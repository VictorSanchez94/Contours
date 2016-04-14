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

	Mat src, grad_x, grad_y, module, orientation;
	Mat grad_y2, grad_x2, orientation2, final;
	Mat grad_x_impr, grad_y_impr, module_impr, orientation_impr;


	src = imread( argv[1], 0 );
	if( !src.data )
	{ return -1; }

	src.copyTo(final);
	cvtColor(final, final, CV_GRAY2BGR);

	Mat horizont = Mat::zeros(1,src.cols, CV_8U);
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


	/*Metodo directo. Se comprueban todos los puntos de la imagen y si su modulo es mayor
	 * que un umbral, se crea su recta con su orientacion para intersectar con la linea del
	 * horizonte.
	 */
	int umbral = 95;
	float err = 0.1;
	for(int i=0; i<module.rows; i++){
		for(int j=0; j<module.cols; j++){
			double mod = module.at<double>(i,j);
			double orient = orientation.at<double>(i,j);
			if(mod>umbral){
				//Filtro para eliminar lineas verticales y horizontales
				/*if((!(orient<(M_PI/2)+err) || !(orient>(M_PI/2)-err)) &&      //PI/2
						(!(orient<(3*M_PI/2)+err) || !(orient>(3*M_PI/2)-err)) &&	//3*PI/2
						(!(orient<(M_PI)+err) || !(orient>(M_PI)-err)) &&			//PI
						(orient>0+err)	&& (orient<(2*M_PI)-err)		 			//0 y 2*PI (por separado)
				){*/
					//double pendiente = tan(orient);
					double pendiente = sin(orient)/cos(orient);

					int votado = i+((module.rows/2 - j)/pendiente);
					if(votado>0 && votado<horizont.cols){
						horizont.at<uchar>(votado) = horizont.at<uchar>(votado) + 1;
						circle(final, Point(j,i), 1, Scalar(0,0,255));
						//line(final, Point(j,i), Point(votado,src.rows/2), Scalar(255,255,0));
						/*imshow( "Original", src );
		    		  	  waitKey(0);
		    		  	  cvDestroyAllWindows();
		    		  	  cout << orient*180/M_PI << endl;*/

					}else{
						cout << "Fuera de rango" << " " << pendiente << " " << votado << endl;
						circle(final, Point(j,i), 1, Scalar(0,0,0));
					}
				//}

			}
		}
	}

	//Se mira a ver cual ha sido el punto del horizonte mas votado
	int masVotado = 0;
	for (int i=0; i<horizont.cols; i++){
		circle(final, Point(i,module.rows/2), 1, Scalar(255,0,0));
		if(horizont.at<uchar>(i) > horizont.at<uchar>(masVotado)){
			masVotado = i;
		}
	}

	cout << "El pixel del horizonte es: " << masVotado << endl;
	//Se pinta una cruz en la imagen para indicar el punto mas votado
	line(final, Point(masVotado-10, module.rows/2-10), Point(masVotado+10, module.rows/2+10), Scalar(0,255,0), 4);
	line(final, Point(masVotado+10, module.rows/2-10), Point(masVotado-10, module.rows/2+10), Scalar(0,255,0), 4);


	//Se convierten las matrices para que se puedan mostrar por pantalla
	grad_x2 = grad_x/2 + 128;
	grad_y2 = grad_y/2 + 128;
	orientation2 = orientation/3.1415*128;
	grad_x2.convertTo(grad_x_impr, CV_8U);
	grad_y2.convertTo(grad_y_impr, CV_8U);
	module.convertTo(module_impr, CV_8U);
	orientation2.convertTo(orientation_impr, CV_8U);
	//final.convertTo(final, CV_8UC3);

	imshow( "Original", src );
	imshow( "Gradiente_X", grad_x_impr );
	imshow( "Gradiente_Y", grad_y_impr );
	imshow( "Modulo", module_impr);
	imshow( "Orientation", orientation_impr);
	imshow( "Final", final);

	waitKey(0);

	return 0;
}



