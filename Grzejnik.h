#ifndef REGULATOR_GRZEJNIK_H
#define REGULATOR_GRZEJNIK_H


class Grzejnik {
private:
    float moc_nominalna;
    float moc_aktualna; // od 0 do 1
public:
    Grzejnik() = delete;
    Grzejnik(float moc_nominalna, float moc_aktualna);

    float GetMocNominalna() const {
        return moc_nominalna;
    };
    float GetMocAktualna() const{
        return moc_aktualna;
    };
    Grzejnik& SetMocAktualna(float tMoc);
    float GetMoc() const {
        return moc_nominalna * moc_aktualna;
    }
};


#endif //REGULATOR_GRZEJNIK_H
