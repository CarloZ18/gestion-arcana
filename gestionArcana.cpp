#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h>
#include <string>
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
    Arista(int origen, int destino, int ponderacion)
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

template <typename T>
class ListE
{
    T *ptr_principal;
    T *ptr_siguiente;
    int size;

public:
    ListE() : ptr_principal(nullptr), size(0) {};

    bool isEmpty()
    {
        return ptr_principal == nullptr;
    }

    void pushArista(int origen, int destino, int ponderacion)
    {
        Arista *newArista = new Arista(origen, destino, ponderacion);
        newArista->modificarPtr_siguiente(nullptr);

        if (isEmpty())
        {
            ptr_principal = newArista;
        }
        else
        {
            T *ptr_aux = ptr_principal;
            while (ptr_aux->obtenerPtr_siguiente() != nullptr)
            {
                ptr_aux = ptr_aux->obtenerPtr_siguiente();
            }
            ptr_aux->modificarPtr_siguiente(newArista);
        }
        size++;
    };

    void push(T *data)
    {
        T *newArista = new T(data);
        newArista->modificarPtr_siguiente(nullptr);

        if (isEmpty())
        {
            ptr_principal = newArista;
        }
        else
        {
            T *ptr_aux = ptr_principal;
            while (ptr_aux->obtenerPtr_siguiente() != nullptr)
            {
                ptr_aux = ptr_aux->obtenerPtr_siguiente();
            }
            ptr_aux->modificarPtr_siguiente(newArista);
        }
        size++;
    };

    T *obtenerPtrPrincipal()
    {
        return ptr_principal;
    }

    T *obtenerPtrSiguiente()
    {
        return ptr_siguiente;
    }

    void *setPtrSiguiente(T *ptr_siguiente)
    {
        return ptr_siguiente;
    }

    int pop()
    {
        if (isEmpty())
        {
            throw out_of_range("List is empty");
        }
        T *aux = ptr_principal;
        ptr_principal = ptr_principal->obtenerPtr_siguiente();
        size--;
        return aux->obtenerPonderacion();
    };

    void
    print()
    {

        T *ptr_indice = ptr_principal;

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

    void agregarArista(int origen, int destino, int ponderacion)
    {
        aristasAsociadas.pushArista(origen, destino, ponderacion);
    }

    Arista *obtenerPrimeraArista()
    {
        return aristasAsociadas.obtenerPtrPrincipal();
    }

    Arista *imprimirAristas() { aristasAsociadas.print(); }
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
    ListE<Hechizo> hechizosLegales;
    ListE<Hechizo> hechizosIlegales;
    char runaElemental;
    int IDHechizo;
    int numVertices;
    int numAristas;
    string cadenaTipos;

public:
    // Se crea el grafo con un número determinado de vértices.
    Hechizo(int IDHechizo, Hechicero hechicero, int numVertices, string cadenaDeTipos, int numAristas, int **aristas)
    {
        this->cadenaTipos = cadenaDeTipos;
        this->runaElemental = ' ';
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
            // 6,1,2
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

    ListE<Hechizo> obtenerHechizosLegales()
    {
        while (hechizosLegales.obtenerPtrPrincipal() != nullptr)
        {
            /* code */
        }
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

    void print()
    {
        cout << "Hechicero: " << hechicero.obtenerNombreCompleto() << "\n";
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
            // 6,1,2
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
    cout << "Hechizo: " << hechizo->obtenerID() << endl;
    cout << "Hechicero: " << hechizo->obtenerHechicero().obtenerNombreCompleto() << endl;
    cout << "  ";
    for (int i = 0; i < numVertices; i++)
    {
        cout << " " << static_cast<char>(vertices[i].obtenerTipo()) << "  ";
    }
    cout << endl;

    for (int i = 0; i < numVertices; i++)
    {
        cout << static_cast<char>(vertices[i].obtenerTipo()) << " ";
        for (int j = 0; j < numVertices; j++)
        {
            cout << "[" << matriz[i][j] << "]" << " ";
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
};

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
        {
            hechizo->setRunaElemental(c);
            contadorI++;
            if (contadorI > 3 || (contadorI > 0 && (contadorQ + contadorT + contadorV + contadorL + contadorO) > 0))
            {
                return false;
            }

            break;
        }
        case 'Q':
        {
            hechizo->setRunaElemental(c);
            contadorQ++;
            if (contadorQ > 3 || (contadorQ > 0 && (contadorI + contadorT + contadorV + contadorL + contadorO) > 0))
            {
                return false;
            }
            break;
        }
        case 'T':
        {
            hechizo->setRunaElemental(c);
            contadorT++;
            if (contadorT > 3 || (contadorT > 0 && (contadorQ + contadorI + contadorV + contadorL + contadorO) > 0))
            {
                return false;
            }
            break;
        }
        case 'V':
        {
            hechizo->setRunaElemental(c);
            contadorV++;
            if (contadorV > 3 || (contadorV > 0 && (contadorQ + contadorT + contadorI + contadorL + contadorO) > 0))
            {
                return false;
            }
            break;
        }
        case 'L':
        {
            hechizo->setRunaElemental(c);
            contadorL++;
            if (contadorL > 3 || (contadorL > 0 && (contadorQ + contadorT + contadorV + contadorI + contadorO) > 0))
            {
                return false;
            }
            break;
        }
        case 'O':
        {
            hechizo->setRunaElemental(c);
            contadorO++;
            if (contadorO > 3 || (contadorO > 0 && (contadorQ + contadorT + contadorV + contadorL + contadorI) > 0))
            {
                return false;
            }
            break;
        }
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
            if (matriz[i][j] != 0 && (hechizo->obtenerTiposVertices()[j] == 'T' || 'I' || 'Q' || 'V' || 'L' || 'O' && hechizo->obtenerTiposVertices()[i] == 'D'))
            {
                return false;
            }
        }
    }
    return true;
}

// Función que recorre el grafo en búsqueda de ciclos.
// Retorna el número de aristas del ciclo más largo encontrado o -1 si no hay ciclos.
int cicloDfs(int **matriz, int numVertices, int inicio, int actual, bool *visitado, int cuenta)
{
    int maxCiclo = -1; // Inicia en -1 para indicar que, si no se encuentra ciclo, se retorna -1 <sup data-citation="1" className="inline select-none [&>a]:rounded-2xl [&>a]:border [&>a]:px-1.5 [&>a]:py-0.5 [&>a]:transition-colors shadow [&>a]:bg-ds-bg-subtle [&>a]:text-xs [&>svg]:w-4 [&>svg]:h-4 relative -top-[2px] citation-shimmer"><a href="https://www.techiedelight.com/es/depth-first-search/">1</a></sup><sup data-citation="6" className="inline select-none [&>a]:rounded-2xl [&>a]:border [&>a]:px-1.5 [&>a]:py-0.5 [&>a]:transition-colors shadow [&>a]:bg-ds-bg-subtle [&>a]:text-xs [&>svg]:w-4 [&>svg]:h-4 relative -top-[2px] citation-shimmer"><a href="https://trspos.com/dfs-complejidad-del-tiempo/">6</a></sup>.

    // Recorrer todos los posibles vecinos
    for (int vecino = 0; vecino < numVertices; vecino++)
    {
        // Si existe una arista del vértice actual al vecino
        if (matriz[actual][vecino] != 0)
        {
            // Si se regresa al vértice de inicio y se ha recorrido al menos 1 arista
            if (vecino == inicio && cuenta >= 1)
            {
                int cicloActual = cuenta + 1; // Contamos la arista final que cierra el ciclo
                if (cicloActual > maxCiclo)
                {
                    maxCiclo = cicloActual;
                }
            }
            // Si el vecino no ha sido visitado, continuar la búsqueda recursivamente
            else if (!visitado[vecino])
            {
                visitado[vecino] = true;
                int cicloEncontrado = cicloDfs(matriz, numVertices, inicio, vecino, visitado, cuenta + 1);
                if (cicloEncontrado > maxCiclo)
                {
                    maxCiclo = cicloEncontrado;
                }
                visitado[vecino] = false; // Backtracking para explorar otras rutas
            }
        }
    }
    return maxCiclo;
}

// Regla 5
int cicloMasLargo(Hechizo *hechizo, int **matriz)
{
    int numVertices = hechizo->obtenerNumVertices();
    int maxCicloGlobal = 0; // Inicialmente no se ha encontrado ningún ciclo

    // Crear y inicializar arreglo visitado para todos los vértices
    bool *visitado = new bool[numVertices];
    for (int i = 0; i < numVertices; i++)
    {
        visitado[i] = false;
    }

    // Probar cada vértice como punto de inicio
    for (int i = 0; i < numVertices; i++)
    {
        visitado[i] = true;
        int cicloDesdeI = cicloDfs(matriz, numVertices, i, i, visitado, 0);
        if (cicloDesdeI > maxCicloGlobal)
        {
            maxCicloGlobal = cicloDesdeI;
        }
        visitado[i] = false;
    }

    delete[] visitado;
    return maxCicloGlobal;
}

nodoHechizero *buscarOAgregarHechicero(const std::string &nombre)
{
    nodoHechizero *actual = listaHechizeros;
    nodoHechizero *anterior = nullptr;

    // Buscar el hechicero en la lista
    while (actual != nullptr)
    {
        if (actual->nombre == nombre)
        {
            return actual; // Si lo encuentra, retorna el nodo
        }
        anterior = actual;
        actual = actual->next;
    }

    // Si no está en la lista, agregar un nuevo nodo
    nodoHechizero *nuevo = new nodoHechizero;
    nuevo->nombre = nombre;
    nuevo->cantHIlegales = 0;
    nuevo->next = nullptr;

    if (anterior == nullptr)
    { // Lista vacía
        listaHechizeros = nuevo;
    }
    else
    { // Añadir al final de la lista
        anterior->next = nuevo;
    }

    return nuevo;
}

bool detectarHechizoArcante(Hechizo *hechizo)
{
    string tipos = hechizo->obtenerTiposVertices();
    for (int i = 0; i < hechizo->obtenerNumVertices(); i++)
    {
        if (tipos[i] == 'I' || tipos[i] == 'Q' || tipos[i] == 'T' || tipos[i] == 'V' || tipos[i] == 'L' || tipos[i] == 'O' || tipos[i] == 'D' || tipos[i] == 'F')
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
        // Si existe arista de 'actual' a 'v' y el vértice 'v' no ha sido visitado
        if (matriz[actual][v] != 0 && !visitado[v])
        {
            int nuevoPeso = acumulado + matriz[actual][v];
            // Si el vértice 'v' es un endpoint (runa catalítica o de estabilidad: 'D' o 'F')
            if (tipos[v] == 'D' || tipos[v] == 'F')
            {
                if (nuevoPeso > maxPeso)
                {
                    maxPeso = nuevoPeso;
                }
            }
            visitado[v] = true;
            dfsCaminoPesado(matriz, numVertices, v, visitado, nuevoPeso, tipos, maxPeso);
            visitado[v] = false; // Backtracking
        }
    }
}

string caminoMasPesado(Hechizo *hechizo, int **matriz)
{
    int numVertices = hechizo->obtenerNumVertices();
    string tipos = hechizo->obtenerTiposVertices();
    int maxPeso = 0;

    if (!detectarHechizoArcante(hechizo))
    {

        return "Arcante";
    }
    else
    {
        // Crear arreglo dinámico para marcar visitados.
        bool *visitado = new bool[numVertices];
        for (int i = 0; i < numVertices; i++)
        {
            visitado[i] = false;
        }

        // Probar cada vértice como punto de partida si es 'A' (punto de confluencia).
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
        {
            return "modicum ";
        }
        else if (maxPeso < cicloMasLargo(hechizo, matriz))
        {
            return "maximus ";
        }
    }
}
bool legalidad(Hechizo *hechizo, int **matriz)
{
    if (cuantasRunas(hechizo) == false)
    {
        return false;
    }
    if (cuantasA(hechizo) == false)
    {
        return false;
    }
    if (distribucionCorrecta(hechizo, matriz) == false)
    {
        return false;
    }

    if (adyacenciaDeRunas(hechizo, matriz) == false)
    {
        return false;
    }

    if (cicloMasLargo(hechizo, matriz) % 2 != 0)
    {
        return false;
    }
    return true;
}
// Regla 6

// Regla 7
void esSospechozo(Hechizo *hechizo, int **matriz)
{
    string nombreH = hechizo->obtenerHechicero().obtenerNombreCompleto();
    nodoHechizero *nodo = buscarOAgregarHechicero(nombreH);
    if (legalidad(hechizo, matriz) == false)
    {
        nodo->cantHIlegales++;
    }
    if (nodo->cantHIlegales >= 3)
    {
        ofstream archivo2("underInvestigation.in", ios::app);
        if (archivo2.is_open())
        {
            archivo2 << nodo->nombre << endl;
            archivo2.close();
        }
    }
}
// Regla 8
string nombramientoDeHechizo(Hechizo *hechizo, int **matriz)
{
    string nombreHechizo = "";
    string apellidoHechicero = hechizo->obtenerHechicero().obtenerApellido();
    char ultimo = apellidoHechicero[apellidoHechicero.size() - 1];
    char runa = hechizo->obtenerRunaElemental();
    string sufijo;
    if (runa == 'I')
        nombreHechizo = "Ignatum";
    else if (runa == 'Q')
        nombreHechizo = "Aquos";
    else if (runa == 'T')
        nombreHechizo = "Terraminium";
    else if (runa == 'V')
        nombreHechizo = "Ventus";
    else if (runa == 'L')
        nombreHechizo = "Lux";
    else if (runa == 'O')
        nombreHechizo = "Tenebrae";

    if (ultimo == 'a' || ultimo == 'e' || ultimo == 'i' || ultimo == 'o' || ultimo == 'u')
        sufijo = apellidoHechicero.substr(0, apellidoHechicero.size() - 1) + "ium";
    else
        sufijo = apellidoHechicero + "um";

    string indicador = caminoMasPesado(hechizo, matriz);

    return nombreHechizo + " " + sufijo + " " + indicador;
};

void liberarLista(nodoHechizero *&listaHechizeros)
{
    while (listaHechizeros != nullptr)
    {
        nodoHechizero *temp = listaHechizeros;
        listaHechizeros = listaHechizeros->next;
        delete temp;
    }
}

void salida()
{
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
        Hechizo *hechizo1 = new Hechizo(i + 1, hechizo.nombreMago, hechizo.numVertices, hechizo.tiposVertices, hechizo.numAristas, hechizo.arrAristas);
        // hechizo1->print();
        int **matriz = matrizAdyacencia(hechizo1);

        legalidad(hechizo1, matriz);
        cout << nombramientoDeHechizo(hechizo1, matriz) << endl;
        hechizo1->setNombre(nombramientoDeHechizo(hechizo1, matriz));

        if(!legalidad){

        }
        for (int i = 0; i < hechizo.numVertices; i++)
        {
            delete[] matriz[i]; // Libera cada fila
        }
        delete[] matriz;
        //  Liberar la instancia del Hechizo (que a su vez libera el arreglo de vértices)
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
    // Liberar la memoria asignada para cada arreglo de aristas
    for (int i = 0; i < hechizo.numAristas; i++)
    {
        delete[] hechizo.arrAristas[i];
    }
    // Liberar el arreglo de punteros
    delete[] hechizo.arrAristas;

    liberarLista(listaHechizeros);

    return 0;
}