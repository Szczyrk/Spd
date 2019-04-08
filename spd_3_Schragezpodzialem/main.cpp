#include <iostream>
#include <stdio.h>
#include <fstream>
#include <queue>
#include <vector>
#include <iomanip>

using namespace std;
struct zadanie {
    int nr;
    int ri; // moment dostepnosci
    int pi; // czas wykonywania tab
    int qi; // ogonek
};

ostream& operator << (ostream& os, const zadanie& z1) {
    os << z1.nr << ' ' << z1.ri << ' ' << z1.pi << ' ' << z1.qi;
    return os;
}



class rosnaco_zadanie {
public:
    bool operator()(const zadanie& z1, const zadanie& z2) {
        return z1.ri > z2.ri;
    }
};

class malejaco_zadanie {
public:
    bool operator()(const zadanie& z1, const zadanie& z2) {
        return z1.qi < z2.qi;
    }
};

int main() {
    priority_queue<zadanie, vector<zadanie>, rosnaco_zadanie> k1;
    priority_queue<zadanie, vector<zadanie>, malejaco_zadanie> k2;
    //kolejki priorytetowa, bo sa najszybciej (O(logn)), lista O(n), tablica z qsortem O(nlogn)

    int c = 0;
    char nazwapliku[100];
    fstream plik;
    int n = -1; // liczba zadan
    zadanie *wsk = NULL;
    int index =0;

    cout << "Podaj nazwe pliku" << endl;
    cin >> nazwapliku;
    plik.open(nazwapliku, ios::in);
    if (plik.good() != true) {
        cout << "Blad wczytywania pliku" << endl;
        return -1;
    }
    plik >> n;
    cout << "ilosc zadan:" << n << endl;
    zadanie tab[n];
    int pi[n];
    for (int i = 0; i < n; i++) {
        wsk = new zadanie;
        tab[i].nr = i;
        wsk->nr = i;
        plik >> tab[i].ri;
        wsk->ri = tab[i].ri;
        plik >> tab[i].pi;
        wsk->pi = tab[i].pi;
        plik >> tab[i].qi;
        wsk->qi = tab[i].qi;
        k1.push(*wsk);
    }

    // nlogn - co najwyzej (n-1) podzialow, czyli mamy 2nlogn, czyli nlogn

    c = 0;
    int licznik = 1, d=0, e=0; //d - spamietywanie ile trwac bedzie najdluzsze dostarczenie zadania, e - zmienna to przechowywania biezacego czasu dostarczenia
    zadanie temp,temp1,temp2,temp3;
    temp = k1.top();
    c=temp.ri;//zaczynamy prace w momencie dostepnosci pierwszego zadania
    int f=0;


    while (!(k1.empty() && k2.empty())) //dopoki nie oproznie kolejki z zadaniami
    {
        if (k2.empty()) f = -1; //jezeli mamy przerwe to flaga do gory
        while (c>=temp.ri && (!k1.empty()) || f<0) // dopoki mamy zadania gotowe do wykonania i nie skonczyly sie wszystkie zadania lub flaga jest w gorze
        {
            k1.pop();
            k2.push(temp);
            f=0;
            temp = k1.top();
        }
        temp1 = k2.top(); // biore zadanie z najwiekszym ogonem, ktore jest dostepne
        k2.pop();
        if (c<temp1.ri){
             //cout << "kkkkkkkk przerwa przed " << temp1.nr << " o " << temp1.ri - c << endl;
             c = temp1.ri; //jezeli mamy przerwe to zaczynamy z czasem zadania po przerwie

         }
        //sprawdzic nastepne zadanie z k1 - jezeli q wieksze to wrzucic na maszyne, a obecne na k1,
        //jak nie to przejsc do czasu, w ktorym jest dostepne nastepne przed koncem obecnego lub skonczyc obecne.
        cout << licznik << ". permutacja z zadaniem nr: " << temp1.nr << endl;//" czas dostepnosci zadania byl " << temp1.ri << " czas maszyny " << c << " czas wykonania " << temp1.pi << " czas dostarczenia " << temp1.qi << endl;
        licznik ++;
        if (!(k1.empty()))
        {
            temp3 = k1.top();
            if (temp3.ri <= c+temp1.pi && temp3.qi > temp1.qi) //jezeli spelnione warunki przerwania
            {
                temp1.pi = temp1.pi - (temp3.ri - c);
                c = temp3.ri;
                temp2 = k1.top();
                k1.pop();
                k1.push(temp1);
                temp1=temp2;
                temp = k1.top();
                //cout<< "Przerwanie zadania " <<  przerwane << " przez " << przerwalo << " w chwili " << c << " po " << po << " czas zadania przerw. przed " << czas_zad << " czas zadania po " << zos << endl;
                cout << "Przerwanie !!! " <<licznik << " permutacja z zadaniem nr: " << temp1.nr << endl;//" czas maszyny " << c << " czas wykonania " << temp1.pi << " czas dostarczenia " << temp1.qi << endl;
                licznik ++;
            }
        }
        pi[index++] = temp1.nr;
        c=c+temp1.pi;
        e = c + temp1.qi; //czas realizacji to moment dostepnosci maszyn + czas dostarczenia ostatniego
        if (d<e) d=e; //przypisuje o ile bedzie dluzej trwalo

    }
    cout << endl;
    cout << "PI :" << endl;
    for(int x =0;x<n;x++)
        cout<<pi[x]<<",";
    cout << endl;
    cout << endl;
    cout << d << endl;
    return 0;
}

