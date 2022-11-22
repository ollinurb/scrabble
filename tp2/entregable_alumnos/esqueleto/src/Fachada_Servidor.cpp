#include "Fachada_Servidor.h"

Fachada_Servidor::Fachada_Servidor(Nat cantJugadores, const Fachada_Variante& variante, Repositorio& r) :
                                    _servidor(servidor(cantJugadores, variante.nuestroVariante(), r)){};

IdCliente Fachada_Servidor::conectarCliente(){
    return _servidor.conectarCliente();
}

void Fachada_Servidor::recibirMensaje(IdCliente id, const Ocurrencia& o){
    _servidor.recibirMensaje(id, o);
}

Nat Fachada_Servidor::jugadoresEsperados(){
    return _servidor.esperados();
}

Nat Fachada_Servidor::jugadoresConectados(){
    return _servidor.conectados();
}

std::list<Notificacion> Fachada_Servidor::notificaciones(IdCliente id){
    return _servidor.consultarNotificaciones(id);
}

