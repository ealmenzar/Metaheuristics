#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//#include <stdlib.h>
#include "random_ppio.hpp"
#include <time.h>
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
	//cout << "Tamaño de ejemplo: 5" << endl;
	//cout << "matriz.size()=" << matriz.size() << endl;
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
	int aux;
	while (t<3) {
		//cout << "t = " << t << endl;
		for (int k = 0; k<distancias.size(); k++) {
			if (distancias[k]<min && distancias[k] != 0) {
				min = distancias[k];	//me interesa la posición en el vector distancias
				pos_min = k;
				//cout << "Se ha hecho min = " << min << endl;
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
							//cout << "La clase es = " << clase << endl;
	return clase;
}

bool metrop(float dif, float T) {
	bool exito = false;
	if (dif < 0.0 || Rand() < exp(-dif / T)) exito = true;
	return exito;
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
		for (int j = 0; j < filas; j++) {
			datosn[j][i] = (datos[j][i] - min) / (max - min);
		}
	}
	return datosn;
}



int main() {
	vector < vector<float> > prueba1, prueba2, prueba3, prueba4, prueba5;
	vector < vector<float> > conjunto1, conjunto2, conjunto3, conjunto4, conjunto5;
	vector < vector<float> > datoss, datos, datos1, datos2, datos3, datos4, datos5;
	vector <int> s;
	vector <int> mejor_s;
	Set_random(14); 
	datoss = readfilewdbc();
	datos = normaliza_datos(datoss);
	//cout << "datos.size() = " << datos.size() << endl;

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

	// **************************************************************************************************

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
		//cout << "s[" << i << "] = " << s[i] << endl;
	}

	/******************************************************************************************************************************/

	mejor_s = s;

	/*********************************** CÁLCULO DEL COSTE (se puede resumir en una función global) **************************************/

	vector< vector<float> > prueba_s;
	vector<float> v;
	for (int i = 0; i < prueba1.size(); i++) {
		for (int k = 0; k < s.size(); k++) {
			v.push_back(prueba1[i][k] * s[k]);
			//cout << k << " -> Meto en v: " << prueba[i][k] * s[k] << endl;
		}
		v.push_back(prueba1[i][s.size()]);
		prueba_s.push_back(v);	// prueba_s es la prueba con las caracteristicas elegidas
		v.clear();
	}
	int clase;
	float aciertos = 0.0;

	for (int i = 0; i < prueba_s.size(); i++) {
		//cout << "[prueba[i].size() - 1] = " << prueba1[i].size() - 1 << endl;
		clase = clas3nn(prueba_s[i], conjunto1);
		//cout << "para prueba_s[" << i << "] el valor de clase es = " << clase << endl;
		//cout << " y clase = prueba[i][prueba[i].size() - 1] = " << prueba1[i][prueba1[i].size() - 1] << endl;
		if (clase == prueba1[i][prueba1[i].size() - 1]) {
			aciertos += 1;
		}
	}	// en aciertos quedan almacenados los aciertos del clasificador con el s inicial

	//Calculo tasa de aciertos

	float tasa_clas = 100 * (aciertos / prueba1.size());
	//aciertos = tasa_clas;
	//cout << "aciertos iniciales = " << aciertos << " de " << prueba1.size() <<  endl;
	//cout << "tasa inicial = " << tasa_clas << endl;

	/*************************************************************/

	int eval = 0;
	vector<int> listatabu;
	for (int i = 0; i<s.size() / 3; i++) {
		listatabu.push_back(-1);
	}
	vector<int> posiciones;		// almacena las 30 posiciones i que se hacen para generar los 30 vecinos en cada iteración
	bool repetido = false;		// 30 vecinos sin repetición ( flip(s,i) 30 veces con un i distinto )
	bool norepetido = false;	// idem
	bool enLT = false;			// en Lista Tabú
	bool criterio = false;		// criterio de aceptación
	vector<int> vecino;
	vector< vector<float> > prueba_vecino;
	float aciertos_vecino = 0.0;
	float mejor_tasa = tasa_clas;
	float mejor_tasa_vecino = tasa_clas;
	float mejor_red;
	float mejor_aciertos;
	float tasa_vecino;
	int jj = 0;					// índice para moverme por la lista tabú, jj está entre 0 y 9 (en el caso de wdbc.arff, con 30 características)
	float unos = 0.0;
	clock_t start_time, principal_start;
	double elapsed;
	double total_time;
	int mejor_r;

	/*****************************         ALGORITMO DE BÚSQUEDA TABÚ          ********************************/

	principal_start = clock();
	for (int i = 0; eval < 15000 && mejor_tasa < 100; i++) {
		//cout << "EVAL = " << eval << endl;
		for (int m = 0; m < 30; m++) {
			posiciones.push_back(-1);		// Para generar 30 vecinos del nuevo s (mejor vecino de la iteración anterior) limpio el
											// vector posiciones, que almacena las 30 posiciones que generaron los 30 vecinos en la iteración anterior
		}
		//cout << "posiciones.size() = " << posiciones.size() << endl;
		start_time = clock();
		for (int j = 0; j < 30; j++) {
			//cout << "Para j = " << j << endl;
			//cout << "ENTRA 1 " << endl;
			unos = 0.0;
			norepetido = false;
			enLT = false;
			criterio = false;
			repetido = false;
			
			/*while (!norepetido) {
				//cout << "norepetido = " << norepetido << endl;
				r =  Randint(0, s.size() - 1);
				//cout << "r = " << r << endl;
				repetido = false;
				for (int k = 0; k < posiciones.size() && !repetido; k++) {
					//cout << "	posiciones[" << k << "] = " << posiciones[k] << " = r = " << r << endl;
					if (posiciones[k] == r) repetido = true;
				}
				if (!repetido) norepetido = true;
				else norepetido = false;
			}*/

			int r = j; // PORQUE WDBC TIENE 30 CARACTERÍSTICAS, ASÍ QUE SE ACABARÁ GENERANDO TODAS
			
			//cout << "r = " << r << endl;
			//cout << " -----> r esta en posiciones[]? " << repetido << endl;
			posiciones[j] = r;

			for (int k = 0; k < listatabu.size() && !enLT; k++) {
				//cout << endl << "Lista TABU " << endl;
				//cout << "listatabu[" << k << "] = " << listatabu[k] << " = r = " << r << endl;
				if (listatabu[k] == r) enLT = true;
			}
			//cout << " -----> r esta en listabu[]? " << enLT << endl;
			
			//CRITERIO DE ACEPTACIÓN
			if (enLT) {
				vecino = flip(s, r);
				for (int t = 0; t < prueba1.size(); t++) {
					for (int k = 0; k < vecino.size(); k++) {
						v.push_back(prueba1[t][k] * vecino[k]);
					}
					v.push_back(prueba1[t][vecino.size()]); // donde se encuentra la clase a la que pertenece
					prueba_vecino.push_back(v);	// prueba_vecino es la prueba con las caracteristicas elegidas en vecino = flip(s,i)
					v.clear();
				}
				for (int t = 0; t < prueba_vecino.size(); t++) {
					clase = clas3nn(prueba_vecino[t], conjunto1);
					//cout << "para prueba_vecino[" << j << "] el valor de clase es = " << clase << endl;
					if (clase == prueba1[t][prueba1[t].size() - 1]) {
						aciertos_vecino += 1;
						//cout << "aciertos_vecino = " << aciertos_vecino << endl;
					}
				}
				
				tasa_vecino = 100 * (aciertos_vecino / prueba1.size());
				eval++;
				//cout << " ======= aceptacion: mejor_tasa = " << mejor_tasa << " < tasa_vecino = " << tasa_vecino << " && tasa_vecino = " << tasa_vecino << " > " << "mejor_tasa_vecino = " << mejor_tasa_vecino << endl;
				if (mejor_tasa < tasa_vecino && tasa_vecino > mejor_tasa_vecino) { // una vez encontrado un vecino con tasa > que la mejor hasta el momento,
																				   // nos interesa que los siguientes vecinos que se acepten, además de tener 
																				   // una tasa > que la mejor hasta el moento, sea > que la del mejor vecino
																				   // encontrado hasta el momento
					criterio = true;
				}
			}

			//cout << " ------> Acepta criterio de aceptacion? " << criterio << endl;

			if (!enLT || criterio) {
				
				if (criterio) {		// la tasa ya está calculada
					//cout << "ACEPTADO EN CRITERIO: tasa vecino = " << tasa_vecino << " > mejor tasa vecino = " << mejor_tasa_vecino << "?" << endl;
					if (tasa_vecino > mejor_tasa_vecino) {
						//cout << "SI" << endl;
						mejor_s = vecino;
						mejor_tasa_vecino = tasa_vecino;
						//cout << " ++++++++ mejor tasa vecino = " << mejor_tasa_vecino << " +++++++++++++ " << endl;
						aciertos_vecino = mejor_aciertos;
						mejor_r = r;
						for (int h = 0; h < mejor_s.size(); h++) {
							if (mejor_s[h] == 1) unos += 1;
						}
						mejor_red = 100 * ((s.size() - unos) / s.size());
						//cout << "red" << endl;
					}
				}
				else if (!enLT) {	// la tasa aun no esta calculada
					vecino = flip(s, r);
					for (int t = 0; t < prueba1.size(); t++) {
						for (int k = 0; k < vecino.size(); k++) {
							v.push_back(prueba1[t][k] * vecino[k]);
						}
						v.push_back(prueba1[t][vecino.size()]); // donde se encuentra la clase a la que pertenece
						prueba_vecino.push_back(v);	// prueba_vecino es la prueba con las caracteristicas elegidas en vecino = flip(s,i)
						v.clear();
					}
					//cout << "prueba_vecino.size() = " << prueba_vecino.size() << endl;
					for (int t = 0; t < prueba_vecino.size(); t++) {
						clase = clas3nn(prueba_vecino[t], conjunto1);
						//cout << "para prueba_vecino[" << t << "] el valor de clase es = " << clase << endl;
						if (clase == prueba1[t][prueba1[t].size() - 1]) {
							aciertos_vecino += 1;
						}
					}
					//cout << "NO ESTA EN LT: aciertos_vecino = " << aciertos_vecino << endl;
					
					tasa_vecino = 100 * (aciertos_vecino / prueba1.size());
					eval++;
					//cout << "tasa vecino = " << tasa_vecino << " > mejor tasa = " << mejor_tasa_vecino << "?" << endl;
					if (tasa_vecino > mejor_tasa_vecino) {
						//cout << "SI" << endl;
						mejor_s = vecino;
						mejor_tasa_vecino = tasa_vecino;
						//cout << " ++++++++ mejor tasa vecino = " << mejor_tasa_vecino << " +++++++++++++ " << endl;
						mejor_aciertos = aciertos_vecino;
						mejor_r = r;
						for (int h = 0; h < mejor_s.size(); h++) {
							if (mejor_s[h] == 1) unos += 1;
						}
						mejor_red = 100 * ((s.size() - unos) / s.size());
						//cout << "red" << endl;
					}
				}
			}
			aciertos_vecino = 0;
			prueba_vecino.clear();
		}
		elapsed = clock() - start_time;
		total_time = elapsed / CLOCKS_PER_SEC;

		cout << "Hechas " << eval << " evaluaciones en " << total_time << " segundos" << endl;
		start_time = clock();
		if (i < listatabu.size()) jj = i;
		else if (jj == listatabu.size() - 1) jj = 0;
		else jj++;
		//cout << " jj = " << jj << endl;
		listatabu[jj] = mejor_r;
		if ( mejor_tasa < mejor_tasa_vecino) mejor_tasa = mejor_tasa_vecino;
		//cout << "....................... MEJOR TASA HASTA EL MOMENTO : " << mejor_tasa << " %  ............................" << endl;
		for (int l = 0; l < listatabu.size(); l++) {
			//cout << " ~~~~~~ listatabu[" << l << "] = " << listatabu[l] << endl;
		}
		mejor_tasa_vecino = 0;
		s = mejor_s;
		aciertos_vecino = 0;
		posiciones.clear();

		elapsed = clock() - start_time;
		total_time = elapsed / CLOCKS_PER_SEC;

		cout << "Comprobada Lista Tabu en " << total_time << " segundos" << endl;
	}
	elapsed = clock() - principal_start;
	total_time = elapsed / CLOCKS_PER_SEC;
	cout << "Mejor tasa = " << mejor_tasa << endl;
	cout << "Mejor reducción = " << mejor_red << endl;
	cout << "Evaluados " << eval << " vecinos en " << total_time << endl;

	/*
	
	Tasa clasificación = 91.2281 %
	Tiempo = 2496.37 segundos ~ 41 minutos
	Evaluados 15000 vecinos
	Tasa reducción = 60 % 
	
	Tasa clasificación = 91.2281 %
	Tiempo = 3174.81 segundos ~ 52 minutos
	Evaluados 15000 vecinos
	Tasa reducción = 60 %

	PARA 150 EVALUACIONES (EN VEZ DE 15000) ME DA LA MISMA TASA DE CLASIFICACIÓN ==> ALGO VA MAL --> SOLUCIONADO

	- prueba1 = prueba; conjunto1 = conjunto (SIN IMPRESIONES / SEMILLA = 47)
	Tasa clas = 100 % (?)
	Tiempo = 559.69 segundos
	Tasa red = 30 %
	Evaluados 3330 vecinos (?)

	- prueba1 = conjunto; conjunto1 = prueba (CON IMPRESIONES / SEMILLA = 47)
	Tasa clas = 94.6429  % 
	Tiempo = 3843.52 segundos
	Tasa red = 33.3333 %
	Evaluados 15000 vecinos 

	- prueba2 = prueba; conjunto2 = conjunto (CON IMPRESIONES / SEMILLA = 33)
	Tasa clas = 98.2456 %
	Tiempo = 3518.89 segundos
	Tasa red = 26.6666 %
	Evaluados 15000 vecinos
	
	*/

}