#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <math.h> 

#define TEMPERATURA_POCZATKOWA 10000
#define TEMPERATURA_KONCOWA 1


using namespace std;

class new_class
{
public:
  int lzad;
  int lmasz;
  int Cmax;
  int liczba1,liczba2;
  float degree=TEMPERATURA_POCZATKOWA;
  float p;
  int **dane;
  int *kolej, *kolej1;
  int **tab;
//konstr
    new_class (char *nazwa);
  //destruk
   ~new_class ();
  //sortuje rosnaco z pliku dane
  void change ();
//
  int LiczCmax (int *);
  //najlepsza permutacja zadan
  //wypisuje permutacje z tablicy perm
  void wypisz ();
  void zmien();
  void sprawdz();
  void porownaj();
  void wywazanie();

};

new_class::new_class(char *nazwa)
{
    //ile zadan na permutacji
    fstream plik;
    char temp;
    plik.open (nazwa, ios::in);
    if (plik.good ())
    {
      //plik >> temp;  //jesli jest dodatkowy tekst w pliku
      plik >> lzad;
      plik >> lmasz;

      dane = new int *[lzad];
      for (int i = 0; i < lzad; i++)
	  dane[i] = new int[lmasz + 1];
      for (int i = 0; i < lzad; i++)
	  dane[i][lmasz] = 0;
      for (int i = 0; i < lzad; i++)
	  for (int j = 0; j < lmasz; j++)
	  {
	    plik >> dane[i][j];	//pominiC*cie nr maszyny
	    //   plik >> dane[i][j];
	    dane[i][lmasz] += dane[i][j];	//suma czasC3w
	  }
		//pusta lista zadan na maszynie
      kolej = new int[lzad];
      kolej1 = new int[lzad];
      for (int i = 0; i < lzad; i++)
      {
	    kolej[i] = i + 1;
        kolej1[i] = i + 1;
      }
      tab = new int *[lmasz + 1];
      for (int i = 0; i < lmasz + 1; i++)
	  tab[i] = new int[lzad + 1];

      for (int i = 0; i < lmasz + 1; i++)
	  for (int j = 0; j < lzad + 1; j++)
	     tab[i][j] = 0;

      plik.close ();
    }

}

new_class::~new_class ()
{
  delete dane;
  delete kolej;
  delete tab;
  delete kolej1;
}
void new_class::zmien()
{
    degree = degree * 0.99;
}
void new_class::wypisz()
{
    for(int i = 0;i<lzad;i++)
    {
        cout << kolej1[i]<< ' ';
   /* for(int j = 0;j<lmasz;j++)
    {
    cout << dane[i][j]<< ' ';
    }
    cout << endl;*/
    }
    cout << " p = " << p << endl;
}
void new_class::change()
{
    liczba1 = (rand() % lzad)+1;
    liczba2 = (rand() % lzad)+1;
    swap(kolej[liczba1-1],kolej[liczba2-1]);
    cout << liczba1 << ' '<< liczba2<<endl;
}
  int new_class::LiczCmax(int *kolejnosc)
  {
    for(int i=1; i<lmasz+1 ; i++)
      for(int j=1; j<lzad+1 ; j++)
      //max(element poprzedni, element z góry)+wartość w tym
      //punkcie, (tablica danych indeksowana odwrotnie oraz
      //bez zerowego wiersza i kolumny
         tab[i][j]=max(tab[i-1][j],tab[i][j-1])+dane[kolejnosc[j-1]-1][i-1];
    Cmax = tab[lmasz][lzad];
    //cout << Cmax<<' ';
    return Cmax;
  }
  void new_class::sprawdz()
  {
      if(LiczCmax(kolej)<LiczCmax(kolej1))
      p = 1;
      else
      p = exp((LiczCmax(kolej1)-LiczCmax(kolej))/degree);
      
  }
  void new_class::porownaj()
  {
      float h = (float)rand()/ RAND_MAX;
      if(p>=h)
      {
          for(int i = 0;i<lzad;i++)
          kolej1[i]=kolej[i];
      }
      else
      {
          for(int i = 0;i<lzad;i++)
          kolej[i] = kolej1[i];
      }
  }
  void new_class::wywazanie()
  {
      int g = 0;
      while(degree>TEMPERATURA_KONCOWA)
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
  new_class N (nazwa);
  N.wywazanie();
  koniec = clock();
  czas = (float)(koniec - start) / CLOCKS_PER_SEC;
  cout << "\n czas dzialania: " << czas << endl;
  cout<<endl << N.degree;
  return 0;
}