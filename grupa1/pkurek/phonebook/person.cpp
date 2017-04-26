#include "person.h"
#include <iostream>
using namespace std;

person::person() {

}

person::person(int _id, 
               string _name, 
               string _surname, 
               int _phone, 
               string _email)
{
    id = _id;
    name = _name;
    surname = _surname;
    phone = _phone;
    email = _email;
}

int person::get_id() {
    return id;
}

string person::get_name() {
    return name;
}

string person::get_surname() {
    return surname;
}

int person::get_phone() {
    return phone;
}

string person::get_email() {
    return email;
}

void person::set_id(int _id) {
    id = _id;
}

void person::set_name(string _name) {
    name = _name;
}

void person::set_surname(string _surname) {
    surname = _surname;
}

void person::set_phone(int _phone) {
    phone = _phone;
}

void person::set_email(string _email) {
    email = _email;
}

void person::print() {
    cout << "Id: " << id
         << " name: " << name
         << " surname: " << surname
         << " phone: " << phone
         << " email: " << email
         << endl;
}

istream& operator>>(istream& is, person& p) {
    is >> p.name >> p.surname >> p.phone >> p.email; 
    return is;
}

ostream& operator<<(ostream& os, const person& p) {
    return os << p.id << " "
              << p.name << " "
              << p.surname << " "
              << p.phone << " "
              << p.email << "\n";
}

















