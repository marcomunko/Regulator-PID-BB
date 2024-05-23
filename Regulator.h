#ifndef REGULATOR_REGULATOR_H
#define REGULATOR_REGULATOR_H

#include "Pomieszczenie.h"
#include "Grzejnik.h"


class Regulator {
protected:
    Pomieszczenie* pomieszczenie;
    Grzejnik* grzejnik;
public:
    virtual float steruj(float temp_zmierzona, float dt) = 0;
    void steruj(float dt) {
        pomieszczenie->dodajCieplo(
                grzejnik->SetMocAktualna(steruj(
                        pomieszczenie->getTemperatura(), dt
                )).GetMoc()
        );
    }

    void UstawPomieszczenie(Pomieszczenie* _pom){
        this->pomieszczenie = _pom;
    }
    void UstawGrzejnik(Grzejnik* _grz){
        this->grzejnik = _grz;
    }
};

class RegulatorBB : public Regulator {
private:
    float temp_zadana;

public:
    RegulatorBB(float temp_zadana) : temp_zadana(temp_zadana) {}//niekorzystam
    RegulatorBB(float temp_zadana, Pomieszczenie* pom, Grzejnik* grz): temp_zadana(temp_zadana){
            UstawGrzejnik(grz);
            UstawPomieszczenie(pom);
    }
    ~RegulatorBB() = default;
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

    RegulatorPID(float temp_zadana, float kp, float ki, float kd)://niekorzystam
    temp_zadana(temp_zadana), kp(kp), ki(ki), kd(kd) {};

    RegulatorPID(float temp_zadana, float kp, float ki, float kd,Pomieszczenie* pom, Grzejnik* grz):
    temp_zadana(temp_zadana), kp(kp), ki(ki), kd(kd) {
        UstawGrzejnik(grz);
        UstawPomieszczenie(pom);
    };
    ~RegulatorPID() = default;
    RegulatorPID(const RegulatorPID& other) = delete;

    RegulatorPID& operator=(const RegulatorPID& other)= delete;

    float steruj(float temp_zmierzona, float dt) override {
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
