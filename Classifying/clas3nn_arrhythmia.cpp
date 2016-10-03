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

vector< vector<float> > readfilearrhythmia() { 
	ifstream myfile("arrhythmia.arff");
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
				for (int j = 0; j < 279; j++) {
					if (j == 0) i = 0;
					while ((word[i] != ',') && i < word.size()) {
						aux_word += word[i];
						i++;
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

	for (int i = 0; i < columnas - 1; i++) {
		float max = -9999.999;
		float min = 9999.999;
		for (int j = 0; j < filas; j++) {
			if (datos[j][i] < min) min = datos[j][i];
			if (datos[j][i] > max) max = datos[j][i];
		}
		for (int j = 0; j < filas; j++) {
			datosn[j][i] = abs(min - datos[j][i]) / abs(min - max);
		}
	}
	return datosn;
}

int main() {
	Set_random(47);
	vector< vector<float> > datos = readfilearrhythmia();
	datos = normaliza_datos(datos);
	vector< vector<float> > prueba1;
	vector< vector<float> > conjunto1;
	vector<int> clases;
	int c;
	clock_t start_time;
	double elapsed;
	double total_time;
	// *****************************************	5X2 CROSS - VALIDATION		*********************************************************

	vector<int> clase1, clase2, clase6, clase10, clase16;
	vector<int> posiciones1, posiciones2, posiciones6, posiciones10, posiciones16;
	int posicion = 0;
	bool repetida, norepetida = false;


	for (int i = 0; i < datos.size(); i++) {
		if (datos[i][datos[i].size() - 1] == 1) clase1.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 2) clase2.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 6) clase6.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 10) clase10.push_back(i);
		else if (datos[i][datos[i].size() - 1] == 16) clase16.push_back(i);
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

	// Determinación aleatoria de los elementos de clase 16 que van al prueba
	for (int i = 0; i < clase16.size() / 2; i++) {
		norepetida = false;
		while (!norepetida) {
			posicion = Randint(0, clase16.size() - 1);
			repetida = false;
			for (int j = 0; j < posiciones16.size(); j++) {
				if (posiciones16[j] == posicion) repetida = true;
			}
			if (repetida) norepetida = false;
			else norepetida = true;
		}
		prueba1.push_back(datos[clase16[posicion]]);
		posiciones16.push_back(posicion);
	}
	// Determinación de los elementos de clase 16 restantes que van al conjunto
	for (int i = 0; i < clase16.size(); i++) {
		repetida = false;
		for (int j = 0; j < posiciones16.size(); j++) {
			if (posiciones16[j] == i) repetida = true;
		}
		if (!repetida) conjunto1.push_back(datos[clase16[i]]);
	}
	// **************************************************************************************************

	// Intercambio de conjunto y prueba para la ejecución a la viceversa
	/*vector < vector<float> > prueba_aux;
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

	cout << "Aciertos = " << aciertos << " de " << prueba1.size() << endl;
	cout << "Tiempo = " << total_time << " segundos" << endl;
	cout << "Tasa = " << tasa << endl;
	/*for (int j = 0; j < clases.size(); j++) {
	//cout << "clases[" << j << "] = " << clases[j] << endl;
	}*/

}