#include<fstream>
#include<iostream>
#include<vector>
#include<algorithm>
#include <string>
using namespace std;


// Funkcja pomocnicza - odmiana liczby rekordów
string rekord(int LiczbaRekordow){
    if (LiczbaRekordow == 1)
        return "rekord";
    else if (LiczbaRekordow == 2 || LiczbaRekordow == 3 || LiczbaRekordow == 4)
        return "rekordy";
    else
        return "rekordow";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Person
{
	int id;
	string name, surname, phone, email;

public:
	Person() {}

	void setName(string _name) { name = _name; }
	void setSurname(string _surname) { surname = _surname; }
	void setPhone(string _phone) { phone = _phone; }
	void setEmail(string _email) { email = _email; }
	void setId(int _id) { id = _id; }

	string getName() const { return name; }
	string getSurname() const { return surname; }
	string getPhone() const { return phone; }
	string getEmail() const { return email; }
	int getId() const { return id; }

	/*bool operator>(const Person& p) {
		return name > p.name;
	}*/

	friend ostream& operator<<(ostream&, const Person&);
	friend istream& operator>>(istream&, Person&);
};

ostream& operator<<(ostream& os, const Person& p)
{
	return os << p.id << " " << p.name << " " << p.surname << " " << p.phone << " " << p.email << endl;
}

istream& operator>>(istream& is, Person& p)
{
	is >> p.id;
	string tmp; getline(is, tmp);

	getline(is, p.name);
	getline(is, p.surname);
	getline(is, p.phone);
	getline(is, p.email);

	return is;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Addressbook: public Person{
    vector<Person> p;
    int id = 0;
public:
    Addressbook() {};
    // Czytanie danych z pliku
    void read(string filename);
    // Zapisywanie danych do pliku
    void write(string filename);
    // Odczyt wszystkich danych
    vector<Person> getAll();
    // Dodanie nowego rekordu
    void add();
    // Wypisanie wszystkich rekordów
    void list();
    // Do modify i remove
    Person get(int id);
    // Modyfikaja wybranego id
    void askForData(Person r);
    // Usunięcie rekordu
    void remove(int id);
    // Sortowanie
    void Sort(); // po imieniu
    void Sort_Surname(); // po nazwisku
    void Sort_Phone(); // po telefonie
    void Sort_Email(); // po e-mailu
};

void Addressbook::read(string filename)
{
   	fstream f;
	// Czytanie danych
	f.open(filename, ios::in);
	if (!f.is_open()) {
		cout << "Nie moglem otworzyc pliku do odczytu." << endl;
	} else {
        Person tmp;
        string _id; string _name; string _surname; string _phone; string _email;
        while (f >> _id >> _name >> _surname >> _phone >> _email) {
                int id_int = atof(_id.c_str()); // konwersja na int
                tmp.setId(id_int); tmp.setName(_name); tmp.setSurname(_surname); tmp.setPhone(_phone); tmp.setEmail(_email);
                p.push_back(tmp);
                id = id_int;
        }
        f.close();
	}
}

void Addressbook::write(string filename){
    if (filename.find(".txt") == -1) filename.append(".txt");
    ofstream bookfile;
    bookfile.open(filename, ios::trunc);
    Person tmp;
    for (const Person &tmp: p) {
        bookfile << tmp;
    }
    bookfile.close();
}

vector<Person> Addressbook::getAll()
{
    Person tmp;
    vector<Person> q;
    for (const Person &tmp: p) {
        tmp.getId(); tmp.getName(); tmp.getSurname(); tmp.getPhone(); tmp.getEmail();
        q.push_back(tmp);
    }
    return q;
}

void Addressbook::add() {
        Person temp;
        id++;
        temp.setId(id);
        cout << "Podaj imie: "; string _name; cin >> _name; temp.setName(_name);
        cout << "Podaj nazwisko: "; string _surname; cin >> _surname; temp.setSurname(_surname);
        cout << "Podaj telefon: "; string _phone; cin >> _phone; temp.setPhone(_phone);
        cout << "Podaj email: "; string _email; cin >> _email; temp.setEmail(_email);
        p.push_back(temp);
}

void Addressbook::list() {
    Person tmp;
    for (const Person &tmp: p) {
        cout << tmp.getId() << ": " << tmp.getName() << " " << tmp.getSurname() << " " << tmp.getPhone() << " " << tmp.getEmail() << endl;
    }
}

Person Addressbook::get(int id) {
    Person tmp;
    for (const Person &tmp: p) {
        if (tmp.getId() == id)
            return tmp;
    }
}

void Addressbook::askForData(Person r) {
    // Imię
    cout << "Podaj imie: ";
    string _name;
    if (r.getName() == "" || r.getName() == "-")
        {cin >> _name ; r.setName(_name);}
    else
        cout << "[" + r.getName() + "]" << endl;
    // Nazwisko
    cout << "Podaj nazwisko: ";
    string _surname;
    if (r.getSurname() == "" || r.getSurname() == "-")
        {cin >> _surname ; r.setSurname(_surname);}
    else
        cout << "[" + r.getSurname() + "]" << endl;
    // Telefon
    cout << "Podaj telefon: ";
    string _phone;
    if (r.getPhone() == "" || r.getPhone() == "-")
        {cin >> _phone ; r.setPhone(_phone);}
    else
        cout << "[" + r.getPhone() + "]" << endl;
    // Email
    cout << "Podaj email: ";
    string _email;
    if (r.getEmail() == "" || r.getEmail() == "-")
        {cin >> _email ; r.setEmail(_email);}
    else
        cout << "[" + r.getEmail() + "]" << endl;

    // Zapis zmienionych danych w wektorze p (wektor osób)
    p[r.getId() - 1] = r;
}

void Addressbook::remove(int id) {
    p.erase(p.begin() - 1 + id);
}

void Addressbook::Sort() {
	sort(p.begin(), p.end(), [] (const Person& lhs, const Person& rhs) { return lhs.getName() < rhs.getName(); });
}
void Addressbook::Sort_Surname() {
	sort(p.begin(), p.end(), [] (const Person& lhs, const Person& rhs) { return lhs.getSurname() < rhs.getSurname(); });
}
void Addressbook::Sort_Phone() {
    sort(p.begin(), p.end(), [] (const Person& lhs, const Person& rhs) { return lhs.getPhone() < rhs.getPhone(); });
}
void Addressbook::Sort_Email() {
    sort(p.begin(), p.end(), [] (const Person& lhs, const Person& rhs) { return lhs.getEmail() < rhs.getEmail(); });
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	string filename;

	cout << "Podaj nazwe ksiazki adresowej: ";
	cin >> filename;

	Addressbook a;
    a.read(filename);

	cout << endl << "Odczytalem " << a.getAll().size() << " " << rekord(a.getAll().size()) << "." << endl << endl;

	while(true) {
		string cmd;

		cout << "? "; cin >> cmd;

		if (cmd == "list") {
            a.list();
		} else if (cmd == "add") {
            a.add();
		} else if (cmd == "modify") {
			int id;
			cin >> id;
			Person r = a.get(id);
			a.askForData(r);
		} else if (cmd == "remove") {
			int id;
			cin >> id;
			a.remove(id);
		} else if (cmd == "quit") {
			break;
		} else if (cmd == "sort") {
			string tmp; getline(cin,tmp);
			if (tmp == "" || tmp == " name") {
				a.Sort();
			} else if (tmp == " surname") {
                a.Sort_Surname();
			} else if (tmp == " phone") {
                a.Sort_Phone();
			} else if (tmp == " email") {
                a.Sort_Email();
			} else {
				cout << "Nieznany parametr: " << tmp << " (dopuszczalne: name, surname, phone, email)"<< endl;
			}
		} else {
			cout << "Podaj instrukcje: list, add, modify, remove, sort, quit" << endl;
		}

	}

	a.write(filename);

	cout << endl << "Zapisalem " << a.getAll().size() << " " << rekord(a.getAll().size()) << "." << endl;

	return 0;
}

