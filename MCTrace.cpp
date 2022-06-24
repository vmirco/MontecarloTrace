#include <iostream>
#include <time.h>
#include <vector>
#include <cmath>
using namespace std;
#define SIZE 300 //Dimensione delle matrici considerate

//FUNZIONE PER IL CALCOLO DELLA MATRICE TRASPOSTA
void trasposta(int matrix[][SIZE], int matrixT[][SIZE]){
  for(int row = 0; row < SIZE; row++)
    for(int col = 0; col < SIZE; col++)
      matrixT[col][row] = matrix[row][col];
}

//PRODOTTO TRA MATRICI
void product(int matrix1[][SIZE], int matrix2[][SIZE], int product[][SIZE]){
  for(int i = 0; i < SIZE; i++){
    for(int j = 0; j < SIZE; j++){
      product[i][j] = 0;
      for(int k = 0; k < SIZE; k++)
        product[i][j] += matrix1[i][k] * matrix2[k][j];
      }
    }
}

//ORACOLO PER IL CALCOLO DELLA TRACCIA CON MCTrace
int oracolo(int matrice[][SIZE], int vettore[SIZE]){
  int intermedio[SIZE] = {0};
  int result = 0;
  for(int i = 0; i < SIZE; i++)
    for(int j = 0; j < SIZE; j++)
      intermedio[i] += matrice[j][i]*vettore[j];

  for(int k = 0; k < SIZE; k++)
    result += intermedio[k]*vettore[k];

  return result;
}

//CALCOLO LA MATRICE SEMIPOSITIVA
void semi_positive(int B[][SIZE], int A[][SIZE]){
  int BT[SIZE][SIZE];
  trasposta(B, BT);
  product(BT,B,A);
}

//CALCOLO DELLA TRACCIA (da definizione)
int traccia_std(int matrice[][SIZE]){
  int counter = 0;
  int traccia = 0;
  while(counter < SIZE){
    traccia += matrice[counter][counter];
    counter++;
  }
  return traccia;
}

//CALCOLO DELLA NORMA DI FROBENIUS (da definizione)
long int frobenius(int matrice[][SIZE]){
  long int A2 = 0;
  for(int row = 0; row < SIZE; row++)
    for(int col = 0; col < SIZE; col++)
      A2 += matrice[row][col]*matrice[row][col];

  return sqrt(A2);
}

/*FUNZIONE ALGORITMO MCtrace
Campiono un vettore di Rademacher casualmente e calcolo,
tramite la funzione "oracolo", uT*A*u.
Infine restituisco la stima della traccia ottenuta.  */
int MCTrace(int matrix[][SIZE], int M){
  int stima = 0;
  srand(time(NULL));
  for(int m = 0; m < M; m++) {
    int rademacher[SIZE] = { 0 };
    for(int j = 0; j < SIZE; j++){
      int index = rand()%2;
      if(index == 0) rademacher[j] = -1;
      else rademacher[j] = 1;
    }
    int oracle = oracolo(matrix, rademacher);
    if(m == 0) stima = oracle;
    else stima += (oracle - stima)/m;
  }
  return stima;
}

//CALCOLO VARIANZA
double varianza (vector<double> res, double media){
  double var = 0.0;
  for(int i = 0; i < res.size(); i++)
    var += ((res.at(i)-media)*(res.at(i)-media));
  var /= res.size();
  return var;
}

int main() {
  int matrice[SIZE][SIZE];
  int semipositiva[SIZE][SIZE] = {0};
  srand(time(NULL));
  for(int row = 0; row < SIZE; row++)
    for(int col = 0; col < SIZE; col++)
      matrice[row][col] = rand()%2;

  //INIZIALIZZAZIONE VARIABILI E VETTORE RISULTATI
  int traccia = 0;
  long double media = 0;
  long int frob_var = 0;
  vector<double> res;

  //M = 5
  for(int i = 0; i < 100; i++){
    semi_positive(matrice, semipositiva);
    traccia = traccia_std(semipositiva);
    res.push_back(MCTrace(semipositiva, 5));
  }
  //CALCOLI DA DEFINIZIONE
  frob_var = frobenius(semipositiva);
  cout << "Norma di Frobenius: " << frob_var << endl;
  cout << "Traccia effettiva: " << traccia << endl;
  //MEDIA CAMPIONARIA
  for(int j = 0; j < 100; j++) media += res.at(j);
  cout << endl << "Media campionaria(M = 5): " << media/100 << endl;
  //ANALISI RISULTATI
  cout << "Varianza: " << varianza(res,media/100) << endl;
  double frobM = 2*frob_var*frob_var/5;
  cout << "2*Frobenius/M: " << frobM << endl;

  //RESET
  media = 0;
  cout << endl;
  res.clear();

  //M = 10
  for(int i = 0; i < 100; i++){
    semi_positive(matrice, semipositiva);
    res.push_back(MCTrace(semipositiva, 10));
  }
  //MEDIA CAMPIONARIA
  for(int j = 0; j < 100; j++) media += res.at(j);
  cout << "Media campionaria(M = 10): " << media/100 << endl;
  //ANALISI RISULTATI
  cout << "Varianza: " << varianza(res,media/100) << endl;
  frobM = 2*frob_var*frob_var/10;
  cout << "2*Frobenius/M: " << frobM << endl;

  //RESET
  media = 0;
  cout << endl;
  res.clear();

  //M = 25
  for(int i = 0; i < 100; i++){
    semi_positive(matrice, semipositiva);
    res.push_back(MCTrace(semipositiva, 25));
  }
  //MEDIA CAMPIONARIA
  for(int j = 0; j < 100; j++) media += res.at(j);
  cout << "Media campionaria(M = 25): " << media/100 << endl;
  //ANALISI RISULTATI
  cout << "Varianza: " << varianza(res,media/100) << endl;
  frobM = 2*frob_var*frob_var/25;
  cout << "2*Frobenius/M: " << frobM << endl;
  //RESET
  media = 0;
  cout << endl;
  res.clear();

  //M = 100
  for(int i = 0; i < 100; i++){
    semi_positive(matrice, semipositiva);
    res.push_back(MCTrace(semipositiva, 200));
  }
  //MEDIA CAMPIONARIA
  for(int j = 0; j < 100; j++) media += res.at(j);
  cout << "Media campionaria(M = 100): " << media/100 << endl;
  //ANALISI RISULTATI
  cout << "Varianza: " << varianza(res,media/100) << endl;
  frobM = 2*frob_var*frob_var/200;
  cout << "2*Frobenius/M: " << frobM << endl;
}
