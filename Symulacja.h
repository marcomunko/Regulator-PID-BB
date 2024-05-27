#ifndef REGULATOR_SYMULACJA_H
#define REGULATOR_SYMULACJA_H

#include "Pomieszczenie.h"
#include "Grzejnik.h"
#include "Regulator.h"


class Symulacja {
private:
    //agregacja całkowita
    Pomieszczenie* pomieszczenie;
    Grzejnik* grzejnik;
    Regulator* regulator;
public:
    //konstruktor
    Symulacja();
    //destruktor
    ~Symulacja();
    //konstruktor kopiujący
    Symulacja& operator=(const Symulacja& other)= delete;

    void iteracja(float dt);
    void przebieg(int probki, float dt);
    void zapis(const char* str);
    void startuj();

};


#endif //REGULATOR_SYMULACJA_H
