#include "Symulacja.h"
#include "Regulator.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Utils.h"

#define PRECYZJA 4

#define TEMP_ZADANA 23.0f
#define MOC_NOMINALNA 12000.0f
#define SZEROKOSC 2.5f
#define WYSOKOSC 2.0f
#define GLEBOKOSC 2.0f
#define KP 4.0f
#define KI 0.02f
#define KD 0.1f
#define CZAS 10.0f
#define PROBKOWANIE 0.1f


void Symulacja::startuj() {
    std::cout << "Wybierz regulator (PID - P, BB - B): ";
    char c;

    while(std::cin >> c && std::toupper(c) != 'P' && std::toupper(c) != 'B'){
        std::cin.ignore(256, '\n');
        std::cout << "Niepoprawny wybór. (PID - P, BB - B): ";
    }
    std::cin.ignore();

    float temp_zadana;
    if((temp_zadana = Utils::floatUżytkownika("Podaj temperaturę zadaną(domyślna - ENTER): ", false, 0)) == 0){
        temp_zadana = TEMP_ZADANA;
    }

    float moc_nominalna;
    if((moc_nominalna = Utils::floatUżytkownika("Podaj moc nominalną (domyślna - ENTER): ", false, 0)) == 0){
        moc_nominalna = MOC_NOMINALNA;
    }

    float szerokosc;
    if((szerokosc = Utils::floatUżytkownika("Podaj szerokość pomieszczenia (domyślna - ENTER): ", false, 0)) == 0){
        szerokosc = SZEROKOSC;
    }

    float wysokosc;
    if(( wysokosc = Utils::floatUżytkownika("Podaj wysokość pomieszczenia (domyślna - ENTER): ", false, 0)) == 0){
        wysokosc = WYSOKOSC;
    }

    float glebokosc;
    if((glebokosc = Utils::floatUżytkownika("Podaj głębokość pomieszczenia (domyślna - ENTER): ", false, 0)) == 0){
        glebokosc = GLEBOKOSC;
    }

    float kp;
    if((kp = Utils::floatUżytkownika("Podaj kp (domyślna - ENTER): ", false, 0)) == 0){
        kp = KP;
    }

    float ki;
    if((ki = Utils::floatUżytkownika("Podaj ki (domyślna - ENTER): ", false, 0)) == 0){
        ki = KI;
    }

    float kd;
    if((kd = Utils::floatUżytkownika("Podaj kd (domyślna - ENTER): ", false, 0)) == 0){
        kd = KD;
    }

    this-> pomieszczenie = new Pomieszczenie(wysokosc, szerokosc, glebokosc);
    this-> grzejnik = new Grzejnik(moc_nominalna,0);

    if(c == 'P'){
        this->regulator = new RegulatorPID(temp_zadana, kp, ki, kd, pomieszczenie, grzejnik);
    } else {
        this->regulator = new RegulatorBB(temp_zadana, pomieszczenie, grzejnik);
    }
    float czas;
    if((czas = Utils::floatUżytkownika("Podaj czas symulacji (domyślna - ENTER): ", false , 0)) == 0){
        czas = CZAS;
    }
    float probkowanie;
    if((probkowanie = Utils::floatUżytkownika("Podaj czas próbkowania (domyślna - ENTER): ", false, 0)) == 0){
        probkowanie = PROBKOWANIE;
    }

    przebieg(czas / probkowanie, probkowanie);
}

Symulacja::Symulacja() {
    this->pomieszczenie = nullptr;
    this->grzejnik = nullptr;
    this->regulator = nullptr;
}

Symulacja::~Symulacja(){
    delete pomieszczenie;
    delete grzejnik;
}

template<typename T>
std::string vec_to_str(std::vector<T> const &v, std::string const &sep = " "){
    std::stringstream wynik;
    for(auto &i : v){
        wynik << i << sep;
    }

    return wynik.str();
}

struct Wyniki{
    const float czas;
    const float moc;
    const float temp;

    Wyniki(float czas, float moc, float temp) : czas(czas), moc(moc), temp(temp){};

    friend std::ostream& operator<<(std::ostream& o, const Wyniki& w){
        return o << std::fixed << std::setprecision(PRECYZJA) << w.czas << ';' << w.temp << ';' << w.moc;
    }
};

void Symulacja::przebieg(int probki, float dt) {
    std::vector<Wyniki> wyniki;

    for(int probka = 0; probka <= probki; probka++){
        if(probka > 0)
            iteracja(dt);

        //if(probki / 2 == probka)
          //  grzejnik->SetMocAktualna(.2f);

        wyniki.emplace_back((float)probka*dt, grzejnik->GetMoc(), pomieszczenie->getTemperatura());
    }

    auto w = vec_to_str(wyniki, "\n");
    std::replace(w.begin(), w.end(), '.', ',');
    zapis(w.c_str());
}

void Symulacja::iteracja(float dt) {
    if(regulator){
        regulator->steruj(dt);
    }

    pomieszczenie->aktualizuj(dt);
}

void Symulacja::zapis(const char *str){
    std::ofstream plik;
    plik.open("wyniki.csv", std::ios::out);
    if(!plik.is_open()){
        throw std::runtime_error("Nie udało się otworzyć pliku do zapisu.");
    }
    plik << "czas;temperatura;moc\n";
    plik << str;
    plik.close();
    std::cout << str << std::endl;
}
