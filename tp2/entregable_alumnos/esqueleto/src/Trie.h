#ifndef TP_JUEGODEPALABRAS_TRIE_H
#define TP_JUEGODEPALABRAS_TRIE_H
#include "Tipos.h"
#include "Letra.h"

class Trie {
public:
    Trie();
    //~Trie();
    void agregar(const Palabra);
    const bool pertenece(Palabra) const;

private:
    struct Nodo {
        vector<Nodo*> hijos;
        bool finPalabra;

        Nodo(): hijos(TAMANIO_ALFABETO,nullptr), finPalabra(false) {}
    };

    Nodo* _raiz;
};


#endif //TP_JUEGODEPALABRAS_TRIE_H
