#include "ui.h"
#include <iostream>
using namespace std;

ui::ui() {}

void ui::initiate(int argc, char* argv[]){

    if(argc == 1) {
        cout << "No arguments passed, starting interactive menu" << endl;
        run_menu();
    }
    else {
        run_method(argc, argv);
    }

}

void ui::run_menu() {

    while(true){

        cout << "Type 'q' to exit." << endl;
        cout << "Type 'd' to display a record." << endl;
        cout << "Type 'D' do display all records." << endl;
        cout << "Type 'a' to add a record." << endl;
        cout << "Type 'm' to modify a record" << endl;
        cout << "Type 'W' to write to default 'db.txt' file." << endl;
        cout << "Type 'L' to load from default 'db.txt' file." << endl;
        cout << "Type 's' to sort by names." << endl;
        cout << "Type 'r' to remove entry." << endl;

        char c;
        cin >> c;

        if(c == 'q') {break;}
        if(c == 'd') {
            cout << "Enter record number do display: ";
            int k; cin >> k; b.display(k);}
        if(c == 'a') {b.c_add();}
        if(c == 'm') {
            cout << "Enter record number do modify: ";
            int k; cin >> k; b.c_modify(k);}
        if(c == 'D') {b.display_all();}
        if(c == 'W') {b.save("db.txt");}
        if(c == 'L') {b.load("db.txt");}
        if(c == 's') {b.sort_by_name();}
        if(c == 'r') {
            cout << "Enter record number do remove: ";
            int k; cin >> k; b.c_modify(k);}

    }

    cout << "Closing..." << endl;
}

void ui::run_method(int argc, char* argv[]) {

    if(string(argv[1]) == "-m") {
        run_menu();
        return;
    }

}
