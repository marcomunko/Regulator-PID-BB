#ifndef REGULATOR_GRZEJNIK_H
#define REGULATOR_GRZEJNIK_H


class Grzejnik {
private:
    float moc_nominalna;
    float moc_aktualna; // od 0 do 1
public:
    //konstruktor defaultowy
    Grzejnik() = delete;
    //konstruktor przeciążony z parametrami
    Grzejnik(float moc_nominalna, float moc_aktualna);
    //destruktor
    ~Grzejnik() = default;
    //konstruktor kopiujący
    Grzejnik(const Grzejnik& other) = delete;
    //operator przypisania
    Grzejnik& operator=(const Grzejnik& other) = delete;

    //śmieci
    float GetMocNominalna() const {
        return moc_nominalna;
    };
    float GetMocAktualna() const{
        return moc_aktualna;
    };

    //metoda ustawiająca moc aktualną
    Grzejnik& SetMocAktualna(float tMoc);
    float GetMoc() {
        return moc_nominalna * moc_aktualna;
    }
};


#endif //REGULATOR_GRZEJNIK_H
