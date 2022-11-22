#include "servidor.h"

servidor::servidor(Nat n, const variante& v, Repositorio& r) : _juego(n, v, r), timeStamp(0), _esperados(n){
    _notifJugador(_esperados);
}

void servidor::conectarCliente(){
    Nat id = _jugadores.size();
    _jugadores.push_back(id);
    get<0>(_notifJugador[id]).push_back(make_tuple(Notificacion::nuevaIdCliente(id), timeStamp));
    timeStamp++;
    if(_jugadores.size() == _esperados){
        _broadcast.push_back(make_tuple(Notificacion::nuevaEmpezar(_juego.varianteJuego().tamañoTablero()),timeStamp));
        timeStamp++;
        _broadcast.push_back(make_tuple(Notificacion::nuevaTurnoDe(id),timeStamp));
        timeStamp++;
        }
}

void servidor::consultarNotificaciones(Nat c) {
    while(get<0>(_notifJugador[c]).empty() || get<1>(_notifJugador[c]) < _broadcast.size()) {
        // mientras haya notificaciones disponibles.
        if (get<0>(_notifJugador[c]).empty())
            get<1>(_notifJugador[c])++;

        else {
            Nat timeStampJug = get<1>(get<0>(_notifJugador[c]).front());
            Nat timeStampBC = get<1>(_broadcast[get<1>(_notifJugador[c])]);
            if (timeStampJug < timeStampBC) //La prox notif es del jugador.
                get<0>(_notifJugador[c]).pop_front();
            else //La prox notif es general.
                get<1>(_notifJugador[c])++;
        }
    }
}



void servidor::recibirMensaje(Nat id,Ocurrencia o){
    if (_juego.jugadaValida(o)){
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