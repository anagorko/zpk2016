#include "book.h"

book::book() {
    //cout << "book constructor called";
}

void book::add(person &p) {
    v.push_back(p);
    v.back().set_id(v.size() - 1);
}

void book::c_add() {
    cout << "Enter: name surname phone_number email:" << endl;
    person p;
    cin >> p;
    v.push_back(p);
    v.back().set_id(v.size() - 1);
    cout << "record added" << endl;
}

person& book::get(int k) {
    return v[k];
}

person& book::operator[](int k) {
    return v[k];
}

void book::display(int k) {
    cout << get(k);
}

void book::display_all(){
    for(int i=0; i<v.size();i++) {
        display(i);
    }
}

void book::save(string file) {
    ofstream bookfile;
    bookfile.open(file, ios::trunc);
    for(int i=0;i<v.size();i++) {
        bookfile << get(i);
    }
    bookfile.close();
}

void book::load(string file) {
    if(v.size()>0) {v.clear();}
    fstream f;
    f.open(file, ios::in);
    if(!f.is_open()) {
        cout << "Error: file couldnt be opened" << endl;
        return;
    }

    person tmp;
    int id;
    while(f >> id >> tmp) {
        add(tmp);
    }
    f.close();
}

void book::c_modify(int k) {

    cout << "Modifying entry:" << endl;
    cout << get(k) << endl;

    cout << "Enter: name surname phone_number email:" << endl;
    person tmp;
    cin >> tmp;
    v[k] = tmp;
    v[k].set_id(k);

}

void book::correct_ids() {
    for(int i=0; i<v.size(); i++) {
        v[i].set_id(i);
    }
}

bool compare_names(person& l, person& r) {
    return l.get_name() < r.get_name();
}

void book::sort_by_name() {
    sort(v.begin(), v.end(), compare_names);
    correct_ids();
}

void book::remove(int k) {
    v.erase(k);
}








