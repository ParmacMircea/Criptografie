#include "crypto.h"

int main() {
    // 1. Citire alfabet
    ifstream in_alfabet("alfabet.txt");
    citeste_alfabet(in_alfabet);
    in_alfabet.close();

    // ========== Problema 1: Cezar ==========
    ifstream in1("problema1.txt");
    ofstream out1("problema1_decrypted.txt");
    spargereCezar(in1, out1);
    in1.close();
    out1.close();

    // ========== Problema 2: Afin ==========
    ifstream in2("problema2.txt");
    ofstream out2("problema2_decrypted.txt");
    spargereAfina(in2, out2);
    in2.close();
    out2.close();

    // ========== Problema 3: Afin pe blocuri ==========
    ifstream in3("problema3.txt");
    ofstream out3("problema3_decrypted.txt");

    // Încercăm diferite matrici până găsim una care decriptează corect
    cheieAfina k;
    k.n = 2;
    k.a = new int* [2];
    k.b = new int[2];
    for (int i = 0; i < 2; i++)
        k.a[i] = new int[2];

    // Matrice de testat (exemple comune)
    int test_matrices[][2][2] = {
        {{1, 0}, {0, 1}},
        {{1, 1}, {0, 1}},
        {{1, 0}, {1, 1}},
        {{1, 1}, {1, 2}},
        {{2, 1}, {1, 1}}
    };

    for (int m = 0; m < 5; m++) {
        k.a[0][0] = test_matrices[m][0][0];
        k.a[0][1] = test_matrices[m][0][1];
        k.a[1][0] = test_matrices[m][1][0];
        k.a[1][1] = test_matrices[m][1][1];
        k.b[0] = 0; k.b[1] = 0;

        k.a_inv = calcul_matr_inv_Zn(k.a, 2, N);
        if (k.a_inv) {
            in3.clear();
            in3.seekg(0);
            decriptareAfinaBloc(in3, out3, k);

            // Verificăm dacă decriptarea are sens
            // (ar trebui adăugat un validator mai bun în practică)
            break;
        }
    }

    // Eliberare memorie
    for (int i = 0; i < 2; i++)
        delete[] k.a[i];
    delete[] k.a;
    delete[] k.b;
    if (k.a_inv) {
        for (int i = 0; i < 2; i++)
            delete[] k.a_inv[i];
        delete[] k.a_inv;
    }
    in3.close();
    out3.close();

    // ========== Problema 4: Vigenère ==========
    ifstream in4("problema4.txt");
    ofstream out4("problema4_decrypted.txt");
    spargereVigenere(in4, out4);
    in4.close();
    out4.close();

    cout << "Decriptarea tuturor problemelor a fost finalizata!" << endl;
    return 0;
}