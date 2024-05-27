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
//defaltowe ustawienia przypisywane jeżeli damy ENTER
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
#define MOC_MAX 1.0f

//menu wybieranie przez urzytkownika nastawów oraz pozostałych danych
void Symulacja::startuj() {
    std::cout << "Wybierz regulator (PID - P, BB - B): ";
    char c;

    while(std::cin >> c && std::toupper(c) != 'P' && std::toupper(c) != 'B'){
        std::cin.ignore(256, '\n');
        std::cout << "Niepoprawny wybór. (PID - P, BB - B): ";
    }
    std::cin.ignore();

    float temp_zadana;
    //jeżeli temp_zadana = 0 to domyślna wartość
    if((temp_zadana = Utils::floatUżytkownika("Podaj temperaturę zadaną(domyślna - ENTER): ", false, 0)) == 0){
        std::cout << "Ustawiam domyślną temperaturę zadaną: " << TEMP_ZADANA << "°C" << std::endl;
        temp_zadana = TEMP_ZADANA;
    }

    float moc_nominalna;
    if((moc_nominalna = Utils::floatUżytkownika("Podaj moc nominalną (domyślna - ENTER): ", false, 0)) == 0){
        std::cout << "Ustawiam domyślną moc nominalną: " << MOC_NOMINALNA << " W" << std::endl;
        moc_nominalna = MOC_NOMINALNA;
    }

    //przypisujemy wartości nawet jak PID ale nie używamy
    float moc_max;
    if(c == 'B' && (moc_max = Utils::floatUżytkownika("Podaj moc maksymalną z przedziału 0 - 1 (domyślna - ENTER): ", false, 0)) == 0){
        std::cout << "Ustawiam domyślną moc maksymalną: " << MOC_MAX << std::endl;
        moc_max = MOC_MAX;
    }

    float szerokosc;
    if((szerokosc = Utils::floatUżytkownika("Podaj szerokość pomieszczenia (domyślna - ENTER): ", false, 0)) == 0){
        std::cout << "Ustawiam domyślną szerokość: " << SZEROKOSC << " m" << std::endl;
        szerokosc = SZEROKOSC;
    }

    float wysokosc;
    if(( wysokosc = Utils::floatUżytkownika("Podaj wysokość pomieszczenia (domyślna - ENTER): ", false, 0)) == 0){
        std::cout << "Ustawiam domyślną wysokość: " << WYSOKOSC << " m" << std::endl;
        wysokosc = WYSOKOSC;
    }

    float glebokosc;
    if((glebokosc = Utils::floatUżytkownika("Podaj głębokość pomieszczenia (domyślna - ENTER): ", false, 0)) == 0){
        std::cout << "Ustawiam domyślną głębokość: " << GLEBOKOSC << " m" << std::endl;
        glebokosc = GLEBOKOSC;
    }
//przypisujemy wartości nawet jak BB ale nie używamy
    float kp;
    if(c == 'P' && (kp = Utils::floatUżytkownika("Podaj kp (domyślna - ENTER): ", false, 0)) == 0){
        std::cout << "Ustawiam domyślną kp: " << KP << std::endl;
        kp = KP;
    }

    float ki;
    if(c == 'P' && (ki = Utils::floatUżytkownika("Podaj ki (domyślna - ENTER): ", false, 0)) == 0){
        std::cout << "Ustawiam domyślną ki: " << KI << std::endl;
        ki = KI;
    }

    float kd;
    if(c == 'P' && (kd = Utils::floatUżytkownika("Podaj kd (domyślna - ENTER): ", false, 0)) == 0){
        std::cout << "Ustawiam domyślną kd: " << KD << std::endl;
        kd = KD;
    }
    //utworzzenie obiekótów pomieszczenie, grzejnik (agregacja częściowa)
    this-> pomieszczenie = new Pomieszczenie(wysokosc, szerokosc, glebokosc);
    this-> grzejnik = new Grzejnik(moc_nominalna,0);
    //wybór regulatora
    if(c == 'P'){
        this->regulator = new RegulatorPID(temp_zadana, kp, ki, kd, pomieszczenie, grzejnik);
    } else {
        this->regulator = new RegulatorBB(temp_zadana, pomieszczenie, grzejnik,moc_max);
    }

    float czas;
    if((czas = Utils::floatUżytkownika("Podaj czas symulacji (domyślna - ENTER): ", false , 0)) == 0){
        std::cout << "Ustawiam domyślny czas symulacji: " << CZAS << " s" << std::endl;
        czas = CZAS;
    }

    float probkowanie = czas;
    while(probkowanie >= czas) {
        if ((probkowanie = Utils::floatUżytkownika("Podaj czas próbkowania (domyślna - ENTER): ", false, 0)) == 0) {
            std::cout << "Ustawiam domyślne czas próbkowania: " << PROBKOWANIE << " s" << std::endl;
            probkowanie = PROBKOWANIE;
        }
        if(probkowanie < czas) std::cout << "Czas próbkowania musi być mniejszy niż czas symulacji! \n";
    }

    przebieg(czas / probkowanie, probkowanie);
}
//konstruktor = inicjalizacja wskaźników na nullptr (agregacja całkowita) i (cześciowa)
Symulacja::Symulacja() {
    this->pomieszczenie = nullptr;
    this->grzejnik = nullptr;
    this->regulator = nullptr;
}

//destruktor = usówanie pomieszczenia i grzejnika (agregacja całkowita)
Symulacja::~Symulacja(){
    delete pomieszczenie;
    delete grzejnik;
}

//struktura wyników, które zostaną wypisane do pliku oraz do konsoli
struct Wyniki{
    const float czas;
    const float moc;
    const float temp;

    Wyniki(float czas, float moc, float temp) : czas(czas), moc(moc), temp(temp){};

    //przeciążony operator << wypisujący wyniki do konsoli
    friend std::ostream& operator<<(std::ostream& o, const Wyniki& w){
        return o << std::fixed << std::setprecision(PRECYZJA) << w.czas << ';' << w.temp << ';' << w.moc;
    }
};

//funkcja zamieniająca wektor na string
template<typename T>
//tworzymy strumień wyniki który jest ciągiem znaków z wektora
std::string vec_to_str(std::vector<T> const &v, std::string const &sep = " "){
    std::stringstream wynik;
    //dla każdego i w wektorze v dodajemy i do wyniki z separatorem
    for(auto &i : v){
        wynik << i << sep;
    }

    return wynik.str();
}


void Symulacja::przebieg(int probki, float dt) {
    std::vector<Wyniki> wyniki;

    for(int probka = 0; probka <= probki; probka++){
        //zabezpieczenie przed błędem
        try {
            if(probka > 0)
                iteracja(dt);
        } catch (const std::runtime_error& e) {
            std::cerr << "Błąd: " << e.what() << std::endl;
            return;  // Przerwij symulację, jeśli wystąpił błąd
        }

        wyniki.emplace_back((float)probka*dt, grzejnik->GetMoc(), pomieszczenie->getTemperatura());
    }
    //zamiana wyników na string
    auto w = vec_to_str(wyniki, "\n");
    //zamiana kropek na przecinki
    std::replace(w.begin(), w.end(), '.', ',');
    zapis(w.c_str());
}

//iteracja symulacji (jeden czas dt zmiana temperatury pomieszczenia)
void Symulacja::iteracja(float dt) {
    //zabezpieczenie przed błędem baraku inicjalizacji regulatora
    try {
        if(regulator){
            regulator->steruj(dt);
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Błąd inicjalizacji regulatora " << e.what() << std::endl;
        return;  // Przerwij iterację, jeśli wystąpił błąd
    }
    //aktualizuj = metoda pomieszczenia
    pomieszczenie->aktualizuj(dt);
}

//zapis wyników do pliku
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
