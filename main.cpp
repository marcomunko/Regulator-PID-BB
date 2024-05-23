#include <iostream>
#include "Symulacja.h"

int main() {
    //ustawienie UTF 8 -> polskie znaki i ujednolicenie kodowania
    system("chcp 65001 >nul 2>&1");
    setlocale( LC_ALL, "pl_PL.UTF-8" );

    auto symulacja = new Symulacja(); // usunięcie argumentów
    symulacja->startuj();

    system("pause");

    return 0;
}