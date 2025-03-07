#include <iostream>
using namespace std;

// Functie pentru a calcula CMMDC folosind algoritmul lui Euclid
int cmmdc(int a, int b) {
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

// Functie pentru a calcula phi(n) prin numararea numerelor coprime cu n
int phi(int n) {
    int count = 0;

    for (int k = 1; k <= n; k++) {
        if (cmmdc(k, n) == 1) {
            count++;
        }
    }

    return count;
}

int main() {
    int n;
    cout << "Introduceti un numar: ";
    cin >> n;

    cout << "phi(" << n << ") = " << phi(n) << endl;
    return 0;
}
