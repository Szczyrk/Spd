
#include <fstream>
#include <iostream>
#include <climits>
#include <math.h>

using namespace std;

const int nr_zadania = 0, r=1, p=2, q=3, out=4;

#include <vector>
#include <string>

class Carlier {

    int ilosc_zadan; //dlugosc permutacji
    int Cmax_Schrage; //Cmax bez podzialu
    int Cmax_Scharage_podzial; //Cmax z podzialem
    std::vector<int*> permutacja;
    std::vector<int*> kolejka;
    std::vector<int*> oczekujace;
    std::vector<int*> optymalne;

public:
    Carlier() {
    }

    ~Carlier() {
        for (int i = 0; i < ilosc_zadan; i++)
            delete permutacja[i];
    }


    int wczytaj(std::string nazwa_pliku) {
        ifstream plik(nazwa_pliku.c_str());
        int * tmp;
        if (!plik.is_open()) {
            cout << "Blad otwarcia pliku!"<< endl;
            return 1;
        }
        permutacja.clear();
        plik >> ilosc_zadan;
        for (int i = 0; i < ilosc_zadan; i++) {
            tmp = new int[4];
            tmp[nr_zadania] = i + 1;
            plik >> tmp[r] >> tmp[p] >> tmp[q];
            permutacja.push_back(tmp);
        }
        plik.close();
        Cmax_Schrage = 0;
        Cmax_Scharage_podzial = 0;
        return 0;
    }


    // wyznaczanie b
    int max_K(int Cmax) {
        int max_K = 0;
        for (int i = ilosc_zadan-1; i > 0; i--) {
            if (Cmax == (permutacja[i][out] + permutacja[i][q])) {
                max_K = i;
                return max_K;
            }
        }
        return max_K;
    }
    // wyznaczanie a
    int min_K(int max_K, int Cmax) {
        int min_K, tmp = 0;
        for (min_K = 0; min_K < max_K; min_K++) {
            tmp = 0;
            for (int j = min_K; j <= max_K; j++) { //liczy ile wykonujan sie wszyskie zadania
                tmp += permutacja[j][p];
            }
            if (Cmax == (tmp + permutacja[min_K][r] + permutacja[max_K][q])) {
                return min_K;
            }
        }
        return min_K;
    }

    // wyznaczanie c
    int zadanie_interferencyjne_funkcja(int min_K, int max_K) {
        int c = -1;
        for (int i = max_K; i >= min_K; i--) {
        //pierwsze zad kotergo q jest mniejsze niz zadania b
            if (permutacja[i][q] < permutacja[max_K][q]) {
                return i;
            }
        }
        return c;
    }

    int algorithm(int gorne_ogr) {
        int zadanie_interferencyjne, poczatek_K, koniec_K, dolne_ogr = 0;
        int gorne_ogr_tmp = -1, r_pamiec, q_pamiec;
        int r_pi = INT_MAX, q_pi = INT_MAX, p_pi = 0;
        vector<int*> permutacja_tmp;
        shrage();
        if (Cmax_Schrage < gorne_ogr) {
            gorne_ogr = Cmax_Schrage;
        }
        koniec_K = max_K(Cmax_Schrage);	//b
        poczatek_K = min_K(koniec_K, Cmax_Schrage); //a
        zadanie_interferencyjne = zadanie_interferencyjne_funkcja(poczatek_K, koniec_K); // c

        if (zadanie_interferencyjne == -1) {//nie znalazl zadania c tzn Schrage optymalny
            return gorne_ogr;
        }
        for (int i = zadanie_interferencyjne + 1; i <= koniec_K; i++) {
            r_pi = min(r_pi, permutacja[i][r]);
            p_pi += permutacja[i][p];
            q_pi = min(q_pi, permutacja[i][q]);
        }
        // zwiekszamy r
        r_pamiec = permutacja[zadanie_interferencyjne][r];
        permutacja_tmp = permutacja;
        permutacja[zadanie_interferencyjne][r] = max(permutacja[zadanie_interferencyjne][r], r_pi + p_pi);
        sortuj();
        shrage_podzial();
        dolne_ogr = Cmax_Scharage_podzial;
        if (dolne_ogr < gorne_ogr) {
            gorne_ogr_tmp = algorithm(gorne_ogr);
            // jezeli ok
            if (gorne_ogr_tmp != -1 && gorne_ogr_tmp < gorne_ogr)
                gorne_ogr = gorne_ogr_tmp;
        }
        permutacja = permutacja_tmp;
        permutacja[zadanie_interferencyjne][r] = r_pamiec;

        // zwiekszamy q
        q_pamiec = permutacja[zadanie_interferencyjne][q];
        permutacja[zadanie_interferencyjne][q] = max(permutacja[zadanie_interferencyjne][q],q_pi + p_pi);
        sortuj();
        shrage_podzial();
        dolne_ogr = Cmax_Scharage_podzial;
        if (dolne_ogr < gorne_ogr) {
            gorne_ogr_tmp = algorithm(gorne_ogr);
        // jezeli ok
            if (gorne_ogr_tmp != -1 && gorne_ogr_tmp < gorne_ogr)
                gorne_ogr = gorne_ogr_tmp;
        }
        permutacja[zadanie_interferencyjne][q] = q_pamiec;

        return gorne_ogr;
    }


    //Quick Sort
    void zamien(int pierwszy, int drugi) {
        int * tmp;
        tmp = kolejka[pierwszy];
        kolejka[pierwszy] = kolejka[drugi];
        kolejka[drugi] = tmp;
    }

    int podziel(int poczatek, int koniec, int pivot) {
        int wartosc_pivota = kolejka[pivot][r];
        int obecny = poczatek;
        zamien(pivot, koniec);
        for (int i = poczatek; i < koniec; i++) {
            if (kolejka[i][r] > wartosc_pivota) {

                zamien(i, obecny);
                obecny++;
            }
        }
        zamien(obecny, koniec);
        return obecny;
    }

    void quicksort(int lewy, int prawy) {
        if (prawy > lewy) {
            int nowy_pivot = podziel(lewy, prawy, (lewy + prawy) / 2);
            quicksort(lewy, nowy_pivot - 1);
            quicksort(nowy_pivot + 1, prawy);
        }
    }

    void sortuj_r() {
        quicksort(0, ilosc_zadan - 1);
    }


    // sortuje wedlug r i przypisuje do kolejki
    void sortuj() {
        kolejka.clear();
        for (int i = 0; i < ilosc_zadan; i++) {
            kolejka.push_back(permutacja[i]);
        }
        sortuj_r();
    }


    void shrage() {
        int t = 0;
        Cmax_Schrage = 0;
        sortuj();
        optymalne.clear();
        while (!kolejka.empty() || !oczekujace.empty()) {
            if (!kolejka.empty() && kolejka.back()[r] > t && oczekujace.empty())
                t = kolejka.back()[r];
            while (!kolejka.empty() && kolejka.back()[r] <= t) {
                dodaj_q(kolejka.back());
                kolejka.pop_back();
            };
            optymalne.push_back(oczekujace.back());
            optymalne.back()[out] = optymalne.back()[p] + t;
            t += oczekujace.back()[p];
            Cmax_Schrage = max(Cmax_Schrage, t + oczekujace.back()[q]);
            oczekujace.pop_back();
        }
        permutacja = optymalne;
    }

    void shrage_podzial() {
        int t = 0;
        int * obecne;
        obecne = new int [4];
        obecne[q] = INT_MAX;
        obecne[r] = 0;
        obecne[p] = 0;
        sortuj();
        while (!kolejka.empty() || !oczekujace.empty()) {
            while (!kolejka.empty() && kolejka.back()[r] <= t) {
                dodaj_q(kolejka.back());
                if (kolejka.back()[q] > obecne[q]) { //jesli nowe ma wieksze q niz obecne
                    obecne[p] = t - kolejka.back()[r]; //
                    t = kolejka.back()[r];
                    if (obecne[p] > 0) {
                        dodaj_q(obecne);
                        obecne = new int [4];
                        obecne[q] = INT_MAX;
                        obecne[r] = 0;
                        obecne[p] = 0;
                    }
                }
                kolejka.pop_back();
            }
            if (oczekujace.empty()) {
                t = kolejka.back()[r];
            } else {
                for (int i = 0; i < 4; i++)
                    obecne[i] = oczekujace.back()[i];
                t += oczekujace.back()[p];
                Cmax_Scharage_podzial = max(Cmax_Scharage_podzial, t + oczekujace.back()[q]);
                oczekujace.pop_back();
            }
        }
    }

    void dodaj_q(int * dane) {
        vector<int*>::iterator it = oczekujace.begin();
        bool tu = false;
        while (it != oczekujace.end() && !tu) {
            if ((*it)[q] < dane[q])
                it++;
            else {
                tu = true;
            }
        }
        oczekujace.insert(it, dane);
    }
};

int main(int argc, char** argv) {
    Carlier carlier;
    carlier.wczytaj("test.txt");
    int sum=0;
    cout << "Cmax : " << carlier.algorithm(INT_MAX) << endl;
    return 0;
}
