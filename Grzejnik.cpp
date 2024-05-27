#include <stdexcept>
#include "Grzejnik.h"

//stworzenie obiektu grzejnik z podaniem mocy nominalnej i aktualnej
Grzejnik::Grzejnik(float moc_nominalna, float moc_aktualna) {
    //zabezpieczenie przed podaniem wartości mocy aktualnej spoza przedziału 0 - 1 (procent mocy)
    if(0 > moc_aktualna > 1)
        throw std::invalid_argument("Moc aktualna musi być w przedziale 0 - 1");

    this->moc_aktualna = moc_aktualna;
    this->moc_nominalna = moc_nominalna;
}
//metoda ustawiająca moc aktualną
Grzejnik& Grzejnik::SetMocAktualna(float tMoc) {
    //zabezpieczenie przed uzyskaniem/przyjęciem wartości mocy aktualnej spoza przedziału 0 - 1 (procent mocy)
    if(0 > tMoc > 1)
        throw std::invalid_argument("Moc aktualna musi być w przedziale 0 - 1");

    this->moc_aktualna = tMoc;
    return *this;
}