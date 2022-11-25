#ifndef TP_JUEGODEPALABRAS_VARIANTE_H
#define TP_JUEGODEPALABRAS_VARIANTE_H

#include "Tipos.h"
#include "tablero.h"
#include "Letra.h"
#include "Trie.h"

class variante {
public:
    variante(Nat, Nat, map<Letra, Nat>, set<Palabra>);
    variante& operator=(const variante& aCopiar);
    variante(const variante& aCopiar);
    const Nat tamanoTablero() const;
    const Nat cantidadFichas() const;
    Nat puntajeLetra(Letra) const;
    bool palabraLegitima(const Palabra&) const;
    Nat Lmax() const;

private:
    Nat _tamano;
    Nat _cantfichas;
    vector<Nat> _puntajesLetraVec;
    Trie _palabrasValidas;
    Nat _Lmax;
};


#endif //TP_JUEGODEPALABRAS_VARIANTE_H
