#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <math.h> 

#define TEMPERATURA_POCZATKOWA 10000
#define TEMPERATURA_KONCOWA 1

using namespace std;

class Symulowane_wyzarzanie
{
    public:
        int liczbaZadan;
        int liczbaMaszyn;
        int Cmax;
        int liczba1,liczba2;
        float temperatura = TEMPERATURA_POCZATKOWA;
        float prawdopodobienstwo;
        int **dane;
        int *kolej, *kolej1;
        int **tab;
        Symulowane_wyzarzanie(char *nazwa);
        ~Symulowane_wyzarzanie();
        void change();
        int LiczCmax(int *);
        void wypisz();
        void zmien();
        void sprawdz();
        void porownaj();
        void wykonajOperacje();
};

Symulowane_wyzarzanie::Symulowane_wyzarzanie(char *nazwa)
{
    fstream plik;
    char temp;
    plik.open(nazwa, ios::in);
    if (plik.good())
    {
        //plik >> temp;  //jesli jest dodatkowy tekst w pliku
        plik >> liczbaZadan;
        plik >> liczbaMaszyn;

        dane = new int *[liczbaZadan];

        for (int i = 0; i < liczbaZadan; i++)
            dane[i] = new int[liczbaMaszyn + 1];

        for (int i = 0; i < liczbaZadan; i++)
            dane[i][liczbaMaszyn] = 0;
        for (int i = 0; i < liczbaZadan; i++)
        for (int j = 0; j < liczbaMaszyn; j++) {
            plik >> dane[i][j];	//pominiecie nr maszyny
            dane[i][liczbaMaszyn] += dane[i][j];	//suma czasow
        }
        //pusta lista zadan na maszynie
        kolej = new int[liczbaZadan];
        kolej1 = new int[liczbaZadan];
        for (int i = 0; i < liczbaZadan; i++) {
            kolej[i] = i + 1;
            kolej1[i] = i + 1;
        }

        tab = new int *[liczbaMaszyn + 1];
        for (int i = 0; i < liczbaMaszyn + 1; i++)
            tab[i] = new int[liczbaZadan + 1];

        for (int i = 0; i < liczbaMaszyn + 1; i++)
        for (int j = 0; j < liczbaZadan + 1; j++)
            tab[i][j] = 0;

        plik.close();
    }
}

Symulowane_wyzarzanie::~Symulowane_wyzarzanie()
{
  delete dane;
  delete kolej;
  delete tab;
  delete kolej1;
}

void Symulowane_wyzarzanie::zmien()
{
    temperatura = temperatura * 0.29;
}

void Symulowane_wyzarzanie::wypisz()
{
    for(int i = 0; i < liczbaZadan;i++)
    {
        cout << kolej1[i] << ' ';
        for(int j = 0; j < liczbaMaszyn;j++)
        {
            cout << dane[i][j]<< ' ';
        }
        cout << endl;
    }
    cout << " p = " << prawdopodobienstwo << endl;
}


void Symulowane_wyzarzanie::change()
{
    liczba1 = (rand() % liczbaZadan)+1;
    liczba2 = (rand() % liczbaZadan)+1;
    swap(kolej[liczba1-1],kolej[liczba2-1]);
    cout << liczba1 << ' '<< liczba2<<endl;
}


int Symulowane_wyzarzanie::LiczCmax(int *kolejnosc)
{
    for(int i=1; i<liczbaMaszyn+1 ; i++)
    for(int j=1; j<liczbaZadan+1 ; j++)
    //max(element poprzedni, element z góry)+wartość w tym
    //punkcie, (tablica danych indeksowana odwrotnie oraz
    //bez zerowego wiersza i kolumny
    tab[i][j]=max(tab[i-1][j],tab[i][j-1])+dane[kolejnosc[j-1]-1][i-1];

    Cmax = tab[liczbaMaszyn][liczbaZadan];
    return Cmax;
}

void Symulowane_wyzarzanie::sprawdz()
{
    if(LiczCmax(kolej)<LiczCmax(kolej1))
    prawdopodobienstwo = 1;
    else
    prawdopodobienstwo = exp((LiczCmax(kolej1)-LiczCmax(kolej))/temperatura); 
}

void Symulowane_wyzarzanie::porownaj()
{
    float h = (float)rand()/ RAND_MAX;
    if(prawdopodobienstwo>=h)
    {
        for(int i = 0;i<liczbaZadan;i++)
        kolej1[i]=kolej[i];
    }
    else
    {
        for(int i = 0;i<liczbaZadan;i++)
        kolej[i] = kolej1[i];
    }
}

void Symulowane_wyzarzanie::wykonajOperacje()
{
    int g = 0;
    while(temperatura>TEMPERATURA_KONCOWA)
    {
        change();
        sprawdz();
        porownaj();
        zmien();
        wypisz();
        cout<<endl<<"CMAX = "<<LiczCmax(kolej1)<<endl;
        g++;
    }
    cout<<"liczba iteracji = " <<g;
}


int main ()
{
    srand(time(NULL));
    char nazwa[20] = "data.txt";
    float czas;
    clock_t start, koniec;
    start = clock();
    Symulowane_wyzarzanie algorytm(nazwa);
    algorytm.wykonajOperacje();
    koniec = clock();
    czas = (float)(koniec - start) / CLOCKS_PER_SEC;
    cout << "\n czas dzialania: " << czas << endl;
    cout<<endl << algorytm.temperatura;
    return 0;
}