#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

struct Job{
    int nr;
    int r;
    int p;
    int q;
};

int main()
{
    int n;
    char nazwapliku[100];
    fstream plik;

    cout << "Enter file name" << endl;
    cin >> nazwapliku;
    plik.open(nazwapliku, ios::in);

    if(plik.good()!=true)
    cout << "Problem with file occured" << endl;
    plik >> n;

    Job tab[n];

    for(int i=0; i<n; i++){
	 tab[i].nr = i;
	 plik >> tab[i].r;
   	 plik >> tab[i].p;
   	 plik >> tab[i].q;
    }

    /* sortowanie niemalejace przez wstawianie*/
    Job tmp;
    int sum = 0, t = 0;
    for(int i=1, j; i<n; ++i){
        tmp = tab[i];
        for (j = i-1; j >= 0 && tab[j].r >= tmp.r; --j)
            tab[j+1] = tab[j];
        tab[j+1] = tmp;
    }

    for (int i = 0; i < n; i++){
	t = max(t,tab[i].r) + tab[i].p;
	sum = max(sum, t + tab[i].q);
        //cout << "nr tab: " << tab[i].nr << " moment dostepnosci " << tab[i].r << " czas wykonywania " << tab[i].p << " czas wykonania " << tab[i].q << endl;
    }
    cout << sum << endl;
    return 0;
}

