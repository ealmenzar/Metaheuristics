#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "../random_ppio.h"
#include <time.h>
#include <stdio.h>
using namespace std;

vector< vector<float> > readfilewdbc() {
    ifstream myfile("C:\\Users\\marin\\ClionProjects\\c3nn\\wdbc.arff");
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

int clas3nn(vector< vector<float> > conjunto, vector< vector<float> > cdecomparacion, vector<float> s) {
    vector< vector<float> > cs, csdecomp;
    vector<float> v, w;
    float resta, sum = 0.0;
    vector<float> distancias;
    vector<int> pos_distancias;
    float clase;
    float aciertos = 0.0;
    bool iguales = false;
    int TAM;

    if(conjunto == cdecomparacion) iguales = true;

    // Construyo la matriz con las características seleccionadas

    if (iguales) {
        // Construyo la matriz con las características seleccionadas
        for (int i = 0; i < conjunto.size(); i++) { // filas
            for (int j = 0; j < conjunto[i].size() - 1; j++) { // columnas
                if (s[j] == 1){
                    v.push_back(conjunto[i][j]);
                }
            }
            v.push_back(conjunto[i][conjunto[i].size() - 1]); // añado la clase

            cs.push_back(v);
            v.clear();
        }

        for (int i = 0; i < 3; i++) {
            distancias.push_back(9999.99);
        }
        for (int i = 0; i < 3; i++) {
            pos_distancias.push_back(-1);
        }

        // Estimo las clases de cada fila y cuento el número de aciertos

        for (int i = 0; i < cs.size(); i++) {
            for (int k = 0; k < cs.size(); k++) {
                if (i != k) {
                    for (int j = 0; j < cs[i].size() - 1; j++) {
                        resta = cs[i][j] - cs[k][j];
                        sum += resta * resta;
                    }

                    if (sum < distancias[2]) {
                        if (sum < distancias[1]) {
                            if (sum < distancias[0]) {
                                distancias[2] = distancias[1];
                                pos_distancias[2] = pos_distancias[1];
                                distancias[1] = distancias[0];
                                pos_distancias[1] = pos_distancias[0];
                                distancias[0] = sum;
                                pos_distancias[0] = k;
                            }
                            else {
                                distancias[2] = distancias[1];
                                pos_distancias[2] = pos_distancias[1];
                                distancias[1] = sum;
                                pos_distancias[1] = k;
                            }
                        }
                        else {
                            distancias[2] = sum;
                            pos_distancias[2] = k;
                        }
                    }
                }
                sum = 0.0;
            }

            //en distancias quedan almacenadas las 3 distancias más cortas
            //en pos_distancias quedan almacenadas las posiciones correspondientes a esas 3 distancias más cortas

            if (cs[pos_distancias[0]][cs[pos_distancias[0]].size() - 1] ==
                cs[pos_distancias[1]][cs[pos_distancias[1]].size() - 1]
                || cs[pos_distancias[0]][cs[pos_distancias[0]].size() - 1] ==
                   cs[pos_distancias[2]][cs[pos_distancias[2]].size() - 1]) {

                clase = cs[pos_distancias[0]][cs[pos_distancias[0]].size() - 1];
            }


            else if (cs[pos_distancias[1]][cs[pos_distancias[1]].size() - 1] ==
                     cs[pos_distancias[2]][cs[pos_distancias[2]].size() - 1]) {

                clase = cs[pos_distancias[1]][cs[pos_distancias[1]].size() - 1];
            }

            else {

                clase = cs[pos_distancias[0]][cs[pos_distancias[0]].size() - 1];
            }

            if (clase == cs[i][cs[i].size() - 1]) aciertos++;

            distancias.clear();
            for (int i = 0; i < 3; i++) {
                distancias.push_back(9999.99);
            }
            pos_distancias.clear();
            for (int i = 0; i < 3; i++) {
                pos_distancias.push_back(-1);
            }
        }
    }
    else{

        // Construyo la matriz con las características seleccionadas
        for (int i = 0; i < conjunto.size(); i++) { // filas
            for (int j = 0; j < conjunto[i].size() - 1; j++) { // columnas
                if (s[j] == 1){
                    v.push_back(conjunto[i][j]);
                }
            }
            v.push_back(conjunto[i][conjunto[i].size() - 1]); // añado la clase
            cs.push_back(v);
            v.clear();
        }
        cout << "cs.size() = " << cs.size() << " x " << cs[0].size() << endl;
        /*for (int i = 0; i < cs.size(); i++) { // filas
            for (int j = 0; j < cs[i].size() - 1; j++) { // columnas
                cout << "cs[" << i << "][" << j << "] = " << cs[i][j] << endl;
            }
        }*/
        for (int i = 0; i < cdecomparacion.size(); i++) { // filas
            for (int j = 0; j < cdecomparacion[i].size() - 1; j++) { // columnas
                if (s[j] == 1){
                    w.push_back(cdecomparacion[i][j]);
                }
            }
            w.push_back(cdecomparacion[i][cdecomparacion[i].size() - 1]); // añado la clase
            csdecomp.push_back(w);
            w.clear();
        }
        cout << "csdecomp.size() = " << csdecomp.size() << " x " << csdecomp[0].size() << endl;
        /* for (int i = 0; i < csdecomp.size(); i++) { // filas
             for (int j = 0; j < csdecomp[i].size() - 1; j++) { // columnas
                 cout << "csdecomp[" << i << "][" << j << "] = " << csdecomp[i][j] << endl;
             }
         }*/

        for (int i = 0; i < 3; i++) {
            distancias.push_back(9999.99);
        }
        for (int i = 0; i < 3; i++) {
            pos_distancias.push_back(-1);
        }

        // Estimo las clases de cada fila y cuento el número de aciertos

        if(cs.size() < csdecomp.size()) TAM = cs[0].size();
        else TAM = csdecomp[0].size();

        for (int i = 0; i < cs.size(); i++) {
            //cout << "i = " << i << endl;
            for (int k = 0; k < csdecomp.size(); k++) {
                //cout << "k = " << k << endl;
                for (int j = 0; j < TAM - 1; j++) {
                    //cout << "cs["<<i<<"]["<<j<<"] = " << cs[i][j] << endl;
                    //cout << "csdecomp["<<k<<"]["<<j<<"] = " << csdecomp[k][j] << endl;
                    resta = cs[i][j] - csdecomp[k][j];
                    sum += (resta * resta);
                }
                //cout << "sum = " << sum << endl;

                if (sum < distancias[2]) {
                    if (sum < distancias[1]) {
                        if (sum < distancias[0]) {
                            distancias[2] = distancias[1];
                            pos_distancias[2] = pos_distancias[1];
                            distancias[1] = distancias[0];
                            pos_distancias[1] = pos_distancias[0];
                            distancias[0] = sum;
                            pos_distancias[0] = k;
                        }
                        else {
                            distancias[2] = distancias[1];
                            pos_distancias[2] = pos_distancias[1];
                            distancias[1] = sum;
                            pos_distancias[1] = k;
                        }
                    }
                    else {
                        distancias[2] = sum;
                        pos_distancias[2] = k;
                    }
                }

                sum = 0.0;
            }
            //cout << "distancias" << endl;

            /*for (int i = 0; i < 3 ; i ++){
                cout << "pos_distancias["<<i<<"] = " << pos_distancias[i] << endl;
                cout << "distancias[" << i << "] = " << distancias[i] << endl;
            }*/
            //en distancias quedan almacenadas las 3 distancias más cortas
            //en pos_distancias quedan almacenadas las posiciones correspondientes a esas 3 distancias más cortas

            if (csdecomp[pos_distancias[0]][csdecomp[pos_distancias[0]].size() - 1] ==
                csdecomp[pos_distancias[1]][csdecomp[pos_distancias[1]].size() - 1]
                || csdecomp[pos_distancias[0]][csdecomp[pos_distancias[0]].size() - 1] ==
                   csdecomp[pos_distancias[2]][csdecomp[pos_distancias[2]].size() - 1]) {

                clase = csdecomp[pos_distancias[0]][csdecomp[pos_distancias[0]].size() - 1];
            }


            else if (csdecomp[pos_distancias[1]][csdecomp[pos_distancias[1]].size() - 1] ==
                     csdecomp[pos_distancias[2]][csdecomp[pos_distancias[2]].size() - 1]) {

                clase = csdecomp[pos_distancias[1]][csdecomp[pos_distancias[1]].size() - 1];
            }


            else {

                clase = csdecomp[pos_distancias[0]][csdecomp[pos_distancias[0]].size() - 1];
            }


            if (clase == cs[i][cs[i].size() - 1]) aciertos++;

            distancias.clear();
            for (int i = 0; i < 3; i++) {
                distancias.push_back(9999.99);
            }
            pos_distancias.clear();
            for (int i = 0; i < 3; i++) {
                pos_distancias.push_back(-1);
            }
        }
    }

    return aciertos;
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

vector<float> flip(vector<float> s, int i) {
    vector<float> vecino;
    vecino = s;
    if (vecino[i] == 0.0) vecino[i] = 1.0;
    else if (vecino[i] == 1.0) vecino[i] = 0.0;

    return vecino;
}

int main() {
    Set_random(33);
    vector< vector<float> > datos = readfilewdbc();
    datos = normaliza_datos(datos);
    vector< vector<float> > prueba;
    vector< vector<float> > conjunto;

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

    // Intercambio de conjunto y prueba para la ejecución a la viceversa
    vector < vector<float> > prueba_aux;
    prueba_aux = prueba;
    prueba = conjunto;
    conjunto = prueba_aux;

    /***************************** BUSQUEDA LOCAL ****************************************/

    int s_aleatorias = 0;
    vector<float> s, vecino, mejor_vecino;
    int TAM = prueba[0].size();
    float r, tasa, unos, red, M = 0.0;
    int mejor_solucion, contador, solucion;

    clock_t start_time;
    double elapsed;
    double total_time;

    start_time = clock();
    while(s_aleatorias < 25) {
        s.clear();
        // Genero solución incial
        r = Randint(1,100);
        Set_random(r);
        for (int i = 0; i < TAM; i++) {
            r = Randint(0, 1);
            s.push_back(r);
        }

        // Calculo el coste inicial
        mejor_solucion = clas3nn(prueba, prueba, s);

        // Algoritmo de Búsqueda Local
        contador = 0;
        while (contador < s.size()) {
            vecino = flip(s, contador); //Genero vecino de s
            solucion = clas3nn(prueba, prueba, vecino);  //Calculo su coste

            if (solucion > mejor_solucion) { //Comparo con la mejor obtenida desde esta aleatoria
                mejor_solucion = solucion;
                s = vecino;
                contador = 0;
            }
            else contador++;
        }
        if (mejor_solucion > M) {
            M = mejor_solucion; //Comparo con la mejor obtenida desde todas las aleatorios
            mejor_vecino = s;
        }
        s_aleatorias++;
    }

    // Aplico este clasificador construído al otro conjunto
    M = clas3nn(conjunto, prueba, mejor_vecino);
    // Calculo la tasa de clasificación
    tasa = 100 * ( M / prueba.size() );
    // Calculo la tasa de reducción
    for (int h = 0; h < mejor_vecino.size(); h++) {
        if (mejor_vecino[h] == 1) unos += 1;
    }
    red = 100 * ((mejor_vecino.size() - unos) / mejor_vecino.size());

    elapsed = clock() - start_time;
    total_time = elapsed / CLOCKS_PER_SEC;

    cout << "Aciertos = " << M << endl;
    cout << "Tasa = " << tasa << endl;
    cout << "Reduccion = " << red << endl;
    cout << "Tiempo = " << total_time << endl;


}

