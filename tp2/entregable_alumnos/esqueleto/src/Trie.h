#ifndef TP_JUEGODEPALABRAS_TRIE_H
#define TP_JUEGODEPALABRAS_TRIE_H
#include "Tipos.h"
#include "Letra.h"

class Trie {
public:
    Trie();
    ~Trie();
    Trie(const Trie& aCopiar);
    void agregar(const Palabra);
    bool pertenece(Palabra) const;
    Trie& operator=(const Trie& d);
private:
    struct Nodo {
        vector<Nodo*> hijos;
        bool finPalabra;
        Nodo(): hijos(TAMANIO_ALFABETO, nullptr), finPalabra(false) {}
    };
    void copiarNodos(Trie::Nodo *& raiz, const Trie::Nodo* raiz_a_copiar);
    void destruirNodos(Nodo* &);
    Nodo* _raiz;
};


#endif //TP_JUEGODEPALABRAS_TRIE_H
