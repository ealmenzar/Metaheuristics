#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include "../random_ppio.hpp"
#include <time.h>
#include <stdio.h>
using namespace std;

vector< vector<float> > readfilearrhythmia() {
    ifstream myfile("C:\\Users\\marin\\ClionProjects\\P5\\arrhythmia.arff");
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
    vector< vector<float> > datosn = datos;
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
    Set_random(6);
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

    /******************************************************** AGG ********************************************************/

    /* Generación de la poblacion inicial
     * 10 cromosomas generados aleatoriamente */

    int r;
    vector< vector<float> > C;      // C = Población, cada fila es un cromosoma
    vector<float> v;
    for(int i = 0; i < 10; i++){    // 10 cromosomas
        for(int j = 0; j < prueba[0].size()-1; j++){
            r = Randint(0, 1);
            v.push_back(r);
        }
        C.push_back(v);
        v.clear();
    }
    int eval = 0;

    /* Evaluacion de los individuos de la poblacion inicial
     * 10 evaluaciones */

    int MAX = 0;
    vector<int> aciertos;       // almacena la evaluación de los individuos de la población
    vector<float> mejorS;
    for( int i = 0; i < C.size(); i++){
        aciertos.push_back(clas3nn(prueba, prueba, C[i]));
        eval++;
        if (aciertos[i] > MAX){
            MAX = aciertos[i];
            mejorS = C[i];
        }
    }

    vector< vector<float> > C1; // se almacena la nueva población seleccionada
    vector<int> aciertos1;      // se almacena la evaluación de la nueva población seleccionada
    int i1,i2;
    float p_cruce = 0.7;
    float num_cruces = 0.7 * ( (C.size()) / 2.0 );
    int madre, padre;
    vector<int> por_evaluar;      // almacena las posiciones de los cromosomas que deben ser reevaluados
    float aux;
    float p_mut = 0.001;    // por gen
    float num_mut = p_mut * (C.size()) * (C[0].size());
    bool contenido;
    int CROM, GEN;
    bool mantenido = false;
    vector<float> MEJOR;
    int pos_min;
    int MIN;
    clock_t start_time;
    double elapsed;
    double total_time;
    float tasa, unos, red, M;

    // VARIABLES PARA LA BL
    int generaciones = 0;
    int contador, solucion, mejor_solucion;
    vector<float> vecino;
    bool elitismo = false;


    start_time = clock();
    /* Algoritmo Genético */
    while(eval < 1000){

        /* Selección
         * Se escogen dos individuos no repetidos aletoriamente y nos quedamos con el mejor.
         * Repetimos ésto tantas veces como individuos haya en la población */

        for (int i = 0; i < C.size(); i++) {
            i1 = Randint(0, C.size() - 1);
            i2 = Randint(0, C.size() - 1);
            while (i1 == i2) {
                i2 = Randint(0, C.size() - 1);
            }

            if (aciertos[i1] > aciertos[i2]) {
                C1.push_back(C[i1]);
                aciertos1.push_back(aciertos[i1]);
            }
            else {
                C1.push_back(C[i2]);
                aciertos1.push_back(aciertos[i2]);
            }

        }

        /* Cruce
         * Como el mecanismo de selección ya tiene una componente aleatoria, se realiza un emparejamiento fijo:
         * el primero con el segundo, el tercero con el cuarto, etc.
         * Calculamos N = número esperado de cruces, y cruzamos las N primeras parejas
         * Realizamos el cruce clásico en dos puntos */

        for (madre = 0; madre < num_cruces*2; madre+=2){
            padre = madre + 1;

            // Generamos los puntos de cruce
            i1 = Randint(0, C1[madre].size()-1);
            if (i1 != C1[madre].size()-1) i2 = Randint(i1+1, C1[madre].size()-1);
            else i2 = i1;
            // Cruzamos las partes contenidas entre ambos puntos
            if( i1 != i2){
                for (int i = i1; i < i2; i++){
                    aux = C1[madre][i1];
                    C1[madre][i1] = C1[padre][i1];
                    C1[padre][i1] = aux;
                }
                por_evaluar.push_back(madre);
                por_evaluar.push_back(padre);
            }

        }

        /* Mutación
         * Fijamos un número fijo de mutaciones
         * Escogemos aleatoriamente qué cromosoma y qué gen de ese cromosoma mutar */

        for(int i = 0; i < num_mut; i++) {
            CROM = Randint(0, C1.size() - 1);
            GEN = Randint(0, C1[CROM].size() - 1);
            C1[CROM] = flip(C1[CROM],GEN);
            contenido = false;
            for (int j = 0; j < por_evaluar.size(); j++){
                if (por_evaluar[j] == CROM) contenido = true;
            }
            if (!contenido) por_evaluar.push_back(CROM);
        }

        /* Selección elitista y evaluación
         * Comprobamos si se ha mantenido mejorS (mejor cromosoma de la población inicial)
         * - Si no se ha mantenido: se sustituye el peor cromosoma de la población actual por mejorS
         * - Si se ha mantenido no se hace nada
         * Se evalúan solo aquéllos cromosomas que han sido alterados */

        /* for (int i = 0; i < C1.size() && !mantenido; i++){
             if(C1[i] == mejorS) mantenido = true;
         }*/

        MIN = 999.99;
        for( int i = 0; i < por_evaluar.size(); i++){
            aciertos1[por_evaluar[i]] = clas3nn(prueba, prueba, C1[por_evaluar[i]]);
            eval++;
            if (aciertos1[por_evaluar[i]] >= MAX){
                MAX = aciertos1[por_evaluar[i]];
                MEJOR = C1[por_evaluar[i]];
            }
            //if(!mantenido) {
            if (aciertos1[por_evaluar[i]] < MIN) {
                MIN = aciertos1[por_evaluar[i]];
                pos_min = por_evaluar[i];
            }
            //}
        }
        /*if(!mantenido)*/ C1[pos_min] = mejorS;

        // Actualizamos las estructuras de datos para la siguiente iteración del while
        C = C1;                  // para la SELECCIÓN
        aciertos = aciertos1;    // para la SELECCIÓN
        C1.clear();              // para la SELECCIÓN
        aciertos1.clear();       // para la SELECCIÓN
        por_evaluar.clear();     // para CRUCE y MUTACIÓN
        mejorS = MEJOR;          // para ELITISMO

        generaciones++;

        // CADA 10 GENERACIONES SE APLICA LA BL SOBRE TODOS LOS CROMOSOMAS DE LA POBLACIÓN
        if (generaciones == 10){

            generaciones = 0;
            // Aplico Búsqueda Local sobre todos los cromosomas de C
            for(int i=0; i<C.size(); i++){
                contador = 0;
                mejor_solucion = aciertos[i];
                while (contador < C[i].size()){
                    vecino = flip(C[i],contador);
                    solucion = clas3nn(prueba, prueba, vecino);
                    eval++;

                    if (solucion > mejor_solucion){
                        mejor_solucion = solucion;
                        C[i] = vecino;
                        if (mejor_solucion > MAX){
                            mejorS = vecino;
                            MAX = mejor_solucion;
                        }
                        aciertos[i] = mejor_solucion;
                        contador = 0;
                    }
                    else contador++;
                }
            }
        }
    }
    // Aplico este clasificador construído al otro conjunto
    M = clas3nn(conjunto, prueba, mejorS);
    // Calculo la tasa de clasificación
    tasa = 100 * ( M / prueba.size() );
    // Calculo la tasa de reducción
    unos = 0.0;
    for (int h = 0; h < mejorS.size(); h++) {
        if (mejorS[h] == 1) unos += 1;
    }
    red = 100 * ((mejorS.size() - unos) / mejorS.size());

    elapsed = clock() - start_time;
    total_time = elapsed / CLOCKS_PER_SEC;

    cout << "Aciertos = " << M << endl;
    cout << "Tasa = " << tasa << endl;
    cout << "Reduccion = " << red << endl;
    cout << "Tiempo = " << total_time << endl;

}

/*
 * Semilla: 46 - CON intercambio
 *
 * Tasa = 65'9794
 * Reducción = 55'7554
 * Tiempo = 696'655
 *
 * Semilla: 46 - SIN intercambio
 *
 * Tasa = 64'0625
 * Reducción = 51'0791
 * Tiempo = 1209
 *
 * Semilla: 18 - CON intercambio
 *
 * Tasa =
 * Reducción =
 * Tiempo =
 *
 * Semilla: 18 - SIN intercambio
 * Aciertos =
 * Tasa =
 * Reducción =
 * Tiempo =
 *
 * Semilla: 95 - CON intercambio
 *
 * Tasa =
 * Reducción =
 * Tiempo =
 *
 * Semilla: 95 - SIN intercambio
 *
 * Tasa =
 * Reducción =
 * Tiempo =
 *
 * Semilla: 6 - CON intercambio
 *
 * Tasa = 59'2784
 * Reducción = 46'0432
 * Tiempo = 929'662
 *
 * Semilla: 6 - SIN intercambio
 *
 * Tasa = 67'1875
 * Reducción = 53'9568
 * Tiempo = 852'464
 *
 * Semilla: 33 - CON intercambio
 *
 * Tasa = 64'433
 * Reducción = 44'964
 * Tiempo = 748'4
 *
 * Semilla: 33 - SIN intercambio
 *
 * Tasa = 67'7083
 * Reducción = 47'1223
 * Tiempo = 855'646
 *
 *
 */

