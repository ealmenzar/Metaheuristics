/* 
SFS recibe un conjunto de entrenamiento (matriz de datos clasificados) y una prueba (matriz de datos a clasificar), ambos del mismo tamaño
Devuelve un vector s de 0 y 1, que indica las características que se toman y las que no
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>
#include "random_ppio.hpp"
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
	float sum = 0.0;
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
			if (distancias[k]<min && distancias[k] != -1) {
				min = distancias[k];	//me interesa la posición en el vector distancias
				pos_min = k;
			}
		}
		clases.push_back(matriz[pos_min][ejemplo.size() - 1]); //ejemplo.size()-1 corresponde a la posición de la clase
		distancias[pos_min] = -1;
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
	Set_random(33); // esta inicialización debe poder pasarse como parámetro
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
	/*vector < vector<float> > prueba_aux;
	prueba_aux = prueba;
	prueba = conjunto;
	conjunto = prueba_aux;
	/**************************************************/

	int tam = prueba[0].size() - 1;
	for (int i = 0; i < tam; i++) {
		s.push_back(0);		// todas las características a 0 (ninguna sin seleccionar inicialmente)
		//cout << "s[i] = " << s[i] << endl;
	}

	vector<int> s_mejor;
	vector< vector<float> > prueba_s;
	vector<float> v;
	s_mejor = s;
	bool hay_mejora = true;
	float aciertos = 0.0;
	float mas_aciertos = 0.0;
	int clase = 0;
	float aux_aciertos = 0.0;
	float mejor_tasa, mejor_red;
	float unos;
	clock_t start_time;
	double elapsed;
	double total_time;

	start_time = clock();
	while (hay_mejora) {
		for (int j = 0; j < s.size(); j++) {
			//cout << "s[j] == 0?" << s[j] << endl;
			prueba_s.clear();
			if (s[j] == 0) {
				s[j] = 1;
				for (int i = 0; i < prueba.size(); i++) {
					for (int k = 0; k < s.size(); k++) {
						if (s[k] == 0) v.push_back(0);
						else v.push_back(prueba[i][k]);
						//v.push_back(prueba[i][k] * s[k]);	
						//cout << k << " -> Meto en v: " << prueba[i][k] * s[k] << endl;
					}
					v.push_back(prueba[i][s.size()]);
					prueba_s.push_back(v);	// prueba_s es la prueba con las caracteristicas elegidas
					v.clear();
					//cout << "Introducido vector en paso " << i << endl;
					//cout << "	Prueba_s.size() = " << prueba_s.size() << endl;
				}
				// ahora aplico el clasificador para cada fila de prueba_s y cuento cuántas veces acierto con las clases
				aciertos = 0;
				for (int i = 0; i < prueba_s.size(); i++) {
					clase = clas3nn(prueba_s[i],conjunto);
					//cout << "Para prueba_s[" << i << "] el valor de clase es = " << clase << endl;
					if (clase == prueba[i][prueba[i].size() - 1]) {
						aciertos+=1;
					}
				}
				cout << "En j = " << j << ", aciertos = " << aciertos << endl;
				if (aciertos > mas_aciertos) {
					//cout << "Supera" << endl;
					mas_aciertos = aciertos;
					//cout << "MÁXIMO NÚMERO DE ACIERTOS = " << mas_aciertos << endl;
					s_mejor = s;
					for (int i = 0; i < s_mejor.size(); i++) {
						cout << s_mejor[i];
					}
					cout << endl << mas_aciertos;
				} 
				
				s[j] = 0;
			}
		}
		
		s = s_mejor;
		if (aux_aciertos >= mas_aciertos) hay_mejora = false;
		else {
			aux_aciertos = mas_aciertos;
			mejor_tasa = 100 * (mas_aciertos / prueba.size());
			//cout << "		MEJOR TASA HASTA EL MOMENTO = " << mejor_tasa << endl;
			unos = 0.0;
			for (int h = 0; h < s.size(); h++) {		// Se calcula la tasa de reducción
				if (s[h] == 1) unos += 1;
			}
			mejor_red = 100 * ((s.size() - unos) / s.size());
			for (int i = 0; i < s_mejor.size(); i++) {
				cout << s_mejor[i];
			}
			cout << endl << mejor_tasa;
		}
	}
	elapsed = clock() - start_time;
	total_time = elapsed / CLOCKS_PER_SEC;
	cout << "Mejor tasa = " << mejor_tasa << endl;
	cout << "Mejor reduccion = " << mejor_red << endl;
	cout << "Tiempo = " << total_time << endl;
}