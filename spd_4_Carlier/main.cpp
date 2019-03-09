#include <cstdlib>
#include <iostream>
#include <climits>

#include "Schrage.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
    Schrage sh;
    sh.wczytaj(argv[1]);
    cout << "Cmax" << sh.carlier(INT_MAX) << endl;

    return 0;
}

