#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <vector>
#include <thread>
#include <pthread.h>

using namespace std;
#define N 10
pthread_t tid[N]; // arreglo de threads

int global = 99999; // variable global para el menor coste en terminos de camino
pthread_mutex_t mutex;

class Grafo
{

private:
    int matrix[10][10];

public:
    Grafo(int matrix[10][10])
    {

        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                matrix[i][j] = 0;
            }
        }
    }

    void Print() // imprime la matriz
    {
        for (int i = 0; i < 10; i++)
        {
            cout << endl;
            for (int j = 0; j < 10; j++)
            {
                cout << " " << this->matrix[i][j];
            }
        }
        cout << endl;
    }

    void Agregar(int x, int y, int z)
    {
        this->matrix[x][y] = z; // inicio y direccion donde apunta el nodo z = coste dese x a y
    }

    void Recorre(int A, int KH)
    { // A = inicio KH = final del grafo (en este caso 0 es el inicio y 9 el final)

        vector<int> camino;   // camino que se sigue en el grafo
        vector<int> opciones; // opciones para recorrer el grafo
        int random;
        int cont = 0; // coste de los caminos

        while (true)
        {

            for (int w = 0; w < 10; w++)
            { // recorro el grafo para ver la opciones de camino que se tienen
                if (this->matrix[A][w] > 0)
                {
                    opciones.push_back(w); // guardo la posicion de los caminos que se pueden tomar en la matriz
                }
            }

            random = rand() % opciones.size(); // numero aleatorio para poder seguir caminos randoms
            cont = cont + this->matrix[A][opciones[random]];
            camino.push_back(A);
            A = opciones[random]; // para buscar las siguientes opciones

            while (opciones.empty() != true) // se limpia para si poder seguir con los siguientes caminos posibles
            {
                opciones.pop_back();
            }
            random = 0;

            if (A == KH)
            {
                camino.push_back(A); // ingresa el ultimo nodo
                break;
            }
        }

        /*for (int s = 0; s < camino.size(); s++)  // Esto muestra los caminos que se siguieron y los costes de estos
         {
             cout << camino[s] << " ";
         }

         cout << endl;
         cout << "Coste del camino: " << cont << endl;*/
        // Coste de cada camino

        pthread_mutex_lock(&mutex);
        if (cont < global) // contador del mejor camino global
        {
            global = cont;
            cout << "Menor costo global Global: " << global << endl;
            cout << "Camino: ";

            for (int i = 0; i < camino.size(); i++)
            {
                cout << camino[i] << " ";
            }
            cout << endl;
        }
        pthread_mutex_unlock(&mutex);
    }
};

void *Rec_conThread(void *arg)
{

    int matrix[10][10];
    Grafo *n = new Grafo(matrix);

    n->Agregar(0, 1, 3);
    n->Agregar(0, 2, 4);
    n->Agregar(0, 3, 1);
    n->Agregar(1, 5, 2);
    n->Agregar(2, 5, 3);
    n->Agregar(3, 4, 5);
    n->Agregar(4, 6, 4);
    n->Agregar(4, 7, 3);
    n->Agregar(5, 8, 2);
    n->Agregar(6, 8, 1);
    n->Agregar(7, 8, 2);
    n->Agregar(7, 9, 3);
    n->Agregar(8, 9, 1);

    n->Recorre(0, 9);

    // cout << "cuento: " << contar << endl;
}

int main()
{
    srand(time(NULL));

    for (int i = 0; i < N; i++)
    {
        pthread_create(&(tid[i]), NULL, &Rec_conThread, NULL);
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(tid[i], NULL);
    }
    pthread_mutex_destroy(&mutex);

    return 0;
}