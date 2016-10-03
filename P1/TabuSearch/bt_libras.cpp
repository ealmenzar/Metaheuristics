#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
//#include <stdlib.h>
#include "random_ppio.hpp"
#include <time.h>
using namespace std;

vector< vector<float> > readfile() {
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

vector< vector<float> > divide1(vector< vector<float> > datos) {
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
	for (int i = quinto; i < 2 * quinto; i++) {
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
	for (int i = 4 * quinto; i < 5 * quinto; i++) {
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
	/*cout << "distancias.size() = " << distancias.size() << endl;
	cout << "distancia[0]=" << distancias[0] << endl;
	cout << "distancia[1]=" << distancias[1] << endl;
	cout << "distancia[2]=" << distancias[2] << endl;
	cout << "distancia[3]=" << distancias[3] << endl;
	cout << "distancia[4]=" << distancias[4] << endl;*/
	vector<int> clases;
	float min = 1000;
	int pos_min = 0;
	int t = 0;
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
	/*cout << "clase[0] = " << clases[0] << endl;
	cout << "clase[1] = " << clases[1] << endl;
	cout << "clase[2] = " << clases[2] << endl;*/
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
	Set_random(47); // esta inicialización debe poder pasarse como parámetro ?????????????????????????????????????????????????????
	datoss = readfile();
	datos = normaliza_datos(datoss);
	
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
		if (!repetida) conjunto.push_back(datos[clase2[i]]);
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
	/*cout << "prueba.size() = " << prueba.size() << endl;
	for (int i = 0; i < prueba.size(); i++) {
	cout << "prueba[" << i << "][clase] = " << prueba[i][prueba[i].size() - 1] << endl;
	}
	cout << "conjunto.size() = " << conjunto.size() << endl;
	for (int i = 0; i < conjunto.size(); i++) {
	cout << "conjunto[" << i << "][clase] = " << conjunto[i][prueba[i].size() - 1] << endl;
	}*/

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
	//cout << ":::::::::::::::::::::::::::::::::::: s.size() - 1 = " << s.size() - 1 << endl;

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
	//cout << "ACIERTOS INICIALES = " << aciertos << endl;

		//Calculo tasa de aciertos

	float tasa_clas = 100 * (aciertos / prueba1.size());
	//aciertos = tasa_clas;
	//cout << "s.size() = " << s.size() << endl;

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
		for (int m = 0; m < 30; m++) {
			posiciones.push_back(-1);		// Para generar 30 vecinos del nuevo s (mejor vecino de la iteración anterior) limpio el
											// vector posiciones, que almacena las 30 posiciones que generaron los 30 vecinos en la iteración anterior
		}
		//cout << "posiciones.size() = " << posiciones.size() << endl;

		for (int j = 0; j < 30; j++) {
			//cout << " ******* Para j = " << j << " *********" << endl;
			//cout << "ENTRA 1 " << endl;
			unos = 0.0;
			norepetido = false;
			enLT = false;
			criterio = false;
			repetido = false;
			for (int f = 0; f < posiciones.size(); f++) {
				//cout << "posiciones[" << f << "] = " << posiciones[f] << endl;
			}
			while (!norepetido) {
				//cout << "norepetido = " << norepetido << endl;
				r = Randint(0, s.size() - 1);
				//cout << "r = " << r << endl;
				repetido = false;
				for (int k = 0; k < posiciones.size() && !repetido; k++) {
					//cout << "posiciones[" << k << "] = " << posiciones[k] << " = r = " << r << endl;
					if (posiciones[k] == r) repetido = true;
				}
				if (!repetido) norepetido = true;
				else norepetido = false;
			}
			//cout << "r = " << r << endl;
			//cout << " -----> r esta en posiciones[]? " << repetido << endl;
			posiciones[j] = r;

			for (int k = 0; k < listatabu.size() && !enLT; k++) {
				//cout << "Lista TABU " << endl;
				//cout << "listatabu[" << k << "] = " << listatabu[k] << " = r = " << r << endl;
				if (listatabu[k] == r) enLT = true;
			}
			//cout << " -----> r esta en listatbu[]? " << enLT << endl;

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
					}
				}
				//cout << "     Aciertos_vecino = " << aciertos_vecino << endl;

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
				vecino = flip(s, r);
				if (criterio) {		// la tasa ya está calculada
					//cout << "tasa vecino = " << tasa_vecino << " > mejor tasa vecino = " << mejor_tasa_vecino << "?" << endl;
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
					}
				}
				else if (!enLT) {	// la tasa aun no esta calculada
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
					//cout << "     Aciertos_vecino = " << aciertos_vecino << endl;

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
					}
				}
			}
			aciertos_vecino = 0;
			prueba_vecino.clear();
		}

		if (i < listatabu.size()) jj = i;
		else if (jj == listatabu.size() - 1) jj = 0;
		else jj++;
		//cout << " jj = " << jj << " y listatabu["<<jj<<"] = " << mejor_r << endl;
		listatabu[jj] = mejor_r;
		if (mejor_tasa < mejor_tasa_vecino) mejor_tasa = mejor_tasa_vecino;
		//cout << "....................... MEJOR TASA HASTA EL MOMENTO : " << mejor_tasa << " %  ............................" << endl;
		//for (int l = 0; l < listatabu.size(); l++) {
			//cout << " ~~~~~~ listatabu[" << l << "] = " << listatabu[l] << endl;
		//}
		mejor_tasa_vecino = 0;
		s = mejor_s;
		aciertos_vecino = 0;
		posiciones.clear();
	}
	elapsed = clock() - principal_start;
	total_time = elapsed / CLOCKS_PER_SEC;
	cout << "Mejor tasa = " << mejor_tasa << endl;
	cout << "Mejor reducción = " << mejor_red << endl;
	cout << "Evaluados " << eval << " vecinos en " << total_time << endl;

	/*


	- prueba1 = prueba; conjunto1 = conjunto (CON IMPRESIONES / SEMILLA = 47)
	Tasa clas = 30.5556 %	<--- deja de producirse mejora muy temprano
	Tiempo = 3041.82 segundos
	Tasa red = 43.3333 %
	Evaluados 15000 vecinos

	- prueba1 = prueba; conjunto1 = conjunto (SIN IMPRESIONES / SEMILLA = 47)
	Tasa clas = 30.5556 %	<--- deja de producirse mejora muy temprano
	Tiempo = 2495.11 segundos
	Tasa red = 43.3333 %
	Evaluados 15000 vecinos



	*/

}