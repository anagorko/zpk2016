#include <iostream>
#include <chrono>
#include <cstdio>
using namespace std;

void swap(int &a, int &b)
{
    int c = a; a = b; b = c;
}

// sortuje t[p] .. t[k - 1] - bez t[k] !
void bubble_sort(int t[], int p, int k)
{
    /*
    //bubble sort: loops n^2 times
    for(int j = p; j < k - 1; j++) {
        for(int i = p; i < k - 1; i++) {
            if(t[i] > t[i + 1]) {swap(t[i], t[i + 1]);}
        }
    }
    */

    /*
    //bubble sort: remembers if did an action - stops if no action is taken
    bool swapped = true;
    while(swapped) {
    swapped = false;
        for(int i = p; i < k - 1; i++) {
            if(t[i] > t[i + 1]) {swap(t[i], t[i + 1]); swapped = true;}
        }
    }
    */

    /*
    //bubble sort: assumes last element is on its position
    int sunk = 0; //number of sunken elements
    for(int j = p; j < k - 1; j++) {
        for(int i = p; i < k - 1 - sunk; i++) {
            if(t[i] > t[i + 1]) {swap(t[i], t[i + 1]);}
        }
        sunk++;
    }
    */

    //bubble sort: remembers last swap position, 
    //then calculates number of sunk elements
    //number of sunk elements
    while(k > 1){
        int last_swap = p;
        for(int i = p; i < k - 1; i++) {
            if(t[i] > t[i + 1]) {swap(t[i], t[i + 1]);last_swap = i;}
        }
        k = last_swap + 1;
    }
}

// sortuje t[p] .. t[k - 1] - bez t[k] !

bool is_sorted(int t[], int p, int k) {
    for(int i = p; i < k-1; i++) {
        if(t[i] > t[i+1]) {
            //cout << "unsorted: " << p << ":" << k << endl;
            return false;
        }
    }
    //cout << "sorted: " << p << ":" << k << endl;
    return true;
}

void do_merge(int t[], int p, int half, int k) {
    //cout << "do_merge init" << p << k << endl;
    int length = k - p;
    if(length == 1) {return;}
    int length1 = length / 2;
    int length2 = length - length1;

    int* tmp1 = new int[length1];
    int* tmp2 = new int[length2];

    for(int i = 0; i < length1 ; i++) {
        tmp1[i] = t[p + i];
        //cout << "part1" << endl;
        //cout << i << tmp1[i] << endl;
    }

    for(int i = 0; i < length2; i++) {
        tmp2[i] = t[half + i];
        //cout << "part2" << endl;
        //cout << i << tmp2[i] << endl;
    }
    
    int t1 = 0;
    int t2 = 0;
    //cout << "sorted_part: ";
    for(int i = p; i < k; i++) {
        if(t1==length1){
            //cout << "part1empty" << endl;
            t[i] = tmp2[t2];
            //cout << tmp2[t2] << " ";
            t2++;
            continue;
        }
        if(t2==length2){
            //cout << "part2empty" << endl;
            t[i] = tmp1[t1];
            //cout << tmp1[t1] << " ";
            t1++;
            continue;
        }
        if(tmp1[t1] <= tmp2[t2]) {
            t[i] = tmp1[t1];
            //cout << tmp1[t1] << " ";
            t1++;
        } else {
            t[i] = tmp2[t2];
            //cout << tmp2[t2] << " ";
            t2++;
        }
    }
    //cout << endl;
    delete[] tmp1;
    delete[] tmp2;
}

void merge_sort(int t[], int p, int k)
{
    if(!is_sorted(t, p, k)) {
        int half = p + (k-p) / 2;
        if(p != half) {
            merge_sort(t, p, half);
            merge_sort(t, half, k);
        }
        do_merge(t, p, half, k);
    }
}

void test(int n)
{
    int t1[n], t2[n];
    
    for (int i = 0; i < n; i++) {
        t1[i] = t2[i] = rand();
        //cout << t1[i] << endl;
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
    
    cout << "  Bubble sort time: " 
         << chrono::duration <double> (diff_bubble).count() 
         << endl;
    cout << "  Merge sort time: " 
         << chrono::duration <double> (diff_merge).count() 
         << endl;
    cout << "  Ratio: " 
         << (chrono::duration <double> (diff_bubble).count() / chrono::duration <double> (diff_merge).count()) 
         << endl;
}

int main()
{   test(60);
    test(100);
    test(1000);
    test(10000);
    test(20000);
    test(30000);
    test(50000);
    test(100000);
    
}
