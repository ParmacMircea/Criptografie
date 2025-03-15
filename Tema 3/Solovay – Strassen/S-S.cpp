#include <iostream>
using namespace std;

int powmod(int a, int b, int n) {
    int result = 1;
    a = a % n;
    while (b > 0) {
        if (b % 2 == 1)
            result = (result * a) % n;
        a = (a * a) % n;
        b = b / 2;
    }
    return result;
}

int Simbol_Jacobi(int a, int n) {
    if (n <= 0 || n % 2 == 0) return 0;
    int r = 1;
    a = a % n;
    while (a != 0) {
        while (a % 2 == 0) {
            a = a / 2;
            if (n % 8 == 3 || n % 8 == 5)
                r = -r;
        }
        int temp = a;
        a = n;
        n = temp;
        if (a % 4 == 3 && n % 4 == 3)
            r = -r;
        a = a % n;
    }
    if (n == 1)
        return r;
    else
        return 0;
}

int TEST_Solovay_Strassen(int n, int attempts) {
    if (n == 2) return 1;
    if (n < 2 || n % 2 == 0) return 0;

    for (int i = 0; i < attempts; i++) {
        int b = 2 + (i * 3) % (n - 2);
        int j = Simbol_Jacobi(b, n);
        if (j == 0 || powmod(b, (n - 1) / 2, n) != (j + n) % n)
            return 0;
    }
    return 1;
}

int main() {
    int n, k;
    cout << "Test Solovay-Strassen (primalitate)\n";
    cout << "Introdu numarul n: ";
    cin >> n;
    cout << "Numar de incercari (martori): ";
    cin >> k;

    if (TEST_Solovay_Strassen(n, k) == 1)
        cout << "Numarul " << n << " este probabil prim.\n";
    else
        cout << "Numarul " << n << " este compus.\n";

    return 0;
}
