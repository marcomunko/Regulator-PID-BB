#ifndef REGULATOR_UTILS_H
#define REGULATOR_UTILS_H

#include <string>
#include <iostream>
#include <algorithm>

class Utils {
public:
    //sprawdzenie czy string jest intem (liczbą)
    static int IntUżytkownika(std::string wiadomość, bool oczekiwany = true, int zakresStart = -1, int zakresEnd = -1){
        std::cout << wiadomość;
        int n;
        std::cin >> n;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(256, '\n');

            if(oczekiwany)
                throw std::exception();
            else
                return IntUżytkownika(wiadomość, oczekiwany, zakresStart, zakresEnd);
        }
        std::cin.ignore();

        if(zakresStart != -1 && zakresEnd != -1 && (n < zakresStart || n > zakresEnd)){
            std::cout << "Podana liczba nie mieści się w zakresie" << std::endl;
            return IntUżytkownika(wiadomość, oczekiwany, zakresStart, zakresEnd);
        }

        return n;
    }
    // sprawdzanie poprawności stringa
    static std::string stringUżytkownika(std::string wiadomość, int maxL = -1){
        std::cout << wiadomość;
        std::string s;
        getline(std::cin, s);

        if(maxL != -1 && s.length() > maxL){
            std::cout << "Podany tekst jest za długi\n";
            return stringUżytkownika(wiadomość, maxL);
        }

        if(s.length() < 1){
            std::cout << "Tekst musi istnieć lol\n";
            return stringUżytkownika(wiadomość, maxL);
        }

        return s;
    }

    static float floatUżytkownika(std::string wiadomość, bool oczekiwany = true, float zakresStart = -1.0f, float zakresEnd = -1.0f){
        // Lambda expression do obsługi błędów
        auto ObsługaBłędów = [&](const std::string msg = "Podana wartość nie jest liczbą") {
            if (oczekiwany)
                throw std::runtime_error(msg);
            else
                std::cout << msg << std::endl;
            return floatUżytkownika(wiadomość, oczekiwany, zakresStart, zakresEnd);
        };

        std::cout << wiadomość;
        float n;
        std::string s;
        std::getline(std::cin, s);

        if(s.empty()) return 0;

        std::replace(s.begin(), s.end(), ',', '.');

        if(std::find_if(s.begin(), s.end(), [](char c){
            return !std::isdigit(c) && c != '.' && c != '-';}) != s.end()){
            return ObsługaBłędów();
        }

        try {
            n = std::stof(s);
        } catch (std::exception& e){
            return ObsługaBłędów();
        }

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(256, '\n');

            return ObsługaBłędów();
        }

        std::cin.ignore();

        if((zakresStart != -1 && n < zakresStart) || (zakresEnd != -1 && n > zakresEnd)){
            std::cout << "Podana liczba nie mieści się w zakresie" << std::endl;
            return floatUżytkownika(wiadomość, oczekiwany, zakresStart, zakresEnd);
        }

        return n;
    }

};
#endif //REGULATOR_UTILS_H
