#include <iostream>
#include <fstream>

using namespace std;
struct zadanie{
    int nr;
    int r;
    int p; // moment dostepnosci
    int q; // czas wykonywania tab
};

int main()
{
    int c=0;
    char nazwapliku[100];
    fstream plik;
    int n=-1; // liczba zadan

    cout<<"Podaj nazwe pliku"<<endl;
    cin>>nazwapliku;
    plik.open(nazwapliku, ios::in);
    if(plik.good()!=true)
    cout<<"Blad wczytywania pliku"<<endl;
    plik>>n;
    cout<<"tutaj n:"<<n<<endl;
    zadanie tab[n];
    for(int i=0;i<n;i++){
    tab[i].nr=i;
    plik>>tab[i].r;
    plik>>tab[i].p;
    plik>>tab[i].q;
    }


    /* sortowanie niemalejace przez wstawianie*/
    zadanie tmp;
    int i,j;
    for(i=1; i<n;++i)
    {
        tmp=tab[i];
        for (j=i-1;j>=0 && tab[j].r >= tmp.r ; --j)
            tab[j+1]=tab[j];
        tab[j+1] = tmp;
    }


    /*wyliczanie c*/
    c=tab[0].r; //zaczynamy prace w momencie dostepnosci pierwszego tab
    for (int i=0; i<n;i++)
    {

       if (c>=tab[i].p)
            c=c+tab[i].q+tab[i].p;
        else
            c+=tab[i].p; //jezeli mamy przerwe, to zoczynami z czasem po przerwie
       /*cout << "nr tab: " << tab[i].nr << " moment dostepnosci " << tab[i].p << " czas wykonania " << tab[i].q << endl;
       cout << c << endl; // do testow*/
    }
    cout << c <<endl;
    return 0;
}

