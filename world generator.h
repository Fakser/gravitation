#include <iostream>
#include <cstdlib>
#include <time.h>
#include "system.h"

using namespace std;
int stablex=0; ///zmienna odpowiadająca za zwiększenie parametru x
int stabley=0; ///zwiększenie parametru y
int clusterx=0;
int clustery=0;
class line ///klasa odpowiadająca za tworzenie linii "gruntu"
{
public:
    int height_map[48]; ///przypisanie maksymalnych wysokości mapy danym parametron x
    line(int initial)
    {
        int height_max; ///maksymalna wysokość mapy
        int height_min; ///minimalna wysokość mapy
        int height; ///wysokość dla tanego przejścia pętli
        int lenght; ///długość przejścia pętli
        while((height_max=rand()%20)<7){} ///nadanie maksymalnej wysokości regionu
        while((height_min=rand()%2)<1){} ///nadanie minimalnej wysokości regionu
        while((height=rand()%3)<1){}
        while((lenght=rand()%3)<1){}
        height=height+initial; ///uwzględnienie wysokości ostatniego bloku poprzedniego regionu
        int rising=1; ///zmienna odpowiadająca za rośnięcie i malenie fukcji
        for(int x=0;x<48;x++)
        {
            if(height>=height_max)
                rising=-1;
            else if (height<=height_min)
                rising=1;
            if(x%lenght==0)
            {
                while((lenght=rand()%3)<1){}
                height=height+rising*rand()%3;
            }
            height_map[x]=height;

        }

    }
    void show_line(){
    for(int j=0;j<10;j++)
    {
        for(int i=0;i<48;i++)
        {
            if(j==height_map[i])
                cout << '-';
            else
                cout << ' ';
            if(i==47)
                cout << endl;
        }
    }
    }
};

class block
{
public:
    char sign;
    int x; ///coordinate x
    int y; ///coordinate y
    int acessible;
public:
    void create(int sx,int sy,int type)
    {
    if (type==1)
        {
        sign='#';
        acessible=1;
        }
    else if (type==2){
        sign=' ';
        acessible=0;
        }
        x=sx;
        y=sy;
    }

};

class player: public block ///klasa dziedzicz¹ca "gracz"
{
public:
    int jump; ///zmienna odpowiadajaca za wysokoœæ skoku/upadku
    bool fall; ///sprawdzenie czy postaæ spada
    bool jumpb; ///sprawdzenie czy postaæ skacze
    bool double_jump;
    int current_cluster[2];
    player()
    {
        jump=0;
        fall=false;
        jumpb=false;
        double_jump=false;
        sign='*';
        x=1;
        y=16;
        acessible=1;
    }
};

class cluster
{
public:
    block blocks[16][16];
    void create_cluster()
    {
        for(int i=0;i<16;i++)
        {
            for(int j=0;j<16;j++)
            {
                blocks[i][j].create(i+stablex,j+stabley,2);

        }
        }

    }
public:
    void create_ground(line Line)
    {
        for(int i=0;i<16;i++)
        {
            for(int j=0;j<16;j++)
            {
                if(blocks[i][j].y<=Line.height_map[i+stablex])
                {
                    blocks[i][j].create(blocks[i][j].x,blocks[i][j].y,1);
                }
            }
        }
        stabley=stabley+16;
        if(stabley==48)
        {
            stablex=stablex+16;
            stabley=0;
        }
    }
    void show_cluster(player T1)
    {
        for(int j=15;j>-1;j--)
        {
            for(int i=0;i<16;i++)
            {
                if(i==T1.x-16*clusterx && j==T1.y-16*clustery)
                    cout << T1.sign;
                else
                cout << blocks[i][j].sign;
                if(i==15)
                    cout << endl;
            }
        }
    }
};

class Region
{
    public:
    player player1;
    cluster clusters[3][3];
    int region_type;
    int init_right;
    int init_left;

    Region()
    {

        line Region_line(0);
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
                {
                    clusters[i][j].create_cluster();
                    clusters[i][j].create_ground(Region_line);
                }
        }

    }
    void show_Region()
    {
        for(int j=2;j>-1;j--)
        {
            for(int i=0;i<3;i++){
                clusters[i][j].show_cluster(player1);
                cout << "_________________________________" << endl;}
        }
    }
    bool check_fall()
    {
        if(player1.y - clustery*16>0){
        if(player1.acessible>clusters[clusterx][clustery].blocks[player1.x-(16*clusterx)][player1.y-(16*clustery)-1].acessible)
            return 1;
        else
            return 0;
        }else
        if(player1.acessible>clusters[clusterx][clustery-1].blocks[player1.x-(16*clustery)][15].acessible)
                return 1;
            else
                return 0;

    }
    bool check_right()
    {
        if(player1.x-clusterx*16<15){
        if(player1.acessible>clusters[clusterx][clustery].blocks[player1.x-(16*clusterx)+1][player1.y-(16*clustery)].acessible)
            return 1;
        else
            return 0;}
        else
        {
            if(player1.acessible>clusters[clusterx+1][clustery].blocks[0][player1.y-(16*clustery)].acessible)
                return 1;
            else
                return 0;

        }
    }
    bool check_left()
    {
        if((player1.x-clusterx*16>0)){
        if(player1.acessible>clusters[clusterx][clustery].blocks[player1.x-(16*clusterx)-1][player1.y-(16*clustery)].acessible)
            return 1;
        else
            return 0;}
        else
            if(player1.acessible>clusters[clusterx-1][clustery].blocks[15][player1.y-(16*clustery)].acessible)
                return 1;
            else
                return 0;
    }
    void moving_sides(unsigned char znak)
    {
        if(znak==77 && check_right())
        {
            player1.x++;
        }
        if(znak==75 && check_left())
        {
            player1.x--;
        }
    }
    void moves(unsigned char znak)
    {
        moving_sides(znak);
        if(((znak==72) || player1.jumpb==true)&& (player1.jump<jump_hight && player1.fall==false))
        {
           player1.y++;
           player1.jump++;
           player1.jumpb=true;
           player1.fall=false;
        }
        else if(player1.jump==jump_hight || player1.fall==true || (check_fall() && player1.jumpb!=true))
        {
            if(check_fall()){
                player1.y--;
                player1.fall=true;
                player1.jumpb=false;
                if(player1.jump>0)
                    player1.jump--;

            }
            else
                {
                player1.fall=false;
                player1.jumpb=false;
                player1.jump=0;
                }
        }


    }
    void check_cluster()
    {
        if((player1.x-16*clusterx)>=16)
            clusterx++;
        if((player1.y-16*clustery)>=16)
            clustery++;
        if((player1.x-16*clusterx)<=-1)
            clusterx--;
        if((player1.y-16*clustery)<=-1)
            clustery--;
    }


};
