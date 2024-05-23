#ifndef REGULATOR_POMIESZCZENIE_H
#define REGULATOR_POMIESZCZENIE_H


#pragma once

class Pomieszczenie {
public:
    Pomieszczenie(float wysokosc, float szerokosc, float glebokosc);
    float getTemperatura() const { return temperaturaWewnetrzna; }
    float getCieploWchodzace() const { return cieploWchodzace; }
    void dodajCieplo(float cieplo);
    void aktualizuj(float timeStep=1.0);

private:
    float wysokosc;
    float szerokosc;
    float glebokosc;
    float powierzchniaScian;
    float masaPowietrza;
    float temperaturaWewnetrzna;
    float cieploWchodzace;
    const float cieploWlasciwe;
    const float gestoscPowietrza;
    const float gruboscScian;
    const float przewodnoscScian;
    const float temperaturaZewnetrzna;

    float utrataCiepla() const;
    float zmianaTemperatury(float cieplo, float czas=1.0) const;
};


#endif //REGULATOR_POMIESZCZENIE_H
