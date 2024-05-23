#include <stdexcept>
#include "Grzejnik.h"


Grzejnik::Grzejnik(float moc_nominalna, float moc_aktualna) {
    if(0 > moc_aktualna > 1)
        throw std::invalid_argument("Moc aktualna musi być w przedziale 0 - 1");

    this->moc_aktualna = moc_aktualna;
    this->moc_nominalna = moc_nominalna;
}

Grzejnik& Grzejnik::SetMocAktualna(float tMoc) {
    if(0 > tMoc > 1)
        throw std::invalid_argument("Moc aktualna musi być w przedziale 0 - 1");

    this->moc_aktualna = tMoc;
    return *this;
}