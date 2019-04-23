#include <cmath>
#include <iostream>
#include <fstream>

#define MAX 99999999	// Max do porownan

using namespace std;

int liczba_zadan;

long int permutacje;
long int obliczenia[33554444][2];	// Tablica na obliczenia


int main(int argc, char ** argv)
{
    fstream in;	// Plik wejsciowy

    long int k1;
    long int min;
    long int min_2 = -1;

    in.open(argv[1], fstream::in);

    if (!in.good())
    {
        cout << "Blad otwarcia pliku wejsciowego!";
        return(-1);
    }

        in >> liczba_zadan;
        long int **zadania = new long int * [liczba_zadan]; // Tablica na dane zadan
        int *kolejnosc = new int [liczba_zadan]; // Tablica na dane zadan

        for (int j = 0; j < liczba_zadan; j++)
        {
         zadania[j] = new long int[4];
        }

        long int **adresy = new long int * [liczba_zadan + 1]; // Tablica na adresy
        for (int j = 0; j < liczba_zadan +1; j++)
        {
         adresy[j] = new long int[2];
        }


        // Wczytanie zadan
        for(int i = 0; i < liczba_zadan; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                in >> zadania[i][j];
            }
            zadania[i][3] = i;
            }


        permutacje = pow(2,liczba_zadan);			// Wyliczenie liczby permutacji = 2^N
        adresy[liczba_zadan-1][0] = permutacje/2;	// Wpisanie danych do tablicy adresow
        permutacje--;								// zmniejszenie liczby permutacji

        // Wstawienie adresow
        for(int i = liczba_zadan-2; i >= 0; i--)
        {
            adresy[i][0] = adresy[i+1][0]/2;
            adresy[i][1] = adresy[i][0];
            adresy[i+1][1] = permutacje-adresy[i+1][0];
        }

        adresy[0][0] = 1;
        adresy[0][1] = permutacje-1;
        adresy[liczba_zadan][0] = MAX;


        cout << permutacje << endl;

        for(int i = 1; i <= permutacje; i++)
        {
            int j = 0;

            min = MAX;
            min_2 = -1;
            while(adresy[j][0] <= i) // dopóki nr bitu jest mniejszy od it
            {
                //(czas wykonania końcowego zadania w zbiorze + wykonania większej części - deadline) * kara
                k1 = (zadania[j][0] + obliczenia[adresy[j][1]&i][1] - zadania[j][2]) * zadania[j][1];
                if(k1 < 0)
                {
                    k1 = 0; //kasowanie bitu
                }
              //k1 = k1 + kara od zbioru
                k1 = k1 + obliczenia[adresy[j][1]&i][0];
                if(k1 < min) //czy mniejsze od optymalnego
                {
                    //nowe optymalne
                    min = k1;
                    //jezeli znalezlismy wartosc optymalna
                    min_2 = j;
                }
                j++;
            }
            //jezeli znalezliśmy nowe optymalne
            if(min_2 >= 0)
            {
                obliczenia[i][0] = min; //wstawiamy nowa wartosc optymalna kary zbioru
                obliczenia[i][1] = obliczenia[adresy[min_2][1]&i][1] + zadania[min_2][0]; //wstawiamy czas wykonania się zbioru
            }
            obliczenia[i+1][0] = MAX;
            obliczenia[i+1][1] = MAX;
        }
        cout << obliczenia[permutacje][0] << "\n";
        min = permutacje;

    in.close();

    for (int j = 0; j < liczba_zadan; j++)
    {
     delete [] zadania[j];
    }
    for (int j = 0; j < liczba_zadan; j++)
    {
     delete [] adresy[j];
    }
    delete [] adresy;
    delete [] zadania;
    return(0);
}

