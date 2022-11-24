#include "juego.h"
#include "Tipos.h"
#include "variante.h"

juego::juego(const Nat k, const class variante& v, Repositorio& r) : _cantJugadores(k), _tablero(v.tamañoTablero()), _variante(v),
                                                                _repositorio(r), _turno(0), _ronda(0), _ultimaReposicionxJugador(k) {
    vector<Nat> puntaje(k, 0);
    _puntaje = puntaje;

    vector<vector<Nat>> fichasEnMano(k, vector<Nat>(TAMANIO_ALFABETO, 0));
    _fichasEnMano = fichasEnMano;
    int i = 0;
    while (i < k) {
        int j = 0;
        while (j < v.cantidadFichas()){
            _fichasEnMano[i][ord(_repositorio.front())] = _fichasEnMano[i][ord(_repositorio.front())] + 1;
            _ultimaReposicionxJugador[i].push_back(_repositorio.front());
            _repositorio.pop_front();
            j++;
        }
        i++;
    }
    vector<vector<vector<tuple<Letra, Nat, Nat, Nat>>>> ultimasFichasxJugador(k);
    _ultimasFichasxJugador = ultimasFichasxJugador;

}

const variante juego::varianteJuego(){
    return _variante;
}

const Nat juego::turno(){
    return _turno;
}

void juego::ubicar(const Ocurrencia &o){
    vector<tuple<Letra, Nat, Nat, Nat>> ocurs;
    _ultimaReposicionxJugador[_turno].clear();
    for (tuple<Nat, Nat, Letra> const &x : o) {
        ocurs.push_back(make_tuple(get<2>(x), _ronda, get<1>(x), get<0>(x)));
        _tablero.ponerLetra(get<0>(x), get<1>(x), get<2>(x), _ronda);
        // Le sacamos la ficha al jugador.
        _fichasEnMano[_turno][ord(get<2>(x))]--;
        // Le repone 1 ficha al jugador
        _ultimaReposicionxJugador[_turno].push_back(_repositorio.front());
        _fichasEnMano[_turno][ord(_repositorio.front())]++;
        _repositorio.pop_front();
    }
    _ultimasFichasxJugador[_turno].push_back(ocurs);
    _ronda++;
    _turno = (_turno + 1 ) % _cantJugadores;
}


const bool juego::jugadaValida(const Ocurrencia& ocurs){
    bool res = true;
    Nat Lmax = _variante.Lmax();
    if (Lmax < ocurs.size()) {
        res = false;
    }
    Palabra palabra;
    vector<tuple<Nat, Nat, Letra>> o;
    for(const auto& x : ocurs){
        o.push_back(make_tuple(get<1>(x), get<0>(x), get<2>(x)));
    }


    bool ocurrenciaComunVertical = true;
    bool ocurrenciaComunHorizontal = true;
    if(o.size() == 0){ //jugada valida por default
        return true;
    }

    Nat fila = get<0>(o[0]);
    Nat columna = get<1>(o[0]);
    Nat maxFila = fila;
    Nat maxColumna = columna;
    Nat minFila = fila;
    Nat minColumna = columna;

    /*
     * P A R A
     * O
     * R
     */

    for(const auto& ficha : o){
        if(get<0>(ficha) != fila) {
            ocurrenciaComunHorizontal = false;
        }
        if(get<1>(ficha) != columna) {
            ocurrenciaComunVertical = false;
        }
        if(get<0>(ficha) > maxFila || get<0>(ficha) < minFila){
            maxFila = (get<0>(ficha) > maxFila) ? get<0>(ficha) : maxFila;
            minFila = (get<0>(ficha) < minFila) ? get<0>(ficha) : minFila;
        }
        if(get<1>(ficha) > maxColumna || get<1>(ficha) < minColumna){
            maxColumna = (get<1>(ficha) > maxColumna) ? get<1>(ficha) : maxColumna;
            minColumna = (get<1>(ficha) < minColumna) ? get<1>(ficha) : minColumna;
        }
    }
    if(!ocurrenciaComunHorizontal && !ocurrenciaComunVertical) {
        res = false;
    }

    if((ocurrenciaComunVertical && (maxFila - minFila) > Lmax) || (ocurrenciaComunVertical && (maxColumna - minColumna) > Lmax)) {
        res = false;
    }

    else if(o.size() > 1 && res){ //chequea ocurrencia discontinua
        Nat espaciosTablero = 0;
        if(ocurrenciaComunVertical){
            for(Nat i = minFila; i <= maxFila; i++){
                if(!_tablero.hayLetra(i, get<1>(o[0]))) {
                    espaciosTablero++;
                }
            }
            if(espaciosTablero > o.size()) {
                res = false;
            }
        }
        else{
            for(Nat i = minColumna; i <= maxColumna; i++){
                if(!_tablero.hayLetra(get<0>(o[0]), i)) {
                    espaciosTablero++;
                }
            }
            res = espaciosTablero <= o.size();
        }
    }

    if (ocurrenciaComunHorizontal || o.size() == 1) {
        int j = 0;
        Nat f = get<0>(o[j]); // fila
        Nat c = get<1>(o[j]); // columna

        while (_tablero.enTablero(f, c - 1) && _tablero.hayLetra(f, c - 1) && res) { // Obtener primer letra de palabra horizontal.
            c--;
        }
        while (_tablero.enTablero(f, c) && (_tablero.hayLetra(f, c) || c == get<1>(o[j])) && res){ // Avanzamos hasta la última para conseguir la palabra horizontal.
            if (c == get<1>(o[j])) {
                palabra.push_back(get<2>(o[j]));
                j++;
            }
            else {
                palabra.push_back(get<0>(_tablero.letra(f, c)));
            }
            c++;
        }

        if (!_variante.palabraLegitima(palabra)){ //Chequear validez de palabra.
            res = false;
        }

        palabra.clear();

        Nat i = 0; //i-esima ficha de la jugada (jugada_actual[j])
        while(i < o.size() && res) {

            f = get<0>(o[i]);// fila
            c = get<1>(o[i]); // columna
            while (_tablero.hayLetra(f - 1, c) && _tablero.enTablero(f - 1, c)) {
                f--;
            }
            while (_tablero.hayLetra(f, c) && _tablero.enTablero(f, c) || f == get<0>(o[i])) {
                if (f == get<0>(o[i])){
                    palabra.push_back(get<2>(o[i]));
                } else {
                    palabra.push_back(get<0>(_tablero.letra(f,c)));
                }
                f++;
            }
            if(!_variante.palabraLegitima(palabra)){
                res = false;
            }
            palabra.clear();
            i++;
        }
    }
    else if(ocurrenciaComunVertical) {
        int j = 0;
        Nat f = get<0>(o[0]); // fila
        Nat c = get<1>(o[0]); // columna
        while (_tablero.hayLetra(f - 1, c) && _tablero.enTablero(f - 1, c)){ //busca comienzo ocurrencia comun vertical.
            f--;
        }

        while (_tablero.hayLetra(f, c) && _tablero.enTablero(f,c) || f == get<0>(o[j])){ //revisa ocurrencia comun vertical.
            if (f == get<0>(o[j])){
                palabra.push_back(get<2>(o[j]));
                j++;
            } else {
                palabra.push_back(get<0>(_tablero.letra(f,c)));
            }
            f++;
        }
        if (!_variante.palabraLegitima(palabra)){
            res = false;
        }
        palabra.clear();

        j = 0; // Esta j es para iterar por sobre las fichas de la jugada
        while(j < o.size() && res) {
            f = get<0>(o[j]); // fila
            c = get<1>(o[j]);// columna

            while(_tablero.hayLetra(f,c - 1) && _tablero.enTablero(f,c - 1)){
                c--;
            }

            while (_tablero.hayLetra(f, c) && _tablero.enTablero(f, c) || c == get<1>(o[j])){
                if (c == get<1>(o[j])){
                    palabra.push_back(get<2>(o[j]));
                } else {
                    palabra.push_back(get<0>(_tablero.letra(f,c)));
                }
                c++;
            }

            if (!_variante.palabraLegitima(palabra)){
                res = false;
            }
            palabra.clear();

            j++;
        }
    }
    return res;
}

const Nat juego::puntaje(IdCliente id){
    for(int i = 0; i < _ultimasFichasxJugador[id].size(); i++) { //iteras sobre las rondas/jugadas
        vector<tuple<Letra, Nat, Nat, Nat>> jugadaActual = _ultimasFichasxJugador[id][i];

        bool ocurrenciaComunVertical = (jugadaActual.size() > 1 && get<3>(jugadaActual[0]) == get<3>(jugadaActual[1]));
        bool ocurrenciaComunHorizontal = (jugadaActual.size() > 1 && get<2>(jugadaActual[0]) == get<2>(jugadaActual[1]));
        if (ocurrenciaComunHorizontal || jugadaActual.size() == 1) {
            Nat ronda = get<1>(jugadaActual[0]);
            Nat f = get<2>(jugadaActual[0]); // fila
            Nat c = get<3>(jugadaActual[0]); // columna
            _puntaje[id] += _variante.puntajeLetra(get<0>(jugadaActual[0]));
            while (_tablero.hayLetra(f, c - 1) && _tablero.enTablero(f, c - 1) && get<1>(_tablero.letra(f, c - 1)) <= ronda) {
                _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f, c - 1)));
                c--;
            }
            c = get<3>(jugadaActual[0]);
            while (_tablero.hayLetra(f, c + 1) && _tablero.enTablero(f, c + 1) && get<1>(_tablero.letra(f, c + 1)) <= ronda){
                _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f, c+1)));
                c++;
            }
            Nat j = 0; //j-esima ficha de la jugada (jugada_actual[j])
            while(j < jugadaActual.size()) {
                f = get<2>(jugadaActual[j]);// fila
                c = get<3>(jugadaActual[j]); // columna
                while (_tablero.hayLetra(f - 1, c) && _tablero.enTablero(f - 1, c) && get<1>(_tablero.letra(f - 1, c)) <= ronda) {
                    _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f - 1, c)));
                    f--;
                }

                f = get<2>(jugadaActual[j]); //se reinicia la fila
                while (_tablero.hayLetra(f + 1, c) && _tablero.enTablero(f + 1, c) && get<1>(_tablero.letra(f + 1, c)) <= ronda) {
                    _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f + 1, c)));
                    f++;
                }
                j++;
            }
        }

        else if(ocurrenciaComunVertical) {
            Nat ronda = get<1>(jugadaActual[0]);
            Nat f = get<2>(jugadaActual[0]); // fila
            Nat c = get<3>(jugadaActual[0]); // columna
            _puntaje[id] += _variante.puntajeLetra(get<0>(jugadaActual[0]));
            while (_tablero.hayLetra(f-1,c) && _tablero.enTablero(f - 1, c) && get<1>(_tablero.letra(f - 1, c)) <= ronda){
                    _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f - 1, c)));
                    f--;
            }
            f = get<2>(jugadaActual[0]); //reinicias la fila
            while (_tablero.hayLetra(f + 1, c) && _tablero.enTablero(f + 1,c) && get<1>(_tablero.letra(f + 1, c)) <= ronda){
                _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f + 1, c)));
                f++;
            }
            Nat j = 0; // Esta j es para iterar por sobre las fichas de la jugada
            while(j < jugadaActual.size()) {
                f = get<2>(jugadaActual[j]); // fila
                c = get<3>(jugadaActual[j]);// columna
                while (_tablero.hayLetra(f, c + 1) && _tablero.enTablero(f, c + 1) && get<1>(_tablero.letra(f, c + 1)) <= ronda){
                    _puntaje[id] += get<0>(_tablero.letra(f, c + 1));
                    c++;
                }
                c = get<3>(jugadaActual[j]);
                while(_tablero.hayLetra(f,c - 1) && _tablero.enTablero(f,c - 1) && get<1>(_tablero.letra(f, c - 1)) <= ronda){
                    _puntaje[id] += get<0>(_tablero.letra(f, c - 1));
                    c--;
                }
                j++;
            }
        }
        for(const auto& x : jugadaActual){
            _puntaje[id] += _variante.puntajeLetra(get<0>(x));
        }
    }
    _ultimasFichasxJugador[id].clear();
    return _puntaje[id];
}

const Letra juego::contenidoCasilla(Nat i, Nat j){
    return get<0>(_tablero.letra(i,j));
}

const Nat juego::cuantasFichasTiene(IdCliente id, Letra l){
    return _fichasEnMano[id][ord(l)];
}

const Repositorio juego::repositorio(){
    return _repositorio;
}

const vector<Letra> juego::ultimaReposicion(IdCliente id){
    return _ultimaReposicionxJugador[id];
}