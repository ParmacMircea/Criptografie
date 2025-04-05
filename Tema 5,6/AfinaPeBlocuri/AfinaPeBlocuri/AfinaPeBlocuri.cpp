#include"crypto.h"
int main()
{
	citeste_alfabet(ifstream("alfabet.txt"));
	//cout << N << endl;
	//for (int i = 0; i < N; i++)
		//cout << alfabet[i];
	//criptareCezar(ifstream("sursa.txt"),ofstream("destinatie.txt"), -1);
	ifstream in("sursa.txt");
	caracter* frecv = frecvente(in);
	char cheie = frecv[0].c;
	char cheie1 = frecv[1].c;
	//count<<cheie<<" "<<cheie1<endl;
	cout << da_cod(cheie);
	in.clear();
	in.seekg(0);
	//criptareCezar(in , ofstream("destinatie.txt"), modulo(da_cod(' ') - da_cod(cheie),N));
	int ap = modulo(invers(modulo(da_cod(cheie) - da_cod(cheie1), N), N) * (da_cod(' ') - da_cod('a')),N);
	int bp = modulo(da_cod(' ') - ap * da_cod(cheie), N);
	//count << ap << " " << bp << endl;
	//modulo(da_cod(' ')-da_cod(cheie),N));
	//criptareAfina(in, ofstream("destinatie.txt"), ap, bp);
	int** a = new int* [2];
	int* b = new int[2];
	for (int i = 0; i < 2; i++)
		a[i] = new int[2];
	a[0][0] = 2;
	a[0][1] = 3;
	a[1][0] = 7;
	a[1][1] = 8;
	b[0] = 0;
	b[1] = 0;
	cheieAfina k;
	k.n = 2;
	k.a = a;
	k.b = b;
	//criptareAfina(in, ofstream("destinatie.txt"), k);
	decriptareAfina(in, ofstream("destinatie.txt"), k);
	in.close();
}