#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <string>
using namespace std;

/* Modelo de entrada:
1                   // Numero de hechizos
Kharlion Malondi    // Hechicero
6                   // Vertices del hechizo
ABFBDB              // Tipo de cada vertice
8                   // Aristas del hechizo
6 1 2               // (origen, destino, ponderacion)
6 4 1
1 4 4
1 2 1
2 4 7
2 5 2
5 3 1
4 3 3
*/

// Enumeración de tipos permitidos.
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
    F = 'F'  // Runa de estabilidad
};

struct nodoHechizero
{
    string nombre;
    int cantHIlegales;
    nodoHechizero *next = nullptr;
};

nodoHechizero *listaHechizeros = nullptr;

class Arista
{
    int origen;
    int destino;
    int ponderacion;
    Arista *siguiente;

public:
    Arista(int *arista)
    {
        this->origen = arista[0];
        this->destino = arista[1];
        this->ponderacion = arista[2];
        this->siguiente = nullptr;
    }

    int obtenerOrigen() const { return origen; }
    int obtenerDestino() const { return destino; }
    int obtenerPonderacion() const { return ponderacion; }
    Arista *obtenerPtr_siguiente()
    {
        return siguiente;
    }
    void modificarPtr_siguiente(Arista *siguiente)
    {
        this->siguiente = siguiente;
    }
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

template <typename T>
class ListE
{
public:
    // Nodo interno para la lista.
    struct Node
    {
        T *data; // Puntero al objeto almacenado.
        Node *next;
        Node(T *data) : data(data), next(nullptr) {}
    };
    Node *head;
    int size;

public:
    ListE() : head(nullptr), size(0) {}

    bool isEmpty() const
    {
        return head == nullptr;
    }

    // Inserta al final de la lista un elemento (puntero a T)
    void push(T *data)
    {
        Node *newNode = new Node(data);
        if (isEmpty())
        {
            head = newNode;
        }
        else
        {
            Node *current = head;
            while (current->next != nullptr)
            {
                current = current->next;
            }
            current->next = newNode;
        }
        size++;
    }

    // Método para obtener el nodo principal (head)
    Node *getHead() const
    {
        return head;
    }

    int getSize() const
    {
        return size;
    }

    // Función print: recorre la lista independientemente del tipo (requiere que operator<< esté sobrecargado para T, o se implemente la impresión correspondiente).
    void print() const
    {
        Node *current = head;
        while (current != nullptr)
        {
            // Aquí se asume que el objeto T tiene implementado operator<< o se adapta la impresión.
            cout << *(current->data) << " -> ";
            current = current->next;
        }
        cout << "NULL" << endl;
    }

    ~ListE()
    {
        Node *current = head;
        while (current != nullptr)
        {
            Node *nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }
};

class Vertice
{
    ListE<Arista> aristasAsociadas;
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

    void agregarArista(int *arista)
    {
        Arista *nuevaArista = new Arista(arista);
        aristasAsociadas.push(nuevaArista);
    }

    Arista *obtenerPrimeraArista()
    {
        return aristasAsociadas.getHead()->data;
    }
};

class Hechicero
{
    string nombreCompleto;
    string nombre;
    string apellido;
    int hechizosIlegales;

public:
    Hechicero() {} // Default constructor

    Hechicero(string nombreCompleto)
    {
        size_t pos = nombreCompleto.find(' ');
        this->nombreCompleto = nombreCompleto;
        this->nombre = nombreCompleto.substr(0, pos);
        this->apellido = nombreCompleto.substr(pos + 1);
    }

    string obtenerApellido()
    {
        return this->apellido;
    }

    string obtenerNombreCompleto()
    {
        return this->nombreCompleto;
    }
};

class Hechizo
{
    Hechicero hechicero;
    string nombre;
    Vertice *vertice;
    char runaElemental;
    int IDHechizo;
    int numVertices;
    int numAristas;
    string cadenaTipos;

public:
    // Construye el grafo a partir de los datos
    Hechizo(int IDHechizo, Hechicero hechicero, int numVertices, string cadenaDeTipos, int numAristas, int **aristas)
    {
        this->cadenaTipos = cadenaDeTipos;
        this->runaElemental = ' '; // Por defecto, se asignará luego en otra función.
        this->IDHechizo = IDHechizo;
        this->numVertices = numVertices;
        this->hechicero = hechicero;
        this->numAristas = numAristas;
        vertice = new Vertice[numVertices];
        for (int i = 0; i < numVertices; ++i)
        {
            // Asigna el vértice usando el carácter correspondiente
            vertice[i] = Vertice(i + 1, TipoAsociado(cadenaDeTipos[i]));
        }
        for (int j = 0; j < numAristas; j++)
        {
            // 6,1,2
            int origen = aristas[j][0];
            int destino = aristas[j][1];
            int ponderacion = aristas[j][2];
            // Verifica que los índices sean válidos (suponiendo índices de 1 a numVertices)
            if ((origen > 0 && origen <= numVertices) && (destino > 0 && destino <= numVertices))
            {
                // Asigna la arista a los vértices involucrados.
                vertice[origen - 1].agregarArista(aristas[j]);
                vertice[destino - 1].agregarArista(aristas[j]);
            }
            else
            {
                cout << "Indice de vertice invalido." << endl;
            }
        }
    }

    void setNombre(string nombre)
    {
        this->nombre = nombre;
    }

    string obtenerNombre()
    {
        return this->nombre;
    }

    int obtenerID()
    {
        return this->IDHechizo;
    }

    Hechicero obtenerHechicero()
    {
        return this->hechicero;
    }

    Vertice *obtenerVertices()
    {
        return vertice;
    }

    char obtenerRunaElemental()
    {
        return this->runaElemental;
    }

    void setRunaElemental(char runaElemental)
    {
        this->runaElemental = runaElemental;
    }

    int obtenerNumVertices() const
    {
        return this->numVertices;
    }

    string obtenerTiposVertices()
    {
        return this->cadenaTipos;
    }

    ~Hechizo()
    {
        // Libera el arreglo de vértices
        delete[] vertice;
    }
};
//
// Aquí, en lugar de tener listas de hechizos dentro de cada objeto Hechizo,
// usamos listas globales para almacenar los hechizos legales e ilegales:
//
ListE<Hechizo> listaLegalesGlobal;
ListE<Hechizo> listaIlegalesGlobal;

int **matrizAdyacencia(Hechizo *hechizo)
{
    int numVertices = hechizo->obtenerNumVertices();
    int **matriz = new int *[numVertices];
    for (int i = 0; i < numVertices; i++)
    {
        matriz[i] = new int[numVertices];
        for (int j = 0; j < numVertices; j++)
        {
            matriz[i][j] = 0;
        }
    }

    Vertice *vertices = hechizo->obtenerVertices();
    for (int i = 0; i < numVertices; i++)
    {
        Arista *aristaPtr = vertices[i].obtenerPrimeraArista();
        while (aristaPtr != nullptr)
        {
            int fila = aristaPtr->obtenerOrigen() - 1;
            int columna = aristaPtr->obtenerDestino() - 1;
            int peso = aristaPtr->obtenerPonderacion();
            matriz[fila][columna] = peso;
            matriz[columna][fila] = peso;
            aristaPtr = aristaPtr->obtenerPtr_siguiente();
        }
    }

    // (Se muestra la matriz por consola)
    cout << "Hechizo: " << hechizo->obtenerID() << endl;
    cout << "Hechicero: " << hechizo->obtenerHechicero().obtenerNombreCompleto() << endl;
    cout << "  ";
    for (int i = 0; i < numVertices; i++)
    {
        // Se imprime el tipo de cada vértice (convertido a char)
        // (Asegúrese de tener operator char() o use static_cast<char>)
        cout << " " << static_cast<char>(hechizo->obtenerTiposVertices()[i]) << "  ";
    }
    cout << endl;
    for (int i = 0; i < numVertices; i++)
    {
        cout << static_cast<char>(hechizo->obtenerTiposVertices()[i]) << " ";
        for (int j = 0; j < numVertices; j++)
        {
            cout << "[" << matriz[i][j] << "] ";
        }
        cout << endl;
    }
    cout << endl;
    return matriz;
}

// Regla 1
bool cuantasA(Hechizo *hechizo)
{
    string tipos = hechizo->obtenerTiposVertices();
    int numVertices = hechizo->obtenerNumVertices();
    int contadorA = 0;
    for (int i = 0; i < numVertices; i++)
    {
        if (tipos[i] == 'A' || tipos[i] == 'a')
        {
            contadorA++;
        }
    }
    return contadorA == 1;
}

// Regla 2
bool distribucionCorrecta(Hechizo *hechizo, int **matriz)
{
    int numVertices = hechizo->obtenerNumVertices();
    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < numVertices; j++)
        {
            if (matriz[i][j] != 0 && (hechizo->obtenerTiposVertices()[i] == 'A' && hechizo->obtenerTiposVertices()[j] != 'B'))
            {
                return false;
            }
        }
    }
    return true;
}

// Regla 3
bool cuantasRunas(Hechizo *hechizo)
{
    string tipos = hechizo->obtenerTiposVertices();
    int contadorI = 0, contadorQ = 0, contadorT = 0, contadorV = 0, contadorL = 0, contadorO = 0;
    for (char c : tipos)
    {
        switch (c)
        {
        case 'I':
            hechizo->setRunaElemental(c);
            contadorI++;
            if (contadorI > 3 || (contadorI > 0 && (contadorQ + contadorT + contadorV + contadorL + contadorO) > 0))
                return false;
            break;
        case 'Q':
            hechizo->setRunaElemental(c);
            contadorQ++;
            if (contadorQ > 3 || (contadorQ > 0 && (contadorI + contadorT + contadorV + contadorL + contadorO) > 0))
                return false;
            break;
        case 'T':
            hechizo->setRunaElemental(c);
            contadorT++;
            if (contadorT > 3 || (contadorT > 0 && (contadorQ + contadorI + contadorV + contadorL + contadorO) > 0))
                return false;
            break;
        case 'V':
            hechizo->setRunaElemental(c);
            contadorV++;
            if (contadorV > 3 || (contadorV > 0 && (contadorQ + contadorT + contadorI + contadorL + contadorO) > 0))
                return false;
            break;
        case 'L':
            hechizo->setRunaElemental(c);
            contadorL++;
            if (contadorL > 3 || (contadorL > 0 && (contadorQ + contadorT + contadorV + contadorI + contadorO) > 0))
                return false;
            break;
        case 'O':
            hechizo->setRunaElemental(c);
            contadorO++;
            if (contadorO > 3 || (contadorO > 0 && (contadorQ + contadorT + contadorV + contadorL + contadorI) > 0))
                return false;
            break;
        }
    }
    return true;
}

// Regla 4
bool adyacenciaDeRunas(Hechizo *hechizo, int **matriz)
{
    int numVertices = hechizo->obtenerNumVertices();
    for (int i = 0; i < numVertices; i++)
    {
        for (int j = 0; j < numVertices; j++)
        {
            if (matriz[i][j] != 0 &&
                (hechizo->obtenerTiposVertices()[j] == 'T' ||
                 hechizo->obtenerTiposVertices()[j] == 'I' ||
                 hechizo->obtenerTiposVertices()[j] == 'Q' ||
                 hechizo->obtenerTiposVertices()[j] == 'V' ||
                 hechizo->obtenerTiposVertices()[j] == 'L' ||
                 hechizo->obtenerTiposVertices()[j] == 'O') &&
                hechizo->obtenerTiposVertices()[i] == 'D')
            {
                return false;
            }
        }
    }
    return true;
}

// Función que recorre el grafo en búsqueda de ciclos.
int cicloDfs(int **matriz, int numVertices, int inicio, int actual, bool *visitado, int cuenta)
{
    int maxCiclo = -1;
    for (int vecino = 0; vecino < numVertices; vecino++)
    {
        if (matriz[actual][vecino] != 0)
        {
            if (vecino == inicio && cuenta >= 1)
            {
                int cicloActual = cuenta + 1;
                if (cicloActual > maxCiclo)
                    maxCiclo = cicloActual;
            }
            else if (!visitado[vecino])
            {
                visitado[vecino] = true;
                int cicloEncontrado = cicloDfs(matriz, numVertices, inicio, vecino, visitado, cuenta + 1);
                if (cicloEncontrado > maxCiclo)
                    maxCiclo = cicloEncontrado;
                visitado[vecino] = false;
            }
        }
    }
    return maxCiclo;
}

// Regla 5
int cicloMasLargo(Hechizo *hechizo, int **matriz)
{
    int numVertices = hechizo->obtenerNumVertices();
    int maxCicloGlobal = 0;
    bool *visitado = new bool[numVertices];
    for (int i = 0; i < numVertices; i++)
        visitado[i] = false;

    for (int i = 0; i < numVertices; i++)
    {
        visitado[i] = true;
        int cicloDesdeI = cicloDfs(matriz, numVertices, i, i, visitado, 0);
        if (cicloDesdeI > maxCicloGlobal)
            maxCicloGlobal = cicloDesdeI;
        visitado[i] = false;
    }

    delete[] visitado;
    return maxCicloGlobal;
}

nodoHechizero *buscarOAgregarHechicero(const string &nombre)
{
    string nombreBuscado = nombre;
    nodoHechizero *actual = listaHechizeros;
    nodoHechizero *anterior = nullptr;
    while (actual != nullptr)
    {
        string nombreActual = actual->nombre;
        if (nombreActual == nombreBuscado)
            return actual;
        anterior = actual;
        actual = actual->next;
    }
    nodoHechizero *nuevo = new nodoHechizero;
    nuevo->nombre = nombre;
    nuevo->cantHIlegales = 0;
    nuevo->next = nullptr;
    if (anterior == nullptr)
        listaHechizeros = nuevo;
    else
        anterior->next = nuevo;
    return nuevo;
}

bool detectarHechizoArcante(Hechizo *hechizo)
{
    string tipos = hechizo->obtenerTiposVertices();
    for (int i = 0; i < hechizo->obtenerNumVertices(); i++)
    {
        if (tipos[i] == 'I' || tipos[i] == 'Q' || tipos[i] == 'T' || tipos[i] == 'V' ||
            tipos[i] == 'L' || tipos[i] == 'O' || tipos[i] == 'D' || tipos[i] == 'F')
        {
            return true;
        }
    }
    return false;
}

void dfsCaminoPesado(int **matriz, int numVertices, int actual, bool *visitado, int acumulado, const string &tipos, int &maxPeso)
{
    for (int v = 0; v < numVertices; v++)
    {
        if (matriz[actual][v] != 0 && !visitado[v])
        {
            int nuevoPeso = acumulado + matriz[actual][v];
            if (tipos[v] == 'D' || tipos[v] == 'F')
            {
                if (nuevoPeso > maxPeso)
                    maxPeso = nuevoPeso;
            }
            visitado[v] = true;
            dfsCaminoPesado(matriz, numVertices, v, visitado, nuevoPeso, tipos, maxPeso);
            visitado[v] = false;
        }
    }
}

string caminoMasPesado(Hechizo *hechizo, int **matriz)
{
    int numVertices = hechizo->obtenerNumVertices();
    string tipos = hechizo->obtenerTiposVertices();
    int maxPeso = 0;
    if (!detectarHechizoArcante(hechizo))
        return "Arcante";
    else
    {
        bool *visitado = new bool[numVertices];
        for (int i = 0; i < numVertices; i++)
            visitado[i] = false;
        for (int i = 0; i < numVertices; i++)
        {
            if (tipos[i] == 'A' || tipos[i] == 'a')
            {
                visitado[i] = true;
                dfsCaminoPesado(matriz, numVertices, i, visitado, 0, tipos, maxPeso);
                visitado[i] = false;
            }
        }
        delete[] visitado;
        if (maxPeso > cicloMasLargo(hechizo, matriz))
            return "modicum ";
        else if (maxPeso < cicloMasLargo(hechizo, matriz))
            return "maximus ";
        else
            return ""; // Si son iguales, se puede retornar cadena vacía u otra indicación.
    }
}

bool legalidad(Hechizo *hechizo, int **matriz)
{
    if (!cuantasRunas(hechizo))
        return false;
    if (!cuantasA(hechizo))
        return false;
    if (!distribucionCorrecta(hechizo, matriz))
        return false;
    if (!adyacenciaDeRunas(hechizo, matriz))
        return false;
    if (cicloMasLargo(hechizo, matriz) % 2 != 0)
        return false;
    return true;
}

// Regla 7: Si el hechizo no es legal, se registra el hechicero en UNDER INVESTIGATION.
void esSospechozo(Hechizo *hechizo, int **matriz)
{
    string nombreH = hechizo->obtenerHechicero().obtenerNombreCompleto();
    nodoHechizero *nodo = buscarOAgregarHechicero(nombreH);
    if (!legalidad(hechizo, matriz))
    {
        int cuentaAnterior = nodo->cantHIlegales;
        nodo->cantHIlegales++;
        if (cuentaAnterior == 2)
        {

            ofstream archivo2("underInvestigation.in", ios::app);

            if (archivo2.is_open())
            {
                archivo2 << nombreH << endl;
                archivo2.close();
            }
        }
    }
}

// Regla 8: Nombramiento del hechizo.
string nombramientoDeHechizo(Hechizo *hechizo, int **matriz)
{
    string nombreHechizo;
    string apellidoHechicero = hechizo->obtenerHechicero().obtenerApellido();
    char ultimo = apellidoHechicero[apellidoHechicero.size() - 1];
    cuantasRunas(hechizo);
    char runa = hechizo->obtenerRunaElemental();
    if (runa == 'I')
        nombreHechizo = "Ignatum ";
    else if (runa == 'Q')
        nombreHechizo = "Aquos ";
    else if (runa == 'T')
        nombreHechizo = "Terraminium ";
    else if (runa == 'V')
        nombreHechizo = "Ventus ";
    else if (runa == 'L')
        nombreHechizo = "Lux ";
    else if (runa == 'O')
        nombreHechizo = "Tenebrae ";

    string sufijo;
    if (ultimo == 'a' || ultimo == 'e' || ultimo == 'i' || ultimo == 'o' || ultimo == 'u')
        sufijo = apellidoHechicero.substr(0, apellidoHechicero.size() - 1) + "ium";
    else
        sufijo = apellidoHechicero + "um";

    string indicador = caminoMasPesado(hechizo, matriz);
    return nombreHechizo + sufijo + " " + indicador;
}

// Función para liberar la lista de hechiceros.
void liberarLista(nodoHechizero *&listaHechizeros)
{
    while (listaHechizeros != nullptr)
    {
        nodoHechizero *temp = listaHechizeros;
        listaHechizeros = listaHechizeros->next;
        delete temp;
    }
}

// Función de salida: escribe en "processedSpell.out" los hechizos legales e ilegales.
void escribirEnArchivo1(const ListE<Hechizo> &listaLegales, const ListE<Hechizo> &listaIlegales)
{
    ofstream archivo3("processedSpell.out");
    if (archivo3.is_open())
    {
        archivo3 << "Hechizos Legales" << endl;
        // Iterar sobre la lista de hechizos legales
        typename ListE<Hechizo>::Node *nodoActual;
        nodoActual = listaLegales.getHead();
        while (nodoActual != nullptr)
        {
            archivo3 << endl;
            archivo3 << nodoActual->data->obtenerNombre() << endl;
            archivo3 << nodoActual->data->obtenerHechicero().obtenerNombreCompleto() << endl;

            nodoActual = nodoActual->next;
        }
        archivo3 << endl;
        archivo3 << "Hechizos Ilegales" << endl;
        nodoActual = listaIlegales.getHead();
        while (nodoActual != nullptr)
        {
            archivo3 << endl;
            archivo3 << nodoActual->data->obtenerNombre() << endl;
            archivo3 << nodoActual->data->obtenerHechicero().obtenerNombreCompleto() << endl;

            nodoActual = nodoActual->next;
        }
        archivo3.close();
    }
}

int main()
{
    Hechizo *hechizo1;
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

    // Procesar cada hechizo y agregarlos a las listas globales
    for (int i = 0; i < numHechizos; i++)
    {
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
        // Crear el hechizo
        hechizo1 = new Hechizo(i + 1, Hechicero(hechizo.nombreMago), hechizo.numVertices, hechizo.tiposVertices, hechizo.numAristas, hechizo.arrAristas);

        // Crear la matriz de adyacencia y asignar el nombre mediante el nombramiento
        int **matriz = matrizAdyacencia(hechizo1);
        hechizo1->setNombre(nombramientoDeHechizo(hechizo1, matriz));

        // Clasificar el hechizo (se utiliza legalidad; si NO es legal se considera "sospechozo")
        if (!legalidad(hechizo1, matriz))
        {
            listaIlegalesGlobal.push(hechizo1);
            esSospechozo(hechizo1, matriz);
        }
        else
        {
            listaLegalesGlobal.push(hechizo1);
        }

        // Liberar la matriz de adyacencia
        for (int k = 0; k < hechizo.numVertices; k++)
        {
            delete[] matriz[k];
        }
        delete[] matriz;

        archivo.ignore(1000, '\n');
        // Nota: No se elimina hechizo1 porque se lo necesita en la lista para la salida.
    }
    archivo.close();

    // Escribir la salida final a archivo
    escribirEnArchivo1(listaLegalesGlobal, listaIlegalesGlobal);

    ifstream archivo2("underInvestigation.in");
    if (!archivo2.is_open())
    {
        cout << "error al abrir el archivo" << endl;
        return 1;
    }
    Nodo *inicio = nullptr;
    Nodo *actual = nullptr;
    string linea2;
    while (getline(archivo2, linea2))
    {
        Nodo *nuevonodo = new Nodo;
        nuevonodo->nombre = linea2;
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

    // Liberar la memoria asignada para cada arreglo de aristas en dataHechizos
    for (int i = 0; i < hechizo.numAristas; i++)
    {
        delete[] hechizo.arrAristas[i];
    }
    delete[] hechizo.arrAristas;

    liberarLista(listaHechizeros);

    return 0;
}