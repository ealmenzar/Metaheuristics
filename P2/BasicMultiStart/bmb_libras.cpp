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

vector< vector<float> > readfilelibras() {
    ifstream myfile("C:\\Users\\marin\\ClionProjects\\c3nn\\movement_libras.arff");
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
    vector< vector<float> > datos = readfilelibras();
    datos = normaliza_datos(datos);
    vector< vector<float> > prueba;
    vector< vector<float> > conjunto;

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


    // Determinaci�n aleatoria de los elementos de clase 1 que van al prueba
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
        prueba.push_back(datos[clase1[posicion]]);
        posiciones1.push_back(posicion);
    }

    // Determinaci�n de los elementos de clase 1 restantes que van al conjunto
    for (int i = 0; i < clase1.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones1.size(); j++) {
            if (posiciones1[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase1[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 2 que van al prueba
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
        prueba.push_back(datos[clase2[posicion]]);
        posiciones2.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 2 restantes que van al conjunto
    for (int i = 0; i < clase2.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones2.size(); j++) {
            if (posiciones2[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase2[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 3 que van al prueba
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
        prueba.push_back(datos[clase3[posicion]]);
        posiciones3.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 3 restantes que van al conjunto
    for (int i = 0; i < clase3.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones3.size(); j++) {
            if (posiciones3[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase3[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 4 que van al prueba
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
        prueba.push_back(datos[clase4[posicion]]);
        posiciones4.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 4 restantes que van al conjunto
    for (int i = 0; i < clase4.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones4.size(); j++) {
            if (posiciones4[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase4[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 5 que van al prueba
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
        prueba.push_back(datos[clase5[posicion]]);
        posiciones5.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 5 restantes que van al conjunto
    for (int i = 0; i < clase5.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones5.size(); j++) {
            if (posiciones5[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase5[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 6 que van al prueba
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
        prueba.push_back(datos[clase6[posicion]]);
        posiciones6.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 6 restantes que van al conjunto
    for (int i = 0; i < clase6.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones6.size(); j++) {
            if (posiciones6[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase6[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 7 que van al prueba
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
        prueba.push_back(datos[clase7[posicion]]);
        posiciones7.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 7 restantes que van al conjunto
    for (int i = 0; i < clase7.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones7.size(); j++) {
            if (posiciones7[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase7[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 8 que van al prueba
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
        prueba.push_back(datos[clase8[posicion]]);
        posiciones8.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 8 restantes que van al conjunto
    for (int i = 0; i < clase8.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones8.size(); j++) {
            if (posiciones8[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase8[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 9 que van al prueba
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
        prueba.push_back(datos[clase9[posicion]]);
        posiciones9.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 9 restantes que van al conjunto
    for (int i = 0; i < clase9.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones9.size(); j++) {
            if (posiciones9[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase9[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 10 que van al prueba
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
        prueba.push_back(datos[clase10[posicion]]);
        posiciones10.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 10 restantes que van al conjunto
    for (int i = 0; i < clase10.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones10.size(); j++) {
            if (posiciones10[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase10[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 11 que van al prueba
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
        prueba.push_back(datos[clase11[posicion]]);
        posiciones11.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 11 restantes que van al conjunto
    for (int i = 0; i < clase11.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones11.size(); j++) {
            if (posiciones11[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase11[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 12 que van al prueba
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
        prueba.push_back(datos[clase12[posicion]]);
        posiciones12.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 12 restantes que van al conjunto
    for (int i = 0; i < clase12.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones12.size(); j++) {
            if (posiciones12[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase12[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 13 que van al prueba
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
        prueba.push_back(datos[clase13[posicion]]);
        posiciones13.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 13 restantes que van al conjunto
    for (int i = 0; i < clase13.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones13.size(); j++) {
            if (posiciones13[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase13[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 14 que van al prueba
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
        prueba.push_back(datos[clase14[posicion]]);
        posiciones14.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 14 restantes que van al conjunto
    for (int i = 0; i < clase14.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones14.size(); j++) {
            if (posiciones14[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase14[i]]);
    }

    // Determinaci�n aleatoria de los elementos de clase 15 que van al prueba
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
        prueba.push_back(datos[clase15[posicion]]);
        posiciones15.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 15 restantes que van al conjunto
    for (int i = 0; i < clase15.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones15.size(); j++) {
            if (posiciones15[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase15[i]]);
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
    tasa = 100 * ( M / conjunto.size() );
    // Calculo la tasa de reducción
    unos = 0;
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

