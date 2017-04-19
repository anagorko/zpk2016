#ifndef BOOK_H
#define BOOK_H
#include "person.h"
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

class book {

    private:
        vector<person> v;
    protected:
    public:

        book();

        void add(person &p);
        void c_add(); //adds person from console

        person& get(int k); //gets person at k's position
        person& operator[](int k); //same

        void display(int k);
        void display_all();

        void save(string file);
        void load(string file);

        void c_modify(int k); // modifies k's entry from console

        void correct_ids();

        void sort_by_name();

        void remove(int k);

};

#endif
