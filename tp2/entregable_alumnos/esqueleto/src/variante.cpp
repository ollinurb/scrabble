#include "variante.h"

variante::variante(Nat n, Nat fichas, map<Letra, Nat> puntajes, set<Palabra> palabras){
    _tamaño = n;
    _cantfichas = fichas;
    vector<Nat> puntajesVec(TAMANIO_ALFABETO, 1);
    _puntajesLetraVec = puntajesVec;
    for(map<Letra, Nat>::iterator it = puntajes.begin(); it != puntajes.end(); ++it){
        _puntajesLetraVec[ord(it->first)] = it->second;
    }
    _palabrasValidas = Trie();
    for(set<Palabra>::iterator it = palabras.begin(); it != palabras.end(); ++it){
        _palabrasValidas.agregar(*it);
    }
}

const Nat variante::tamañoTablero() const {
    return _tamaño;
}

const Nat variante::cantidadFichas() const{
    return _cantfichas;
}

Nat variante::puntajeLetra(Letra l) const{
    return _puntajesLetraVec[ord(l)];
}

const bool variante::palabraLegitima(const Palabra p) const {
    return _palabrasValidas.pertenece(p);
}