/*
BÚSQUEDA LOCAL
Recibe dos conjuntos de datos (dos matrices) una de prueba y otra de entrenamiento
Se genera un s (vector inicial de ceros y unos) de forma aleatoria
Se generan soluciones vecinas (aplicando flip(s,i) siendo i la posición en s) hasta encontrar una mejor que la actual
Se vuelve a generar el vecindario del nuevo s
Paramos cuando exploramos todo el vecindario sin encontrar mejora
*/

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

vector<int> flip(vector<int> s, int i) {
	vector<int> vecino;
	vecino = s;
	if (vecino[i] == 0) vecino[i] = 1;
	else if (vecino[i] == 1) vecino[i] = 0;

	return vecino;
}

int clas3nn(vector<float> ejemplo, vector< vector<float> > matriz) {
	vector<float> distancias;
	float sum = 0;
	float resta;
	for (int i = 0; i<matriz.size(); i++) {
		for (int j = 0; j < ejemplo.size() - 1 /*no me interesa almacenar la clase*/; j++) {
			resta = ejemplo[j] - matriz[i][j];
			sum += resta*resta;
		}
		distancias.push_back(sum);
		sum = 0;
	}
	vector<int> clases;
	float min = 1000;
	int pos_min = 0;
	int t = 0;
	while (t<3) {
		for (int k = 0; k<distancias.size(); k++) {
			if (distancias[k]<min && distancias[k] != 0) {
				min = distancias[k];	//me interesa la posición en el vector distancias
				pos_min = k;
			}
		}
		clases.push_back(matriz[pos_min][ejemplo.size() - 1]); //ejemplo.size()-1 corresponde a la posición de la clase
		distancias[pos_min] = 0;
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
			datosn[j][i] = (datos[j][i] - min) / (max - min);
		}
	}
	return datosn;
}

int main() {
	vector < vector<float> > prueba;
	vector < vector<float> > conjunto;
	vector < vector<float> > datoss, datos;
	vector <int> s;
	Set_random(14); // esta inicialización debe poder pasarse como parámetro
	datoss = readfilewdbc();
	datos = normaliza_datos(datoss);
	

	// *********************** 5x2 CROSS - VALIDATION ********************************************************

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
		prueba.push_back(datos[claseM[posicion]]);
		posicionesM.push_back(posicion);
	}

	for (int i = 0; i < claseM.size(); i++) {
		repetida = false;
		for (int j = 0; j < posicionesM.size(); j++) {
			if (posicionesM[j] == i) repetida = true;
		}
		if (!repetida) conjunto.push_back(datos[claseM[i]]);
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
		prueba.push_back(datos[claseB[posicion]]);
		posicionesB.push_back(posicion);
	}

	for (int i = 0; i < claseB.size(); i++) {
		repetida = false;
		for (int j = 0; j < posicionesB.size(); j++) {
			if (posicionesB[j] == i) repetida = true;
		}
		if (!repetida) conjunto.push_back(datos[claseB[i]]);
	}

	// **************************************************************************************************

	// Intercambio de conjunto y prueba para la ejecución a la viceversa
	vector < vector<float> > prueba_aux;
	prueba_aux = prueba;
	prueba = conjunto;
	conjunto = prueba_aux;

	// Generación de la solución inicial

	int r;
	int tam = prueba[0].size() - 1;
	//cout << "tam bien" << endl;
	for (int i = 0; i < tam; i++) {
		r = Randint(0, 1);
		s.push_back(r);		// vector de características iniciales inicializado de forma aleatorio con semilla inicializada arriba
		//cout << "s["<<i<<"] = " << s[i] << endl;
	}
	vector< vector<float> > prueba_s;
	vector<float> v;
	for (int i = 0; i < prueba.size(); i++) {
		for (int k = 0; k < s.size(); k++) {
			v.push_back(prueba[i][k] * s[k]);
		//	cout << k << " -> Meto en v: " << prueba[i][k] * s[k] << endl;
		}
		v.push_back(prueba[i][s.size()]);
		prueba_s.push_back(v);	// prueba_s es la prueba con las caracteristicas elegidas
		v.clear();
	}

	// Cálculo del coste inicial

	int clase;
	float aciertos = 0.0;
	for (int i = 0; i < prueba_s.size(); i++) {
		clase = clas3nn(prueba_s[i], conjunto);
		//cout << "para prueba_s[" << i << "] el valor de clase es = " << clase;
		//cout << " y clase = " << prueba[i][prueba[i].size() - 1] << endl;
		if (clase == prueba[i][prueba[i].size() - 1]) {
			aciertos+=1;
			//cout << "aciertos = " << aciertos << endl;
		}
	}	// en aciertos quedan almacenados los aciertos del clasificador con el s inicial
	//cout << "aciertos = " << aciertos << endl;
	float mejor_tasa = 100 * (aciertos / prueba.size());
	//cout << "tasa inicial = " << mejor_tasa << endl;
	float tasa_clas;
	float mejor_red;
	float unos = 0.0;
	for (int h = 0; h < s.size(); h++) {		// Se calcula la tasa de reducción inicial
		if (s[h] == 1) unos += 1;
	}
	mejor_red = 100 * ((s.size() - unos) / s.size());
	unos = 0.0;
	int i = 0;
	float aciertos_vecino = 0.0;
	vector<int> vecino;
	vector< vector<float> > prueba_vecino;
	clock_t start_time;
	double elapsed;
	double total_time;

	start_time = clock();
	while ( i<s.size() ) {
		//cout << endl << " ********** i = " << i << endl;
		vecino = flip(s, i);
		//cout << "Hecho el flip(s," << i << ")" << endl;
		for (int j = 0; j < prueba.size(); j++) {
			for (int k = 0; k < vecino.size(); k++) {
				v.push_back(prueba[j][k] * vecino[k]);
			}
			v.push_back(prueba[j][vecino.size()]); // donde se encuentra la clase a la que pertenece
			prueba_vecino.push_back(v);	// prueba_vecino es la prueba con las caracteristicas elegidas en vecino = flip(s,i)
			v.clear();
		}
		for (int j = 0; j < prueba_vecino.size(); j++) {
			clase = clas3nn(prueba_vecino[j], conjunto);
			//cout << "para prueba_vecino[" << j << "] el valor de clase es = " << clase;
			//cout << " y clase = " << prueba[j][prueba[j].size() - 1] << endl;
			if (clase == prueba[j][prueba[j].size() - 1]) {
				aciertos_vecino+=1;
				//cout << "aciertos_vecino = " << aciertos_vecino << endl;
			}
		}

		tasa_clas = 100 * (aciertos_vecino / prueba.size());
		//cout << "Tasa clas = " << tasa_clas << " y mejor tasa = " << mejor_tasa << endl;

		if (tasa_clas > mejor_tasa) {
			s = vecino;
			//cout << "Hay mejora" << endl;
			aciertos = aciertos_vecino;
			mejor_tasa = tasa_clas;
			for (int h = 0; h < s.size(); h++) {
				if (s[h] == 1) unos += 1;
			}
			mejor_red = 100 * ((s.size() - unos) / s.size());
			i = 0;
		}
		else {
			//cout << "NO HAY MEJORA EN i = " << i << endl;
			i++;
		
	}
		vecino.clear();
		aciertos_vecino = 0;
		prueba_vecino.clear();
		unos = 0.0;
	}
	elapsed = clock() - start_time;
	total_time = elapsed / CLOCKS_PER_SEC;
	cout << "Mejor tasa = " << mejor_tasa << endl;
	cout << "Mejor reduccion = " << mejor_red << endl;
	cout << "Tiempo = " << total_time << endl;

	/*
	
	WDBC ---------------------------------

	1 - 1 semilla 47 *
		Mejor tasa = 93.3099
		Mejor reduccion = 23.3333
		Tiempo = 1164.31

	1 - 2 semilla 47 *
		Mejor tasa = 62.807
		Mejor reduccion = 66.6667
		Tiempo = 115.877

	1 - 3	semilla 33 *
		Mejor tasa = 97.5352
		Mejor reduccion = 16.6667
		Tiempo = 892.485

	3 - 1	semilla 33 *
		Mejor tasa = 94.7368
		Mejor reduccion = 16.6667
		Tiempo = 730.492

	1 - 4	semilla 21 *
		Mejor tasa = 98.2394
		Mejor reduccion = 20
		Tiempo = 1695.76

	4 - 1	semilla 21 *
		Mejor tasa = 62.807
		Mejor reduccion = 60
		Tiempo = 118.869

	1 - 5	semilla 87 *
		Mejor tasa = 97.8873
		Mejor reduccion = 0
		Tiempo = 1902.82

	5 - 1	semilla 87 *
		Mejor tasa = 62.807
		Mejor reduccion = 73.3333
		Tiempo = 123.979

	1 - 6	semilla 14 *
		Mejor tasa = 94.7183
		Mejor reduccion = 23.3333
		Tiempo = 602.013

	6 - 1	semilla 14 *
		Mejor tasa = 97.8947
		Mejor reduccion = 23.3333
		Tiempo = 745.383

	*/
}