#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "random_ppio.hpp"
#include <time.h>
#include <stdio.h>
using namespace std;

vector< vector<float> > readfilelibras() {
	ifstream myfile("movement_libras.arff");
	vector<float> v;
	vector< vector<float> > datos;
	string word, aux_word = "";
	float f = 0;
	bool b = false;
	int i;
	if (myfile.is_open())
	{
		while (myfile >> word)
		{
			if (b == true) {
				for (int j = 0; j < 91; j++) {
					if (j == 0) i = 0;
					while ((word[i] != ',') && i < word.size()) {
						aux_word += word[i];
						i++;
						//j=i;
					}
					istringstream iss(aux_word);
					iss >> f;
					v.push_back(f);
					i++;
					aux_word = "";
				}
			}
			/*for (int i = 0; i < v.size(); i++) {
			cout << "= v[i] = " << v[i] << endl;
			}*/		// VECTOR LEÍDO
			if (b == true) {
				datos.push_back(v);
				v.clear();
			}
			if (word == "@data") b = true;

		}
		/*for (int j = 0; j < datos.size(); j++) {
		for (int k = 0; k < datos[j].size(); k++) {
		cout << "datos[j][k] = " << datos[j][k] << endl;
		}
		}*/		// MATRIZ RESULTANTE
		myfile.close();
	}

	else cout << "Unable to open file" << endl;
	return datos;
}
int clas3nn(vector<float> ejemplo, vector< vector<float> > matriz) {
	//clock_t start_time;
	//double elapsed;
	//double total_time;
	vector<float> distancias;
	float sum = 0;
	float resta;
	//start_time = clock();
	for (int i = 0; i<matriz.size(); i++) {
		for (int j = 0; j < ejemplo.size() - 1 /*no me interesa almacenar la clase*/; j++) {
			resta = ejemplo[j] - matriz[i][j];
			sum += resta*resta;
		}
		distancias.push_back(sum);
		sum = 0;
	}
	//elapsed = clock() - start_time;
	//total_time = elapsed / CLOCKS_PER_SEC;
	//cout << "1	Tarda en el doble bucle " << total_time << " segundos" << endl;
	vector<int> clases;
	float min = 1000;
	int pos_min = 0;
	int t = 0;
	while (t<3) {
		for (int k = 0; k<distancias.size(); k++) {
			if (distancias[k]<min && distancias[k] != 0) {
				min = distancias[k];
				pos_min = k;	//me interesa la posición en el vector distancias
				distancias[k] = 0;
			}
		}
		clases.push_back(matriz[pos_min][ejemplo.size() - 1]); //ejemplo.size()-1 corresponde a la posición de la clase
		min = 1000;
		t++;
	}

	int clase;
	if (clases[0] == clases[1] || clases[0] == clases[2]) clase = clases[0];
	else if (clases[1] == clases[0] || clases[1] == clases[2]) clase = clases[1];
	else clase = clases[0]; //si hay empate se toma la más cercana

	return clase;
}

vector< vector<float> > normaliza_datos(vector< vector<float> > datos) {
	int columnas = datos[0].size();
	int filas = datos.size();
	vector< vector<float> > datosn = datos;
	//vector<float> vn;
	//float valorn;

	for (int i = 0; i < columnas - 1; i++) {
		float max = -9999.999;
		float min = 9999.999;
		for (int j = 0; j < filas; j++) {
			if (datos[j][i] < min) min = datos[j][i];
			if (datos[j][i] > max) max = datos[j][i];
		}
		/*cout << "Columna " << i << endl;
		cout << "El minimo es " << min << " y el maximo es " << max << endl;*/
		for (int j = 0; j < filas; j++) {
			datosn[j][i] = (datos[j][i] - min) / (max - min);
		}
	}
	return datosn;
}

int main() {
	Set_random(14);
	vector< vector<float> > datos = readfilelibras();
	datos = normaliza_datos(datos);
	vector< vector<float> > prueba1;
	vector< vector<float> > conjunto1;
	vector<int> clases;
	int c;
	clock_t start_time;
	double elapsed;
	double total_time;
	// *****************************************	5X2 CROSS - VALIDATION		*********************************************************

	vector<int> clase1, clase2, clase3, clase4, clase5, clase6, clase7, clase8, clase9, clase10, clase11, clase12, clase13, clase14, clase15;
	vector<int> posiciones1, posiciones2, posiciones3, posiciones4, posiciones5, posiciones6, posiciones7, posiciones8, posiciones9, posiciones10, posiciones11, posiciones12, posiciones13, posiciones14, posiciones15;
	int max, posicion = 0;
	bool repetida, norepetida = false;

	for (int i = 0; i < datos.size(); i++) {
		if (datos[i][datos[i].size() - 1] == 1) clase1.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 2) clase2.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 3) clase3.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 4) clase4.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 5) clase5.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 6) clase6.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 7) clase7.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 8) clase8.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 9) clase9.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 10) clase10.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 11) clase11.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 12) clase12.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 13) clase13.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 14) clase14.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 15) clase15.push_back(i);
	}


	// Determinación aleatoria de los elementos de clase 1 que van al prueba
	for (int i = 0; i < clase1.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase1.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones1.size(); j++) {
				if (posiciones1[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase1[posicion]]);
		posiciones1.push_back(posicion);
	}

	// Determinación de los elementos de clase 1 restantes que van al conjunto
	for (int i = 0; i < clase1.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones1.size(); j++) {
			if (posiciones1[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase1[i]]);
	}

	// Determinación aleatoria de los elementos de clase 2 que van al prueba
	for (int i = 0; i < clase2.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase2.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones2.size(); j++) {
				if (posiciones2[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase2[posicion]]);
		posiciones2.push_back(posicion);
	}
	// Determinación de los elementos de clase 2 restantes que van al conjunto
	for (int i = 0; i < clase2.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones2.size(); j++) {
			if (posiciones2[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase2[i]]);
	}

	// Determinación aleatoria de los elementos de clase 3 que van al prueba
	for (int i = 0; i < clase3.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase3.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones3.size(); j++) {
				if (posiciones3[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase3[posicion]]);
		posiciones3.push_back(posicion);
	}
	// Determinación de los elementos de clase 3 restantes que van al conjunto
	for (int i = 0; i < clase3.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones3.size(); j++) {
			if (posiciones3[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase3[i]]);
	}

	// Determinación aleatoria de los elementos de clase 4 que van al prueba
	for (int i = 0; i < clase4.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase4.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones4.size(); j++) {
				if (posiciones4[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase4[posicion]]);
		posiciones4.push_back(posicion);
	}
	// Determinación de los elementos de clase 4 restantes que van al conjunto
	for (int i = 0; i < clase4.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones4.size(); j++) {
			if (posiciones4[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase4[i]]);
	}

	// Determinación aleatoria de los elementos de clase 5 que van al prueba
	for (int i = 0; i < clase5.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase5.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones5.size(); j++) {
				if (posiciones5[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase5[posicion]]);
		posiciones5.push_back(posicion);
	}
	// Determinación de los elementos de clase 5 restantes que van al conjunto
	for (int i = 0; i < clase5.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones5.size(); j++) {
			if (posiciones5[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase5[i]]);
	}

	// Determinación aleatoria de los elementos de clase 6 que van al prueba
	for (int i = 0; i < clase6.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase6.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones6.size(); j++) {
				if (posiciones6[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase6[posicion]]);
		posiciones6.push_back(posicion);
	}
	// Determinación de los elementos de clase 6 restantes que van al conjunto
	for (int i = 0; i < clase6.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones6.size(); j++) {
			if (posiciones6[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase6[i]]);
	}

	// Determinación aleatoria de los elementos de clase 7 que van al prueba
	for (int i = 0; i < clase7.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase7.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones7.size(); j++) {
				if (posiciones7[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase7[posicion]]);
		posiciones7.push_back(posicion);
	}
	// Determinación de los elementos de clase 7 restantes que van al conjunto
	for (int i = 0; i < clase7.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones7.size(); j++) {
			if (posiciones7[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase7[i]]);
	}

	// Determinación aleatoria de los elementos de clase 8 que van al prueba
	for (int i = 0; i < clase8.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase8.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones8.size(); j++) {
				if (posiciones8[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase8[posicion]]);
		posiciones8.push_back(posicion);
	}
	// Determinación de los elementos de clase 8 restantes que van al conjunto
	for (int i = 0; i < clase8.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones8.size(); j++) {
			if (posiciones8[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase8[i]]);
	}

	// Determinación aleatoria de los elementos de clase 9 que van al prueba
	for (int i = 0; i < clase9.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase9.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones9.size(); j++) {
				if (posiciones9[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase9[posicion]]);
		posiciones9.push_back(posicion);
	}
	// Determinación de los elementos de clase 9 restantes que van al conjunto
	for (int i = 0; i < clase9.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones9.size(); j++) {
			if (posiciones9[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase9[i]]);
	}

	// Determinación aleatoria de los elementos de clase 10 que van al prueba
	for (int i = 0; i < clase10.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase10.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones10.size(); j++) {
				if (posiciones10[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase10[posicion]]);
		posiciones10.push_back(posicion);
	}
	// Determinación de los elementos de clase 10 restantes que van al conjunto
	for (int i = 0; i < clase10.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones10.size(); j++) {
			if (posiciones10[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase10[i]]);
	}

	// Determinación aleatoria de los elementos de clase 11 que van al prueba
	for (int i = 0; i < clase11.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase11.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones11.size(); j++) {
				if (posiciones11[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase11[posicion]]);
		posiciones11.push_back(posicion);
	}
	// Determinación de los elementos de clase 11 restantes que van al conjunto
	for (int i = 0; i < clase11.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones11.size(); j++) {
			if (posiciones11[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase11[i]]);
	}

	// Determinación aleatoria de los elementos de clase 12 que van al prueba
	for (int i = 0; i < clase12.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase12.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones12.size(); j++) {
				if (posiciones12[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase12[posicion]]);
		posiciones12.push_back(posicion);
	}
	// Determinación de los elementos de clase 12 restantes que van al conjunto
	for (int i = 0; i < clase12.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones12.size(); j++) {
			if (posiciones12[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase12[i]]);
	}

	// Determinación aleatoria de los elementos de clase 13 que van al prueba
	for (int i = 0; i < clase13.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase13.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones13.size(); j++) {
				if (posiciones13[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase13[posicion]]);
		posiciones13.push_back(posicion);
	}
	// Determinación de los elementos de clase 13 restantes que van al conjunto
	for (int i = 0; i < clase13.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones13.size(); j++) {
			if (posiciones13[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase13[i]]);
	}

	// Determinación aleatoria de los elementos de clase 14 que van al prueba
	for (int i = 0; i < clase14.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase14.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones14.size(); j++) {
				if (posiciones14[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase14[posicion]]);
		posiciones14.push_back(posicion);
	}
	// Determinación de los elementos de clase 14 restantes que van al conjunto
	for (int i = 0; i < clase14.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones14.size(); j++) {
			if (posiciones14[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase14[i]]);
	}

	// Determinación aleatoria de los elementos de clase 15 que van al prueba
	for (int i = 0; i < clase15.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase15.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones15.size(); j++) {
				if (posiciones15[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase15[posicion]]);
		posiciones15.push_back(posicion);
	}
	// Determinación de los elementos de clase 15 restantes que van al conjunto
	for (int i = 0; i < clase15.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones15.size(); j++) {
			if (posiciones15[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase15[i]]);
	}

	// Intercambio de conjunto y prueba para la ejecución a la viceversa
	vector < vector<float> > prueba_aux;
	prueba_aux = prueba1;
	prueba1 = conjunto1;
	conjunto1 = prueba_aux;

	/***************************** CLASIFICA ****************************************/
	float aciertos = 0.0;
	float tasa;

	start_time = clock();
	for (int i = 0; i < prueba1.size(); i++) {
		c = clas3nn(prueba1[i], conjunto1);
		if (c == prueba1[i][prueba1[i].size() - 1]) {
			aciertos += 1;
		}
	}
	tasa = 100 * (aciertos / prueba1.size());
	elapsed = clock() - start_time;
	total_time = elapsed / CLOCKS_PER_SEC;

	cout << "Aciertos = "<< aciertos << " de " << prueba1.size() << endl;
	cout << "Tiempo = " << total_time << " segundos" << endl;
	cout << "Tasa = " << tasa << endl;
	/*for (int j = 0; j < clases.size(); j++) {
	//cout << "clases[" << j << "] = " << clases[j] << endl;
	}*/

}