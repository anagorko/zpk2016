#include<string>
#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
using namespace std;

class Person
{
	string name, surname, phone, email;
	int id;

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

	bool operator<(const Person& p) const {
		return name < p.name;
	}

	friend ostream& operator<<(ostream&, const Person&);
	friend istream& operator>>(istream&, Person&);
};

ostream& operator<<(ostream& os, const Person& p)
{
	return os << p.id << endl << p.name << endl << p.surname << endl << p.phone << endl << p.email << endl;
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

class Addressbook
{
	vector<Person> p;

	int getFreeId()
	{
		int max_id = 0;
		for (auto &r: p) {
			if (r.getId() > max_id) { max_id = r.getId(); }
		}
		return max_id+1;
	}

public:
	Addressbook() {}

	void read(string filename) {
		Person r;

		fstream f;
		f.open(filename, ios::in);

		while (f>>r) {
			p.push_back(r);
		}
		f.close();
	}

	void write(string filename) {
		fstream f;

		f.open(filename, ios::out | ios::trunc);

		for (auto& r: p) {
			f << r;
		}
		f.close();
	}

	void add(Person &r) {
		r.setId(getFreeId());
		p.push_back(r);
	}

/*iterator tutaj uzyty, mozna by i bez tego poprostu petla i przestawianie ostatniego elementu*/

	void remove(int id) {
		for (auto i = p.begin(); i < p.end(); i++) {
			if (i -> getId() == id) {
				p.erase(i); return;
			}
		}
	}

	Person& get(int id) {
		for (auto &r: p) {
			if (r.getId() == id) return r;
		}
		Person r;
		r.setId(id);
		p.push_back(r);
		return p.back();
	}

	vector<Person>& getAll() { return p; }
};

string rekord(int liczba)
{
	if (liczba == 0 || liczba >= 5) {
		return "rekordów";
	} else if (liczba == 1) {
		return "rekord";
	} else {
		return "rekordy";
	}
}

void askForData(Person &p)
{
	/* cin przy wpisywaniu komendy czyta tylko do 1szego bialego znaku np. spacji. jezeli z przodu, to ignoruje te znaki
     dla odmiany getline czyta az do entera, wiec podajac komende na tego pierwszego tempa wczyta tego entera po zatwierdzeniu komendy*/
  
  string tmp;
	getline(cin,tmp);

	cout << "Podaj imie: ";
	if (p.getName() != "") { cout << "[" << p.getName() << "] "; }
	getline(cin,tmp); if (tmp != "") { p.setName(tmp); }

	cout << "Podaj nazwisko: ";
	if (p.getSurname() != "") { cout << "[" << p.getSurname() << "] "; }
	getline(cin,tmp); if (tmp != "") { p.setSurname(tmp); }

	cout << "Podaj telefon: ";
	if (p.getPhone() != "") { cout << "[" << p.getPhone() << "] "; }
	getline(cin,tmp); if (tmp != "") { p.setPhone(tmp); }

	cout << "Podaj email: ";
	if (p.getEmail() != "") { cout << "[" << p.getEmail() << "] "; }
	getline(cin,tmp); if (tmp != "") { p.setEmail(tmp); }
}

int main(int argc, char* argv[])
{
	string filename;

	cout << "Podaj nazwę książki adresowej: ";
	cin >> filename;

	Addressbook a;
	a.read(filename);

	cout << endl << "Odczytałem " << a.getAll().size() << " " << rekord(a.getAll().size()) << "." << endl;


	while(true) {
		string cmd;

		cout << "? "; cin >> cmd;

		if (cmd == "list") {
			string tmp; getline(cin, tmp);

			vector<Person>& p = a.getAll();
			for (const auto &r: p) {
				cout << r.getId() << ": " << r.getName() << " " << r.getSurname() << " "
				     << r.getPhone() << " " << r.getEmail() << endl;
			}

		} else if (cmd == "add") {
			Person r;
			askForData(r);
			a.add(r);
		} else if (cmd == "modify") {
			int id;
			cin >> id;
			Person &r = a.get(id);
			askForData(r);
		} else if (cmd == "remove") {
			int id;
			cin >> id;
			a.remove(id);
		} else if (cmd == "quit") {
			break;
		} else if (cmd == "sort") {
			string tmp; getline(cin,tmp);
			if (tmp == "" || tmp == " name") {
				sort(a.getAll().begin(), a.getAll().end());
			} else if (tmp == " surname") {
				sort(a.getAll().begin(), a.getAll().end(),
					[](const Person &l, const Person &r) { return l.getSurname() <  r.getSurname(); });
			} else if (tmp == " phone") {
				sort(a.getAll().begin(), a.getAll().end(),
					[](const Person &l, const Person &r) { return l.getPhone() <  r.getPhone(); });
			} else if (tmp == " email") {
				sort(a.getAll().begin(), a.getAll().end(),
					[](const Person &l, const Person &r) { return l.getEmail() <  r.getEmail(); });
			} else {
				cout << "Nieznany parametr: " << tmp << " (dopuszczalne: name, surname, phone, email)"<< endl;
			}
		} else {
			cout << "Podaj instrukcję: list, add, modify, remove, sort, quit" << endl;
		}

	}

	a.write(filename);

	cout << "Zapisałem " << a.getAll().size() << " " << rekord(a.getAll().size()) << "." << endl;

	return 0;
}
