#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <string.h>
#include <fstream>
#include <thread>
#include <mutex>
#pragma comment(lib, "winmm.lib")
using namespace std;

#define WIDTH 60
#define HEIGHT 35
#define WALL 'X'



enum eDirecton { STOP = 0, LEFT, RIGHT, UP, DOWN };
extern eDirecton snake_move;

extern int level;

extern bool Mouse_On;

extern bool gameover, gameend, menuend, snakedead, pausegame;
extern bool Music_on, SoundEffect_on;

extern int score, speed;
extern int Snake_long;

extern int index_start, index_end;

extern int Menu_choice, pause_choice;

extern bool Continue;

extern char name[41];

struct position {
	int x, y;
};

extern position Snake, Food, Tail[100];
extern position lasttail;
extern position Wall[500];
extern int index_wall;

extern int Gate_direction;
extern position Gate, Block[2];
extern int col;
extern bool gate_on;

extern bool gate_into, gate_out;

struct PLAYER
{
	string Name;
	int ScoreSaved{};
};
extern PLAYER* Player;

extern int ticks_count;
extern bool warning_exist;
extern int index_warning;
extern position Warning[37];
extern bool wall_into;

extern mutex mtx;

extern POINT real_mouse;
extern position mouse;


struct MEMBER
{
	char id[10];
	char Name[100];
	char pos[100];
	char prop[500];
};

extern bool bossfight;
extern int ticks_count;
extern int index_warning;
extern bool warning_exist;
extern int direction , randomstop;
extern bool meteor_check;
extern bool meteoralive;
extern bool meteordeleted;
extern int meteorsize;
extern int delcount;
extern int meteor_index_start, meteor_index_end;

extern char meteormodel[10][11];
extern eDirecton meteor_move;

extern position meteor, meteorTail[200], lastmeteortail, lastmeteortail2, Warning[37];


extern char intromodel[32][59];
extern eDirecton intro_move;
extern position intro;
extern int intro_index_end;
extern int introsize;
extern bool introplayed;
extern position introTail[1900], lastintrotail;
extern int stop_ticks_count;



extern int setup_speed;


void maxsc();
void ConsoleSize(SHORT width, SHORT height);
void BanZoom();
void HideScrollBar(BOOL Show);
void Console();
void Hidemouse(bool CursorVisibility);
void setupconsole();
void Color(int backgound_color, int text_color);
void GotoXY(int x, int y);



void drawblack(position start_point, int Width, int height);
void drawmenubox();
void menu();
void snake_menu();

void choicebar();
void highscore_menu();
void setting_menu(int move_speed);
void introduction();
int member(int member_choice, bool Member_check = false);




bool save_slot_out(bool check);
void save_slot_in();



void setup();
void drawWALL();
void drawbox(position start_point,int Width,int height,int color);
void drawdetail();
void drawmap(bool sleep_right);
void drawsnake();





void drawfood();
void drawgate();
void Gate_collision();
void newword();


void Before_mapD2();
void Before_mapD3();
void Before_mapD4();
void drawwall(bool check);


void drawwarning();
void deletewarning();
void spawnmeteor();
void drawmeteor_ht(int part);
void drawmeteor_vt(int part);
void meteortailxy();
void meteormove();
void deletemeteor(int part);
void mapboss();
void boss();













void spawnfood();
void Input();
void reasonofdead();
void tailxy();
void move();
void Logic();


void pause();
void pausemenu();
void setting_in_game();


void Dead();
void Afterdead();


void savedetail(string text);
void Loaddetail(string text);

void SaveScore();

int Down(const void* a, const void* b);
void Sort_Score();

void File_To_Player();

bool sure_board(string word, position start_point, int width, int height);

bool Gate_near_wall();



void drawintro(int part);
void intromove();
void snakeintro();




void win();