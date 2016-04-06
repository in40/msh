#define _BSD_SOURCE
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 200201L
#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>
#include <sys/time.h>
#include <sys/fcntl.h>
#include <menu.h>
#include <signal.h>
#include <sys/types.h>
#include <sqlite3.h>
//
#define P1_READ     0
#define P2_WRITE    1
#define P2_READ     2
#define P1_WRITE    3
#define P3_READ     4
#define P4_WRITE    5
#define P4_READ     6
#define P3_WRITE    7
#define P5_READ     8
#define P5_WRITE    9
#define NUM_PIPES   5 
#define foreach(item, array) \
    for(int keep = 1, \
            count = 0,\
            size = sizeof (array) / sizeof *(array); \
        keep && count != size; \
        keep = !keep, count++) \
      for(item = (array) + count; keep; keep = !keep)

int cmdval=0,stop=0,delay=1,manual=0;
int fd[2*NUM_PIPES];
int val = -1, val1=-1, val2=0, len, i;
pid_t pid,cpid;
char PVnS[256], PVnS2[256], line[256],PVfile[256],PVfile2[256], switch_file[256], panic_switch_file[256];
int linenum=0;
int x,hw,Pmax,period,p1,p2,p3;
double Cp,Ci, Cd, P, ErrP, panic_t;
double SV,PVn,PVn2,ut,et,et1,Pt,Dt,It,It1,percent=40;
char val3[256]={"x"};
long elapsed_time;
struct timeval starts, ends;
char brasenham[256];//={0};
sqlite3 *conn;
sqlite3_stmt *res;
int sqlerror=0;

WINDOW *create_newwin( int height, int width, int start_y, int start_x);

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

int create_win(int pp1, int pp2, int pp3){}

int screen_update(){
double SV1=SV;
	
 close(fd[P1_WRITE]);
 close(fd[P2_WRITE]);
 close(fd[P3_WRITE]);
 close(fd[P4_WRITE]);
 close(fd[P5_READ]);
	
	//curs_set(0);
initscr();
curs_set(0);
noecho();
start_color();
init_pair(1,COLOR_WHITE,COLOR_BLUE);
init_pair(2,COLOR_WHITE,COLOR_RED);
init_pair(3,COLOR_WHITE,COLOR_GREEN);
init_pair(4,COLOR_BLACK,COLOR_WHITE);
init_pair(5,COLOR_BLACK,COLOR_BLACK);


WINDOW *parent1;
parent1=create_newwin(3,70,16,2);
box(parent1,0,0);
wbkgd(parent1,COLOR_PAIR(4));
		

WINDOW *my_win;
my_win=create_newwin(3, 40, 9, 2);
box(my_win, 0, 0);
wbkgd(my_win,COLOR_PAIR(1));


WINDOW *hbrasdisp;
hbrasdisp=create_newwin(3,/*Pmax+2*/40,6,2);
wbkgd(hbrasdisp,COLOR_PAIR(1));
box(hbrasdisp,0,0);

WINDOW *hbrasdisp2;
hbrasdisp2=create_newwin(3,10,0,2);
box(hbrasdisp2,0,0);
wbkgd(hbrasdisp2,COLOR_PAIR(1));

WINDOW *stemp;
stemp=create_newwin(3,10,0,12);
box(stemp,0,0);
wbkgd(stemp,COLOR_PAIR(1));

WINDOW *hbrasdisp1;
hbrasdisp1=create_newwin(3,10,0,22);
box(hbrasdisp1,0,0);
if(manual==0){
wbkgd(hbrasdisp1,COLOR_PAIR(1));}
else{wbkgd(hbrasdisp1,COLOR_PAIR(5));}
	

WINDOW *hbrasdisp9;
hbrasdisp9=create_newwin(3,10,0,32);
box(hbrasdisp9,0,0);
wbkgd(hbrasdisp9,COLOR_PAIR(1));

WINDOW *hbrasdisp6;
hbrasdisp6=create_newwin(3,30,9,42);
box(hbrasdisp6,0,0);
wbkgd(hbrasdisp6,COLOR_PAIR(1));

WINDOW *hbrasdisp5;
hbrasdisp5=create_newwin(3,15,3,42);
box(hbrasdisp5,0,0);
wbkgd(hbrasdisp5,COLOR_PAIR(1));

WINDOW *hbrasdisp4;
hbrasdisp4=create_newwin(3,15,6,57);
box(hbrasdisp4,0,0);
wbkgd(hbrasdisp4,COLOR_PAIR(1));

WINDOW *hbrasdisp3;
hbrasdisp3=create_newwin(3,15,6,42);
box(hbrasdisp3,0,0);
wbkgd(hbrasdisp3,COLOR_PAIR(1));

WINDOW *hbrasdisp7;
hbrasdisp7=create_newwin(3,15,3,57);
box(hbrasdisp7,0,0);
wbkgd(hbrasdisp7,COLOR_PAIR(1));

WINDOW *hbrasdisp8;
hbrasdisp8=create_newwin(3,15,0,57);
box(hbrasdisp8,0,0);
wbkgd(hbrasdisp8,COLOR_PAIR(1));

WINDOW *hbrasdisp10;
hbrasdisp10=create_newwin(3,15,0,42);
box(hbrasdisp10,0,0);
wbkgd(hbrasdisp10,COLOR_PAIR(1));

WINDOW *brasdisp;
brasdisp=create_newwin(3,/*Pmax+2*/40,3,2);
box(brasdisp,0,0);
wbkgd(brasdisp,COLOR_PAIR(1));

wattron(brasdisp,A_BOLD);
mvwprintw(brasdisp,0,1,"Brasenhaim sequence");
wattroff(brasdisp,A_BOLD);
wrefresh(brasdisp);
	
wattron(hbrasdisp10,A_BOLD);
mvwprintw(hbrasdisp10,0,1,"e(t)");
wattroff(hbrasdisp10,A_BOLD);
wrefresh(hbrasdisp10);
	
wattron(hbrasdisp8,A_BOLD);
mvwprintw(hbrasdisp8,0,1,"e(t-1)");
wattroff(hbrasdisp8,A_BOLD);
wrefresh(hbrasdisp8);

wattron(hbrasdisp7,A_BOLD);
mvwprintw(hbrasdisp7,0,1,"P(t)");
wattroff(hbrasdisp7,A_BOLD);
wrefresh(hbrasdisp7);

wattron(hbrasdisp3,A_BOLD);
mvwprintw(hbrasdisp3,0,1,"I(t)");
wattroff(hbrasdisp3,A_BOLD);
wrefresh(hbrasdisp3);

wattron(hbrasdisp4,A_BOLD);
mvwprintw(hbrasdisp4,0,1,"I(t-1)");
wattroff(hbrasdisp4,A_BOLD);
wrefresh(hbrasdisp4);
	
wattron(hbrasdisp5,A_BOLD);
mvwprintw(hbrasdisp5,0,1,"D(t)");
wattroff(hbrasdisp5,A_BOLD);
wrefresh(hbrasdisp5);
	
wattron(hbrasdisp6,A_BOLD);
mvwprintw(hbrasdisp6,0,1,"u(t)");
wattroff(hbrasdisp6,A_BOLD);
wrefresh(hbrasdisp6);
	
wattron(hbrasdisp9,A_BOLD);
mvwprintw(hbrasdisp9,0,1,"Elapsed");
wattroff(hbrasdisp9,A_BOLD);
wrefresh(hbrasdisp9);
	
	
wattron(hbrasdisp2,A_BOLD);
mvwprintw(hbrasdisp2,0,1,"t1, C ");
wattroff(hbrasdisp2,A_BOLD);
wrefresh(hbrasdisp2);

wattron(stemp,A_BOLD);
mvwprintw(stemp,0,1,"t2, C ");
wattroff(stemp,A_BOLD);
wrefresh(stemp);
	
wattron(hbrasdisp1,A_BOLD);
mvwprintw(hbrasdisp1,0,1,"t, C ");
wattroff(hbrasdisp1,A_BOLD);
wrefresh(hbrasdisp1);
	
wattron(my_win,A_BOLD);
mvwprintw(my_win,0,1,"Last cycle completed in:");
wattroff(my_win,A_BOLD);
mvwprintw(my_win,1,20,"Initializing...");
wrefresh(my_win);

wattron(hbrasdisp,A_BOLD);
mvwprintw(hbrasdisp,0,1,"Heating sequence execution");
wattroff(hbrasdisp,A_BOLD);
wrefresh(hbrasdisp);

wattron(parent1,A_BOLD);
mvwprintw(parent1,0,1,"Command window");
wattroff(parent1,A_BOLD);
mvwprintw(parent1,1,1,"New temp to reach: ");
mvwprintw(parent1,1,56,"%s","Delay mode   ");
wrefresh(parent1);
int cmd;
nodelay(parent1,TRUE);
keypad(parent1, TRUE);	
while (stop<1){

// used for output
pid = getpid();

//len
read(fd[P4_READ], &val2, sizeof(val2));	
read(fd[P3_READ], &val3, sizeof(val3));	
read(fd[P2_READ], &val, sizeof(val));
read(fd[P1_READ], &val1, sizeof(val1));	
//read_cmd();
cmd=wgetch(parent1);
cmdval=0;
if(cmd!=ERR){
switch (cmd){

case 'd':
//SV1=SV1+0.1;
if(delay==0){
delay=1;
cmdval=3;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,56,"%s","Delay mode   ");}

else{
delay=0;
cmdval=3;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,56,"%s","No delay mode");}
wrefresh(parent1);
break;

case 'm':
//SV1=SV1+0.1;
if(manual==1){
manual=0;
cmdval=11;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,67,"%s","-");}

else{
manual=1;
cmdval=11;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,67,"%s","M");}
wrefresh(parent1);
break;



case KEY_RIGHT:
if(manual==0){
SV1=SV1+0.1;
cmdval=10;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,20,"%f%s",SV1," UP    ");
wrefresh(parent1);
break;}
else{
percent=percent+10;
cmdval=20;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,20,"%f%s",percent,"pUP    ");
wrefresh(parent1);
break;
}
		
case KEY_LEFT:
if(manual==0){
SV1=SV1-0.1;
cmdval=4;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,20,"%f%s",SV1," DOWN ");
wrefresh(parent1);
break;
}
else{
percent=percent-10;
cmdval=21;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,20,"%f%s",percent,"pDOWN    ");
wrefresh(parent1);
break;}
	
case KEY_UP:
if(manual==0){
SV1++;
cmdval=1;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,20,"%f%s",SV1," UP    ");
wrefresh(parent1);
break;
}
else{
percent=percent+10;
cmdval=20;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,20,"%f%s",percent,"pUP    ");
wrefresh(parent1);
break;
}
case KEY_DOWN:
if(manual==0){
SV1--;
cmdval=2;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));
mvwprintw(parent1,1,20,"%f%s",SV1," DOWN ");
wrefresh(parent1);
//cmdval=0;
break;
}
else{
percent=percent-10;
cmdval=21;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));	
mvwprintw(parent1,1,20,"%f%s",percent,"pUP    ");
wrefresh(parent1);
break;
}

case '0':
SV1=85;
cmdval=5;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));
mvwprintw(parent1,1,20,"%f%s",SV1," Preheat   ");
wrefresh(parent1);
break;

case '1':
SV1=88;
cmdval=6;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));
mvwprintw(parent1,1,20,"%f%s",SV1," Boil       ");
wrefresh(parent1);
break;
	
case '2':
SV1=90;
cmdval=7;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));
mvwprintw(parent1,1,20,"%f%s",SV1," Heads       ");
wrefresh(parent1);
break;

case '3':
SV1=96;
cmdval=8;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));
mvwprintw(parent1,1,20,"%f%s",SV1," Body        ");
wrefresh(parent1);
break;

case '4':
SV1=99;
cmdval=9;
write(fd[P5_WRITE],&cmdval, sizeof(cmdval));
mvwprintw(parent1,1,20,"%f%s",SV1," Tails       ");
wrefresh(parent1);
break;



}
}
	
if(val3){
char cval3[2]={"a"};
char sv1[20];
sscanf(val3,"%c%s",cval3,sv1);

if(strncmp(val3,"c",1)==0){
mvwprintw(hbrasdisp1,1,1,"%s",sv1);
if(manual==0){
wbkgd(hbrasdisp1,COLOR_PAIR(1));}
else{wbkgd(hbrasdisp1,COLOR_PAIR(5));}
wrefresh(hbrasdisp1);
}

if(strncmp(val3,"t",1)==0){
if(SV<atof(sv1)){wbkgd(hbrasdisp2,COLOR_PAIR(2));wattron(hbrasdisp2,A_BOLD);
mvwprintw(hbrasdisp2,0,1,"t1, C ");
/*wattroff(hbrasdisp2,A_BOLD);*/beep();flash();beep();flash();beep();flash();}
if(SV==atof(sv1)){wbkgd(hbrasdisp2,COLOR_PAIR(3));wattron(hbrasdisp2,A_BOLD);
mvwprintw(hbrasdisp2,0,1,"t1, C ");
wattroff(hbrasdisp2,A_BOLD);beep();flash();}
mvwprintw(hbrasdisp2,1,1,"%s",sv1);
wrefresh(hbrasdisp2);
wbkgd(hbrasdisp2,COLOR_PAIR(1));wattron(hbrasdisp2,A_BOLD);
mvwprintw(hbrasdisp2,0,1,"t1, C" );
wattroff(hbrasdisp2,A_BOLD);

}

if(strncmp(val3,"T",1)==0){
mvwprintw(stemp,1,1,"%s",sv1);
wrefresh(stemp);
}

if(strncmp(val3,"x",1)==0){
mvwprintw(parent1,1,45,"Set: %s",sv1);
wrefresh(parent1);
SV=atof(sv1);
}

if(strncmp(val3,"m",1)==0){
mvwprintw(parent1,1,45,"Set: %s",sv1);
wrefresh(parent1);
percent=atof(sv1);
}

if(strncmp(val3,"i",1)==0){
mvwprintw(hbrasdisp3,1,1,"%s",sv1);
wrefresh(hbrasdisp3);
}

if(strncmp(val3,"z",1)==0){
mvwprintw(hbrasdisp4,1,1,"%s",sv1);
wrefresh(hbrasdisp4);
}

if(strncmp(val3,"d",1)==0){
mvwprintw(hbrasdisp5,1,1,"%s",sv1);
wrefresh(hbrasdisp5);
}

if(strncmp(val3,"u",1)==0){
mvwprintw(hbrasdisp6,1,1,"%s",sv1);
wrefresh(hbrasdisp6);
}

if(strncmp(val3,"p",1)==0){
mvwprintw(hbrasdisp7,1,1,"%s",sv1);
wrefresh(hbrasdisp7);
}

if(strncmp(val3,"r",1)==0){
mvwprintw(hbrasdisp8,1,1,"%s",sv1);
wrefresh(hbrasdisp8);
}

if(strncmp(val3,"l",1)==0){
mvwprintw(hbrasdisp9,1,1,"%s%s",sv1,"  ");
wrefresh(hbrasdisp9);
}

if(strncmp(val3,"e",1)==0){
mvwprintw(hbrasdisp10,1,1,"%s",sv1);
wrefresh(hbrasdisp10);
}

if(strncmp(val3,"b",1)==0){
mvwprintw(brasdisp,1,(40-Pmax)/2,"%s",sv1);
wrefresh(brasdisp);
}

}
if(val2){
mvwprintw(my_win,1,20,"                   ");
mvwprintw(my_win,1,20,"%ds",val2);
wrefresh(my_win);
}

if(val1==-1){
mvwprintw(hbrasdisp,1,1,"                                     ");
wrefresh(hbrasdisp);
}
else{
if(val!=-1){
mvwprintw(hbrasdisp,1, ((40-Pmax)/2+val1),"%d",val);
wrefresh(hbrasdisp);
}usleep(50000);}


}
curs_set(1);
echo();
endwin();
printf("SLAVE: killing process...\n");
//FILE *heatswitch;
//heatswitch =  fopen(switch_file, "w");
//heatswitch =  fopen(switch_file, "w");
//fprintf(heatswitch,"0");
//fclose(heatswitch);
//printf("Exitig...\n");
//sleep(5);
kill(getpid(),SIGINT);
}


int elapsed(){
gettimeofday(&ends, NULL);
elapsed_time=(ends.tv_sec - starts.tv_sec);
if(elapsed_time<59){
	sprintf(val3,"l%lis ",elapsed_time);
write(fd[P4_WRITE], &val3, sizeof(val3));
}
if(elapsed_time>=60){
	elapsed_time=elapsed_time/60;
	sprintf(val3,"l%lim    ",elapsed_time);
write(fd[P4_WRITE], &val3, sizeof(val3));
}

}

int switch_heat(){
	

FILE *heatswitch;
heatswitch=fopen(switch_file, "a");
val1=-1;
write(fd[P2_WRITE], &val1, sizeof(val1));
int y=0;
for(x=0;x<Pmax;x++)
{
fprintf(heatswitch,"%d",brasenham[x]);
fflush(heatswitch);
if (y<Pmax){
read(fd[P5_READ],&cmdval,1);
if (cmdval!=0){/*exit(0);*/y=Pmax+1;return(cmdval);}
val=brasenham[x];//*v;
val1=y;
write(fd[P1_WRITE], &val, sizeof(val));
write(fd[P2_WRITE], &val1, sizeof(val1));
if(delay==1){usleep((1000000*period)/Pmax);}
y++;
}

}
fclose(heatswitch);
return(0);
	}



int calc_braz(){

if(manual==0){
			char br1[256],br2[256];
		int ErrPn1;
	P=(ut/hw)*100*Pmax/100/10;
	ErrP=Pmax-P;
	
		for(x=0;x<Pmax;x++)
		{
		char bptr0;
//		if(ErrP<(Pmax/2)){ErrP=ErrP+Pmax;brasenham[x]=1;}
//		else{ErrP=ErrP-P;brasenham[x]=0;}
		if(ErrP<(Pmax/2)){ErrPn1=ErrP+Pmax;ErrP=ErrPn1-P;brasenham[x]=1;}
		else{ErrPn1=ErrP;ErrP=ErrPn1-P;brasenham[x]=0;}
			
		char *bptr;
		if(x==0){bptr=&brasenham[x];sprintf(br2,"%d",*bptr);}
		else{bptr=&brasenham[x];sprintf(br2,"%s%d",br2,*bptr);}
		}
sprintf(val3,"b%s",br2);
write(fd[P4_WRITE], &val3, sizeof(val3));
}

else{
	char br1[256],br2[256];
	int ErrPn1;
		P=percent/10;
		ErrP=Pmax-P;
	
		for(x=0;x<Pmax;x++)
		{
		if(ErrP<(Pmax/2)){ErrPn1=ErrP+Pmax;ErrP=ErrPn1-P;brasenham[x]=1;}
		else{ErrPn1=ErrP;ErrP=ErrPn1-P;brasenham[x]=0;}
		char *bptr;
		if(x==0){bptr=&brasenham[x];sprintf(br2,"%d",*bptr);}
		else{bptr=&brasenham[x];sprintf(br2,"%s%d",br2,*bptr);}
		}
sprintf(val3,"b%s",br2);
write(fd[P4_WRITE], &val3, sizeof(val3));

}
}

int set_et(){
et=(SV-PVn);
sprintf(val3,"e%.5f",et);
write(fd[P4_WRITE], &val3, sizeof(val3));
}

int calc_pid(){
Pt=Cp*et;
sprintf(val3,"p%.5f",Pt);
write(fd[P4_WRITE], &val3, sizeof(val3));

It=It1+Ci*et;
sprintf(val3,"i%.5f",It);
write(fd[P4_WRITE], &val3, sizeof(val3));

Dt=Cd*et-Cd*et1;
sprintf(val3,"d%.5f",Dt);
write(fd[P4_WRITE], &val3, sizeof(val3));

ut=Pt+It+Dt;
sprintf(val3,"u%.5f",ut);
write(fd[P4_WRITE], &val3, sizeof(val3));

sprintf(val3,"r%.5f",et1);
write(fd[P4_WRITE], &val3, sizeof(val3));
et1=et;

sprintf(val3,"z%.5f",It1);
write(fd[P4_WRITE], &val3, sizeof(val3));
It1=It;

}

long read_pv(){
FILE *pvfile, *pvfile2;
struct timeval start, end;
long secs_used, micros_used;

gettimeofday(&start, NULL);
pvfile =  fopen(PVfile, "r");
fscanf(pvfile,"%s", PVnS);
fclose(pvfile);

	//sleep (3);
gettimeofday(&end, NULL);

	PVn=atof(PVnS);
secs_used=(end.tv_sec - start.tv_sec);
sprintf(val3,"t%.4f",PVn);
write(fd[P4_WRITE], &val3, sizeof(val3));

pvfile2 =  fopen(PVfile2, "r");
fscanf(pvfile2,"%s", PVnS2);
fclose(pvfile2);
PVn2=atof(PVnS2);
sprintf(val3,"T%.4f",PVn2);
write(fd[P4_WRITE], &val3, sizeof(val3));

return secs_used;
}

int read_config(){
FILE *file;
file =  fopen("settings.txt", "r");


while(fgets(line, 256, file) != NULL)
{
        char key[256], value[256], delimiter[256];
 
        linenum++;
        if(line[0] == '#') continue;
 
        if(sscanf(line, "%[^=]%[=]%s", key, delimiter, value) != 3)
        {
                fprintf(stderr, "Syntax error, line %d\n", linenum);
                continue;
        }
 

if(strcmp(key,"Cp")==0){Cp=atof(value);}    
if(strcmp(key,"Ci")==0){Ci=atof(value);}    
if(strcmp(key,"Cd")==0){Cd=atof(value);}    
if(strcmp(key,"SV")==0){SV=atof(value);}    
if(strcmp(key,"PVn")==0){PVn=atof(value);}    
if(strcmp(key,"et")==0){et=atof(value);}    
if(strcmp(key,"et1")==0){et1=atof(value);}    
if(strcmp(key,"Pt")==0){Pt=atof(value);}    
if(strcmp(key,"Dt")==0){Dt=atof(value);}    
if(strcmp(key,"It")==0){It=atof(value);}    
if(strcmp(key,"It1")==0){It1=atof(value);}    
if(strcmp(key,"hw")==0){hw=atoi(value);}    
if(strcmp(key,"Pmax")==0){Pmax=atoi(value);}    
if(strcmp(key,"period")==0){period=atoi(value);}    
if(strcmp(key,"P")==0){P=atof(value);}    
if(strcmp(key,"ErrP")==0){ErrP=atof(value);}    
if(strcmp(key,"panic_t")==0){panic_t=atof(value);}    
if(strcmp(key,"PVfile")==0){strcpy(PVfile,value);}    
if(strcmp(key,"PVfile2")==0){strcpy(PVfile2,value);}    
if(strcmp(key,"switch_file")==0){strcpy(switch_file,value);}    
if(strcmp(key,"panic_swith_file")==0){strcpy(panic_switch_file,value);}    

}
fclose(file);
}

int init_scr(){
sprintf(val3,"c%.4f",SV);
write(fd[P4_WRITE], &val3, sizeof(val3));

//sprintf(val3,"u%.1f",ut);
//write(fd[P4_WRITE], &val3, sizeof(val3));

}

/*int read_cmd(){
//while(1==1){
//timeout(1);
int cmd;
cmd=getch();
sprintf(val3,"m%d",cmd);
write(fd[P4_WRITE], &val3, sizeof(val3));
}
//}
*/

int read_cmdval(int lcmdval){

if(lcmdval==0){	cmdval=0;
read(fd[P5_READ],&cmdval,1);}
else{cmdval=lcmdval;}
	if(cmdval!=0)
	{
		if(cmdval==1)
		{
			SV=SV+1;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));	
			init_scr();
			cmdval=0;
		}
		
		if(cmdval==2)
		{
			SV=SV-1;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));

			init_scr();
			cmdval=0;
		}

		if(cmdval==10)
		{
			SV=SV+0.1;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			init_scr();
			cmdval=0;
		}

		if(cmdval==20)
		{
			percent=percent+10;
			sprintf(val3,"m%.2f",percent);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			init_scr();
			cmdval=0;
		}
			if(cmdval==21)
		{
			percent=percent-10;
			sprintf(val3,"m%.2f",percent);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			init_scr();
			cmdval=0;
		}	
		if(cmdval==4)
		{
			SV=SV-0.1;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			init_scr();
			cmdval=0;
		}
		
		if(cmdval==5)
		{
			SV=85;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			init_scr();
			cmdval=0;
		}
		
		if(cmdval==6)
		{
			SV=88;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			init_scr();
			cmdval=0;
		}
		
		if(cmdval==7)
		{
			SV=90;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));

			init_scr();
			cmdval=0;
		}

		if(cmdval==8)
		{
			SV=96;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			init_scr();
			cmdval=0;
		}

		if(cmdval==9)
		{
			SV=99;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			init_scr();
			cmdval=0;
		}

		if(cmdval==3)
		{
			if(delay==0){
			delay=1;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			cmdval=0;}
			else{
			delay=0;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			cmdval=0;}
		}
		if(cmdval==11)
		{
			if(manual==0){
			manual=1;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			sprintf(val3,"c%.4f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			cmdval=0;}
			else{
			manual=0;
			sprintf(val3,"x%.2f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			sprintf(val3,"c%.4f",SV);
			write(fd[P4_WRITE], &val3, sizeof(val3));
			cmdval=0;}
		}
	}

}
void signal_callback_handler(int signum){
stop=1;
initscr();
endwin();
printf("MASTER: Switching off heater...\n");
FILE *heatswitch;
heatswitch = fopen(switch_file, "w");
////heatswitch =  fopen(switch_file, "w");
fprintf(heatswitch,"%i",0);
fclose(heatswitch);
printf("Exitig...\n");
//sleep(5);
/*kill(getpid(),SIGKILL);
exit(signum);
*/
}

int dbupdate(){
	char *query=0;
	char *bptrsql;
	char *brascp;
	char valbras[256];
	char brsql[256];
for (int ii=0; ii<Pmax;ii++){

if(ii==0){bptrsql=&brasenham[ii],sprintf(brsql,"%d",*bptrsql);}
else{bptrsql=&brasenham[ii];sprintf(brsql,"%s%d",brsql,*bptrsql);}
sprintf(valbras,"%s",brsql);
}	

//asprintf(&query, "INSERT INTO history_records (time_date, brasenham, set_temp, current_temp1, current_temp2, pid_ci, pid_cd, pid_cp, pid_ut, pid_et, pid_it, pid_dt) VALUES('1234567890','textik','0.33','0.33','0.33','0.33','0.33','0.33','0.33','0.33','0.33','0.33');");

////asprintf(&query, "INSERT INTO history_records (time_date, brasenham, set_temp, current_temp1, current_temp2, pid_ci, pid_cd, pid_cp, pid_ut, pid_et, pid_it, pid_dt) VALUES('%u','%s','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f');",(unsigned)time(NULL),valbras,SV,PVn,PVn2,Cp,Ci,Cd,ut,et,It,Dt);


asprintf(&query, "INSERT INTO history_records (time, brasenham, SV,PV,PVn2,Cp,Ci,Cd,ut,et,It,Dt,Pt) values('%u','%s','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f','%.2f');",(unsigned)time(NULL),valbras,SV,PVn,PVn2,Cp,Ci,Cd,ut,et,It,Dt,Pt);
//asprintf(&query, "INSERT INTO history_records (time_date, brasenham) values('2','3');");

	
sqlite3_prepare_v2(conn, query, strlen(query), &res, NULL);

sqlite3_step(res);
sqlite3_finalize(res);
free(query);
	
}

int main(){	
sqlerror=sqlite3_open("./mshdata.sl3", &conn);
if (sqlerror) {
	puts("Database error");
	exit (0);
}


		
signal(SIGINT, signal_callback_handler);
read_config();
gettimeofday(&starts, NULL);
	
// create all the descriptor pairs we need
for (i=0; i<NUM_PIPES; ++i){
if (pipe(fd+(i*2)) < 0){perror("Failed to allocate pipes");exit(EXIT_FAILURE);}
}
fcntl(fd[P4_READ], F_SETFL, O_NONBLOCK); 
fcntl(fd[P3_READ], F_SETFL, O_NONBLOCK);
fcntl(fd[P2_READ], F_SETFL, O_NONBLOCK); 
fcntl(fd[P1_READ], F_SETFL, O_NONBLOCK);
fcntl(fd[P5_READ], F_SETFL, O_NONBLOCK);
// fork() returns 0 for child process, child-pid for parent process.

//if((cpid=fork())==0){/*read_cmd();*/}
//else{
if((pid=fork())<0){
perror("Failed to fork process");return EXIT_FAILURE;}
// if the pid is zero, this is the child process

if (pid == 0){
screen_update();
}


if(pid>0){
close(fd[P1_READ]);
close(fd[P2_READ]);
close(fd[P3_READ]);
close(fd[P4_READ]);

    // used for output
pid = getpid();
struct timeval startm, endm;
long secread, secs_usedm, micros_usedm;

init_scr();

while(stop<1){

elapsed();
gettimeofday(&startm, NULL);
secread=read_pv();
set_et();
calc_pid();
calc_braz();
cmdval=switch_heat();
//	dbupdate();
read_cmdval(cmdval);
gettimeofday(&endm, NULL);
secs_usedm=(endm.tv_sec - startm.tv_sec);
val2=secs_usedm;
write(fd[P3_WRITE], &val2, sizeof(val2));
dbupdate();
usleep(100000);
}

}
}
//}
