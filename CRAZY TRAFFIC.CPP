#include<graphics.h>
#include<dos.h>
#include<process.h>
#include<conio.h>
#include<iostream.h>
#include<stdlib.h>
#include<string.h>
#include<fstream.h>
#include<stdio.h>

unsigned long dela=30000,tim=0;

int lane1=8,pos=30,cu=0,prevl=0,prevp=0,switc=0;

int gmod=0,bgc=4,fgc=YELLOW,fluid=0,sw=0,cdis=fluid*10;

float score2,score=0;

void intro1(int,char*);
void score1();
void player(int,int);
void init();
void init1(int);
void cmp(int,int);
void obstacle(int,int,int,int);
void drawplayer(int x,int y);
void startup();
int menu();
void game();
int close();
void instruct();
void reset();
void main();
void up();
void clear();

class  settings
{public:
 int gmode,gdif;
 void inpsettings(int x,int y);
 void assisettings();
 void changeset();
 char* getgdif(char* a);
 char* getgmode(char* a);
  }set;

class obstlane
{public:
 int lane,posdat[8][2],dir;
 obstlane(int);
 void draw();
 void incre();
}*obst[8];

class highscore
{
public:
float gd[2][3][3];
char names[6][20];
void assihigh();
void inphighscore(char*,float);
void disphigh();
void cmphi();
}high;

//settings class functions
char* settings::getgmode(char* a)
 {if(gmode==0)
  strcpy(a,"Classic");
 else
 strcpy(a,"Time Trail");
 return a;
 }
 char* settings::getgdif(char* a)
 {switch(gdif)
  {case 0:strcpy(a,"Easy");
	  break;
   case 1:strcpy(a,"Medium");
	  break;
   case 2:strcpy(a,"Hard");
	  break;
  }
 return a;
}
 void settings::inpsettings(int x,int y)
{
 gmode=x;
 gdif=y;
 }
void settings::assisettings()
{
   gmod=gmode;
   switch(gdif)
   {case 0:dela=30000;break;
    case 1:dela=20000;break;
    case 2:dela=5000;break;
    }

}
void settings::changeset()
{
 closegraph();
 char a1[50];
 _setcursortype(_NOCURSOR);
 textcolor(bgc);
 gotoxy(36,1);
 cprintf("SETTINGS");
 gotoxy(27,3);
 cprintf("Game Mode   :");
 gotoxy(21,5);
 cprintf("Game Difficulty   :");
 textcolor(fgc);
 gotoxy(40,3);
 cprintf("   %s",getgmode(a1));
 textcolor(bgc);
 gotoxy(40,5);
 cprintf("   %s",getgdif(a1));
 int x=gmode,a;
 textcolor(fgc);
 do{
    a=getch();
    if(a==0)getch();
    gotoxy(40,3);
    cout<<"               ";
    if(a!=13)
    gmode=++x%2;
    gotoxy(40,3);
    cprintf("   %s",getgmode(a1));
    }while(a!=13);
    textcolor(bgc);
    gotoxy(40,3);
    cprintf("   %s",getgmode(a1));
    textcolor(fgc);
    x=gdif;
    gotoxy(40,5);
    cprintf("   %s",getgdif(a1));
 do{
    a=getch();
    if(a==0)getch();
    gotoxy(40,5);
    cout<<"           ";
    if(a!=13)
    gdif=++x%3;
    gotoxy(40,5);
    cprintf("   %s",getgdif(a1));
    }while(a!=13);
    textcolor(bgc);
    gotoxy(40,5);
    cprintf("   %s",getgdif(a1));
    textcolor(fgc);
    assisettings();
    ofstream f1("settings1.dat",ios::out|ios::binary);
    f1.write((char*)&set,sizeof(set));
    f1.close();
    int gd=DETECT,gm;
 initgraph(&gd,&gm,"");

    }
//settings class ends here

//obstlane class functions
obstlane::obstlane(int lan)
 {int centre=4;
  lane=lan;
  dir=cu%2;
  ++cu;
  for(int i=0;i<=7;++i)
  {int rnd=random(4)+2;
   posdat[i][0]=centre-(rnd/2);
   posdat[i][1]=rnd;
   centre+=8;
  }
 }

void obstlane::draw()
{

 for(int i=0;i<=7;++i)
 {
  obstacle(lane,posdat[i][0],posdat[i][1],dir);
 }
  cmp((pos*10)+5,(lane1*40)+80+cdis);
  player(lane1,pos);

 }

void obstlane::incre()
 {
   for(int i=0;i<8;++i)
  {
   if(dir==0)
   {
   ++posdat[i][0];
   if(posdat[i][0]>64)posdat[i][0]=1;
   }
      else
  { --posdat[i][0];
   if(posdat[i][0]<(0-posdat[i][1]))posdat[i][0]=63-posdat[i][1];
    }
  }
    if(lane==lane1)
   {if(dir==0)++pos; else --pos;}

  }
//obstlane class ends here

//highscores class functions
void highscore::assihigh()
{for(int x=0;x<2;x++)
 for(int y=0;y<3;++y)
 {gd[x][y][0]=0;
  gd[x][y][1]=0;
  gd[x][y][2]=0;
 }
 for(x=0;x<6;x++)
 strcpy(names[x],"NONAME");
}
void highscore::cmphi()
{
 char namc[20];
 if(gmod==1)
score=score2;
else
score-=1;
  if(gd[gmod][set.gdif][0]<score)
 {
  closegraph();
  gotoxy(32,5);
  cprintf("NEW HIGHSCORE!!!");
  gotoxy(1,8);
  cprintf("Enter your name for the Hall Of Fame: ");
  gets(namc);
  if(gmod==0)
  inphighscore(namc,score);
  else
  inphighscore(namc,score2);
  ofstream f1("highscores1.dat",ios::out|ios::binary);
  f1.write((char*)&high,sizeof(high));
  f1.close();}
}


void highscore::inphighscore(char*name,float scor)
{
 gd[gmod][set.gdif][0]=scor;
 if(gmod==0)
  strcpy(names[set.gdif],name);
 else
 {
  strcpy(names[3+set.gdif],name);
  gd[gmod][set.gdif][1]=scor*(tim/10);
  gd[gmod][set.gdif][2]=tim/10;
 }
}

void highscore::disphigh()
{char a[20];
 closegraph();
 _setcursortype(_NOCURSOR);
 textcolor(MAGENTA);
 gotoxy(35,1);
 cprintf("HALL OF FAME");
 gotoxy(11,4);
 textcolor(CYAN);
 cprintf("Classic   ,Easy  :%s  -  %d",names[0],int(gd[0][0][0]));
 gotoxy(11,7);
 cprintf("Classic   ,Medium:%s - %d",names[1],int(gd[0][1][0]));
 gotoxy(11,10);
 cprintf("Classic   ,Hard  :%s - %d",names[2],int(gd[0][2][0]));
 gotoxy(11,13);
 cprintf("Time Trail ,Easy  :%s - %d in %d sec*",names[3],int(gd[1][0][1]),int(gd[1][0][2]));
 gotoxy(11,16);
 cprintf("Time Trail ,Medium:%s - %d in %d sec*",names[4],int(gd[1][1][1]),int(gd[1][1][2]));
 gotoxy(11,19);
 cprintf("Time Trail ,Hard  :%s - %d in %d sec*",names[5],int(gd[1][2][1]),int(gd[1][2][2]));
 cout<<"\n\n\n\t\t\t\t";
 cprintf("BEAT THEM IF U CAN!!!");
 cout<<endl<<"*not exactly in sec mind you";
 getch();
 int gd=DETECT,gm;
 initgraph(&gd,&gm,"");
 }
//highscores class ends here

//standalone functions
void obstacle(int lane,int place,int len,int dir)
{int x=(place*10)+5,y=(lane*40)+80+cdis;
 setcolor(BLACK);
 if(dir==0)
 rectangle(x-15,y-5,x-15+(10*len),y+5);
 else
 rectangle(x+5,y+5,x+5+(10*len),y-5);
 init1(lane);
 setcolor(12);
 rectangle(x-5,y-5,x-5+(10*len),y+5);
 if(dir==0&&place+len>64)
 {
 int lent=(place+len)-64;
 setcolor(0);
 rectangle(0,(lane*40)+75+cdis,(lent*10)-10,(lane*40)+85+cdis);
 setcolor(12);
 rectangle(0,(lane*40)+75+cdis,(lent*10),(lane*40)+85+cdis);
 }
 if(dir==1&&place<0)
 {int le=0-place;
 setcolor(0);
 rectangle(640,(lane*40)+75+cdis,640-(le*10)+10,(lane*40)+85+cdis);
 setcolor(12);
 rectangle(640,(lane*40)+75+cdis,640-(le*10),(lane*40)+85+cdis);}

 }


void init()
{
setcolor(WHITE);
setlinestyle(0,0,1);
line(1,80+cdis,639,80+cdis);
line(1,120+cdis,639,120+cdis);
line(1,160+cdis,639,160+cdis);
line(1,200+cdis,639,200+cdis);
line(1,240+cdis,639,240+cdis);
line(1,280+cdis,639,280+cdis);
line(1,320+cdis,639,320+cdis);
line(1,360+cdis,639,360+cdis);
setcolor(LIGHTBLUE);
setlinestyle(0,0,1);
//line(1,80,639,80);
line(1,400,639,400);
setcolor(14);
line(1,1,1,480);
line(639,1,639,480);
}

void init1(int lan)
{
setcolor(WHITE);
setlinestyle(0,0,1);
switch(lan)
{
case 0:line(1,80+cdis,639,80+cdis);break;
case 1:line(1,120+cdis,639,120+cdis);break;
case 2:line(1,160+cdis,639,160+cdis);break;
case 3:line(1,200+cdis,639,200+cdis);break;
case 4:line(1,240+cdis,639,240+cdis);break;
case 5:line(1,280+cdis,639,280+cdis);break;
case 6:line(1,320+cdis,639,320+cdis);break;
case 7:line(1,360+cdis,639,360+cdis);break;
}
setlinestyle(0,0,1);
setcolor(14);
line(0,0,0,480);
line(639,0,639,479);
}

void cmp(int x,int y)
{int col=getpixel(x-5,y-5);
 int col1=getpixel(x+5,y+5);
if((col==12&&col1==12)||(col==14||col1==14)){
player(lane1,pos);getch();
high.cmphi();
main();}
}


void drawplayer(int x,int y)
{setcolor(BLACK);
 rectangle(prevp-4,prevl-4,prevp+4,prevl+4);
 init1(lane1);
 setcolor(WHITE);
 prevl=y;prevp=x;
 rectangle(x-4,y-4,x+4,y+4);
}

void player(int lane,int place)
{
drawplayer((place*10)+5,(lane*40)+80+cdis);
}

void main()
{ int gd=DETECT,gm;
  randomize();
initgraph(&gd,&gm,"");
if(switc==0)
startup();
reset();
men:
switc++;
switch(menu())
{
 case 1:cleardevice();game();goto men;
 case 2:instruct();goto men;
  case 3:set.changeset();goto men;
  case 4:high.disphigh();goto men;
  case 5:if(close()==0)exit(0);
	else goto men;
   }
}


void game(){
 init();
 player(lane1,pos);

 for(int i=0;i<=7;++i)
 {
  obst[i]=new obstlane(i);

 }
  for(i=1;i<=7;++i)
  obst[i]->draw();
  score1();
unsigned long count=0;
 while(1)
 {
  if(kbhit())
  {int cmd=getch();
  if(cmd==0)cmd=getch();
  switch(cmd)
  {
   case 72:if(lane1>0)--lane1;
   if(gmod==0){++score;score1();}break;
   case 80:if(lane1<8)++lane1;if(gmod==0){--score;score1();}break;
   case 77:if(pos<63)++pos;break;
   case 75:if(pos>0)--pos;break;
   case 13:if(close()==0)return;
  }

  player(lane1,pos);
   }
  if(gmod==0)
  {
  if(lane1==6)sw=1;
  if(count%dela==0)
 { for(int i=0;i<=7;++i)

   {if(obst[i]->lane>0||fluid!=0)
   {
    obst[i]->incre();
    obst[i]->draw();
    }
   }
    if(count%(dela*5)==0&&sw==1)
    {++fluid;
    clear();
    cdis=fluid*5;
    count=0;}
    if(fluid>7)
    {up();
     fluid=0;
     cdis=0;}
    }
  }
 else
 {
 if(count%dela==0)
 { for(int i=0;i<=7;++i)
   {if(obst[i]->lane>0)
   {
    obst[i]->incre();
    obst[i]->draw();
    }
   }
  count=0;
  tim++;
 if(tim!=0&&tim%10==0)
{
 gotoxy(2,2);
 cout<<"Time";
 gotoxy(4,3);
 cout<<tim/10;
 score2=score/(tim/10);
 }
}
if(lane1==0)
 {
  score++;
  score1();
  lane1=8;pos=30;
 }
}
++count;
}
}


void startup()
{
 fstream setting("settings1.dat",ios::in|ios::binary);
 if(!setting)
 {setting.open("settings1.dat",ios::out|ios::binary|ios::noreplace);
 set.inpsettings(0,0);
 setting.write((char*)&set,sizeof(set));
 setting.close();
 setting.open("settings1.dat",ios::in|ios::binary); }
 fstream hig("highscores1.dat",ios::in|ios::binary);
 if(!hig)
 {
  hig.open("highscores1.dat",ios::out|ios::binary|ios::noreplace);
  high.assihigh();
  hig.write((char*)&high,sizeof(high));
  hig.close();
  hig.open("highscores1.dat",ios::in|ios::binary);
 }
 hig.read((char*)&high,sizeof(high));
 setting.read((char*)&set,sizeof(set));
 set.assisettings();
 delay(2000);
  char geek[5],ere[5];
  int tru=0;
 settextstyle(0,0,6);
 setlinestyle(0,0,3);
 geek[5]='\0';
  setcolor(4);
  rectangle(1,1,639,479);
  setfillstyle(1,4);
  floodfill(620,420,4);
  for(int m=0;m<=60;m++)
 {
  setcolor(14);
  outtextxy(200,100,geek);
  strcpy(ere,geek);
  for(int i=tru;i<=4;i++)
  geek[i]=random(91-65)+65;
  delay(50);
  setcolor(4);
  outtextxy(200,100,ere);
  switch(m)
  {case 20:geek[0]='G';tru++;break;
   case 30:geek[1]='E';tru++;break;
   case 40:geek[2]='E';tru++;break;
   case 50:geek[3]='K';tru++;break;
   case 60:geek[4]='S';tru++;break;
   }
  }
  setcolor(14);
  outtextxy(200,100,geek);
  fflush(stdin);
  setcolor(1);
  settextstyle(1,0,7);
  char prod[11]="PRODUCTIONS";
  for(int ord=0;ord<=5;++ord)
  {char pl[2]={prod[5-ord],'\0'};
   char ll[2]={prod[5+ord],'\0'};
   outtextxy(300-(ord*40),200,pl);
   outtextxy(300+(ord*40),200,ll);
   delay(400);}
   setcolor(9);
   settextstyle(1,0,4);
   outtextxy(235,280,"PRESENTS");
   sleep(4);
  cleardevice();
  setcolor(7);
  setfillstyle(1,7);
  int rect[]={0,0,0,480,640,480,640,0,0,0};
for(int x1=320,y1=240,x2=320,y2=240;x1>0;--x1,--y1,++x2,++y2)
 {delay(1);
 rectangle(x1,y1,x2,y2);}
settextstyle(1,0,7);
intro1(200,"T");
intro1(215,"R");
fillpoly(5,rect);
outtextxy(200,200,"TR");
intro1(230,"A");
fillpoly(5,rect);
outtextxy(200,200,"TRA");
intro1(245,"F");
fillpoly(5,rect);
outtextxy(200,200,"TRAF");
intro1(260,"F");
fillpoly(5,rect);
outtextxy(200,200,"TRAFF");
intro1(275,"I");
fillpoly(5,rect);
outtextxy(200,200,"TRAFFI");
intro1(290,"C");
fillpoly(5,rect);
outtextxy(200,200,"TRAFFIC");
delay(1000);
while(!kbhit()){
setcolor(7);
fillpoly(5,rect);
setcolor(10);
settextstyle(1,0,7);
outtextxy(200,200,"TRAFFIC");
setcolor(random(9)+7);
settextstyle(random(10),0,6);
outtextxy(255,100,"C");
setcolor(random(14)+1);
settextstyle(random(10),0,6);
outtextxy(285,100,"R");
setcolor(random(14)+1);
settextstyle(random(10),0,6);
outtextxy(310,100,"A");
setcolor(random(14)+1);
settextstyle(random(10),0,6);
outtextxy(340,100,"Z");
setcolor(random(14)+1);
settextstyle(random(10),0,6);
outtextxy(370,100,"Y");
delay(100);
}
 getch();
 fflush(stdin);
 flushall();
cleardevice();
}


int menu()
{
 int inp=0,cmd=1;
 settextstyle(2,0,7);
 while(inp!=13)
 {
 switch(inp)
 {
  case 72:cmd--;break;
  case 80:cmd++;break;
 }
 if(cmd>5)cmd=1;
 if(cmd<1)cmd=5;
 switch(cmd)
 {
  case 1:setcolor(fgc);outtextxy(100,100,"Start");setcolor(bgc); outtextxy(100,120,"Instructions");outtextxy(100,140,"Settings");outtextxy(100,160,"Hall Of Fame");outtextxy(100,180,"Exit");break;  case 2:setcolor(bgc);outtextxy(100,100,"Start");setcolor(fgc);outtextxy(100,120,"Instructions");setcolor(bgc);outtextxy(100,140,"Settings");outtextxy(100,160,"Hall Of Fame");outtextxy(100,180,"Exit");break;
  case 3:setcolor(bgc);outtextxy(100,100,"Start");outtextxy(100,120,"Instructions");setcolor(fgc);outtextxy(100,140,"Settings");setcolor(bgc);outtextxy(100,160,"Hall Of Fame");outtextxy(100,180,"Exit");break;
  case 4:setcolor(bgc);outtextxy(100,100,"Start");outtextxy(100,120,"Instructions");outtextxy(100,140,"Settings");setcolor(fgc);outtextxy(100,160,"Hall Of Fame");setcolor(bgc);outtextxy(100,180,"Exit");break;
  case 5:setcolor(bgc);outtextxy(100,100,"Start");outtextxy(100,120,"Instructions");outtextxy(100,140,"Settings");outtextxy(100,160,"Hall Of Fame");setcolor(fgc);outtextxy(100,180,"Exit");break;
 }
 inp=getch();
 }
 return cmd;
}

int close()
{
 cleardevice();
 setcolor(bgc);
 outtextxy(200,200,"do you really want to quit?");
 int ab=0,abc=0;
 while(ab!=13)
  {
    switch(ab)
       {
	case 72:abc--;break;
	case 80:abc++;break;
       }
    abc=abc<0?1:abc;
    abc=abc>1?0:abc;
    switch(abc)
      {
       case 0:setcolor(fgc);outtextxy(200,220,"Yes");setcolor(bgc);outtextxy(200,240,"No");break;
       case 1:setcolor(bgc);outtextxy(200,220,"Yes");setcolor(fgc);outtextxy(200,240,"No");break;
      }
    ab=getch();
 }
fflush(stdin);
cleardevice();
return abc;
}

void instruct()
{
 closegraph();
 _setcursortype(_NOCURSOR);
 gotoxy(34,1);
 textcolor(RED);
 cprintf("INSTRUCTIONS");
 textcolor(MAGENTA);
 gotoxy(1,3);
 cprintf("Imagine yourself stuck in an endless traffic trying to find your way out,its HOR");
 gotoxy(1,5);
 cprintf("RIFIC.So help people find their way through the most craziest traffic in the wor");
 gotoxy(1,7);
 cprintf("ld.GOOD LUCK.The simplest controls are ready for you to use:");
 gotoxy(1,9);
 cout<<"\t\t\t\t";
 cprintf("Movement - Arrow keys");
 gotoxy(1,11);
 cout<<"\t\t\t\t";
 cprintf("Pause - Space");
 gotoxy(1,13);
 cout<<"\t\t\t\t";
 cprintf("Exit - Enter");
 gotoxy(1,15);
 cprintf("Tweak settings to your favorite in the settings menu.");
 gotoxy(1,19);
 textcolor(YELLOW);
 cout<<"\t\t\t\t";
 cprintf("ENJOY THE GAME!");
 gotoxy(38,22);
 textcolor(BLUE);
 cprintf("ABOUT");
 gotoxy(1,23);
 textcolor(LIGHTBLUE);
 cout<<"\t\t\t";
 cprintf("Devoloped by:GEEK'S PRODUCTIONS");
 getch();
 int gd=DETECT,gm;
 initgraph(&gd,&gm,"");
 }

void reset()
 {
 lane1=8,pos=30,cu=0,prevl=0,prevp=0;sw=0;score=0;fluid=0;cdis=0;tim=0;
 }

void up()
{
 for(int i=0;i<8;i++)
  { obst[i]->lane++;
  if(obst[i]->lane>7)obst[i]->lane=0;
  }
  lane1++;
}

void clear ()
{
setcolor(8);
setfillstyle(1,BLACK);
rectangle(1,80+cdis-15,639,80+cdis-5);
rectangle(1,120+cdis-15,639,120+cdis-5);
rectangle(1,160+cdis-15,639,160+cdis-5);
rectangle(1,200+cdis-15,639,200+cdis-5);
rectangle(1,240+cdis-15,639,240+cdis-5);
rectangle(1,280+cdis-15,639,280+cdis-5);
rectangle(1,320+cdis-15,639,320+cdis-5);
rectangle(1,360+cdis-15,639,360+cdis-5);
floodfill(20,80+cdis-1,8);
floodfill(20,120+cdis-1,8);
floodfill(20,160+cdis-1,8);
floodfill(20,200+cdis-1,8);
floodfill(20,240+cdis-1,8);
floodfill(20,280+cdis-1,8);
floodfill(20,320+cdis-1,8);
floodfill(20,360+cdis-1,8);
setcolor(BLACK);
rectangle(1,80+cdis-15,639,80+cdis-5);
rectangle(1,120+cdis-15,639,120+cdis-5);
rectangle(1,160+cdis-15,639,160+cdis-5);
rectangle(1,200+cdis-15,639,200+cdis-5);
rectangle(1,240+cdis-15,639,240+cdis-5);
rectangle(1,280+cdis-15,639,280+cdis-5);
rectangle(1,320+cdis-15,639,320+cdis-5);
rectangle(1,360+cdis-15,639,360+cdis-5);
}

void score1()
{
 gotoxy(75,1);
 cout<<"Score";
 gotoxy(77,2);
 cout<<score;
}


void intro1(int cor,char* a)
{
 for(int i=0;i<=cor;++i)
 {
 setcolor(7);
 outtextxy(i-1,200,a);
 setcolor(10);
 outtextxy(i,200,a);
 delay(1);
 }
}
