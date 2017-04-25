#include <iostream>
#include <chrono>
#include <cstdio>
#include <random>
#include <vector>
using namespace std;

void BubbleSort(vector<unsigned int> &vec, int s, int k){
 for(int i=s; i<k;i++){
 for(int j=s; j<k-i-1;j++){
 if(vec[j]>vec[j+1]) swap(vec[j],vec[j+1]);
 }
 }
 }


void merge(vector<unsigned int> &arr, int l, int m, int r);

void MergeSort(vector<unsigned int> &arr, int l, int r)
{
   if (l < r)
   {

      int m = l+(r-l)/2;
      MergeSort(arr, l, m);
      MergeSort(arr, m+1, r);
      merge(arr, l, m, r);
   } else return;
}

void merge(vector<unsigned int> &arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<unsigned int> L(n1);
    vector<unsigned int> R(n2);
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0;
    int j = 0;
    int k = l;
    while (i < n1 && j < n2)
     {

          if (L[i] <= R[j]){
               arr[k]=L[i];
               i++;

          }else{
               arr[k]=R[j];
               j++;
          }
               k++;

     }

     while (i < n1)
     {
          arr[k++]=L[i++];
     }

    while (j < n2)
    {
        arr[k++]=R[j++];
    }
}

void test(int n)
{
    vector<unsigned int> t1(n);
    vector<unsigned int> t2(n);

    for (int i = 0; i < n; i++) {
        t1[i] = t2[i] = rand();
    }

    cout << "Table size: " << n << endl;

    auto start = chrono::steady_clock::now();
    BubbleSort(t1, 0, n);
    auto end = chrono::steady_clock::now();

    auto diff_bubble = end - start;

    start = chrono::steady_clock::now();
    MergeSort(t2, 0, n);
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
}
