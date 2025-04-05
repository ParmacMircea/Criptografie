#pragma once
#include<iostream>
#include<fstream>
using namespace std;
int modulo(int a, int n)
{
	if (a >= 0 && n > 0)
		return a % n;
	if (a < 0 && n>0)
		return (a % n) + n;
	if (n == 0)
		return -1;
}
int invers(int a, int n) {
	int q, r, x0 = 1, x1 = 0, copy_n = n;
	a = modulo(a, n);
	while (n != 0)
	{
		r = n;
		q = a / n;
		n = a % n;
		a = r;
		r = x1;
		x1 = x0 - q * x1;
		x0 = r;
	}
	if (a == 1)
		return modulo(x0, copy_n);
	return -1;
}

char alfabet[100];
int N = 0;
int da_cod(char c)
{
	for (int i = 0; i < N; i++)
		if (alfabet[i] == c)
			return i;
}
char da_caracter(int cod)
{
	return alfabet[modulo(cod, N)];
}
void citeste_alfabet(ifstream in)
{
	char c;
	while (in >> noskipws >> c)
	{
		alfabet[N++] = c;
	}
	if (N == 0)
		cout << "Alfabetul nu are caractere";
}
void criptareCezar(ifstream& in, ofstream out, int k)
{
	char c;
	while (in >> noskipws >> c)
	{
		out << da_caracter(da_cod(c) + k);
	}
}
/*void criptareAfina(ifstream& in, ofstream out, int a, int b)
{
	char c;
	while (in >> noskipws >> c)
	{
		out << da_caracter(da_cod(c)*a + b);
	}
}*/

struct caracter
{
	char c;
	int f;
};

caracter* frecvente(ifstream& in)
{
	caracter vector_frecvente[100] = { 0 };
	char c;
	while (in >> noskipws >> c) {
		vector_frecvente[da_cod(c)].c = c;
		vector_frecvente[da_cod(c)].f++;
	}
	bool ordonat = 0;
	while (!ordonat)
	{
		ordonat = 1;
		for (int i = 0; i < N - 1; i++)
		{
			if (vector_frecvente[i].f < vector_frecvente[i + 1].f)
			{
				ordonat = 0;
				caracter temp = vector_frecvente[i];
				vector_frecvente[i] = vector_frecvente[i + 1];
				vector_frecvente[i + 1] = temp;
			}
		}
	}
	return vector_frecvente;
}

struct cheieAfina
{
	int n;
	int** a;
	int* b;
};
void criptareAfina(ifstream& in, ofstream out, cheieAfina k)
{
	char m;
	int* M = new int[k.n];
	int* C = new int[k.n];
	int i = 0;
	while (in >> noskipws >> m)
	{
		M[i++] = da_cod(m);
		if (i == k.n)
		{
			for (int j = 0; j < k.n; ++j)
			{
				C[j] = 0;
				for (int l = 0; l < k.n; ++l)
					C[j] += k.a[j][l] * M[l] + k.b[l];
				out << da_caracter(C[j]);
			}
			i = 0;
		}

	}
}

int** calcul_minor(int** matrice, int lin, int col, int n) {
	int i, j, ** a = new int* [n - 1];
	for (i = 0; i < n - 1; i++) {
		a[i] = new int[n - 1];
	}
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - 1; j++)
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
	return a;
}

int calcul_det(int** matrice, int n) {
	if (n <= 1) return matrice[0][0];
	int S = 0;
	for (int i = 0; i < n; i++)
		S += matrice[0][i] * (i % 2 ? -1 : 1) *
		calcul_det(calcul_minor(matrice, 0, i, n), n - 1);
	return S;
}

int** calcul_matr_inv_Zn(int** a, int n, int Zn) {
	int i, j, ** rez;
	rez = new int* [n];
	for (i = 0; i < n; i++)
		rez[i] = new int[n];
	int det = calcul_det(a, n);
	int inv_determinant = invers(modulo(det, Zn), Zn);
	if (inv_determinant == -1) return nullptr;

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++) {
			rez[i][j] = modulo(inv_determinant * ((i + j) % 2 ? -1 : 1) *
				calcul_det(calcul_minor(a, j, i, n), n - 1), Zn);
		}
	return rez;
}

void decriptareAfina(ifstream& in, ofstream out, cheieAfina k) {
	int** a_inv = calcul_matr_inv_Zn(k.a, k.n, N);
	if (a_inv == nullptr) {
		cout << "Matrix is not invertible modulo " << N << endl;
		return;
	}

	char m;
	int* C = new int[k.n];
	int* M = new int[k.n];
	int i = 0;

	while (in >> noskipws >> m) {
		C[i++] = da_cod(m);
		if (i == k.n) {
			for (int j = 0; j < k.n; ++j) {
				M[j] = 0;
				for (int l = 0; l < k.n; ++l) {
					M[j] += a_inv[j][l] * modulo(C[l] - k.b[l], N);
				}
				M[j] = modulo(M[j], N);
				out << da_caracter(M[j]);
			}
			i = 0;
		}
	}
}
