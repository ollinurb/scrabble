#ifndef TP_JUEGODEPALABRAS_SERVIDOR_H
#define TP_JUEGODEPALABRAS_SERVIDOR_H

#include "juego.h"
#include "Tipos.h"
#include "Notificacion.h"

class servidor {
public:
    servidor(Nat, const variante&, Repositorio&);
    IdCliente conectarCliente();
    list<Notificacion> consultarNotificaciones(Nat);
    void recibirMensaje(Nat,Ocurrencia);
    const Nat esperados();
    const Nat conectados();
    const class juego juego();
    bool tieneLasFichas(Nat, Ocurrencia);

private:
    Nat _esperados;
    class juego _juego;
    vector<IdCliente> _jugadores;
    Nat timeStamp;
    vector<tuple<list<tuple<Notificacion,Nat>>, Nat>> _notifJugador;
    vector<tuple<Notificacion, Nat>> _broadcast;
};


#endif //TP_JUEGODEPALABRAS_SERVIDOR_H
