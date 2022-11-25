#include "servidor.h"

servidor::servidor(Nat n, const variante& v, Repositorio& r) : _juego(n, v, r), timeStamp(0), _esperados(n), _broadcast(),
                    _jugadores() {
    list<tuple<Notificacion, Nat>> listaVacia;
    auto tuplaNotif = make_tuple(listaVacia, 0);
    vector<tuple<list<tuple<Notificacion,Nat>>, Nat>> notifJugador(_esperados, tuplaNotif);
    _notifJugador = notifJugador;
}

IdCliente servidor::conectarCliente(){
    Nat id = _jugadores.size();
    _jugadores.push_back(id);
    get<0>(_notifJugador[id]).push_back(make_tuple(Notificacion::nuevaIdCliente(id), timeStamp));
    timeStamp++;
    if(_jugadores.size() == _esperados){
        _broadcast.push_back(make_tuple(Notificacion::nuevaEmpezar(_juego.varianteJuego().tamanoTablero()), timeStamp));
        timeStamp++;
        _broadcast.push_back(make_tuple(Notificacion::nuevaTurnoDe(0),timeStamp));
        timeStamp++;
        for(int i = 0; i < _esperados; i++){
            multiset<Letra> fichas;
            for (int j = 0; j < _juego.ultimaReposicion(i).size(); j++ ){
                fichas.insert(_juego.ultimaReposicion(i)[j]);
            };
            get<0>(_notifJugador[i]).push_back(make_tuple(Notificacion::nuevaReponer(fichas), timeStamp));
            timeStamp ++;
            fichas.clear();
        }
    }
    return id;
}

list<Notificacion> servidor::consultarNotificaciones(Nat c) {
    list<Notificacion> lista;
    while(!get<0>(_notifJugador[c]).empty() || get<1>(_notifJugador[c]) < _broadcast.size()) {
        // mientras haya notificaciones disponibles.
        if (get<0>(_notifJugador[c]).empty()) { // si el jugador solo tiene notifs generales
            lista.push_back(get<0>(_broadcast[get<1>(_notifJugador[c])]));
            get<1>(_notifJugador[c])++;
        }
        else if (get<1>(_notifJugador[c]) < _broadcast.size()) { // si tiene ambas notifs
            Nat timeStampJug = get<1>(get<0>(_notifJugador[c]).front());
            Nat timeStampBC = get<1>(_broadcast[get<1>(_notifJugador[c])]);
            if (timeStampJug < timeStampBC) {
                lista.push_back(get<0>(get<0>(_notifJugador[c]).front())); //La prox notif es del jugador.
                get<0>(_notifJugador[c]).pop_front();
            }
            else {//La prox notif es general.
                lista.push_back(get<0>(_broadcast[get<1>(_notifJugador[c])]));
                get<1>(_notifJugador[c])++;
            }
        }
        else { // si solo tiene notifs privadas
            lista.push_back(get<0>(get<0>(_notifJugador[c]).front())); //La prox notif es del jugador.
            get<0>(_notifJugador[c]).pop_front();
        }
    }
    return lista;
}

bool servidor::tieneLasFichas(Nat id, Ocurrencia o){
    bool res = true;
    map<Letra, Nat> cantApariciones;
    for(const auto& x : o){
        cantApariciones[get<2>(x)]++;
    }
    for(const auto& x : o){
        if(_juego.cuantasFichasTiene(id, get<2>(x)) < cantApariciones[get<2>(x)])
            res = false;
    }
    return res;
}

void servidor::recibirMensaje(Nat id,Ocurrencia o){
    if (tieneLasFichas(id,o) && _esperados == _jugadores.size() && _juego.turno() == id && _juego.jugadaValida(o)){
        Nat puntajeAntesUbicar = _juego.puntaje(id);
        _juego.ubicar(o);

        _broadcast.push_back(make_tuple(Notificacion::nuevaUbicar(id,o), timeStamp));
        timeStamp++;

        Nat puntajeActual = _juego.puntaje(id);

        _broadcast.push_back(make_tuple(Notificacion::nuevaSumaPuntos(id,puntajeActual - puntajeAntesUbicar), timeStamp));
        timeStamp++;

        multiset<Letra> fichas;

        for (Letra x : _juego.ultimaReposicion(id)){
            fichas.insert(x);
        }

        get<0>(_notifJugador[id]).push_back(make_tuple(Notificacion::nuevaReponer(fichas), timeStamp));
        timeStamp++;

        _broadcast.push_back(make_tuple(Notificacion::nuevaTurnoDe(id+1 % _esperados), timeStamp));
        timeStamp ++;
    }
    else {
        get<0>(_notifJugador[id]).push_back(make_tuple(Notificacion::nuevaMal(), timeStamp));
        timeStamp ++;
    }
}

const Nat servidor::esperados(){
    return _esperados;
}

const Nat servidor::conectados(){
    return _jugadores.size();
}

const class juego servidor::juego(){
    return _juego;
}