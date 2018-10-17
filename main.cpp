#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "world generator.h"
#include "system.h"


using namespace std;


double delay = 0.2; ///zmienna dla funkcji "wait"

void wait ( int seconds ) ///function "wait"
{
	clock_t endwait;
	endwait = clock () + seconds *delay* CLOCKS_PER_SEC ;
	while (clock() < endwait) {}
}



int main()
{   srand(time(NULL));
    Region region;
    unsigned char t;
    while (true)
    {
        t='0';
        system("cls");
        if(kbhit())
           t = getch();
        region.moves(t);
        region.check_cluster();
        region.clusters[clusterx][clustery].show_cluster(region.player1);
        wait(1);
        cout << clusterx;
    }

}
