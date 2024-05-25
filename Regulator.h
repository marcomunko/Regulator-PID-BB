#ifndef REGULATOR_REGULATOR_H
#define REGULATOR_REGULATOR_H

#include "Pomieszczenie.h"
#include "Grzejnik.h"
#include <stdexcept>
#include <iostream>

class Regulator {
protected:
    Pomieszczenie* pomieszczenie;
    Grzejnik* grzejnik;
public:
    //konstruktor (ale i tak jest to abstrakcyjna więc nom)
    Regulator(){
        pomieszczenie = nullptr;
        grzejnik = nullptr;
    };
    //destruktor
    virtual ~Regulator(){
        delete pomieszczenie;
        delete grzejnik;
    };
    //konstruktor kopiujący
    Regulator(const Regulator& other) = delete;
    //operator przypisania
    Regulator& operator=(const Regulator& other) = delete;

    virtual float steruj(float temp_zmierzona, float dt) = 0;
    void steruj(float dt) {
        if (pomieszczenie == nullptr || grzejnik == nullptr) {
            throw std::runtime_error("Regulator nie jest prawidłowo skonfigurowany");
        }
        try {
            pomieszczenie->dodajCieplo(
                    grzejnik->SetMocAktualna(steruj(
                            pomieszczenie->getTemperatura(), dt
                    )).GetMoc()
            );
        } catch (std::exception &e){
            std::cerr << "Błąd " << e.what() << " Grzejnik ustawiany jest na maksymalną moc"<< std::endl;
            // Jeśli wystąpił błąd, ustaw grzejnik na pełną moc na daną iterację
            pomieszczenie->dodajCieplo(grzejnik->SetMocAktualna(1.0f).GetMoc());
        }
    }

    void UstawPomieszczenie(Pomieszczenie* _pom){
        delete this->pomieszczenie;

        this->pomieszczenie = _pom;
    }
    void UstawGrzejnik(Grzejnik* _grz){
        delete this->grzejnik;

        this->grzejnik = _grz;
    }
};

class RegulatorBB : public Regulator {
private:
    float temp_zadana;

public:
    RegulatorBB(float temp_zadana, Pomieszczenie* pom, Grzejnik* grz): temp_zadana(temp_zadana){
            UstawGrzejnik(grz);
            UstawPomieszczenie(pom);
    }
    ~RegulatorBB() override = default;
    RegulatorBB(const RegulatorBB& other) = delete;
    RegulatorBB& operator=(const RegulatorBB& other)= delete;

    float steruj (float temp_zmierzona, float dt) override {
        if (temp_zmierzona < temp_zadana) {
            return 1.0f; // grzej z pełną mocą
        } else {
            return 0.0f; // brak grzania
        }
    }
};

class RegulatorPID : public Regulator{
private:
    float temp_zadana;
    float sum_uchyb = 0;
    float poprzedni_uchyb = 0;
public:
    float kp; //4.0
    float ki; //0.02
    float kd; //0.1


    RegulatorPID(float temp_zadana, float kp, float ki, float kd,Pomieszczenie* pom, Grzejnik* grz):
    temp_zadana(temp_zadana), kp(kp), ki(ki), kd(kd) {
        UstawGrzejnik(grz);
        UstawPomieszczenie(pom);
    };
    ~RegulatorPID() override = default;
    RegulatorPID(const RegulatorPID& other) = delete;
    RegulatorPID& operator=(const RegulatorPID& other)= delete;

    float steruj(float temp_zmierzona, float dt) override {
        if (dt == 0) {
            throw std::invalid_argument("dt nie może być równy zero");
        }

        float e = temp_zadana - temp_zmierzona;
        auto u_p = kp * e;
        sum_uchyb += e * dt;

        auto u_i = ki * sum_uchyb;
        auto d_e = (e - poprzedni_uchyb) / dt;
        poprzedni_uchyb = e;

        auto u_d = kd * d_e;
        return u_p + u_i + u_d;
    }
};

#endif //REGULATOR_REGULATOR_H
