//TETRIS
//correct rotate
//add buttons if possible
//add special block
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <time.h>
#include <sstream>
using namespace std;

static const int side = 30; //side of each basic square
static const int length = 24;
static const int breadth = 15;
static const int bg = 25; //background colour

void assign(int i, int x[], int y[], bool &r)     //here i is a random int from 0 to 6
{                                                  //a function to assign coordinates whenever a Block or next block is created 
  switch(i){
    case 0:{x[0]=x[2]=7; x[1]=6; x[3]=8;
      y[0]=y[1]=y[3]=1; y[2]=0; r=1; } break;
    case 1:{x[0]=x[3]=6; x[1]=x[2]=7;   //square block
      y[0]=y[1]=0; y[2]=y[3]=1; r=0;} break;
    case 2:{x[0]=x[1]=x[2]=x[3]=7;      //straight line
      y[0]=3; y[1]=2; y[2]=1; y[3]=0; r=1; } break;
    case 3:{x[3]=7; x[1]=x[2]=x[0]=6;   //right L
      y[0]=y[3]=2; y[1]=1; y[2]=0; r=1;} break;
    case 4:{x[3]=6; x[1]=x[2]=x[0]=7;    //left L
      y[0]=y[3]=2; y[1]=1; y[2]=0; r=1;} break;
    case 5:{x[0]=x[1]=7; x[2]=8; x[3]=6;  //right something
      y[1]=y[3]=0; y[2]=y[0]=1; r=1;} break; 
    case 6:{x[0]=x[1]=7; x[2]=6; x[3]=8;  // left something
      y[1]=y[3]=0; y[2]=y[0]=1; r=1;} break;
  }
}
class Screen1 : public Fl_Widget{

  class Block{                       //class Block is encapsulated within Screen1
  public:
  int x[4], y[4], c; bool r;   //the 2 array represent the x and y coord. of the current piece and c represents color and r its rotatibility.   
  int nx[4], ny[4], nc; bool nr; //same as above for the next piece
  
  Block(int k, int c);
  bool left();                     // functions to move the obj of class Block left, right, down or to rotate it          
  bool right();
  bool down();
  bool rotate(bool b, Screen1 *s);
  void createNext();               //this function creates new attributes for the next block
};
  
  Block obj; bool go; 
  int colour[breadth][length], score;
  Fl_Box * scoreBox, * comment, *endBox, *next;
  char* scoreLabel; // label used to display the score
  
  public: int l;
  Screen1(int k, int c);  //constructor
  bool checkSpace(int i); //to check space on screen in a given direction
  void freeFall();        //to directly settle the Block
  int periodic1();
  int periodic();         //to move the piece down automatically after a fixed period of time
  void draw();            
  int handle(int e);
  bool checkRow(int rowNo);  //to check whether a given row is filled
  void deleteRow(int rowNo); //to delete a filled row
  bool settlePiece();        //to transfer the attributes of a Block to the screen
  void setScoreBox(Fl_Box ** scoreBox, Fl_Box ** commentBox, Fl_Box ** end, Fl_Box ** nx); //to initialize various Fl_Box pointers
};
void timeractions1(void *p) {              //callback function for timeout during freeFall
        ((Screen1 *)p)->freeFall ();
}
void Screen1::freeFall()
{
  if(checkSpace(1)){ obj.down();redraw();}
  else return;
  Fl::repeat_timeout(0.01, timeractions1, this);
}
  
void timeractions(void *p) {               //callback function for timeout to move the Block down periodically 
        ((Screen1 *)p)->periodic ();
}
void timeractions2(void *p) {               //callback function for timeout to move the Block down periodically 
        ((Screen1 *)p)->periodic1 ();
}
int Screen1::periodic1() {
  Fl::repeat_timeout (0.1,timeractions2,this);
}
void Screen1:: draw()
{
  if(l==0) Fl::repeat_timeout (0.1,timeractions2,this);
  else{for(int j=0; j<length; j++){ 
    for(int i=0; i<breadth; i++)             //to draw main screen
      fl_draw_box(FL_BORDER_BOX,i*side+120,j*side,side,side,colour[i][j]);
    for(int i=breadth; i<breadth+4; i++)      //to draw adjacent area containing next block
      fl_draw_box(FL_BORDER_BOX,i*side+120,j*side,side,side,bg+2);
  }
  
  for(int i=0; i<4; i++){
    fl_draw_box(FL_BORDER_BOX,side*(obj.x[i])+120,side*(obj.y[i]),side,side,obj.c);
    fl_draw_box(FL_BORDER_BOX,side*(obj.nx[i])+side*10+120,side*(obj.ny[i])+300,side,side,obj.nc);
  }
 
  next->label("Next");  
  if(go){ endBox->size(200,200);endBox->label("Game Over ");
    endBox->box(FL_UP_BOX);
    endBox->labelfont(FL_BOLD+FL_ITALIC);
    endBox->labelsize(36);
    endBox->labeltype(FL_SHADOW_LABEL);
    endBox->box(FL_UP_BOX);
    endBox->labelfont(FL_BOLD+FL_ITALIC);
    endBox->labelsize(36);
    endBox->labeltype(FL_SHADOW_LABEL);
    endBox->labelcolor(23); redraw();
    
    } }
}
void Screen1::setScoreBox(Fl_Box** sb,Fl_Box** comb, Fl_Box** end, Fl_Box** nx)
{
  scoreBox = *sb ;
  comment=*comb;
  endBox=*end;
  next=*nx;
  scoreLabel = (char*) malloc(sizeof(char)*10);
}
Screen1::Screen1(int k, int c) : Fl_Widget (120,0,breadth*side+4*side,length*side,"TETRIS"), obj(Block(k,c)) //Constructor of class Screen1
{
  for(int i=0; i<breadth; i++)
    for(int j=0; j<length; j++)
      colour[i][j]=bg; l=0;
  score=0; go=false;// for gameover if go=true, then gameover
}
int Screen1::handle(int e)
{  if(l==0) Fl::repeat_timeout (0.9,timeractions2,this);
  else{if(go) {redraw(); return 1;}
  if(e==8)
  {//int k=Fl::event_key()-60000;         //spacebar=32; a=97
    switch(Fl::event_key()){
      case 65361: 
      if(checkSpace(0)) obj.left(); break;
      case 65364:
      if(checkSpace(1)) obj.down(); break;
      case 65363:
      if(checkSpace(2)) obj.right(); break;
      case 115:
      obj.rotate(true,this); break;  //to rotate clockwise
      case 97:
      obj.rotate(false,this); break;  //to rotate counterclockwise
      case 32:
      Fl::repeat_timeout(0.01, timeractions1, this); break;
    }}
    stringstream strs;        //to convert score into string
        strs << score;
        string temp_str = strs.str();
        strcpy(scoreLabel,"Score: \n");
        strcat(scoreLabel,(char*) temp_str.c_str() );
    scoreBox->label(scoreLabel);
    redraw();}
}  
bool Screen1::checkSpace(int i)
{
  bool ret=true;
  switch(i){
    case 0:
       
      for(int i=0; i<4; i++){
        if(obj.x[i]==0){return false;}
        if(colour[obj.x[i]-1][obj.y[i]]!=bg) {ret=false; break;}
      }
      break;
    case 1:
       for(int i=0; i<4; i++){
        if(obj.y[i]==23){return false;}
        if(colour[obj.x[i]][obj.y[i]+1]!=bg) {ret=false; break;}
      }
      break;
    case 2:
       for(int i=0; i<4; i++){
        if(obj.x[i]==14){return false;}
        if(colour[obj.x[i]+1][obj.y[i]]!=bg) {ret=false; break;}
      }
      break;
      
  }
  return ret;
}
int Screen1::periodic() {
  if(l==0) Fl::repeat_timeout (0.1,timeractions,this);
  else {if(go) return 1;
  if(checkSpace(1))obj.down();
  else settlePiece(); 
  redraw();
  Fl::repeat_timeout (1-l*0.2,timeractions,this);}
}
bool Screen1::checkRow(int rowNo)
{
  bool ret=true;
  for(int i=0; i<breadth; i++)
    if(colour[i][rowNo]==bg){
          return false; break;
        }
  return ret;
}

bool Screen1::settlePiece()
{ int row[4];
  //vector <int> row;
  if(go) return false; int num=0;
  for(int i=0; i<4; i++)
    colour[obj.x[i]][obj.y[i]]=obj.c;
  for(int i=0; i<4; i++){
   if(i!=0)if(obj.y[i]==obj.y[i-1]) continue;
   if(checkRow(obj.y[i])) {deleteRow(obj.y[i]); i--; score+=10; num++;}
  }
  switch(num){
    case 1: comment->label("Good job"); break;
    case 2: comment->label("Well Done"); break;
    case 3: comment->label("Awesome"); break;
    case 4: comment->label("You Rock!"); break;
  }
  redraw();
  for(int i=0; i<4; i++){
    obj.x[i]=obj.nx[i]; obj.y[i]=obj.ny[i];}
  obj.c=obj.nc; obj.r=obj.nr;  
  obj.createNext();
  if(!checkSpace(1)) go=true;
}
void Screen1::deleteRow(int rowNo)
{
  for(int i=0; i<breadth; i++){
    for(int j=rowNo; j>0; j--)
      colour[i][j]=colour[i][j-1];
    colour[i][0]=bg;
  }
}
    
Screen1::Block::Block(int k, int ck)
{ 
  int z=rand()%7;
  nc=rand()%256;
  if(nc==bg || nc== bg+2) nc=nc+10;
  c=ck;
  assign(k, x, y, r); 
  assign(z, nx, ny, nr);
}
bool Screen1::Block::left()
{
  for(int i=0; i<4; i++) x[i]--;
}
bool Screen1::Block::right()
{
  for(int i=0; i<4; i++) x[i]++;
}
bool Screen1::Block::down()
{
  for(int i=0; i<4; i++) y[i]++;
}
void Screen1::Block::createNext()
{
  int z=rand()%7;
  nc=rand()%256;
  if(nc==bg || nc==bg+2)nc=nc+10;
  assign(z, nx, ny, nr);
}
bool Screen1::Block::rotate(bool b, Screen1 * s)        //b=0 counterclockwise; b=1 clockwise;
{  
  int p[4], q[4];
  if(!r) return false;
  for(int i=0; i<4; i++) {p[i]=x[i]; q[i]=y[i];}
  if(b)for(int i=0;i<4;i++){ 
   x[i]=x[0]-y[0]+q[i]; y[i]=y[0]-p[i]+x[0];}
   else for(int i=0;i<4;i++){ 
   x[i]=x[0]+y[0]-q[i]; y[i]=y[0]+p[i]-x[0];}
   for(int i=0; i<4; i++) {if(!(x[i]<14 && x[i]>=0 && y[i]<25)){for(int i=0; i<4; i++){x[i]=p[i];y[i]=q[i];}return false;}
   if(s->colour[x[i]][y[i]]!=bg){for(int i=0; i<4; i++){x[i]=p[i];y[i]=q[i];}return false;}      //to not do anything when the new coordinates are already occupied
  }
} 
void level1_cb(Fl_Widget* obj, void* p) {((Screen1 *)p)->l=1;}
void level2_cb(Fl_Widget* obj, void* p) {((Screen1 *)p)->l=2;}
void level3_cb(Fl_Widget* obj, void* p) {((Screen1 *)p)->l=3;}
void cb_quit( Fl_Button*obj , void* v ) { exit(0);}
int main()
{
  time_t seconds; //Get value from system clock and place in seconds variable. 
  time(&seconds);//Convert seconds to a unsigned integer.
  int m=1; srand((unsigned int) seconds);  //seeding random function
  Fl_Window * window = new Fl_Window(1000,800,"TETRIS");
  
  window->color(0);
  int num=rand()%7;             //to generate a random color and num for the first Block
  int col=rand()%256;
  if(col==bg || col==bg+2)col=col+10;
  Screen1 *s = new Screen1(num, col);
 
  Fl_Box * level= new Fl_Box(20,250,100,40,"LEVELS");  //buttons to select level
  Fl_Button * level1 = new Fl_Button(40,300,50,20,"Level 1");
  Fl_Button * level2 = new Fl_Button(40,330,50,20,"Level 2");
  Fl_Button * level3 = new Fl_Button(40,360,50,20,"&Level 3");
  level1->callback((Fl_Callback*) level1_cb,s);
  level2->callback((Fl_Callback*) level2_cb,s);
  level3->callback((Fl_Callback*) level3_cb,s);
  Fl_Button * quit = new Fl_Button( side*breadth+260, 420,  70, 30, "&Quit" );
  quit->callback(( Fl_Callback*)cb_quit, s);

  Fl_Box * scoreBox = new Fl_Box(side*breadth+240, 50, 200, 180, "Score :\n 0");
  Fl_Box * next = new Fl_Box(side*breadth+140, 100, 80, 80, "Next");
  Fl_Box * commentBox = new Fl_Box(side*breadth+240, 500, 250, 250, "Welcome To\n TETRIS\n Please select\nLEVEL");
  Fl_Box * Box1 = new Fl_Box(0, 400, 100, 300, "Please \npress -> \nor <-\n 3-4 times\n after \nselecting \nlevel");
  Fl_Box *endBox = new Fl_Box(180,100,0,0,"");
  scoreBox->box(FL_UP_BOX);  next->box(FL_UP_BOX);  commentBox->box(FL_UP_BOX); level->box(FL_UP_BOX); Box1->box(FL_UP_BOX);
  scoreBox->labelfont(FL_BOLD+FL_ITALIC); next->labelfont(FL_BOLD+FL_ITALIC);  commentBox->labelfont(FL_BOLD+FL_ITALIC);
  scoreBox->labelsize(36);  next->labelsize(36); commentBox->labelsize(10);
  scoreBox->labeltype(FL_SHADOW_LABEL); next->labeltype(FL_SHADOW_LABEL);
  scoreBox->box(FL_UP_BOX); next->box(FL_UP_BOX); commentBox->labeltype(FL_SHADOW_LABEL);
  scoreBox->labelfont(FL_BOLD+FL_ITALIC); next->labelfont(FL_BOLD+FL_ITALIC);
  scoreBox->labelsize(36); next->labelsize(18); commentBox->box(FL_UP_BOX);
  scoreBox->labeltype(FL_SHADOW_LABEL);  next->labeltype(FL_SHADOW_LABEL);  commentBox->labelfont(FL_BOLD+FL_ITALIC);
  commentBox->labelsize(36);
  commentBox->labeltype(FL_SHADOW_LABEL);
  s->setScoreBox(&scoreBox, &commentBox, &endBox, &next);
  window->end();
  window->show();
  Fl::add_timeout(2, timeractions,s);
  return(Fl::run());
}
