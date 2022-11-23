#ifndef TP_JUEGODEPALABRAS_VARIANTE_H
#define TP_JUEGODEPALABRAS_VARIANTE_H

#include "Tipos.h"
#include "tablero.h"
#include "Letra.h"
#include "Trie.h"

class variante {
public:
    variante(Nat, Nat, map<Letra, Nat>, set<Palabra>);
    const Nat tamañoTablero() const;
    const Nat cantidadFichas() const;
    Nat puntajeLetra(Letra) const;
    const bool palabraLegitima(const Palabra) const;
    Nat Lmax() const;

private:
    Nat _tamaño;
    Nat _cantfichas;
    vector<Nat> _puntajesLetraVec;
    Trie _palabrasValidas;
    Nat _Lmax;
};


#endif //TP_JUEGODEPALABRAS_VARIANTE_H
