#include <iostream>>
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
    F = 'F',
    // Runa de estabilidad
};

class Arista
{
    int origen;
    int destino;
    int ponderacion;

public:
    Arista(int ponderacion, int origen, int destino)
    {
        this->origen = origen;
        this->destino = destino;
        this->ponderacion = ponderacion;
    }

    int obtenerOrigen() const { return origen; }
    int obtenerDestino() const { return destino; }
    int obtenerPonderacion() const { return ponderacion; }
};

class Vertice
{
    Arista *aristasAsociadas;
    TipoAsociado tipoAsociado;
    int indice;

public:
    Vertice(int indice, TipoAsociado tipoAsociado)
    {
        this->tipoAsociado = tipoAsociado;
        this->indice = indice;
    }

    void agregarArista(const Arista &arista)
    {
    }

    int obtenerIndice() const { return indice; }

    Arista *obtenerAristas() const { return aristasAsociadas; }
};

class Hechizo
{
    Vertice *vertices;

public:
    // Se crea el grafo con un número determinado de vértices.
    Hechizo(int numVertices)
    {
        // Para este ejemplo, asignamos el tipo 'A' a todos.
        for (int i = 0; i < numVertices; ++i)
        {
            // Crear vertice con indice y tipo;
        }
    }

    // Agrega una arista desde el vértice de origen hasta el destino.
    void agregarArista(int origen, int destino, int ponderacion)
    {
        if (origen > 0 && vertices[origen].obtenerIndice() != NULL && destino > 0 && vertices[destino].obtenerIndice() != NULL)
        {
            Arista nuevaArista(origen, destino, ponderacion);
            vertices[origen].agregarArista(nuevaArista);
        }
        else
        {
            cout << "Índice de vértice inválido." << endl;
        }
    }
};

int main()
{

    return 0;
}