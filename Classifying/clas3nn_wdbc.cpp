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

vector< vector<float> > readfilewdbc() {
	ifstream myfile("wdbc.arff");
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
				for (int j = 0; j<30; j++) {
					if (j == 0) i = 2;
					while ((word[i] != ',') && i<word.size()) {
						aux_word += word[i];
						i++;
					}
					istringstream iss(aux_word);
					iss >> f;
					v.push_back(f);
					i++;
					aux_word = "";
				}
				if (word[0] == 'B') v.push_back(0.0);
				else if (word[0] == 'M') v.push_back(1.0);
			}
	
			if (b == true) {
				datos.push_back(v);
				v.clear();
			}
			if (word == "@data") b = true;

		}
		myfile.close();
	}

	else cout << "Unable to open file" << endl;
	return datos;
}

int clas3nn(vector<float> ejemplo, vector< vector<float> > matriz) {
	clock_t start_time;
	double elapsed;
	double total_time;
	vector<float> distancias;
	float sum = 0;
	float resta;
	start_time = clock();
	for (int i = 0; i<matriz.size(); i++) {
		for (int j = 0; j < ejemplo.size() - 1 /*no me interesa almacenar la clase*/; j++) {
			resta = ejemplo[j] - matriz[i][j];
			sum += resta*resta;
		}
		distancias.push_back(sum);
		sum = 0;
	}
	elapsed = clock() - start_time;
	total_time = elapsed / CLOCKS_PER_SEC;
	cout << "Tarda en calcular el vector distancias " << total_time << " segundos" << endl; // 0.01 segundos
	vector<int> clases;
	float min = 1000;
	int pos_min = 0;
	int t = 0;
	start_time = clock();
	while (t<3) {
		for (int k = 0; k<distancias.size(); k++) {
			if (distancias[k]<min && distancias[k] != -1) {
				min = distancias[k];
				pos_min = k;	//me interesa la posición en el vector distancias
			}
		}
		clases.push_back(matriz[pos_min][ejemplo.size() - 1]); // ejemplo.size()-1 corresponde a la posición de la clase
		distancias[pos_min] = -1;
		min = 1000;
		t++;
	}
	elapsed = clock() - start_time;
	total_time = elapsed / CLOCKS_PER_SEC;
	cout << "Tarda en calcular las tres distancias más cortas " << total_time << " segundos" << endl; // 0.001 segundos

	start_time = clock();
	int clase;
	if (clases[0] == clases[1] || clases[0] == clases[2]) clase = clases[0];
	else if (clases[1] == clases[0] || clases[1] == clases[2]) clase = clases[1];
	else clase = clases[0]; //si hay empate se toma la más cercana

	elapsed = clock() - start_time;
	total_time = elapsed / CLOCKS_PER_SEC;
	cout << "Tarda en escoger la clase más cercana " << total_time << " segundos" << endl; // 0 segundos
	
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
	Set_random(47);
	vector< vector<float> > datos = readfilewdbc();
	datos = normaliza_datos(datos);
	vector< vector<float> > prueba1;
	vector< vector<float> > conjunto1;
	vector<int> clases;
	int c;
	clock_t start_time;
	double elapsed;
	double total_time;
	// *********************** 5x2 CROSS - VALIDATION ****************************************

	vector<int> claseM, claseB, posicionesM, posicionesB;
	int max, posicion = 0;
	bool repetida, norepetida = false;

	for (int i = 0; i < datos.size(); i++) {
		if (datos[i][datos[i].size() - 1] == 1) claseM.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 0) claseB.push_back(i);
	}

	for (int i = 0; i < claseM.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, claseM.size() - 1);
			repetida = false;
			for (int j = 0; j < posicionesM.size(); j++) {
				if (posicionesM[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[claseM[posicion]]);
		posicionesM.push_back(posicion);
	}

	for (int i = 0; i < claseM.size(); i++) {
		repetida = false;
		for (int j = 0; j < posicionesM.size(); j++) {
			if (posicionesM[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[claseM[i]]);
	}

	for (int i = 0; i < claseB.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, claseB.size() - 1);
			repetida = false;
			for (int j = 0; j < posicionesB.size(); j++) {
				if (posicionesB[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[claseB[posicion]]);
		posicionesB.push_back(posicion);
	}

	for (int i = 0; i < claseB.size(); i++) {
		repetida = false;
		for (int j = 0; j < posicionesB.size(); j++) {
			if (posicionesB[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[claseB[i]]);
	}

	// Intercambio de conjunto y prueba para la ejecución a la viceversa
	/*vector < vector<float> > prueba_aux;
	prueba_aux = prueba1;
	prueba1 = conjunto1;
	conjunto1 = prueba_aux;*/

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
	cout << aciertos << endl;
	cout << "Tiempo = " << total_time << " segundos" << endl;
	cout << "Tasa = " << tasa << endl;
	/*for (int j = 0; j < clases.size(); j++) {
		//cout << "clases[" << j << "] = " << clases[j] << endl;
	}*/
	
}