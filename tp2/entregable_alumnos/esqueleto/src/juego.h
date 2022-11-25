#ifndef TP_JUEGODEPALABRAS_JUEGO_H
#define TP_JUEGODEPALABRAS_JUEGO_H

#include "Tipos.h"
#include "variante.h"
#include "tablero.h"

class juego {
public:
    juego(const Nat, const variante&, Repositorio&);

    const variante& varianteJuego();
    const Nat turno();
    void ubicar(const Ocurrencia&);
    const bool jugadaValida(const Ocurrencia&);
    const Nat puntaje(IdCliente);
    const Letra contenidoCasilla(Nat, Nat);
    const Nat cuantasFichasTiene(IdCliente, Letra);
    const Repositorio repositorio();
    const vector<Letra> ultimaReposicion(IdCliente);

private:
    tablero _tablero;
    class variante _variante;
    Nat _turno;
    vector<vector<Nat>> _fichasEnMano;
    vector<Nat> _puntaje;
    Repositorio _repositorio;
    vector<vector<vector<tuple<Letra, Nat, Nat, Nat>>>> _ultimasFichasxJugador;
    Nat _cantJugadores;
    Nat _ronda;
    vector<vector<Letra>> _ultimaReposicionxJugador;
    void ocurrenciaDiscontinua(vector<tuple<Nat, Nat, Letra>> o, const bool &ocurrenciaComunVertical, const bool& ocurrenciaComunHorizontal, bool& res);
};


#endif //TP_JUEGODEPALABRAS_JUEGO_H
