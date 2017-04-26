#include<iostream>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<string>
#include<vector>

using namespace std;

class Person
{
string name, surname, email, number;
int id;

public:
    Person() {}
    Person(string name_, string surname_, string email_, string number_, int id_)
    {
    name = name_;
    surname = surname_;
    email = email_;
    number = number_;
    id = id_;
    }

    void load (int i)
    {
        string name_, surname_, email_, number_;
        cout << "Name: ";
        cin >> name_;
        cout << "Surname: ";
        cin >> surname_;
        cout << "Email: ";
        cin >> email_;
        cout << "Number: ";
        cin >> number_;
        name = name_;
        surname = surname_;
        email = email_;
        number = number_;
        id = i;
    }

    int getId() const{return id;}

    string getName() const{return name;}

    string getSurname() const{return surname;}

    string getEmail() const{return email;}

    string getNumber(){return number;}

    friend ostream& operator << (ostream&, Person&);
};

    ostream & operator << (ostream &os, Person &p)
    {
        cout << p.id << " " << p.name << " " << p.surname << " " << p.email << " " << p.number << endl;
        return os;
    }


class Adressbook
{
    public:
    vector<Person> book;
    Adressbook() {}
    void add();
    void Delete();
    void modify();
    void sort();
    void sort_surname();
    void sort_email();
    void list();
    void read(string name);
    void write(string name);
};


    void Adressbook::add()
    {
        Person p;
        int id_;
        if (book.size()==0) id_ = -1;
        else id_ = book[book.size()-1].getId();
        p.load(id_ + 1);
        book.push_back(p);
    }

    void Adressbook::Delete()
    {
        cout << "Provide id -> deletion" << endl;
        int i;
        cin >> i;
        book.erase(book.begin()+i);
    }

    void Adressbook::modify()
    {
        cout << "Provide id -> modification" << endl;
        int i;
        cin >> i;
        Person p;
        p.load(i);
        book[i] = p;
    }

    void Adressbook::list ()
    {
        for (int i=0; i<book.size();i++)
        {
            if(book[i].getId() != -1)
            cout << book[i];
        }
    }

    void Adressbook::sort()
    {
        vector<Person> v = book;
        std::sort(begin(book), end(book), [] (const Person& l, const Person& r) { return l.getName() < r.getName(); });
        list();
        book = v;
    }

        void Adressbook::sort_surname()
    {
        vector<Person> v = book;
        std::sort(begin(book), end(book), [] (const Person& l, const Person& r) { return l.getSurname() < r.getSurname(); });
        list();
        book = v;
    }

        void Adressbook::sort_email()
    {
        vector<Person> v = book;
        std::sort(begin(book), end(book), [] (const Person& l, const Person& r) { return l.getEmail() < r.getEmail(); });
        list();
        book = v;
    }

    void Adressbook::read(string name)
    {
        name.append(".txt");
        string line;
        ifstream file (name.c_str());
        while (getline(file, line))
        {
            istringstream iss(line);
            int nr;
            iss >> nr;
            int id = nr;
            string word;
            vector<string> D(4);
            for (int i=0; i<=3; i++){iss >> word; D[i] = word;}
            string name = D[0];
            string surname = D[1];
            string email = D[2];
            string number = D[3];
            Person p(name, surname, email, number, id);
            book.push_back(p);
        }
        file.close();
    }

    void Adressbook::write(string name)
    {
        ofstream file ;
        file.open (name.c_str(), ios::trunc);
        for (int i=0; i<book.size(); i++)
        {
            file << book[i].getId();
            file << " ";
            file << book[i].getName();
            file << " ";
            file << book[i].getSurname();
            file << " ";
            file << book[i].getEmail();
            file << " ";
            file << book[i].getNumber();
            file << "\n";
        }
    }

class App
{
    Adressbook a;
    string Bookname;

public:

 App() {}

 void open();
 void func();

};

   void App::open()
    {
        int abc;
        cout << "Which adress book you want to use (e.g. Nowak)" << endl;
        string ab;
        cin >> ab ;
        a.read(ab);
        ab.append(".txt");
        Bookname = ab;
    }

    void App::func()
    {
        while(1)
        {
            cout << "What next: 0-print 1()-sort and print (name:1 surname:2 email:3 e.g. 11 ), 2-modify, 3-delete, 4-add, 5-end session" << endl;
            int odczyt;
            cin >> odczyt;
            switch(odczyt){
            case 0 : { a.list() ; break;}
            case 1 : { a.sort(); break;}
            case 11 : { a.sort(); break;}
            case 12 : { a.sort_surname(); break;}
            case 13 : { a.sort_email(); break;}
            case 2 : { a.modify();a.write(Bookname.c_str()); break;}
            case 3 : { a.Delete();a.write(Bookname.c_str()); break;}
            case 4 : { a.add();a.write(Bookname.c_str()); break;}
            case 5 : { exit(0); break;}
            }
            }

    }

int main()
{
App u;
u.open();
u.func();

return 0;
}
