#include <iostream>
#include <chrono>
#include <cstdio>
#include <random>
#include <string.h>
using namespace std;

void swap(int &a, int &b)
{
    int c = a; a = b; b = c;
}

// sortuje t[p] .. t[k - 1] - bez t[k] !
void bubble_sort( int t[], int p, int k)
{
    for( int i = p; i < k; i++ )
    {
        for( int j = p; j < k - 1; j++ )
        {
            if( t[ j ] > t[ j + 1 ] )
                 swap( t[ j ], t[ j + 1 ] );
        }
    }
}

/*
src - tablica wejściowa
dst - tablica wyjściowa
p - początek pierwszej ze scalanych części
s - koniec pierwszej części - początek drugiej
k - koniec drugiej części
*/
void merge(int *src, int* dst, int p, int s, int k){
    int i = p, j = s;
    int d = p;
    while ( d < k ){
    if ( j < k && (i == s || src[i] > src[j]) ){
        dst[d] = src[j++];
       } else {
        dst[d] = src[i++];
    }
    ++d;
    }
}

// sortuje t[p] .. t[k - 1] - bez t[k] !
void merge_sort(int *tab, int p, int k){
    int gap = 1;
    int *tab0 = tab;
    int *tab2 = new int[k];
    for (gap = 1; gap < k; gap *= 2){
    //scalamy 2 części o długości gap
    //w tab - dane wejściowe, tab2 - wyjściowe
    int i;
    for (i = 0; i < k; i += gap * 2){
        merge(tab, tab2, i, i + gap, min(i + 2 * gap, k));
    }
    int *tmp = tab2;
    tab2 = tab;
    tab = tmp;
    }
    if (tab != tab0){
    //jeśli skończyliśmy w taki sposób, że dane
    //posortowane wylądowały w tablicy tymczasowej
    //to je przenosimy
   memcpy(tab0, tab, k * sizeof(int));
    delete []tab;
    }else delete []tab2;
}

void test(int n)
{
    int t1[n], t2[n];

    for (int i = 0; i < n; i++) {
        t1[i] = t2[i] = rand();
    }

    cout << "Table size: " << n << endl;

    auto start = chrono::steady_clock::now();
    bubble_sort(t1, 0, n);
    auto end = chrono::steady_clock::now();

    auto diff_bubble = end - start;

    start = chrono::steady_clock::now();
    merge_sort(t2, 0, n);
    end = chrono::steady_clock::now();

    auto diff_merge = end - start;

    for (int i = 0; i < n - 1; i++) {
        if (t1[i] > t1[i + 1]) {
            cout << "  Wrong answer - bubble sort." << endl; break;
        }
        if (t2[i] > t2[i + 1]) {
            cout << "  Wrong answer - merge sort." << endl; break;
        }
    }

    cout << "  Bubble sort time: " << chrono::duration <double> (diff_bubble).count() << endl;
    cout << "  Merge sort time: " << chrono::duration <double> (diff_merge).count() << endl;
    cout << "  Ratio: " << (chrono::duration <double> (diff_bubble).count() / chrono::duration <double> (diff_merge).count()) << endl;
}

int main()
{
    test(100);
    test(1000);
    test(10000);
    test(20000);
    test(30000);
    test(50000);
    test(100000);

    return 0;
}
