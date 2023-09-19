#include "game.h"



eDirecton snake_move = STOP;

int level = 0;

bool Mouse_On = false;

bool gameover = false, gameend = false, menuend = false, snakedead = true, pausegame = false;
bool Music_on = true, SoundEffect_on = true;

int score = 0, speed = 150;
int Snake_long = 0;

int index_start =1, index_end=1;

int Menu_choice=0, pause_choice=0;

bool Continue=false;

char name[41] = "2112042921120433211204342112043621120451";


position Snake = { 0,0 }, Food = { 0,0 }, Tail[100] = { 0,0 };
position lasttail = { 0,0 };
position Wall[500] = { 0,0 };
int index_wall = 0;


int Gate_direction = 0;
position Gate = { 0,0 }, Block[2] = { 0,0 };
int col = rand() % 15 + 1;
bool gate_on = false;

bool gate_into = false, gate_out = false;

PLAYER* Player = new PLAYER[25];


mutex mtx;

POINT real_mouse = { 0,0 };
position mouse = {0,0};


bool bossfight = false;
int ticks_count = 0;
int index_warning = 0;
bool warning_exist = false;
int direction = 0, randomstop =0;
bool meteor_check = false;
bool meteoralive = false;
bool meteordeleted = false;
int meteorsize = 12;
int delcount = 0;
int meteor_index_start = 0, meteor_index_end = 1;

char meteormodel[10][11] =
{ "/OOOOOOOO/",
 "OXXXXXXXXO",
 "OXXXXXXXXO",
 "OXXXXXXXXO",
 "OXXXXXXXXO",
 "OXXXXXXXXO",
 "OXXXXXXXXO",
 "OXXXXXXXXO",
 "OXXXXXXXXO",
 "/OOOOOOOO/" };



eDirecton meteor_move = STOP;

position meteor = {0,0}, meteorTail[200]={0,0}, lastmeteortail={0,0}, lastmeteortail2={0,0}, Warning[37] = {0,0};

char intromodel[32][59] =
{ "/////////BBBB//BBBB//////////////////////////////////////",
"//BB////BGGDDBBBGGGB//////BBB//BB////B///BB///B///B/BBBBB",
"/BGGBBBBDDGBBBGGGGGGB////B///B/B/B///B//B//B//B//B//B////",
"/BGLGGGGGLGGGGGWBBGGGB///B/////B/B///B/B////B/B/B///B////",
"BDDGGGLGGGGGGGGGGGGGGB////BBB//B//B//B/BBBBBB/BB////BBBBB",
"BBDDDDDDDDDDDDGGGGGGGB///////B/B///B/B/B////B/B/B///B////",
"B/BBBBBBBBBBBDDGLGGLGB///B///B/B///B/B/B////B/B//B//B////",
"B/B///BBRRRRRBDGGGGGGB////BBB//B////BB/B////B/B///B/BBBBB",
"/BB////BBBBRRBDGGGGGGB///////////////////////////////////",
"//B///B////BRBDLGLGGB////////////////////////////////////",
"//BB///////BBDDGGGGGB////////////////////////////////////",
"///////BBBBBBDGGGGGB///////////BBBBBB////////////////////",
"//////BBGGGGGGLGGGLB//////////BGGGGGGBB//////////////////",
"//////BBBBBBBDGLGGB//////////BGGGLGGGGGB/////////////////",
"//////////BYYBDGGGB/////////BDGGGGGGGLGGB/////////BBBB///",
"///////////BYBDGGGB////////BDGLGGGGGGGGGB///////BBGLGBB//",
"///////////BBBLGGUBB//////BDGGGGLGGGLGGGGB////BBGGGGB////",
"///////////BYBDGGGGB/////BDDGGGGGGLGGGGGGB///BDDGLGB/////",
"///////////BYYBLGGGGB////BLGGGGGGGGGGGGLGB//BDDGGGB//////",
"///////////BBBBDGLGGGB//BDDGGLGLGGGBBGGGGGB/BDLGLGB//////",
"///////////BYYBDDGGGLGBBDDBGGGGGGBBDDBGGGGBBDDGGGB///////",
"////////////BYYBDDGGGGLBBBGLGGGGBYBDDDBGGGGBDDGGGB///////",
"////////////BBBBDDDGGGGGGGGGGGGBBYBBDLDGGLGBDLGGLB///////",
"////////////BYYYBDLGGLGGLGGLGDB//BYBDDDGGGGBDDGGGB///////",
"/////////////BYYYBDDGGGGGGGGGDB//BYYBDLDGGLGBDGGGB///////",
"/////////////BBBBBDDDGGGLGGGLDB//BBBBDDDGGGGGBGGB////////",
"//////////////BYYYBDDDGGGGGDDB////BYBDDDGGGGGGGGB////////",
"//////////////BYYYYBDLDGGLGDDB////BYYBDDLGGGGGLGB////////",
"///////////////BBBBBBDDDDDDDB/////BBBBDDDDLGGGGGB////////",
"///////////////BBYYYYBDDDLDDB//////BYYBDDDGGGGLGB////////",
"/////////////////BYYYYBDDDDB///////BBYBBDLDGGGGB/////////",
"/////////////////BBBBBBBBBB/////////BBBBBBBBBBBB/////////" };


position intro = {58,2};
int intro_index_end = 0;
eDirecton intro_move = STOP;
int introsize = 60;
bool introplayed = false;
position introTail[1900] = {0,0}, lastintrotail={0,0};

int stop_ticks_count = 0;



int setup_speed = speed;








int main( ) {
	srand(time(NULL));
	PlaySound(NULL, NULL, 0);
	setupconsole();
	/*while (introplayed == false) {
		PlaySound(TEXT("opening.wav"), NULL, SND_ASYNC | SND_NOSTOP);
		snakeintro();
	}*/
	while (!gameend) {
		if (!menuend) {
			drawmenubox();
			if (Music_on) PlaySound(TEXT("realsoundrtack.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

		}
		while (!menuend) {

			menu();
		}
		if (!gameover) {
			PlaySound(NULL, NULL, 0);
			drawmap(false);
			drawfood();
		}
		while (!gameover) {
			drawsnake();
			if (level >= 4) boss();
			drawdetail();
			if (Continue) {

				if (gate_on) {
					drawgate();
					GotoXY(Food.x, Food.y);
					Color(0, 0);
					cout << " ";
					Color(0, 7);
				}
				switch (_getch())
				{
				case 27:
					gameover = true;
					snakedead = false;
					menuend = false;
					save_slot_out(false);
				default:
					Continue = false;
					break;
				}
				Menu_choice = 0;
			}
			Logic();
			if (gate_on) drawgate();
			Sleep(speed);
			GotoXY(0, 0);
			
		}
		if (snakedead) {
			Dead();
			Afterdead();
			SaveScore();
		}
	}
	system("cls");
	GotoXY(0, 0);


	return 0;
}
