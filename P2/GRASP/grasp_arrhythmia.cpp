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

vector< vector<float> > readfilearrhythmia() {
    ifstream myfile("C:\\Users\\marin\\ClionProjects\\c3nn\\arrhythmia.arff");
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
    //cout << datos.size() << " x " << datos[0].size() << endl;
    vector< vector<float> > datosn = datos;
    /*for(int i =0; i < datosn.size();i++){
        for(int j=0; j<datosn[i].size();j++){
            cout << "datosn["<<i<<"]["<<j<<"] = " << datosn[i][j] << endl;
        }
    }*/
    float max;
    float min;
    for (int i = 0; i < columnas - 1; i++) {
        max = -999999999999999999.999;
        min = 9999999999999999999.999;
        for (int j = 0; j < filas; j++) {
            if (datos[j][i] < min) min = datos[j][i];
            if (datos[j][i] > max) max = datos[j][i];
        }
        if(max == 0 && min == 0) {
            for (int j = 0; j < filas; j++) {
                datosn[j][i] = 0;
            }
        }
        else {
            for (int j = 0; j < filas; j++) {
                datosn[j][i] = (datos[j][i] - min) / (max - min);
            }
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
    vector< vector<float> > datos = readfilearrhythmia();
    datos = normaliza_datos(datos);
    vector< vector<float> > prueba;
    vector< vector<float> > conjunto;
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

    // Determinaci�n aleatoria de los elementos de clase 16 que van al prueba
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
        prueba.push_back(datos[clase16[posicion]]);
        posiciones16.push_back(posicion);
    }
    // Determinaci�n de los elementos de clase 16 restantes que van al conjunto
    for (int i = 0; i < clase16.size(); i++) {
        repetida = false;
        for (int j = 0; j < posiciones16.size(); j++) {
            if (posiciones16[j] == i) repetida = true;
        }
        if (!repetida) conjunto.push_back(datos[clase16[i]]);
    }
    // **************************************************************************************************

    // Intercambio de conjunto y prueba para la ejecución a la viceversa
    vector < vector<float> > prueba_aux;
    prueba_aux = prueba;
    prueba = conjunto;
    conjunto = prueba_aux;

    /****************************************** GRASP **************************************************/

    vector<float> s;
    int TAM = prueba[0].size()-1;
    bool hay_mejora;
    int aciertos, contador, cmejor, cpeor, rand,s_aleatorias = 0,solucion,r;
    vector<int> LC, LRC;
    vector<float> vecino, mejor_vecino;
    float mu, alpha = 0.3, tasa, red, MAX_aciertos, unos = 0.0, mejor_solucion=0.0;
    clock_t start_time;
    double elapsed;
    double total_time;

    start_time = clock();
    while (s_aleatorias < 25) {
        r = Randint(1,100);
        Set_random(r);
        hay_mejora = true;
        MAX_aciertos = 0.0;
        s.clear();
        for(int i = 0; i<TAM ; i++){
            s.push_back(0);
        }

        // Generamos solución inicial mediante SFS aleatorizado
        while (hay_mejora) {
            LC.clear();
            LRC.clear();
            cmejor = 0;
            cpeor = 9999;
            for (int j = 0; j < s.size(); j++) {
                if (s[j] == 0) {
                    s[j] = 1;
                    aciertos = clas3nn(prueba, prueba, s);
                    LC.push_back(aciertos);
                    if (aciertos > cmejor) cmejor = aciertos;
                    else if (aciertos < cpeor) cpeor = aciertos;
                    s[j] = 0;
                }
                else LC.push_back(-1);
            }
            /*for (int i = 0; i < LC.size(); i++){
                cout << "LC["<<i<<"] = " << LC[i] << endl;
            }*/

            mu = cmejor - alpha * (cmejor - cpeor);
            for (int i = 0; i < LC.size(); i++) {
                if (LC[i] > mu && LC[i] != -1) LRC.push_back(i);
            }

            /* for (int i = 0; i < LRC.size(); i++){
                 cout << "LRC["<<i<<"] = " << LRC[i] << endl;
             }*/
            rand = Randint(0, LRC.size()-1);
            /* cout << "rand = " << rand << endl;
             cout << "mejor solucion en SFS aleatorizado = " << LC[LRC[rand]] << " en LC[" << LRC[rand] <<"]"<< endl;*/
            if (MAX_aciertos > LC[LRC[rand]]) hay_mejora = false;
            else {
                MAX_aciertos = LC[LRC[rand]];
                s[LRC[rand]] = 1;
            }
        }

        // Aplicamos Búsqueda Local
        contador = 0;
        while (contador < s.size()) {
            // if (s[contador] == 0) {
            vecino = flip(s, contador); //Genero vecino de s
            solucion = clas3nn(prueba, prueba, vecino);  //Calculo su coste

            if (solucion > MAX_aciertos) { //Comparo con la mejor obtenida desde esta aleatoria
                /* cout << "MAX_aciertos = " << MAX_aciertos << " es menor que ";
                 cout << "solucion = " << solucion << endl;*/
                MAX_aciertos = solucion;
                s = vecino;
                contador = 0;
            }
            else {
                contador++;
                // cout << "contador = " << contador << endl;
            }
            // }
            // else contador++;
        }

        // Comparamos con la mejor solución obtenida hasta el momento de las obtenidas en el SFS aleatorizado
        if (MAX_aciertos > mejor_solucion) {
            /*cout << "MAX_aciertos = " << MAX_aciertos << " es mayor que ";
            cout << "mejor_solucion = " << mejor_solucion << endl;*/
            mejor_solucion = MAX_aciertos;
            mejor_vecino = s;
        }/*else cout << "MAX_aciertos = " << MAX_aciertos << endl;*/
        s_aleatorias++;
    }

    mejor_solucion = clas3nn(conjunto, prueba, mejor_vecino);
    tasa = 100*(mejor_solucion / prueba.size());
    for (int i = 0; i < mejor_vecino.size(); i++){
        if (mejor_vecino[i] == 1) unos += 1;
    }
    /*cout << "unos = " << unos << endl;
    cout << "mejor_vecino.size() = " << mejor_vecino.size() << endl;*/
    red = 100*((mejor_vecino.size() - unos) / mejor_vecino.size());

    elapsed = clock() - start_time;
    total_time = elapsed / CLOCKS_PER_SEC;

    cout << "Aciertos = " << mejor_solucion << endl;
    cout << "Tasa = " << tasa << endl;
    cout << "Reduccion = " << red << endl;
    cout << "Tiempo = " << total_time << endl;
    cout << " s_aleatorias = " << s_aleatorias << endl;

}

