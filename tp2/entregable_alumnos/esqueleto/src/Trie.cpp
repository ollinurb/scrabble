#include "Trie.h"

Trie::Trie() : _raiz(new Nodo()) {};
/*
Trie::~Trie(){
    destruirNodos(_raiz);
}

void Trie::destruirNodos(Trie::Nodo *& raiz) {
    for(int i = 0; i < TAMANIO_ALFABETO; i++){
        if(raiz->hijos[i] != nullptr){
            destruirNodos(raiz->hijos[i]);
        }
    }
    delete(raiz);
}
*/
void Trie::agregar(const Palabra p){
    Nodo* actual = _raiz;
    for (int i = 0; i < p.size(); i++){
        if (actual->hijos[ord(p[i])] == nullptr) {
            actual->hijos[ord(p[i])] = new Nodo();
        }
        actual = actual->hijos[ord(p[i])];
    }
    actual->finPalabra = true;
}

const bool Trie::pertenece(Palabra p) const{
    Nodo* nodoActual = _raiz;
    int i = 0;
    while(i < p.size() && nodoActual->hijos[ord(p[i])] != nullptr){
        nodoActual = nodoActual->hijos[ord(p[i])];
        i++;
    }
    return (i == p.size() && nodoActual->finPalabra);
}