#include "juego.h"
#include "Tipos.h"
#include "variante.h"

juego::juego(Nat k, const class variante& v, Repositorio& r) : _cantJugadores(k), _tablero(v.tamañoTablero()), _variante(v),
                                                                _repositorio(r), _turno(0), _ronda(0) {
    vector<Nat> _puntaje(k, 0);

    vector<vector<Nat>> fichasEnMano(k, vector<Nat>(TAMANIO_ALFABETO, 0));
    _fichasEnMano = fichasEnMano;
    int i = 0;
    while (i < k) {
        int j = 0;
        while (j < v.cantidadFichas()){
            _fichasEnMano[i][ord(r.front())] = _fichasEnMano[i][ord(r.front())] + 1;
            r.pop_front();
            j++;
        }
        i++;
    }
    vector<vector<tuple<Letra, Nat, Nat, Nat>>> ultimasFichasxJugador(k);
    _ultimasFichasxJugador = ultimasFichasxJugador;
    vector<vector<Letra>> ultimaReposicionxJugador(k);
    _ultimaReposicionxJugador = ultimaReposicionxJugador;
}

const variante juego::variante(){
    return _variante;
}

const Nat juego::turno(){
    return _turno;
}

void juego::ubicar(const Ocurrencia &o){
    for (tuple<Nat, Nat, Letra> const &x : o) {
        _tablero.ponerLetra(get<0>(x), get<1>(x), get<2>(x), _ronda);
    }
}

bool juego::jugadaValida(const Ocurrencia& o){

}

const Nat juego::puntaje(IdCliente id){

}

const Letra juego::contenidoCasilla(Nat i, Nat j){

}

Nat juego::cuantasFichasTiene(IdCliente id, Letra l){

}

Repositorio juego::repositorio(){

}

vector<Letra> juego::ultimaReposicion(IdCliente id){

}