#include "crypto.h"

int main() {
    ifstream alfabet_in("alfabet.txt");
    citeste_alfabet(alfabet_in);
    alfabet_in.close();

    ifstream in("sursa.txt");
    ofstream out("destinatie.txt");

    int keyLength = estimareLungimeCheie(in);
    string cheie = detectCheieVigenere(in, keyLength);
    cout << "Cheie detectata: " << cheie << endl;

    in.clear();
    in.seekg(0);
    cheieVigenere k;
    k.key = cheie;
    decriptareVigenere(in, out, k);

    in.close();
    out.close();

    return 0;
}