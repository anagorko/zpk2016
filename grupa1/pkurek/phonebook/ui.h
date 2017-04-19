#ifndef UI_H
#define UI_H
#include "book.h"
class ui {
    private:
        book b;
    protected:
    public:

        ui();

        void initiate(int argc, char* argv[]);

        void run_menu(); //starts interactive menu
        void run_method(int argc, char* argv[]); //runs a method from cl

};

#endif
