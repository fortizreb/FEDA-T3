#include <iostream>
using namespace std;

struct Nodo {
    int clave;
    Nodo* izquierdo;
    Nodo* derecho;

    Nodo(int val) : clave(val), izquierdo(nullptr), derecho(nullptr) {}
};

class BST {
private:
    Nodo* raiz;

    Nodo* insertar(Nodo* nodo, int clave) {
        if (!nodo) return new Nodo(clave);
        if (clave < nodo->clave)
            nodo->izquierdo = insertar(nodo->izquierdo, clave);
        else if (clave > nodo->clave)
            nodo->derecho = insertar(nodo->derecho, clave);
        return nodo;
    }

    Nodo* buscar(Nodo* nodo, int clave) {
        if (!nodo || nodo->clave == clave)
            return nodo;
        if (clave < nodo->clave)
            return buscar(nodo->izquierdo, clave);
        return buscar(nodo->derecho, clave);
    }

    Nodo* encontrarMin(Nodo* nodo) {
        while (nodo && nodo->izquierdo)
            nodo = nodo->izquierdo;
        return nodo;
    }

    Nodo* eliminar(Nodo* nodo, int clave) {
        if (!nodo) return nullptr;
        if (clave < nodo->clave)
            nodo->izquierdo = eliminar(nodo->izquierdo, clave);
        else if (clave > nodo->clave)
            nodo->derecho = eliminar(nodo->derecho, clave);
        else {
            if (!nodo->izquierdo && !nodo->derecho) {
                delete nodo;
                return nullptr;
            }
            if (!nodo->izquierdo) {
                Nodo* temp = nodo->derecho;
                delete nodo;
                return temp;
            } 
            if (!nodo->derecho) {
                Nodo* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }
            Nodo* temp = encontrarMin(nodo->derecho);
            nodo->clave = temp->clave;
            nodo->derecho = eliminar(nodo->derecho, temp->clave);
        }
        return nodo;
    }

    void imprimirPreOrden(Nodo* nodo) {
        if (!nodo) return;
        cout << nodo->clave << " ";
        imprimirPreOrden(nodo->izquierdo);
        imprimirPreOrden(nodo->derecho);
    }

    void imprimirInOrden(Nodo* nodo) {
        if (!nodo) return;
        imprimirInOrden(nodo->izquierdo);
        cout << nodo->clave << " ";
        imprimirInOrden(nodo->derecho);
    }

    void imprimirPostOrden(Nodo* nodo) {
        if (!nodo) return;
        imprimirPostOrden(nodo->izquierdo);
        imprimirPostOrden(nodo->derecho);
        cout << nodo->clave << " ";
    }

    void destruirArbol(Nodo* nodo) {
        if (!nodo) return;
        destruirArbol(nodo->izquierdo);
        destruirArbol(nodo->derecho);
        delete nodo;
    }

public:
    BST() : raiz(nullptr) {}

    ~BST() {
        destruirArbol(raiz);
    }

    void agregar(int clave) {
        raiz = insertar(raiz, clave);
    }

    bool buscar(int clave) {
        return buscar(raiz, clave) != nullptr;
    }

    void eliminar(int clave) {
        raiz = eliminar(raiz, clave);
    }

    int min() {
        Nodo* nodo = encontrarMin(raiz);
        if (nodo) return nodo->clave;
        throw runtime_error("Arbol vacio");
    }

    int max() {
        Nodo* nodo = raiz;
        while (nodo && nodo->derecho)
            nodo = nodo->derecho;
        if (nodo) return nodo->clave;
        throw runtime_error("Arbol vacio");
    }

    void imprimirPreOrden() {
        imprimirPreOrden(raiz);
        cout << endl;
    }

    void imprimirInOrden() {
        imprimirInOrden(raiz);
        cout << endl;
    }

    void imprimirPostOrden() {
        imprimirPostOrden(raiz);
        cout << endl;
    }
};

int main() {
    BST arbol;

    // Ejemplo de uso
    arbol.agregar(50);
    arbol.agregar(30);
    arbol.agregar(70);
    arbol.agregar(20);
    arbol.agregar(40);
    arbol.agregar(60);
    arbol.agregar(80);

    cout << "In-Order: ";
    arbol.imprimirInOrden();

    cout << "Pre-Order: ";
    arbol.imprimirPreOrden();

    cout << "Post-Order: ";
    arbol.imprimirPostOrden();

    cout << "Minimo: " << arbol.min() << endl;
    cout << "Maximo: " << arbol.max() << endl;

    cout << "¿Existe 30? " << (arbol.buscar(30) ? "Si" : "No") << endl;

    arbol.eliminar(30);
    cout << "Despues de eliminar 30, In-Order: ";
    arbol.imprimirInOrden();

    return 0;
}
