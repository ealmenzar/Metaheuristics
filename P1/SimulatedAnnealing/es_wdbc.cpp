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

/*vector< vector<float> > divide1(vector< vector<float> > datos) {
	int quinto = datos.size() / 5;
	vector< vector<float> > datos1;
	for (int i = 0; i < quinto; i++) {
		datos1.push_back(datos[i]);
	}
	return datos1;
}

vector< vector<float> > divide2(vector< vector<float> > datos) {
	int quinto = datos.size() / 5;
	vector< vector<float> > datos2;
	for (int i = quinto; i < 2*quinto; i++) {
		datos2.push_back(datos[i]);
	}
	return datos2;
}

vector< vector<float> > divide3(vector< vector<float> > datos) {
	int quinto = datos.size() / 5;
	vector< vector<float> > datos3;
	for (int i = 2 * quinto; i < 3 * quinto; i++) {
		datos3.push_back(datos[i]);
	}
	return datos3;
}

vector< vector<float> > divide4(vector< vector<float> > datos) {
	int quinto = datos.size() / 5;
	vector< vector<float> > datos4;
	for (int i = 3 * quinto; i < 4 * quinto; i++) {
		datos4.push_back(datos[i]);
	}
	return datos4;
}

vector< vector<float> > divide5(vector< vector<float> > datos) {
	int quinto = datos.size() / 5;
	vector< vector<float> > datos5;
	for (int i = 4* quinto; i < 5 * quinto; i++) {
		datos5.push_back(datos[i]);
	}
	return datos5;
}

vector< vector<float> > divide_conjunto(vector< vector<float> > datos) {
	int mitad = datos.size() / 2;
	vector< vector<float> > conjunto;
	for (int j = 0; j < mitad; j++) {
		conjunto.push_back(datos[j]);
	}
	return conjunto;
}

vector< vector<float> > divide_prueba(vector< vector<float> > datos) {
	int mitad = datos.size() / 2;
	vector< vector<float> > prueba;
	for (int j = mitad; j < datos.size(); j++) {
		prueba.push_back(datos[j]);
	}
	return prueba;
}*/

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
	vector < vector<float> > prueba1;
	vector < vector<float> > conjunto1;
	vector < vector<float> > datoss, datos;
	vector <int> s;
	vector <int> mejor_s;
	Set_random(14); // esta inicialización debe poder pasarse como parámetro ?????????????????????????????????????????????????????
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

	cout << "prueba.size() = " << prueba1.size() << endl;
	for (int i = 0; i < prueba1.size(); i++) {
		cout << "prueba[" << i << "][clase] = " << prueba1[i][prueba1[i].size() - 1] << endl;
	}
	cout << "conjunto.size() = " << conjunto1.size() << endl;
	for (int i = 0; i < conjunto1.size(); i++) {
		cout << "conjunto[" << i << "][clase] = " << conjunto1[i][conjunto1[i].size() - 1] << endl;
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
	int max_vecinos = 10*(s.size());
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
		//cout <<endl<< " e e e e e eval = " << eval << endl;
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
					aciertos_vecino+=1;
				}
			}
			//cout << "vecino " << vecinos << " tiene aciertos = " << aciertos_vecino << " de " << prueba1.size() <<  endl;
			// Calcula el coste del vecino
			tasa_vecino = 100 * (aciertos_vecino / prueba1.size());
			//cout << "	su tasa es " << tasa_vecino << endl;
			//cout << "	 Y mejor tasa es " << mejor_tasa << endl;
			eval++;

			// Metropolis
			if (/*tasa_clas != tasa_vecino &&*/ metrop(tasa_clas - tasa_vecino, T)) {
				//cout << "--> Se acepta METROPOLIS" << endl;
				//cout << "--> num_exitos = " << num_exitos << endl << endl;
				num_exitos++;
				//cout << "	--> num_exitos = " << num_exitos << endl << endl;
				s = vecino;
				aciertos = aciertos_vecino;
				tasa_clas = tasa_vecino;
				// Comprueba si es mejor que el global
				if (tasa_clas > mejor_tasa) {
					mejor_s = s;
					mejor_tasa = tasa_clas;
					for (int h = 0; h < mejor_s.size(); h++) {
						if (mejor_s[h] == 1) unos+=1;
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
		//cout << "ENFRÍA" << endl;
		//cout << " ** enfriamientos ** = " << enfriamientos << endl;

	}	// Bucle externo
	elapsed = clock() - principal_start;
	total_time = elapsed / CLOCKS_PER_SEC;
	cout << "Temperatura alcanzada  = " << T << endl;
	cout << "Mejor tasa = " << mejor_tasa << endl;
	cout << "Mejor reduccion = " << mejor_red << endl;

	cout << "Evaluados " << eval << " vecinos y " << enfriamientos << " enfriamientos en " << total_time << endl;

/*
   ----------------------------------------------------------------------------
	WDBC ---------------------------------

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
		Mejor tasa = 95.4225
		Mejor reduccion = 33.3333
		Tiempo = 4800.93
		- Evaluados 1937 vecinos
		- Efectuados 50 enfriamientos

	6 - 1	semilla 14 
		Mejor tasa = 
		Mejor reduccion = 
		Tiempo = 

	*/

}