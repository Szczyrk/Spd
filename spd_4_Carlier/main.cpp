#include <cstdlib>
#include <iostream>
#include <climits>
#include <string>
#include "Carlier.h"

using namespace std;


int main(int argc, char** argv) {
    Carlier carlier;
    carlier.wczytaj(argv[1]);
    cout << "Cmax" << carlier.algorithm(INT_MAX) << endl;
    return 0;
}

