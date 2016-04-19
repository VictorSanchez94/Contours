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

	int respuesta = 0;
	string imagen;
	cout << "¿Que opcion deseas?" << endl << "1.- Punto de Fuga sobre una imagen" << endl << "2.- Punto de Fuga en tiempo real" << endl;
	cin >> respuesta;

	if(respuesta==1){
		cout << "Introduce el nombre de la imagen: ";
		cin >> imagen;
		stringstream ss;
		ss << "./Data/pasillo" << imagen << ".pgm";
		indirectWithHorizont(ss.str());
	}else if(respuesta==2){
		indirectWithoutHorizont();
	}else{
		cout << "Opcion desconocida" << endl << "Fin del programa";
	}

	return 0;
}



