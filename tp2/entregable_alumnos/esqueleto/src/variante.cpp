#include "variante.h"

variante::variante(Nat n, Nat fichas, map<Letra, Nat> puntajes, set<Palabra> palabras) : _puntajesLetraVec(TAMANIO_ALFABETO, 1),
                                                                                         _tamano(n), _cantfichas(fichas), _Lmax(0){
    for(auto & puntaje : puntajes){
        _puntajesLetraVec[ord(puntaje.first)] = puntaje.second;
    }
    for(auto & palabra : palabras){
        _palabrasValidas.agregar(palabra);
        if (palabra.size() > _Lmax){
            _Lmax = palabra.size();
        }
    }
}

variante& variante::operator=(const variante& aCopiar){
    cout << "copio el variante" << endl;
    _tamano = aCopiar.tamanoTablero();
    _cantfichas = aCopiar.cantidadFichas();
    _puntajesLetraVec = aCopiar._puntajesLetraVec;
    _palabrasValidas = aCopiar._palabrasValidas;
    _Lmax = aCopiar.Lmax();
}

variante::variante(const variante& aCopiar){
    cout << "creo variante por copia" << endl;
    *this = aCopiar;
}

const Nat variante::tamanoTablero() const {
    return _tamano;
}

const Nat variante::cantidadFichas() const{
    return _cantfichas;
}

Nat variante::puntajeLetra(Letra l) const{
    return _puntajesLetraVec[ord(l)];
}

bool variante::palabraLegitima(const Palabra &p) const {
    return _palabrasValidas.pertenece(p);
}

Nat variante::Lmax() const{
    return _Lmax;
}
