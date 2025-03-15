#include <iostream>
using namespace std;

int simbol_Jacobi(int a, int n) {
    if (n <= 0 || n % 2 == 0) return 0; // n trebuie sa fie impar și pozitiv
    int rezultat = 1;
    a = a % n;

    while (a != 0) {
        while (a % 2 == 0) {
            a = a / 2;
            if (n % 8 == 3 || n % 8 == 5)
                rezultat = -rezultat;
        }

        // aplicam reciprocitatea patratica
        int temp = a;
        a = n;
        n = temp;

        if (a % 4 == 3 && n % 4 == 3)
            rezultat = -rezultat;

        a = a % n;
    }

    if (n == 1)
        return rezultat;
    else
        return 0;
}

int main() {
    int a, n;
    cout << "Calcul simbol Jacobi (a/n)\n";
    cout << "Introdu a: ";
    cin >> a;
    cout << "Introdu n (impar si pozitiv): ";
    cin >> n;

    int r = simbol_Jacobi(a, n);
    cout << "Simbolul Jacobi (" << a << "/" << n << ") = " << r << endl;

    return 0;
}
