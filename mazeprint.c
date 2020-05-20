/* cc -O2 -g mazeprint.c -o mazeprint -lm */

#define PAGE_X_SZ (8.5) 
#define PAGE_Y_SZ (11.0) 
#define FIG_BORDER (1.0)  \

#define TRUE (1==1) 
#define FALSE (!TRUE)  \

#define I2R(i,nc) ((int) ((i) /(nc) ) ) 
#define I2C(i,nc) ((int) ((i) %(nc) ) ) 
#define RC2I(r,c,nc) (((r) *(nc) ) +(c) )  \

#define MOVR(r1,r2) (r1==r2?0:1) 
#define MOVC(c1,c2) (c1==c2?0:(1<<1) )  \

#define POINTS_PER_INCH (72.0)  \

#define LINE_WIDTH (0.42)  \

#define PATH_WIDTH (0.375)  \

/*1:*/
#line 24 "./mazeprint.w"

/*2:*/
#line 32 "./mazeprint.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h> 
#include <math.h> 
#include <string.h> 

/*:2*/
#line 25 "./mazeprint.w"

/*3:*/
#line 50 "./mazeprint.w"

float page_width= PAGE_X_SZ;
float page_height= PAGE_Y_SZ;
float fig_width= PAGE_X_SZ-(2.0*FIG_BORDER);
float fig_height= PAGE_Y_SZ-(2.0*FIG_BORDER);

/*:3*//*4:*/
#line 62 "./mazeprint.w"

int verbose_mode= FALSE;

/*:4*/
#line 26 "./mazeprint.w"

/*8:*/
#line 137 "./mazeprint.w"

void
showhelp(char**argv)
{
fprintf(stderr,"\nUsage: %s [ options ] [ mazefile ] > output.ps\n\n",*argv);
fprintf(stderr,
" The following options are recognized:\n"
"   -v     : set verbose output\n"
"   -w val : set maximum figure width to 'val' inches\n"
"   -h val : set maximum figure height to 'val' inches\n"
"   -W val : set paper width to 'val' inches\n"
"   -H val : set paper height to 'val' inches\n\n"
" The figure width and height arguments determine the maximum value\n"
" that these parameters may attain. The actual width or height may\n"
" be less if the other dimension is more constrained.\n\n"
" If a 'mazefile' argument is not provided, STDIN is read instead.\n\n");
}

/*:8*//*9:*/
#line 157 "./mazeprint.w"

float
checkopt(char*opt,float defval)
{
float conv= atof(opt);

if(conv> 0.0)return conv;
else{
fprintf(stderr,"Bad option value: %g; using default value %g\n",conv,defval);
return defval;
}
}

/*:9*//*14:*/
#line 275 "./mazeprint.w"

void
make_exit(int r,int c,int rows,int cols)
{
if(r==0)
printf("%d %d northexit\n",r,c);
else if(r==rows-1)
printf("%d %d southexit\n",r,c);
else if(c==0)
printf("%d %d westexit\n",r,c);
else printf("%d %d eastexit\n",r,c);
}

/*:14*//*15:*/
#line 294 "./mazeprint.w"

void
write_path(FILE*outf,int rows,int cols)
{
int r,c,val,i= 0,steps= 0;
int last_move= 0,this_move,lr,lc,shortened= 0;

/*16:*/
#line 311 "./mazeprint.w"

if(scanf("%d",&val)==1){
fprintf(outf,"%%Draw path through maze\nstartmaze\n startwalk");
r= I2R(val,cols);c= I2C(val,cols);
steps++;
}else return;

/*:16*/
#line 301 "./mazeprint.w"

/*17:*/
#line 339 "./mazeprint.w"

while(scanf("%d",&val)==1){
lr= r;lc= c;
r= I2R(val,cols);c= I2C(val,cols);
this_move= MOVR(r,lr)|MOVC(c,lc);
if(this_move!=last_move){
if(i++%7==0)printf("\n  ");
fprintf(outf,"%d %d PS  ",lr,lc);
last_move= this_move;
}else shortened++;
steps++;
}

/*:17*/
#line 302 "./mazeprint.w"

/*18:*/
#line 356 "./mazeprint.w"

fprintf(outf,"%d %d PS\n endwalk\nendmaze\n",r,c);
if(verbose_mode){
fprintf(stderr,"Path length: %d steps\n",steps);
fprintf(stderr,"Path reduction: %d steps\n",shortened);
}

/*:18*/
#line 303 "./mazeprint.w"

}

/*:15*//*20:*/
#line 376 "./mazeprint.w"

void
write_pshead(FILE*outf,int r,int c)
{
char today[40];

/*21:*/
#line 388 "./mazeprint.w"

{
time_t now;
struct tm tm_now;

time(&now);
localtime_r(&now,&tm_now);
strftime(today,sizeof(today),"%B %d, %Y %T",&tm_now);
}

/*:21*/
#line 382 "./mazeprint.w"

/*22:*/
#line 407 "./mazeprint.w"

fprintf(outf,"%%!PS-Adobe-3.0 EPSF-3.0\n");
/*26:*/
#line 483 "./mazeprint.w"

{
float xscale,yscale,scale;
float width,height,startx,starty;
float lw= LINE_WIDTH/POINTS_PER_INCH;

/*27:*/
#line 508 "./mazeprint.w"

xscale= (fig_width-lw)/c;
yscale= (fig_height-lw)/r;
if(xscale<yscale)scale= xscale;
else scale= yscale;
width= c*scale+lw;
height= r*scale+lw;

/*:27*/
#line 489 "./mazeprint.w"

/*28:*/
#line 521 "./mazeprint.w"

startx= ((page_width-width)/2.0);
starty= ((page_height-height)/2.0);

/*:28*/
#line 490 "./mazeprint.w"

/*29:*/
#line 530 "./mazeprint.w"

fprintf(outf,"%%%%Orientation: Portrait\n%%%%BoundingBox: %d %d %d %d\n",
(int)(startx*POINTS_PER_INCH),(int)(starty*POINTS_PER_INCH),
(int)ceilf((startx+width)*POINTS_PER_INCH),
(int)ceilf((starty+height)*POINTS_PER_INCH));
fprintf(outf,"%%%%HiResBoundingBox: %.3f %.3f %.3f %.3f\n",
(startx*POINTS_PER_INCH),(starty*POINTS_PER_INCH),
(startx+width)*POINTS_PER_INCH,
(starty+height)*POINTS_PER_INCH);
if(verbose_mode){
fprintf(stderr,"Figure WxH = %gx%g inches\n",width,height);
fprintf(stderr,"HiResBoundingBox: %.3f %.3f %.3f %.3f\n",
(startx*POINTS_PER_INCH),(starty*POINTS_PER_INCH),
(startx+width)*POINTS_PER_INCH,
(starty+height)*POINTS_PER_INCH);
}

/*:29*/
#line 491 "./mazeprint.w"

}

/*:26*/
#line 409 "./mazeprint.w"

fprintf(outf,"%%%%Pages: 1\n%%%%Creator: mazeprint.w by Bret Whissel, FSU "
"Computer Science Dept\n%%%%Title: %dx%d Maze\n",r,c);
fprintf(outf,"%%%%CreationDate: (%s)\n",today);
fprintf(outf,"%%%%DocumentMedia: Plain %d %d 0 () ()\n",
(int)(page_width*POINTS_PER_INCH),(int)(page_height*POINTS_PER_INCH));
fprintf(outf,"%%%%EndComments\n");
/*23:*/
#line 422 "./mazeprint.w"

fprintf(outf,"%%%%BeginProlog\n");
/*24:*/
#line 446 "./mazeprint.w"

fprintf(outf,"/in { 72 mul } def\n"
"/PageXSz %g in def\n/PageYSz %g in def\n"
"/FigMaxXSz %g in def\n/FigMaxYSz %g in def\n",
page_width,page_height,fig_width,fig_height);

/*:24*/
#line 424 "./mazeprint.w"

/*25:*/
#line 456 "./mazeprint.w"

fprintf(outf,"/mazesetup {\n"
"    /cols exch def\n"
"    /rows exch def\n"
"    /lw %.3f def\n"
"    /xscale FigMaxXSz lw sub cols div def\n"
"    /yscale FigMaxYSz lw sub rows div def\n"
"    xscale yscale lt\n"
"      { /mazescale xscale def }\n"
"      { /mazescale yscale def } ifelse\n"
"    /MazeWidth cols mazescale mul def\n"
"    /MazeHeight rows mazescale mul def\n"
"    /StartLocX PageXSz MazeWidth sub 2 div def\n"
"    /StartLocY PageYSz MazeHeight sub 2 div MazeHeight add def\n"
"} def\n",LINE_WIDTH);

/*:25*/
#line 425 "./mazeprint.w"

/*31:*/
#line 567 "./mazeprint.w"

fprintf(outf,"/m {moveto} bind def\n"
"/l {lineto} bind def\n"
"/cp {closepath} bind def\n"
"/gs {gsave} bind def\n"
"/gr {grestore} bind def\n"
"/tr {translate} bind def\n"
"/n {newpath} bind def\n"
"/s {stroke} bind def\n"
"/sa {s 1 0 tr} bind def\n");

/*:31*/
#line 426 "./mazeprint.w"

/*30:*/
#line 555 "./mazeprint.w"

fprintf(outf,"/startmaze {\n"
"    gs\n"
"    StartLocX StartLocY tr\n"
"    mazescale dup scale\n"
"    %g dup 3 mul mazescale gt\n"
"      { pop 0 }\n"
"      { mazescale div }\n"
"    ifelse setlinewidth\n"
"} def\n"
"/endmaze { gr } def\n",LINE_WIDTH);

/*:30*/
#line 427 "./mazeprint.w"

/*32:*/
#line 583 "./mazeprint.w"

fprintf(outf,""
"/A { n sa } def\n"
"/B { n 0 1 m 1 1 l sa } def\n"
"/C { n 1 1 m 1 0 l sa } def\n"
"/D { n 0 1 m 1 1 l 1 0 l sa } def\n"
"/E { n 0 0 m 1 0 l sa } def\n"
"/F { n 0 1 m 1 1 l 0 0 m 1 0 l sa } def\n"
"/G { n 0 0 m 1 0 l 1 1 l sa } def\n"
"/H { n 0 1 m 1 1 l 1 0 l 0 0 l sa } def\n"
"/I { n 0 0 m 0 1 l sa } def\n"
"/J { n 0 0 m 0 1 l 1 1 l sa } def\n"
"/K { n 0 0 m 0 1 l 1 1 m 1 0 l sa } def\n"
"/L { n 0 0 m 0 1 l 1 1 l 1 0 l sa } def\n"
"/M { n 0 1 m 0 0 l 1 0 l sa } def\n"
"/N { n 1 1 m 0 1 l 0 0 l 1 0 l sa } def\n"
"/O { n 0 1 m 0 0 l 1 0 l 1 1 l sa } def\n"
"/P { n 0 0 m 0 1 l 1 1 l 1 0 l cp sa } def\n"
);

/*:32*/
#line 428 "./mazeprint.w"

/*33:*/
#line 606 "./mazeprint.w"

fprintf(outf,"/startrow { 0 -1 tr gs } def\n"
"/endrow { gr } def\n");

/*:33*/
#line 429 "./mazeprint.w"

/*34:*/
#line 613 "./mazeprint.w"

fprintf(outf,"/mvc { exch neg 1 sub tr } def\n"
"/clr { gs n 0.16 0 m 0.84 0 l 1 mazescale div setlinewidth\n"
"       1 setgray s gr } def\n"
"/tcks { n 0.16 0.16 m 0.16 -0.16 l 0.84 0.16 m 0.84 -0.16 l s } def\n"
"/northexit { gs mvc 0 1 tr clr tcks gr } def\n"
"/southexit { gs mvc clr tcks gr } def\n"
"/westexit { gs mvc 90 rotate clr tcks gr } def\n"
"/eastexit { gs mvc 1 0 tr 90 rotate clr tcks gr } def\n");

/*:34*/
#line 430 "./mazeprint.w"

/*35:*/
#line 624 "./mazeprint.w"

fprintf(outf,
"/red { 1 0 0 setrgbcolor } def\n"
"/green { 0 0.7 0 setrgbcolor } def\n"
"/blue { 0.4 0.4 1 setrgbcolor } def\n"
"/yellow { 1 1 0 setrgbcolor } def\n");

/*:35*/
#line 431 "./mazeprint.w"

/*36:*/
#line 632 "./mazeprint.w"

fprintf(outf,"/Dot { gs mvc n 0.5 0.5 0.33 0 360 arc cp fill gr } def\n");

/*:36*/
#line 432 "./mazeprint.w"

/*37:*/
#line 645 "./mazeprint.w"

fprintf(outf,"/PS2 { exch neg l } bind def\n");
fprintf(outf,"/PS1 { exch neg m /PS { PS2 } bind def } bind def\n");

/*:37*/
#line 433 "./mazeprint.w"

/*38:*/
#line 660 "./mazeprint.w"

fprintf(outf,"/startwalk { gs 0.5 -0.5 tr n /PS { PS1 } bind def } def\n");
fprintf(outf,"/endwalk { blue 1 setlinecap 2 setlinejoin\n"
"    %g setlinewidth s gr } def\n",PATH_WIDTH);

/*:38*/
#line 434 "./mazeprint.w"

fprintf(outf,"%%%%EndProlog\n");

/*:23*/
#line 416 "./mazeprint.w"


/*:22*/
#line 383 "./mazeprint.w"

}

/*:20*/
#line 27 "./mazeprint.w"

/*5:*/
#line 70 "./mazeprint.w"

int
main(int argc,char*argv[])
{
int rows,cols,srow,scol,erow,ecol;

/*6:*/
#line 98 "./mazeprint.w"

{
extern char*optarg;
extern int optind,opterr,optopt;
while(1){
int c;
c= getopt(argc,argv,"vw:h:W:H:?");
if(c==-1)break;
switch(c){
case 0:break;
case'v':verbose_mode= TRUE;break;
case'w':fig_width= checkopt(optarg,fig_width);break;
case'h':fig_height= checkopt(optarg,fig_height);break;
case'W':page_width= checkopt(optarg,page_width);break;
case'H':page_height= checkopt(optarg,page_height);break;
case'?':
default:showhelp(argv);exit(0);
}
}
/*7:*/
#line 128 "./mazeprint.w"

if(optind&&argv[optind]&&strlen(argv[optind])> 0){
if(freopen(argv[optind],"r",stdin)==NULL){
fprintf(stderr,"Maze file \"%s\" not opened\n",argv[optind]);
exit(1);
}
}

/*:7*/
#line 117 "./mazeprint.w"

}

/*:6*/
#line 76 "./mazeprint.w"

/*10:*/
#line 172 "./mazeprint.w"

if(scanf("%d %d\n",&rows,&cols)!=2){
fprintf(stderr,"Scanning for maze parameters failed\n");
exit(1);
}else if((rows<=0)||(cols<=0)){
fprintf(stderr,"Bad maze size parameters: %d rows %d cols\n",
rows,cols);
exit(1);
}else if(verbose_mode)
fprintf(stderr,"Processing %dx%d maze\n",rows,cols);

/*:10*/
#line 77 "./mazeprint.w"


write_pshead(stdout,rows,cols);

/*39:*/
#line 669 "./mazeprint.w"

printf("%%%%Page: 1 1\n"
"%%%%BeginPageSetup\n"
"75 dict begin\n"
"/pagesave save def\ntrue setstrokeadjust\n"
"%%%%EndPageSetup\n");

/*:39*/
#line 81 "./mazeprint.w"

/*11:*/
#line 206 "./mazeprint.w"

{
int i,j,val;
static char outstr[]= "ABCDEFGHIJKLMNOP";

printf("%d %d mazesetup\n%%Draw maze outline\nstartmaze\n",rows,cols);
for(i= 0;i<rows;i++){
printf("  startrow");
for(j= 0;j<cols;j++){
if(j%30==0)printf("\n   ");
if(scanf("%d",&val)!=1)
break;
if(val<0||val> 15)
printf(" A");
else
printf(" %c",outstr[val]);
}
printf("\n  endrow\n");
}
printf("endmaze\n");
}

/*:11*/
#line 82 "./mazeprint.w"

/*13:*/
#line 245 "./mazeprint.w"

{
int startcell,goalcell;

if(scanf("%d %d",&startcell,&goalcell)!=2){
srow= scol= erow= ecol= -1;
if(verbose_mode)
fprintf(stderr,"Couldn't retrieve path start and end positions\n");
}else{
srow= I2R(startcell,cols);scol= I2C(startcell,cols);
erow= I2R(goalcell,cols);ecol= I2C(goalcell,cols);

printf("startmaze\n");
/* make_exit(srow,scol,rows,cols); */ /* RCL: don't want to make a hole in a cell */
/* make_exit(erow,ecol,rows,cols); */
printf("endmaze\n");
if(verbose_mode)
fprintf(stderr,"Start Cell=(%d,%d)  End Cell=(%d,%d)\n",
srow,scol,erow,ecol);
}
}

/*:13*/
#line 83 "./mazeprint.w"


write_path(stdout,rows,cols);

/*19:*/
#line 366 "./mazeprint.w"

if(srow>=0){
printf("startmaze\n");
printf("  red %d %d Dot green %d %d Dot\n",srow,scol,erow,ecol);
printf("endmaze\n");
}

/*:19*/
#line 87 "./mazeprint.w"

/*40:*/
#line 679 "./mazeprint.w"

printf("pagesave restore end\nshowpage\n%%%%EOF\n");

/*:40*/
#line 88 "./mazeprint.w"

exit(0);
}

/*:5*/
#line 28 "./mazeprint.w"


/*:1*/
