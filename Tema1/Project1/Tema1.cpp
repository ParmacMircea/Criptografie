#include<iostream>
using namespace std;

// Functia pentru conversia unui caracter in valoarea sa numerica
unsigned int caracterInValoare(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    return 0;
}

// Functia pentru conversia unui numar din baza b1 in baza 10
unsigned int convertireIn10Dinb1(const char n[], unsigned int b)
{
    unsigned int rezultat = 0;
    unsigned int prod = 1;
    int i = 0;

    while (n[i] != '\0')
    {
        i++;
    }

    for (int j = i - 1; j >= 0; --j)
    {
        unsigned int r = caracterInValoare(n[j]);
        rezultat += r * prod;
        prod *= b;
    }
    return rezultat;
}

// Functia pentru conversia unui numar din baza 10 în baza b2
void convertireDin10Inb2(unsigned int n, unsigned int b, char rezultat[])
{
    int index = 0;

    if (n == 0)
    {
        rezultat[index++] = '0';
        rezultat[index] = '\0';
        return;
    }

    while (n > 0)
    {
        unsigned int rest = n % b;
        if (rest < 10)
            rezultat[index++] = '0' + rest;
        else
            rezultat[index++] = 'A' + (rest - 10);
        n /= b;
    }
    rezultat[index] = '\0';

    for (int i = 0; i < index / 2; ++i)
    {
        char temp = rezultat[i];
        rezultat[i] = rezultat[index - i - 1];
        rezultat[index - i - 1] = temp;
    }
}

int main()
{
    char numarInBazab[50];
    unsigned int b1, b2, numarInb10;
    char rezultatInb2[50];

    // Citire date
    cout << "Introduceti numarul n (in baza b1)" << endl;
    cin >> numarInBazab;
    cout << "Introduceti baza b1 (2-26)" << endl;
    cin >> b1;
    cout << "Introduceti baza b2 (2-26)" << endl;
    cin >> b2;

    // Conversia numarului din baza b1 in baza 10
    numarInb10 = convertireIn10Dinb1(numarInBazab, b1);

    // Conversia numarului din baza 10 in baza b2
    convertireDin10Inb2(numarInb10, b2, rezultatInb2);

    // Afisare rezultat
    cout << "Numarul " << numarInBazab << " din baza " << b1 << " este " << rezultatInb2 << " in baza " << b2 << endl;

    return 0;
}
