#include "juego.h"
#include "Tipos.h"
#include "variante.h"

juego::juego(const Nat k, const class variante& v, Repositorio& r) : _cantJugadores(k), _tablero(v.tamanoTablero()),_repositorio(r), _turno(0), _ronda(0), _ultimaReposicionxJugador(k),_ultimasFichasxJugador(k),
        _variante(v) {
    vector<Nat> puntaje(k, 0);
    _puntaje = puntaje;
    vector<vector<Nat>> fichasEnMano(k, vector<Nat>(TAMANIO_ALFABETO, 0));
    _fichasEnMano = fichasEnMano;
    int i = 0;
    while (i < k) {
        int j = 0;
        while (j < v.cantidadFichas()){  //A cada jugador se le reparten tantas fichas como diga el variante
            _fichasEnMano[i][ord(_repositorio.front())] = _fichasEnMano[i][ord(_repositorio.front())] + 1;
            _ultimaReposicionxJugador[i].push_back(_repositorio.front());
            _repositorio.pop_front();
            j++;
        }
        i++;
    }
}

const variante& juego::varianteJuego(){
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

void juego::ocurrenciaDiscontinua(vector<tuple<Nat, Nat, Letra>> o, const bool &ocurrenciaComunVertical, const bool& ocurrenciaComunHorizontal, bool &res){
    Nat Lmax = _variante.Lmax();
    Nat fila = get<0>(o[0]);
    Nat columna = get<1>(o[0]);
    Nat maxFila = fila;
    Nat maxColumna = columna;
    Nat minFila = fila;
    Nat minColumna = columna;

    for(const auto& ficha : o){ //obtenemos el menor y mayor indice horizontal y vertical de la ocurrencia
        if(get<0>(ficha) > maxFila || get<0>(ficha) < minFila){
            maxFila = (get<0>(ficha) > maxFila) ? get<0>(ficha) : maxFila;
            minFila = (get<0>(ficha) < minFila) ? get<0>(ficha) : minFila;
        }
        if(get<1>(ficha) > maxColumna || get<1>(ficha) < minColumna){
            maxColumna = (get<1>(ficha) > maxColumna) ? get<1>(ficha) : maxColumna;
            minColumna = (get<1>(ficha) < minColumna) ? get<1>(ficha) : minColumna;
        }
    }
    if(!ocurrenciaComunHorizontal && !ocurrenciaComunVertical) { //si es diagonal o estan separadas
        res = false;
    }

    if((ocurrenciaComunVertical && (maxFila - minFila) > Lmax) || (ocurrenciaComunVertical && (maxColumna - minColumna) > Lmax)) {
        res = false;
    }
//Si hay mas o la misma cantidad de espacios ('$') en el tablero que la longitud de la ocurrencia, es porque hay huecos.
    else if(o.size() > 1 && res){
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
            res = (espaciosTablero <= o.size());
        }
    }
}

tuple<bool, bool> ocurrenciaHorizontalOVertical(vector<tuple<Nat, Nat, Letra>> o){
    Nat fila = get<0>(o[0]);
    Nat columna = get<1>(o[0]);
    tuple<bool, bool> res = make_tuple(true, true);
    for(auto& x : o){
        if(get<0>(x) != fila)
            get<0>(res) = false;
        if(get<1>(x) != columna){
            get<1>(res) = false;
        }
    }
    return res; //el primer valor especifica si es horizontal, el segundo si es vertical
}

const bool juego::jugadaValida(const Ocurrencia& ocurs){
    bool res = true;
    Nat Lmax = _variante.Lmax();
    if (Lmax < ocurs.size()) { // si la ocurrencia es de mayor longitud que Lmax, ya sabemos que es invalida
        res = false;
    }
    Palabra palabra;
    vector<tuple<Nat, Nat, Letra>> o;
    for(const auto& x : ocurs){ // convertimos la ocurrencia a vector
        o.push_back(make_tuple(get<1>(x), get<0>(x), get<2>(x)));
    }
    bool ocurrenciaComunHorizontal;
    bool ocurrenciaComunVertical;

    if(o.empty()){ //jugada valida por default
        return true;
    }
    else{
        tuple<bool, bool> verticalUHorizontal = ocurrenciaHorizontalOVertical(o);
        ocurrenciaComunHorizontal = get<0>(verticalUHorizontal);
        ocurrenciaComunVertical = get<1>(verticalUHorizontal);
        ocurrenciaDiscontinua(o, ocurrenciaComunVertical, ocurrenciaComunHorizontal, res);
    }

    if (ocurrenciaComunHorizontal || o.size() == 1) {

        int j = 0;
        Nat f = get<0>(o[j]); // fila
        Nat c = get<1>(o[j]); // columna

        while (_tablero.enTablero(f, c - 1) && _tablero.hayLetra(f, c - 1) && res) { // Obtener primer letra de palabra horizontal que forma la ocurrencia
            c--;
        }
        while (_tablero.enTablero(f, c) && (_tablero.hayLetra(f, c)) || (j <o.size() && c == get<1>(o[j])) && res){ // Avanzamos hasta la última letra para conseguir la palabra
            if (c == get<1>(o[j])) {
                palabra.push_back(get<2>(o[j]));
                j++;
            }
            else {
                palabra.push_back(get<0>(_tablero.letra(f, c)));
            }
            c++;
        }

        if (!_variante.palabraLegitima(palabra)){ //Chequear validez de palabra horizontal que forma la ocurrencia
            res = false;
        }

        palabra.clear();

        Nat i = 0; //Chequear que cada palabra vertical que forma una ficha de la ocurrencia sea valida
        while(i < o.size() && res) {

            f = get<0>(o[i]);// fila
            c = get<1>(o[i]); // columna
            while (_tablero.hayLetra(f - 1, c) && _tablero.enTablero(f - 1, c)) {  //Obtener primer letra de palabra horizontal que contiene a o[i]
                f--;
            }
            while (_tablero.hayLetra(f, c) && _tablero.enTablero(f, c) || f == get<0>(o[i])) { //Avanzar hasta la ultima letra de la palabra
                if (f == get<0>(o[i])){
                    palabra.push_back(get<2>(o[i]));
                } else {
                    palabra.push_back(get<0>(_tablero.letra(f,c)));
                }
                f++;
            }
            if(!_variante.palabraLegitima(palabra)){   //Chequea que la palabra vertical que forma o[i] sea legitima
                res = false;
            }
            palabra.clear();
            i++;
        }
    }
    else if(ocurrenciaComunVertical) { //si la ocurrencia es vertical
        int j = 0;
        Nat f = get<0>(o[0]); // fila
        Nat c = get<1>(o[0]); // columna
        while (_tablero.hayLetra(f - 1, c) && _tablero.enTablero(f - 1, c) && res){ //busca comienzo de ocurrencia comun vertical.
            f--;
        }

        while (_tablero.hayLetra(f, c) && _tablero.enTablero(f,c) || f == get<0>(o[j]) && res){ //obtiene la palabra vertical.
            if (f == get<0>(o[j])){
                palabra.push_back(get<2>(o[j]));
                j++;
            } else {
                palabra.push_back(get<0>(_tablero.letra(f,c)));
            }
            f++;
        }
        if (!_variante.palabraLegitima(palabra)){ // chequea que sea una palabra legitima
            res = false;
        }
        palabra.clear();

        Nat i = 0; // Esta i es para iterar sobre las fichas de la jugada
        while(i < o.size() && res) { // para cada ficha de la ocurrencia, veo que palabras horizontales forma
            f = get<0>(o[i]); // fila
            c = get<1>(o[i]);// columna

            while(_tablero.hayLetra(f,c - 1) && _tablero.enTablero(f,c - 1)){ // voy a la primera letra de la palabra
                c--;
            }

            while (_tablero.hayLetra(f, c) && _tablero.enTablero(f, c) || c == get<1>(o[i])){ // obtengo la palabra
                if (c == get<1>(o[i])){
                    palabra.push_back(get<2>(o[i]));
                } else {
                    palabra.push_back(get<0>(_tablero.letra(f,c)));
                }
                c++;
            }

            if (!_variante.palabraLegitima(palabra)){ //chequeo validez de la palabra
                res = false;
            }
            palabra.clear();

            i++;
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
            _puntaje[id] += _variante.puntajeLetra(get<0>(jugadaActual[0])); //sumamos puntaje de la 1era letra. Se usa como pivot para sumar el resto de puntaje de la ocurrencia.
            while (_tablero.hayLetra(f, c - 1) && _tablero.enTablero(f, c - 1) && get<1>(_tablero.letra(f, c - 1)) <= ronda) {
                _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f, c - 1))); //sumamos letras previas a pivot.
                c--;
            }
            c = get<3>(jugadaActual[0]);
            while (_tablero.hayLetra(f, c + 1) && _tablero.enTablero(f, c + 1) && get<1>(_tablero.letra(f, c + 1)) <= ronda){
                _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f, c+1))); //suamos letras sigientes a pivot.
                c++;
            }
            Nat j = 0; //j-esima ficha de la jugada (jugada_actual[j])
            while(j < jugadaActual.size()) {
                f = get<2>(jugadaActual[j]);// fila
                c = get<3>(jugadaActual[j]); // columna
                while (_tablero.hayLetra(f - 1, c) && _tablero.enTablero(f - 1, c) && get<1>(_tablero.letra(f - 1, c)) <= ronda) {
                    _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f - 1, c)));  //sumamos valores previos a ocurrencia cruzada.
                    f--;
                }

                f = get<2>(jugadaActual[j]); //se reinicia la fila
                while (_tablero.hayLetra(f + 1, c) && _tablero.enTablero(f + 1, c) && get<1>(_tablero.letra(f + 1, c)) <= ronda) {
                    _puntaje[id] += _variante.puntajeLetra(get<0>(_tablero.letra(f + 1, c))); //sumamos valres posteriores a ocurrencia cruzada.
                    f++;
                }
                j++;
            }
        }

        else if(ocurrenciaComunVertical) { //analogo a ocurrenciaComunHorizontal
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