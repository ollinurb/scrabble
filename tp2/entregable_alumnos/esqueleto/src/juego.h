#ifndef TP_JUEGODEPALABRAS_JUEGO_H
#define TP_JUEGODEPALABRAS_JUEGO_H

#include "Tipos.h"
#include "variante.h"

class juego {
public:
    juego(Nat, const variante&, Repositorio&);

    juego(Nat k, const variante v, Repositorio r);

    const variante variante();
    const Nat turno();
    void ubicar(const Ocurrencia&);
    bool jugadaValida(const Ocurrencia& o);
    const Nat puntaje(IdCliente);
    const Letra contenidoCasilla(Nat, Nat);
    Nat cuantasFichasTiene(Nat, Letra);
    Repositorio repositorio();
    vector<Letra> ultimaReposicion(Nat);

private:
    tablero _tablero;
    class variante _variante;
    Nat _turno;
    vector<vector<Nat>> _fichasEnMano;
    vector<Nat> _puntaje;
    Repositorio _repositorio;
    vector<vector<tuple<Letra, Nat, Nat, Nat>>> _ultimasFichasxJugador;
    Nat _cantJugadores;
    Nat _ronda;
    vector<vector<Letra>> _ultimaReposicionxJugador;
};


#endif //TP_JUEGODEPALABRAS_JUEGO_H
