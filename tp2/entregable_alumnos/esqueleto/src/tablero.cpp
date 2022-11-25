#include "tablero.h"

using casilla = tuple<Letra, Nat>;


tablero::tablero(const Nat n) {
    casilla casillaDefault= make_tuple( '$', 0);
    vector<casilla> fila(n, casillaDefault);
    vector<vector<casilla>> tab(n, fila);
    _tablero = tab;
};

const bool tablero::hayLetra(Nat i, Nat j) const{
    return (i < _tablero.size() && j < _tablero.size()) && get<0>(_tablero[i][j]) != '$';
}

const Nat tablero::tamano() const {
    return _tablero.size();
}

const tuple<Letra, Nat> tablero::letra(Nat i, Nat j) const{
    return _tablero[i][j];
}

void tablero::ponerLetra(Nat i, Nat j, Letra l, Nat r){
    _tablero[j][i] = make_tuple(l,r);
}

const bool tablero::enTablero(Nat i, Nat j) const{
    return (i < _tablero.size() && j < _tablero.size());
}