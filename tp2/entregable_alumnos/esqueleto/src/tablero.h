#ifndef TP_JUEGODEPALABRAS_TABLERO_H
#define TP_JUEGODEPALABRAS_TABLERO_H

#include "Tipos.h"

class tablero {
public:
    tablero(const Nat);
    const bool hayLetra(Nat, Nat) const;
    const Nat tamano() const;
    const tuple<Letra, Nat> letra(Nat, Nat) const;
    void ponerLetra(Nat, Nat, Letra, Nat);
    const bool enTablero(Nat, Nat) const;
private:
    vector<vector<tuple<Letra, Nat>>> _tablero;
};

#endif //TP_JUEGODEPALABRAS_TABLERO_H


