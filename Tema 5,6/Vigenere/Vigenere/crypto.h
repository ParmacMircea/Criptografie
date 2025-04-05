#pragma once
#include <iostream>
#include <fstream>
using namespace std;

int modulo(int a, int n) {
    if (a >= 0 && n > 0) return a % n;
    if (a < 0 && n > 0) return (a % n) + n;
    return -1;
}

char alfabet[100];
int N = 0;

int da_cod(char c) {
    for (int i = 0; i < N; i++)
        if (alfabet[i] == c) return i;
    return -1;
}

char da_caracter(int cod) {
    return alfabet[modulo(cod, N)];
}

void citeste_alfabet(ifstream& in) {
    char c;
    while (in >> noskipws >> c) {
        alfabet[N++] = c;
    }
    if (N == 0) cout << "Alfabetul nu are caractere";
}

struct cheieVigenere {
    string key;
};

void criptareVigenere(ifstream& in, ofstream& out, const cheieVigenere& k) {
    char c;
    int keyIndex = 0;
    while (in >> noskipws >> c) {
        int keyChar = da_cod(k.key[keyIndex % k.key.length()]);
        int textChar = da_cod(c);
        out << da_caracter(textChar + keyChar);
        keyIndex++;
    }
}

void decriptareVigenere(ifstream& in, ofstream& out, const cheieVigenere& k) {
    char c;
    int keyIndex = 0;
    while (in >> noskipws >> c) {
        int keyChar = da_cod(k.key[keyIndex % k.key.length()]);
        int textChar = da_cod(c);
        out << da_caracter(textChar - keyChar);
        keyIndex++;
    }
}

string detectCheieVigenere(ifstream& in, int keyLength) {
    in.clear();
    in.seekg(0);

    int frecvente[100][100] = { 0 };
    char c;
    int pos = 0;

    while (in >> noskipws >> c) {
        int keyPos = pos % keyLength;
        frecvente[keyPos][da_cod(c)]++;
        pos++;
    }

    string key;
    for (int i = 0; i < keyLength; i++) {
        int maxFreq = 0;
        int maxChar = 0;
        for (int j = 0; j < N; j++) {
            if (frecvente[i][j] > maxFreq) {
                maxFreq = frecvente[i][j];
                maxChar = j;
            }
        }
        int keyChar = modulo(maxChar - da_cod(' '), N);
        key += da_caracter(keyChar);
    }

    return key;
}

int estimareLungimeCheie(ifstream& in) {
    return 6;
}