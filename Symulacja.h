#ifndef REGULATOR_SYMULACJA_H
#define REGULATOR_SYMULACJA_H

#include "Pomieszczenie.h"
#include "Grzejnik.h"
#include "Regulator.h"


class Symulacja {
private:
    Pomieszczenie* pomieszczenie;
    Grzejnik* grzejnik;
    Regulator* regulator;
public:
    Symulacja();
    ~Symulacja();
    Symulacja& operator=(const Symulacja& other)= delete;

    void iteracja(float dt);
    void przebieg(int probki, float dt);
    void zapis(const char* str);
    void startuj();

};


#endif //REGULATOR_SYMULACJA_H
