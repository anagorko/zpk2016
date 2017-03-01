#include <iostream>

using namespace std;
int n, m;
int c[100][100];

int cena(int a, int b)
{
  int t = 0;
  for (int i = 0; i < m; i++) {
    if (c[a][i] < c[b][i]) {
      t += c[a][i];        
    } else {
      t += c[b][i];
    }
  }
  return t;
}

int main()
{    
  // czytanie danych    
  cin >> n >> m;    
  for (int i = 0; i < n; i++) {        
    for (int j = 0; j < m; j++) {            
      cin >> c[i][j];        
    }    
  }    
  
  int w = 100 * 1000 + 1;   
  for (int a = 0; a < n; a++) {   
    for (int b = a + 1; b < n; b++) {   
      int x = cena(a,b);  
      if (x < w) {    
        w = x;  
      }   
    } 
  } 
  cout << w << endl; 
  return 0;
}
