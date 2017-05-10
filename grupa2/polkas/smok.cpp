#include <iostream>
#include <algorithm>
#include <vector>
#include<fstream>
#include<sstream>
#include <string>
#include <cmath>

using namespace std;

int sizes = 50000;
vector<double> all_x(sizes);
vector<double> all_y(sizes);

main(){


all_x[0] = 1;
all_y[0] = 1;
int ran;
for(int i = 0; i<sizes-1;i++){

 ran = rand() % 2;

if(ran==1){
all_y[i+1] = -0.4*all_y[i] +0.1;
all_x[i+1] = -0.4*all_x[i] - 1;
} else {
all_x[i+1] = 0.76*all_x[i] - 0.4*all_y[i];
all_y[i+1] = 0.4*all_x[i] + 0.76*all_y[i];
}

}

// MAX-MIN
double maxi_x = *max_element(all_x.begin(),all_x.end());
double mini_x = *min_element(all_x.begin(),all_x.end());

double maxi_y = *max_element(all_y.begin(),all_y.end());
double mini_y = *min_element(all_y.begin(),all_y.end());

// WRITE FILE & BUILD MATRIX

vector<vector<double>> mat(101,vector<double>(101));

//CSV
char filename[10] = "smoka.txt";
ofstream fil;
fil.open(filename,ios::trunc);
fil<<"x"<<","<<"y\n";
for (int i=400; i<sizes; i++)
{
fil << all_x[i]<< ","<< all_y[i]<< "\n";
int x_coord = 100*abs((all_x[i]-maxi_x)/(maxi_x-mini_x));
int y_coord = 100*abs((all_y[i]-maxi_y)/(maxi_y-mini_y));
mat[x_coord][y_coord] = 1;
}
fil.close();

//PLOT

for(int i=0;i<100;i++){
    for(int a=0;a<100;a++){
        cout << mat[a][i];
        if(a==99){cout<<endl;};
    }
}

}


