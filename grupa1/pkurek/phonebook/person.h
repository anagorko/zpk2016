#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class person {
    private:
        int id;
        string name;
        string surname;
        int phone;
        string email;
    protected:
    public:
        person();

        person(int _id, 
               string _name, 
               string _surname, 
               int _phone, 
               string _email);

        int    get_id();
        string get_name();
        string get_surname();
        int    get_phone();
        string get_email();

        void set_id(int _id);
        void set_name(string _name);
        void set_surname(string _surname);
        void set_phone(int _phone);
        void set_email(string _email);

        void print();

        friend istream& operator>>(istream&, person&);
        friend ostream& operator<<(ostream&, const person&);

}; 

#endif
