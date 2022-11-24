#include "Trie.h"

Trie::Trie() : _raiz(new Nodo()){};

Trie::~Trie() {
    destruirNodos(_raiz);

}

void Trie::destruirNodos(Trie::Nodo* &raiz) {
    for(int i = 0; i < raiz->hijos.size(); i++){
        if (raiz->hijos[i] != nullptr) {
            destruirNodos(raiz->hijos[i]);
        }
    }
    delete(raiz);
}


void Trie::agregar(const Palabra p){
    Nodo* actual = _raiz;
    for (char c : p){
        if (actual->hijos[ord(c)] == nullptr) {
            actual->hijos[ord(c)] = new Nodo();
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