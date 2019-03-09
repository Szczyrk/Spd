#ifndef SHRAGE_H
#define	SHRAGE_H

#include <vector>
#include <string>

class Schrage {
public:
    Schrage();
    Schrage(const Schrage& orig);
    virtual ~Schrage();

    int ilosc_zadan; //dlugosc permutacji
    int Cmax_Schrage; //Cmax bez podzialu
    int Cmax_Scharage_podzial; //Cmax z podzialem
    std::vector<int*> permutacja;
    std::vector<int*> kolejka;
    std::vector<int*> oczekujace;
    std::vector<int*> optymalne;

    int wczytaj(std::string nazwa_pliku);
    void shrage();
    void shrage_podzial();
    int carlier(int);
private:
    void zamien(int pierwszy, int drugi);
    int podziel(int poczatek, int koniec, int pivot);
    void quicksort(int lewy, int prawy);
    void sortuj_r();
    void dodaj_q(int*);
    void sortuj();
    int c(int, int);
    int max_K(int);
    int min_K(int, int);
    void uzupelnij_optymalne();
};

#endif	/* SHRAGE_H */

