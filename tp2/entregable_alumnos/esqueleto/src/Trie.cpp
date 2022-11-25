#include "Trie.h"

Trie::Trie() : _raiz(new Trie::Nodo()){
    cout << "creo un nuevo trie en " << _raiz << endl;
}

Trie::~Trie() {
    cout << "borro el trie " << _raiz << endl;
    destruirNodos(_raiz);
}


Trie& Trie::operator=(const Trie& d) {
    // COMPLETAR
    cout << "copio el trie" << endl;
    destruirNodos(_raiz); //limpias el arbol
    _raiz = new Nodo();
    copiarNodos(_raiz, d._raiz);
    return *this;
}

void Trie::copiarNodos(Trie::Nodo *& raiz, const Trie::Nodo* raiz_a_copiar) {
    raiz->finPalabra = raiz_a_copiar->finPalabra;

    for(int i = 0; i < TAMANIO_ALFABETO; i++){
        if(raiz_a_copiar->hijos[i] != nullptr){
            raiz->hijos[i] = new Nodo();
            copiarNodos(raiz->hijos[i], raiz_a_copiar->hijos[i]);
        }
    }
}


Trie::Trie(const Trie& aCopiar) : Trie() { *this = aCopiar; }


void Trie::destruirNodos(Trie::Nodo* &raiz) {
    for(int i = 0; i < TAMANIO_ALFABETO; i++){
        if (raiz->hijos.size() != 0 && raiz->hijos[i] != nullptr) {
            destruirNodos(raiz->hijos[i]);
            //raiz->hijos[i] = nullptr;
        }
    }
    delete raiz;
}

void Trie::agregar(const Palabra p){
    Nodo* actual = _raiz;
    for (char c : p){
        if (actual->hijos[ord(c)] == nullptr) {
            actual->hijos[ord(c)] = new Trie::Nodo();
        }
        actual = actual->hijos[ord(c)];
    }
    actual->finPalabra = true;
}

bool Trie::pertenece(Palabra p) const{
    Nodo* nodoActual = _raiz;
    int i = 0;
    while(i < p.size() && nodoActual->hijos[ord(p[i])] != nullptr){
        nodoActual = nodoActual->hijos[ord(p[i])];
        i++;
    }
    return (i == p.size() && nodoActual->finPalabra);
}