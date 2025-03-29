#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

// Structuri si variabile globale
char character[100] = { 0 };
int N = 0;

struct caracter {
    char c;
    int f;
};

struct cheieAfina {
    int n;
    int** a;
    int* b;
    int** a_inv;
};

// Functii de baza
int modulo(int a, int b) {
    int r = a % b;
    return r < 0 ? r + b : r;
}

int da_cod(char c) {
    for (int i = 0; i < N; ++i)
        if (character[i] == c)
            return i;
    return -1;
}

char da_caracter(int cod) {
    return character[modulo(cod, N)];
}

void citeste_alfabet(ifstream& in) {
    if (!in.good())
        perror("Fisier inexistent");
    char c;
    while (in >> noskipws >> c) {
        character[N] = c;
        N++;
    }
    if (N == 0)
        cout << "Alfabetul dat are 0 caractere" << endl;
}

// ================== Criptosistemul Cezar ==================
void criptareCezar(ifstream& in, ofstream& out, int k) {
    char m;
    while (in >> noskipws >> m) {
        int cod = da_cod(m);
        if (cod != -1)
            out << da_caracter(cod + k);
        else
            out << m;
    }
}

caracter* frecv(ifstream& fin) {
    caracter* caractere = new caracter[N];
    for (int i = 0; i < N; i++) {
        caractere[i].c = character[i];
        caractere[i].f = 0;
    }

    char c;
    while (fin >> noskipws >> c) {
        int cod = da_cod(c);
        if (cod >= 0 && cod < N)
            caractere[cod].f++;
    }

    // Sortare prin bubble sort
    bool ordonat = false;
    while (!ordonat) {
        ordonat = true;
        for (int i = 0; i < N - 1; i++) {
            if (caractere[i].f < caractere[i + 1].f) {
                ordonat = false;
                swap(caractere[i], caractere[i + 1]);
            }
        }
    }
    return caractere;
}

void spargereCezar(ifstream& in, ofstream& out, char frecvent = ' ') {
    caracter* caractere = frecv(in);
    int k = modulo(-da_cod(caractere[0].c) + da_cod(frecvent), N);
    in.clear();
    in.seekg(0);
    criptareCezar(in, out, k);
    delete[] caractere;
}

// ================== Criptosistemul Afin ==================
void criptareAfina(ifstream& in, ofstream& out, int a, int b) {
    char m;
    while (in >> noskipws >> m) {
        int cod = da_cod(m);
        if (cod != -1) {
            out << da_caracter(modulo(a * cod + b, N));
        }
        else {
            out << m;
        }
    }
}

int inverse(int a, int n) {
    for (int x = 1; x < n; x++)
        if (modulo(a * x, n) == 1)
            return x;
    return -1;
}

void spargereAfina(ifstream& in, ofstream& out, char frecvent0 = ' ', char frecvent1 = 'e') {
    // Calculăm frecvențele caracterelor
    int freq[256] = { 0 };
    char c;
    int total = 0;

    while (in >> noskipws >> c) {
        int idx = da_cod(c);
        if (idx != -1) {
            freq[idx]++;
            total++;
        }
    }

    // Găsim cele mai frecvente 2 caractere
    int max1 = 0, max2 = 0, idx1 = -1, idx2 = -1;
    for (int i = 0; i < N; i++) {
        if (freq[i] > max1) {
            max2 = max1;
            idx2 = idx1;
            max1 = freq[i];
            idx1 = i;
        }
        else if (freq[i] > max2) {
            max2 = freq[i];
            idx2 = i;
        }
    }

    // Caracterele așteptate în textul clar (spațiu și 'e')
    int x1 = da_cod(frecvent0);
    int x2 = da_cod(frecvent1);

    if (x1 == -1 || x2 == -1) {
        cerr << "Caracterele asteptate nu sunt in alfabet!\n";
        return;
    }

    // Rezolvăm sistemul de ecuații
    int delta_x = modulo(x1 - x2, N);
    int delta_y = modulo(idx1 - idx2, N);

    int inv_delta_x = inverse(delta_x, N);
    if (inv_delta_x == -1) {
        cerr << "Nu se poate rezolva sistemul (delta_x nu este inversabil)\n";
        return;
    }

    int a = modulo(delta_y * inv_delta_x, N);
    int b = modulo(idx1 - a * x1, N);

    // Verificăm dacă a este inversabil
    int inv_a = inverse(a, N);
    if (inv_a == -1) {
        cerr << "Cheia 'a' nu este inversabila\n";
        return;
    }

    // Decriptăm textul
    in.clear();
    in.seekg(0);
    while (in >> noskipws >> c) {
        int idx = da_cod(c);
        if (idx != -1) {
            int dec = modulo(inv_a * (idx - b), N);
            out << da_caracter(dec);
        }
        else {
            out << c;
        }
    }
}

// ================== Criptare Afina pe blocuri ==================
void criptareAfinaBloc(ifstream& in, ofstream& out, cheieAfina k) {
    int i = 0;
    char m;
    int* M = new int[k.n];
    int* C = new int[k.n];

    while (in >> noskipws >> m) {
        M[i++] = da_cod(m);
        if (i == k.n) {
            for (int j = 0; j < k.n; j++) {
                C[j] = 0;
                for (int l = 0; l < k.n; l++) {
                    C[j] += k.a[j][l] * M[l];
                }
                C[j] = modulo(C[j] + k.b[j], N);
                out << da_caracter(C[j]);
            }
            i = 0;
        }
    }

    delete[] M;
    delete[] C;
}

void decriptareAfinaBloc(ifstream& in, ofstream& out) {
    // Citim tot textul criptat
    string ciphertext;
    char c;
    while (in >> noskipws >> c) {
        ciphertext += c;
    }

    // Încercăm diferite matrici până găsim una validă
    const int test_matrices[][2][2] = {
        {{1, 0}, {0, 1}},
        {{1, 1}, {0, 1}},
        {{1, 0}, {1, 1}},
        {{1, 1}, {1, 2}},
        {{2, 1}, {1, 1}},
        {{3, 5}, {1, 2}}
    };

    for (int m = 0; m < 6; m++) {
        cheieAfina k;
        k.n = 2;
        k.a = new int* [2];
        k.b = new int[2] {0, 0}; // Vector nul

        for (int i = 0; i < 2; i++) {
            k.a[i] = new int[2];
            for (int j = 0; j < 2; j++) {
                k.a[i][j] = test_matrices[m][i][j];
            }
        }

        k.a_inv = calcul_matr_inv_Zn(k.a, 2, N);
        if (!k.a_inv) continue;

        // Testăm decriptarea pe primele câteva caractere
        bool valid = true;
        int tested = 0;

        for (size_t i = 0; i < min(ciphertext.size(), size_t(20)); i += 2) {
            if (i + 1 >= ciphertext.size()) break;

            int y1 = da_cod(ciphertext[i]);
            int y2 = da_cod(ciphertext[i + 1]);

            if (y1 == -1 || y2 == -1) continue;

            int x1 = modulo(k.a_inv[0][0] * (y1 - k.b[0]) + k.a_inv[0][1] * (y2 - k.b[1]), N);
            int x2 = modulo(k.a_inv[1][0] * (y1 - k.b[0]) + k.a_inv[1][1] * (y2 - k.b[1]), N);

            char dc1 = da_caracter(x1);
            char dc2 = da_caracter(x2);

            if (!isalpha(dc1) || !isalpha(dc2)) {
                valid = false;
                break;
            }
            tested++;
        }

        if (valid && tested > 0) {
            // Decriptăm tot textul
            for (size_t i = 0; i < ciphertext.size(); i += 2) {
                if (i + 1 >= ciphertext.size()) {
                    out << ciphertext[i];
                    break;
                }

                int y1 = da_cod(ciphertext[i]);
                int y2 = da_cod(ciphertext[i + 1]);

                if (y1 != -1 && y2 != -1) {
                    int x1 = modulo(k.a_inv[0][0] * (y1 - k.b[0]) + k.a_inv[0][1] * (y2 - k.b[1]), N);
                    int x2 = modulo(k.a_inv[1][0] * (y1 - k.b[0]) + k.a_inv[1][1] * (y2 - k.b[1]), N);
                    out << da_caracter(x1) << da_caracter(x2);
                }
                else {
                    out << ciphertext[i] << ciphertext[i + 1];
                }
            }

            // Eliberăm memoria
            for (int i = 0; i < 2; i++) {
                delete[] k.a[i];
                delete[] k.a_inv[i];
            }
            delete[] k.a;
            delete[] k.a_inv;
            delete[] k.b;

            return;
        }

        // Eliberăm memoria dacă nu e valid
        for (int i = 0; i < 2; i++) {
            delete[] k.a[i];
            if (k.a_inv) delete[] k.a_inv[i];
        }
        delete[] k.a;
        if (k.a_inv) delete[] k.a_inv;
        delete[] k.b;
    }

    cerr << "Nu s-a putut decripta cu matricile standard\n";
}

// Functii auxiliare pentru calcul matricial
int** calcul_minor(int** matrice, int lin, int col, int n) {
    int** a = new int* [n - 1];
    for (int i = 0; i < n - 1; i++) {
        a[i] = new int[n - 1];
    }

    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1; j++) {
            if (i < lin) {
                if (j < col)
                    a[i][j] = matrice[i][j];
                else
                    a[i][j] = matrice[i][j + 1];
            }
            else {
                if (j < col)
                    a[i][j] = matrice[i + 1][j];
                else
                    a[i][j] = matrice[i + 1][j + 1];
            }
        }
    }
    return a;
}

int calcul_det(int** matrice, int n) {
    if (n <= 1) return matrice[0][0];
    int S = 0;
    for (int i = 0; i < n; i++) {
        int** minor = calcul_minor(matrice, 0, i, n);
        S += matrice[0][i] * (i % 2 ? -1 : 1) * calcul_det(minor, n - 1);

        for (int j = 0; j < n - 1; j++)
            delete[] minor[j];
        delete[] minor;
    }
    return S;
}

int** calcul_matr_inv_Zn(int** a, int n, int Zn) {
    int** rez = new int* [n];
    for (int i = 0; i < n; i++)
        rez[i] = new int[n];

    int det = calcul_det(a, n);
    int inv_det = inverse(modulo(det, Zn), Zn);

    if (inv_det == -1) {
        cerr << "Matricea nu este inversabila\n";
        return nullptr;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int** minor = calcul_minor(a, j, i, n);
            rez[i][j] = modulo(inv_det * ((i + j) % 2 ? -1 : 1) * calcul_det(minor, n - 1), Zn);

            for (int k = 0; k < n - 1; k++)
                delete[] minor[k];
            delete[] minor;
        }
    }
    return rez;
}

// ================== Criptosistemul Vigenère ==================
void criptareVigenere(ifstream& in, ofstream& out, const string& key) {
    char m;
    int keyIndex = 0;
    while (in >> noskipws >> m) {
        int cod = da_cod(m);
        if (cod != -1) {
            int keyCod = da_cod(key[keyIndex % key.length()]);
            out << da_caracter(modulo(cod + keyCod, N));
            keyIndex++;
        }
        else {
            out << m;
        }
    }
}

void decriptareVigenere(ifstream& in, ofstream& out, const string& key) {
    char m;
    int keyIndex = 0;
    while (in >> noskipws >> m) {
        int cod = da_cod(m);
        if (cod != -1) {
            int keyCod = da_cod(key[keyIndex % key.length()]);
            out << da_caracter(modulo(cod - keyCod, N));
            keyIndex++;
        }
        else {
            out << m;
        }
    }
}

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a % b);
}

void decriptareAfinaBloc(ifstream& in, ofstream& out) {
    // Citim tot textul criptat
    string ciphertext;
    char c;
    while (in >> noskipws >> c) {
        ciphertext += c;
    }

    // Încercăm diferite matrici până găsim una validă
    const int test_matrices[][2][2] = {
        {{1, 0}, {0, 1}},
        {{1, 1}, {0, 1}},
        {{1, 0}, {1, 1}},
        {{1, 1}, {1, 2}},
        {{2, 1}, {1, 1}},
        {{3, 5}, {1, 2}}
    };

    for (int m = 0; m < 6; m++) {
        cheieAfina k;
        k.n = 2;
        k.a = new int* [2];
        k.b = new int[2] {0, 0}; // Vector nul

        for (int i = 0; i < 2; i++) {
            k.a[i] = new int[2];
            for (int j = 0; j < 2; j++) {
                k.a[i][j] = test_matrices[m][i][j];
            }
        }

        k.a_inv = calcul_matr_inv_Zn(k.a, 2, N);
        if (!k.a_inv) continue;

        // Testăm decriptarea pe primele câteva caractere
        bool valid = true;
        int tested = 0;

        for (size_t i = 0; i < min(ciphertext.size(), size_t(20)); i += 2) {
            if (i + 1 >= ciphertext.size()) break;

            int y1 = da_cod(ciphertext[i]);
            int y2 = da_cod(ciphertext[i + 1]);

            if (y1 == -1 || y2 == -1) continue;

            int x1 = modulo(k.a_inv[0][0] * (y1 - k.b[0]) + k.a_inv[0][1] * (y2 - k.b[1]), N);
            int x2 = modulo(k.a_inv[1][0] * (y1 - k.b[0]) + k.a_inv[1][1] * (y2 - k.b[1]), N);

            char dc1 = da_caracter(x1);
            char dc2 = da_caracter(x2);

            if (!isalpha(dc1) || !isalpha(dc2)) {
                valid = false;
                break;
            }
            tested++;
        }

        if (valid && tested > 0) {
            // Decriptăm tot textul
            for (size_t i = 0; i < ciphertext.size(); i += 2) {
                if (i + 1 >= ciphertext.size()) {
                    out << ciphertext[i];
                    break;
                }

                int y1 = da_cod(ciphertext[i]);
                int y2 = da_cod(ciphertext[i + 1]);

                if (y1 != -1 && y2 != -1) {
                    int x1 = modulo(k.a_inv[0][0] * (y1 - k.b[0]) + k.a_inv[0][1] * (y2 - k.b[1]), N);
                    int x2 = modulo(k.a_inv[1][0] * (y1 - k.b[0]) + k.a_inv[1][1] * (y2 - k.b[1]), N);
                    out << da_caracter(x1) << da_caracter(x2);
                }
                else {
                    out << ciphertext[i] << ciphertext[i + 1];
                }
            }

            // Eliberăm memoria
            for (int i = 0; i < 2; i++) {
                delete[] k.a[i];
                delete[] k.a_inv[i];
            }
            delete[] k.a;
            delete[] k.a_inv;
            delete[] k.b;

            return;
        }

        // Eliberăm memoria dacă nu e valid
        for (int i = 0; i < 2; i++) {
            delete[] k.a[i];
            if (k.a_inv) delete[] k.a_inv[i];
        }
        delete[] k.a;
        if (k.a_inv) delete[] k.a_inv;
        delete[] k.b;
    }

    cerr << "Nu s-a putut decripta cu matricile standard\n";
}
