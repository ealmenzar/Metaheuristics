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

bool metrop(float dif, float T) {
	bool exito = false;
	float res = exp(-dif / T);
	float rand = Rand();
	if (dif < 0.0 || rand < res) exito = true;
	return exito;
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
	vector < vector<float> > prueba1, prueba2, prueba3, prueba4, prueba5;
	vector < vector<float> > conjunto1, conjunto2, conjunto3, conjunto4, conjunto5;
	vector < vector<float> > datoss, datos;
	vector <int> s;
	vector <int> mejor_s;
	Set_random(14); 
	datoss = readfilelibras();
	datos = normaliza_datos(datoss);

	// *****************************************	5X2 CROSS - VALIDATION		*********************************************************

	vector<int> clase1, clase2, clase3, clase4, clase5, clase6, clase7, clase8, clase9, clase10, clase11, clase12, clase13, clase14, clase15;
	vector<int> posiciones1,posiciones2,posiciones3, posiciones4, posiciones5, posiciones6, posiciones7, posiciones8, posiciones9, posiciones10, posiciones11, posiciones12, posiciones13, posiciones14, posiciones15;
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
	/*vector < vector<float> > prueba_aux;
	prueba_aux = prueba1;
	prueba1 = conjunto1;
	conjunto1 = prueba_aux;*/

	/*********************************************** INICIALIZACIÓN ***************************************************************/

	int r;
	int tam = prueba1[0].size() - 1;
	for (int i = 0; i < tam; i++) {
		r = Randint(0, 1);
		s.push_back(r);		// vector de características iniciales inicializado de forma aleatorio con semilla inicializada arriba
	}

	/******************************************************************************************************************************/

	mejor_s = s;

	/*********************************** CÁLCULO DEL COSTE (se puede resumir en una función global) **************************************/

	vector< vector<float> > prueba_s;
	vector<float> v;
	for (int i = 0; i < prueba1.size(); i++) {
		for (int k = 0; k < s.size(); k++) {
			v.push_back(prueba1[i][k] * s[k]);
		}
		v.push_back(prueba1[i][s.size()]);
		prueba_s.push_back(v);	// prueba_s es la prueba con las caracteristicas elegidas
		v.clear();
	}
	int clase;
	float aciertos = 0.0;

	for (int i = 0; i < prueba_s.size(); i++) {
		clase = clas3nn(prueba_s[i], conjunto1);
		if (clase == prueba1[i][prueba1[i].size() - 1]) {
			aciertos += 1;
		}
	}	// en aciertos quedan almacenados los aciertos del clasificador con el s inicial
		//cout << "Aciertos iniciales = " << aciertos << endl;
		//Calculo tasa de aciertos

	float tasa_clas = 100 * (aciertos / prueba1.size());
	//cout << "Tasa inicial = " << tasa_clas << endl;
	/************************************************************************************************************************************/

	int eval = 0;
	int enfriamientos = 0;
	float aciertos_vecino = 0.0;
	vector<int> vecino;
	vector< vector<float> > prueba_vecino;
	float mejor_tasa = tasa_clas;
	float mejor_red;
	float tasa_vecino;
	float T; // temperatura
	float T_inicial = (0.3*tasa_clas) / -log(0.3);
	T = T_inicial;
	float T_final = pow(10, -3);
	bool parada = false;
	int num_exitos = 0;
	int max_vecinos = (s.size());
	int max_exitos = 0.1*max_vecinos;
	bool enfriamiento = false;
	float B;
	float M = 15000 / max_vecinos;
	float unos = 0.0;
	for (int h = 0; h < mejor_s.size(); h++) {		// Se calcula la tasa de reducción inicial
		if (mejor_s[h] == 1) unos += 1;
	}
	mejor_red = 100 * ((s.size() - unos) / s.size());
	unos = 0.0;
	clock_t start_time, principal_start;
	double elapsed;
	double total_time;

	/********************************************** ALGORITMO ENFRIAMIENTO SIMULADO *******************************************************/
	principal_start = clock();
	for (int i = 0; eval < 15000 && !parada && enfriamientos < M && mejor_tasa < 100; i++) {		// Bucle externo
		//cout << endl <<  "e e e e e eval = " << eval << endl;
		num_exitos = 0;
		enfriamiento = false;
		start_time = clock();
		for (int vecinos = 0; vecinos < max_vecinos && !enfriamiento && mejor_tasa < 100; vecinos++) {	// Bucle interno

																										// Genera el vecino
			r = Randint(0, s.size() - 1);
			//cout << " r generado en vecino " << vecinos << " es " << r << endl;
			vecino = flip(s, r);
			for (int j = 0; j < prueba1.size(); j++) {
				for (int k = 0; k < vecino.size(); k++) {
					v.push_back(prueba1[j][k] * vecino[k]);
				}
				v.push_back(prueba1[j][vecino.size()]); // donde se encuentra la clase a la que pertenece
				prueba_vecino.push_back(v);	// prueba_vecino es la prueba con las caracteristicas elegidas en vecino = flip(s,i)
				v.clear();
			}

			// Clasifica el vecino
			for (int j = 0; j < prueba_vecino.size(); j++) {
				clase = clas3nn(prueba_vecino[j], conjunto1);
				if (clase == prueba1[j][prueba1[j].size() - 1]) {
					aciertos_vecino += 1;
				}
			}
			//cout << "vecino " << vecinos << " tiene aciertos = " << aciertos_vecino << " de " << prueba1.size() <<  endl;
			// Calcula el coste del vecino
			tasa_vecino = 100 * (aciertos_vecino / prueba1.size());
			//cout << "su tasa es " << tasa_vecino << endl;
			//cout << " Y mejor tasa es " << mejor_tasa << endl;
			eval++;

			// Metropolis
			if (/*tasa_clas != tasa_vecino &&*/ metrop(tasa_clas - tasa_vecino, T)) {
				//cout << "--> Se acepta METROPOLIS" << endl;
				//cout << "--> num_exitos = " << num_exitos << endl << endl;
				num_exitos++;
				s = vecino;
				aciertos = aciertos_vecino;
				tasa_clas = tasa_vecino;
				// Comprueba si es mejor que el global
				if (tasa_clas > mejor_tasa) {
					mejor_s = s;
					mejor_tasa = tasa_clas;
					for (int h = 0; h < mejor_s.size(); h++) {
						if (mejor_s[h] == 1) unos += 1;
					}
					mejor_red = 100 * ((s.size() - unos) / s.size());
				}
			}

			// Comprueba condición de enfriamiento
			if (num_exitos >= max_exitos) {
				enfriamiento = true;
			}
			unos = 0;
			vecino.clear();
			aciertos_vecino = 0;
			prueba_vecino.clear();
		} // Bucle interno
		elapsed = clock() - start_time;
		total_time = elapsed / CLOCKS_PER_SEC;

		// Enfría
		B = (T_inicial - T_final) / M*T_inicial*T_final;
		T = T / (1 + B*T);
		// Comprueba condición de parada
		if (num_exitos == 0) parada = true;
		enfriamientos++;
		//cout << "Enfria" << endl;
		//cout << "Enfriamientos = " << enfriamientos << endl;

	}	// Bucle externo
	elapsed = clock() - principal_start;
	total_time = elapsed / CLOCKS_PER_SEC;
	cout << "Temperatura alcanzada  = " << T << endl;
	cout << "Mejor tasa = " << mejor_tasa << endl;
	cout << "Mejor reduccion = " << mejor_red << endl;

	cout << "Evaluados " << eval << " vecinos y " << enfriamientos << " enfriamientos en " << total_time << endl;

	/*

	LIBRAS ---------------------------------

	1 - 1 semilla 47 
	Mejor tasa = 93.3099
	Mejor reduccion = 23.3333
	Tiempo = 1164.31

	1 - 2 semilla 47 
	Mejor tasa = 62.807
	Mejor reduccion = 66.6667
	Tiempo = 115.877

	1 - 3	semilla 33 
	Mejor tasa = 97.5352
	Mejor reduccion = 16.6667
	Tiempo = 892.485

	3 - 1	semilla 33 
	Mejor tasa = 94.7368
	Mejor reduccion = 16.6667
	Tiempo = 730.492

	1 - 4	semilla 21 
	Mejor tasa = 98.2394
	Mejor reduccion = 20
	Tiempo = 1695.76

	4 - 1	semilla 21 
	Mejor tasa = 62.807
	Mejor reduccion = 60
	Tiempo = 118.869

	1 - 5	semilla 87 
	Mejor tasa = 97.8873
	Mejor reduccion = 0
	Tiempo = 1902.82

	5 - 1	semilla 87 
	Mejor tasa = 62.807
	Mejor reduccion = 73.3333
	Tiempo = 123.979

	1 - 6	semilla 14 *
	Mejor tasa = 51.6667
	Mejor reduccion = 31.1111
	Tiempo = 7069.69
	- Evaluados 1821 vecinos
	- Efectuados 166 enfriamientos

	6 - 1	semilla 14 
	Mejor tasa = 97.8947
	Mejor reduccion = 23.3333
	Tiempo = 745.383

	*/

}