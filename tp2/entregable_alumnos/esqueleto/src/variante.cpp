#include "variante.h"

variante::variante(Nat n, Nat fichas, map<Letra, Nat> puntajes, set<Palabra> palabras) : _puntajesLetraVec(TAMANIO_ALFABETO, 1),
                                                                                            _tamaño(n), _cantfichas(fichas), _Lmax(0){

    for(auto & puntaje : puntajes){
        _puntajesLetraVec[ord(puntaje.first)] = puntaje.second;
    }
    for(const auto & palabra : palabras){
        _palabrasValidas.agregar(palabra);
        if (palabra.size() > _Lmax){
            _Lmax = palabra.size();
        }
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

Nat variante::Lmax() const{
    return _Lmax;
}
