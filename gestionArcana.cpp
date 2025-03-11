#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
using namespace std;

/*Modelo
1 //Numero de hechizos
Kharlion Malondi //Hechicero
6 //Vertices del hechizo
ABFBDB //Tipo de cada vertice con su valor numérico
8  //Aristas del hechizo
6 1 2 //6 y 1 son la ubicacion de la arista y 2 es la ponderación
6 4 1
1 4 4
1 2 1
2 4 7
2 5 2
5 3 1
4 3 3
*/

// Definimos una enumeración con los valores permitidos.
enum class TipoAsociado : char
{
    A = 'A', // Punto de confluencia
    B = 'B', // Punto de soporte energético
    I = 'I', // Fuego
    Q = 'Q', // Agua
    T = 'T', // Tierra
    V = 'V', // Viento
    L = 'L', // Luz
    O = 'O', // Oscuridad
    D = 'D', // Runa catalítica
    F = 'F', // Runa de estabilidad
};

class Arista
{
    int origen;
    int destino;
    int ponderacion;
    Arista *siguiente;

public:
    Arista(int ponderacion, int origen, int destino)
    {
        this->origen = origen;
        this->destino = destino;
        this->ponderacion = ponderacion;
        this->siguiente = nullptr;
    }

    int obtenerOrigen() const { return origen; }
    int obtenerDestino() const { return destino; }
    int obtenerPonderacion() const { return ponderacion; }
    int *obtenerArista() const
    {
        int *arista = new int[3]{this->origen, this->destino, this->ponderacion};
        return arista;
    }
    Arista *obtenerPtr_siguiente()
    {
        return siguiente;
    };

    void modificarPonderacion(int ponderacion)
    {
        this->ponderacion = ponderacion;
    };

    void modificarPtr_siguiente(Arista *siguiente)
    {
        this->siguiente = siguiente;
    };
};

struct dataHechizos
{
    int ID;
    string nombreMago;
    int numVertices;
    string tiposVertices;
    int numAristas;
    int **arrAristas;
};

struct Nodo
{
    string nombre;
    Nodo *next;
};


class ListE
{
    Arista *ptr_principal;
    int size;

public:
    ListE() : ptr_principal(nullptr), size(0) {};

    bool isEmpty()
    {
        return ptr_principal == nullptr;
    }

    void push(int origen, int destino, int ponderacion)
    {
        Arista *newArista = new Arista(ponderacion, origen, destino);
        newArista->modificarPtr_siguiente(nullptr);

        if (isEmpty())
        {
            ptr_principal = newArista;
        }
        else
        {
            Arista *ptr_aux = ptr_principal;
            while (ptr_aux->obtenerPtr_siguiente() != nullptr)
            {
                ptr_aux = ptr_aux->obtenerPtr_siguiente();
            }
            ptr_aux->modificarPtr_siguiente(newArista);
        }
        size++;
    };

    Arista *obtenerPtrPrincipal()
    {
        return ptr_principal;
    }

    int pop()
    {
        if (isEmpty())
        {
            throw out_of_range("List is empty");
        }
        Arista *aux = ptr_principal;
        ptr_principal = ptr_principal->obtenerPtr_siguiente();
        size--;
        return aux->obtenerPonderacion();
    };

    void deleteIndex(int index)
    {
        if (index < 0 || index >= size)
        {
            cout << "Index out of bounds" << endl;
            return;
        }

        Arista *indice = ptr_principal;

        if (index == 0)
        {
            Arista *ptr_aux = ptr_principal;
            ptr_principal = ptr_principal->obtenerPtr_siguiente();
            delete ptr_aux;
        }
        else
        {

            for (int i = 0; i < index - 1; i++)
            {

                indice = indice->obtenerPtr_siguiente();
            }
            Arista *ptr_aux = indice->obtenerPtr_siguiente();

            indice->modificarPtr_siguiente(ptr_aux->obtenerPtr_siguiente());
            delete ptr_aux;
        }
        size--;
    }

    void insertIndex(Arista element, int index)
    {
        Arista *newArista = new Arista(element);
        newArista->modificarPtr_siguiente(nullptr);

        Arista *indice = ptr_principal;

        if (index == 0)
        {
            Arista *ptr_aux = ptr_principal;
            ptr_principal = newArista;
            ptr_principal->modificarPtr_siguiente(ptr_aux);
        }

        else
        {

            for (int i = 0; i < index - 1; i++)
            {
                indice = indice->obtenerPtr_siguiente();
            }
            Arista *ptr_aux = indice->obtenerPtr_siguiente();
            indice->modificarPtr_siguiente(newArista);
            newArista->modificarPtr_siguiente(ptr_aux);
        }
        size++;
    }

    int *devolverAristas()
    {
    }

    void
    print()
    {

        Arista *ptr_indice = ptr_principal;

        cout << "Valores: ";
        while (ptr_indice != nullptr)
        {
            cout << "(" << ptr_indice->obtenerOrigen() << ", " << ptr_indice->obtenerDestino() << ", " << ptr_indice->obtenerPonderacion() << ")";
            if (ptr_indice->obtenerPtr_siguiente() != nullptr)
            {
                cout << " -> ";
            }
            ptr_indice = ptr_indice->obtenerPtr_siguiente();
        }
    }
};

class Vertice
{
    ListE aristasAsociadas;
    TipoAsociado tipoAsociado;
    int indice;

public:
    Vertice() {} // Constructor por defecto

    Vertice(int indice, TipoAsociado tipoAsociado)
    {
        this->tipoAsociado = tipoAsociado;
        this->indice = indice;
    }
    int obtenerIndice() const { return indice; }

    TipoAsociado obtenerTipo()
    {
        return tipoAsociado;
    }

    void agregarArista(int origen, int destino, int ponderacion)
    {
        aristasAsociadas.push(origen, destino, ponderacion);
    }

    Arista *obtenerPrimeraArista()
    {
        return aristasAsociadas.obtenerPtrPrincipal();
    }

    Arista *imprimirAristas() { aristasAsociadas.print(); }
};

class Hechizo
{
    string hechicero;
    Vertice *vertice;
    int IDHechizo;
    int numVertices;
    int numAristas;

public:
    // Se crea el grafo con un número determinado de vértices.
    Hechizo(int IDhechizo, string hechicero, int numVertices, string cadenaDeTipos, int numAristas, int **aristas)
    {
        this->IDHechizo = IDHechizo;
        this->numVertices = numVertices;
        this->hechicero = hechicero;
        this->numAristas = numAristas;
        vertice = new Vertice[numVertices];
        for (int i = 0; i < numVertices; ++i)
        {
            // Se asigna el vértice usando el carácter correspondiente
            vertice[i] = Vertice(i + 1, TipoAsociado(cadenaDeTipos[i]));
        }
        for (int j = 0; j < numAristas; j++)
        {
            int origen = aristas[j][0];
            int destino = aristas[j][1];
            int ponderacion = aristas[j][2];
            // Verifica que los índices sean válidos (suponiendo índices de 1 a numVertices)
            if ((origen > 0 && origen <= numVertices) && (destino > 0 && destino <= numVertices))
            {
                // Asigna la arista a los vértices involucrados.
                vertice[origen - 1].agregarArista(origen, destino, ponderacion);
                vertice[destino - 1].agregarArista(destino, origen, ponderacion);
            }
            else
            {
                cout << "Indice de vertice invalido." << endl;
            }
        }
    }

    int obtenerID()
    {
        return this->IDHechizo;
    }

    Vertice *obtenerVertices()
    {
        return vertice;
    }

    int obtenerNumVertices() const
    {
        return this->numVertices;
    }

    int obtenerNumAristas() const
    {
        return this->numAristas;
    }

    ~Hechizo()
    {
        // Libera el arreglo de vértices
        delete[] vertice;
    }

    void print()
    {
        cout << "Hechicero: " << hechicero << "\n";
        for (int i = 0; i < numVertices; ++i)
        {
            cout << "Vertice " << vertice[i].obtenerIndice() << " -> ";
            // Imprime la lista de aristas asociadas al vértice.
            vertice[i].imprimirAristas();
            cout << "\n";
        }
    }
};

int **matrizAdyacencia(Hechizo *hechizo)
{
    int numVertices = hechizo->obtenerNumVertices();
    // Crear la matriz y asignar cada fila
    int **matriz = new int *[numVertices];
    for (int i = 0; i < numVertices; i++)
    {
        matriz[i] = new int[numVertices];
        // Inicializar la fila en 0
        for (int j = 0; j < numVertices; j++)
        {

            matriz[i][j] = 0;
        }
    }

    // Obtener arreglo de vértices
    Vertice *vertices = hechizo->obtenerVertices();
    // Recorrer cada vértice y llenar la matriz con las ponderaciones de sus aristas
    for (int i = 0; i < numVertices; i++)
    {
        // Obtenemos la primera arista del vértice i
        Arista *aristaPtr = vertices[i].obtenerPrimeraArista();
        while (aristaPtr != nullptr)
        {
            // 1,6,1
            int fila = aristaPtr->obtenerOrigen() - 1;
            int columna = aristaPtr->obtenerDestino() - 1;
            int peso = aristaPtr->obtenerPonderacion();
            // Asignamos el peso correspondiente en la matriz
            matriz[fila][columna] = peso;
            matriz[columna][fila] = peso;
            aristaPtr = aristaPtr->obtenerPtr_siguiente();
        }
    }

    // Imprimir
    cout << "Hechizo " << hechizo->obtenerID() << endl;
    cout << "  ";
    for (int i = 0; i < numVertices; i++)
    {
        cout << vertices[i].obtenerIndice() << " ";
    }
    cout << endl;

    for (int i = 0; i < numVertices; i++)
    {
        cout << vertices[i].obtenerIndice() << " ";
        for (int j = 0; j < numVertices; j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }

    return matriz;
}

int main()
{
    dataHechizos hechizo;
    int numHechizos;
    ifstream archivo("spellList.in");
    if (!archivo.is_open())
    {
        cout << "error al abrir el archivo" << endl;
        return 1;
    }

    archivo >> numHechizos;
    archivo.ignore(1000, '\n');
    string hechicero;
    for (int i = 0; i < numHechizos; i++)
    {
        hechizo.ID = i + 1;
        getline(archivo, hechizo.nombreMago);
        archivo >> hechizo.numVertices;
        archivo >> hechizo.tiposVertices;
        archivo >> hechizo.numAristas;
        hechizo.arrAristas = new int *[hechizo.numAristas];
        for (int j = 0; j < hechizo.numAristas; j++)
        {
            hechizo.arrAristas[j] = new int[3];
            int o, d, p;
            archivo >> o >> d >> p;
            hechizo.arrAristas[j][0] = o;
            hechizo.arrAristas[j][1] = d;
            hechizo.arrAristas[j][2] = p;
        }
        Hechizo *hechizo1 = new Hechizo(hechizo.ID, hechizo.nombreMago, hechizo.numVertices, hechizo.tiposVertices, hechizo.numAristas, hechizo.arrAristas);
        // hechizo1->print();
        int **matriz = matrizAdyacencia(hechizo1);



        for (int i = 0; i < hechizo.numVertices; i++)
        {
            delete[] matriz[i]; // Libera cada fila
        }
        delete[] matriz;
        // Liberar la instancia del Hechizo (que a su vez libera el arreglo de vértices)
        delete hechizo1;

        archivo.ignore(1000, '\n');
    }

    archivo.close();

    ifstream archivo2("underInvestigation.in");
    if (!archivo2.is_open())
    {
        cout << "error al abrir el archivo" << endl;
        return 1;
    }
    Nodo *inicio = nullptr;
    Nodo *actual = nullptr;
    string hechiceroSospechozo;
    while (getline(archivo2, hechiceroSospechozo))
    {
        Nodo *nuevonodo = new Nodo;
        nuevonodo->nombre = hechiceroSospechozo;
        nuevonodo->next = nullptr;
        if (inicio == nullptr)
        {
            inicio = nuevonodo;
            actual = nuevonodo;
        }
        else
        {
            actual->next = nuevonodo;
            actual = actual->next;
        }
    }
    archivo2.close();
    // Liberar la memoria asignada para cada arreglo de aristas
    for (int i = 0; i < hechizo.numAristas; i++)
    {
        delete[] hechizo.arrAristas[i];
    }
    // Liberar el arreglo de punteros
    delete[] hechizo.arrAristas;

    return 0;
}