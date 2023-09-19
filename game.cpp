#include "game.h"


// Setup cái console trước
// Kích cỡ tối đa
void maxsc()
{
	HWND Hwnd = GetForegroundWindow();
	ShowWindow(Hwnd, SW_MAXIMIZE);
}
// Kích thước khi bị thu nhỏ console

void ConsoleSize(SHORT width, SHORT height)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	SMALL_RECT WindowSize;
	WindowSize.Top = 0;
	WindowSize.Left = 0;
	WindowSize.Right = width;
	WindowSize.Bottom = height;

	SetConsoleWindowInfo(hStdout, 1, &WindowSize);

}

//Ngăn cản việc phóng to thu nhỏ làm xấu console
void BanZoom()
{
	HWND hWnd = GetConsoleWindow();
	SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);
}

// Ngăn cản việc trượt qua trượt lại
void HideScrollBar(BOOL Show)
{
	HWND hWnd = GetConsoleWindow();
	ShowScrollBar(hWnd, SB_BOTH, Show);
}

// Set up con trỏ, kiểu chữ
void Console() {
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 18;                   // Width of each character in the font
	cfi.dwFontSize.Y = 18;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Terminal"); // Choose your font
	SetCurrentConsoleFontEx(hStdout, FALSE, &cfi);
	SMALL_RECT rect = { 0,0,WIDTH + 17,HEIGHT };
	SetConsoleWindowInfo(hStdout, TRUE, &rect);
	SetConsoleScreenBufferSize(hStdout, { WIDTH + 17,HEIGHT });
	SetConsoleActiveScreenBuffer(hStdout);
	DWORD BytesWritten = 0;
}

// Ẩn con trỏ
void Hidemouse(bool CursorVisibility)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO ConCurInf;

	ConCurInf.dwSize = 10;
	ConCurInf.bVisible = CursorVisibility;

	SetConsoleCursorInfo(hStdout, &ConCurInf);
}

void setupconsole() {
	ConsoleSize(2, 2);
	Console();
	BanZoom();
	HideScrollBar(0);
	Hidemouse(0);
	// Đặt tên cho terminal
	SetConsoleTitle(L"SNAKE(TEAM 15)");
	maxsc();
}

void Color(int backgound_color, int text_color)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	/*0 = Black
	1 = Blue
	2 = Green
	3 = Aqua
	4 = Red
	5 = Purple
	6 = Yellow
	7 = White
	8 = Gray
	9 = Light Blue
	10 = Light Green
	11 = Light Aqua
	12 = Light Red
	13 = Light Purple
	14 = Light Yellow
	15 = Bright White*/
	int color_code = backgound_color * 16 + text_color;
	SetConsoleTextAttribute(hStdout, color_code);
}

// Đưa con trỏ đến vị trí cần ghi
void GotoXY(int x, int y) {
	HANDLE hhStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hhStdout, coord);
}

void drawblack(position start_point, int Width, int height) {
	Color(0, 0);
	for (int i = 0; i <= Width; i++) {
		for (int j = 0; j <= height; j++) {
			GotoXY(start_point.x + i, start_point.y + j);
			cout << "X";
		}
	}
	Color(0, 7);
}

void drawmenubox() {
	system("cls");
	drawbox({ 0,0 }, WIDTH + 9, HEIGHT, 7);
	snake_menu();
}
void menu() {


	int Saved = 0;
	int temp;
	string name[5] = { "0.txt","1.txt","2.txt","3.txt","4.txt" };

	for (int i = 0; i < 5; i++) {
		ifstream ContinueBySave(name[i]);
		ContinueBySave >> temp;
		Saved += temp;
		ContinueBySave.close();
	}

	string word[6] = { "NEW GAME","CONTINUE","HIGHSCORES","SETTINGS","INTRODUCTION","QUIT" };
	int size_word;
	for (int i = 0; i < 6; i++) {
		if (Saved == 0 && i == 1) continue;
		size_word = word[i].size();
		GotoXY(5 + WIDTH / 2 - size_word / 2, HEIGHT / 2 - 4 + 2 * i);
		if (i == Menu_choice) {
			Color(0, 3);
			cout << word[i];
			Color(0, 7);
		}
		else cout << word[i];
	}


	if (Saved == 0) {
		Color(0, 8);
		GotoXY(5 + WIDTH / 2 - word[1].size() / 2, HEIGHT / 2 -2);
		cout << word[1];
		Color(0, 7);

	}


	if (!Mouse_On) {
		switch (_getch()) {
		case 87:
		case 119: {
			Menu_choice = (Menu_choice + 5) % 6;
			if (Saved == 0 && Menu_choice == 1) Menu_choice--;
			break;
		}
		case 83:
		case 115: {
			Menu_choice = (Menu_choice + 1) % 6;
			if (Saved == 0 && Menu_choice == 1) Menu_choice++;
			break;
		}
		case 13:
			choicebar();
			break;
		}
	}


	if (Mouse_On) {
		bool right_pos = false;
		GetCursorPos(&real_mouse);
		mouse.x = real_mouse.x / 16;
		mouse.y = max((real_mouse.y - 23), 0) / 16;
		if (mouse.x >= 31 && mouse.x <= 38 && mouse.y == 13) { Menu_choice = 0;	right_pos = true; }
		if (mouse.x >= 31 && mouse.x <= 38 && mouse.y == 15) { Menu_choice = 1; right_pos = true; }
		if (mouse.x >= 30 && mouse.x <= 39 && mouse.y == 17) { Menu_choice = 2; right_pos = true; }
		if (mouse.x >= 31 && mouse.x <= 38 && mouse.y == 19) { Menu_choice = 3; right_pos = true; }
		if (mouse.x >= 29 && mouse.x <= 40 && mouse.y == 21) { Menu_choice = 4; right_pos = true; }
		if (mouse.x >= 33 && mouse.x <= 36 && mouse.y == 23) { Menu_choice = 5; right_pos = true; }
		if (GetAsyncKeyState(0x02)) {
			if (right_pos) choicebar();
		}
	}

}

void snake_menu() {
	Color(0, 3);
	GotoXY(7, 6);  cout << "  __             _            ___    ";
	GotoXY(7, 7);  cout << " \/ _\\_ __   __ _| | _ ___    / _ \\__ _ _ __ __    ___";
	GotoXY(7, 8);  cout << " \\ \\| `_ \\ / _\\'| |/ / _ \\  / /_\\/ _\\'| `_ ` _`\\ / _ \\  ";
	GotoXY(7, 9);  cout << " _\\ \\ | | | (_| |   <  __/ / /_\\\\ (_| | | | | | |  __/";
	GotoXY(7, 10); cout << " \\__/_| |_|\\__,_|_|\\_\\___| \\____/\\__,_|_| |_| |_|\\___|  ";
	Color(0, 7);
}

void choicebar() {
	switch (Menu_choice)
	{
	case 0: {
		menuend = true;
		gameover = false;
		snakedead = false;
		setup();
		break;
	}
	case 1:
		save_slot_in();
		break;
	case 2:
		highscore_menu();
		break;
	case 3:
		setting_menu((150 - speed) / 10);
		break;
	case 4:
		introduction();
		break;
	case 5: {
		if (SoundEffect_on) PlaySound(TEXT("Ending.wav"), NULL, SND_FILENAME | SND_ASYNC);
		position a = { 15,10 };
		if (sure_board("Are you sure to quit the game?", a, 40, 15)) {
			snakedead = false;
			menuend = true;
			gameover = true;
			gameend = true;
			PlaySound(NULL, NULL, 0);
		}
		else {
			drawmenubox();
			PlaySound(NULL, NULL, 0);
			if (Music_on) PlaySound(TEXT("realsoundrtack.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);
		}
		break;
	}
	default:
		break;
	}
}

void highscore_menu() {
	bool menu3 = true;
	int move = 0;
	int old_move = -1;


	File_To_Player();

	drawblack({ 29,10 }, 12, 14);
	GotoXY(WIDTH / 2, 5);
	Color(0, 4);
	cout << "HIGHSCORE";
	Color(0, 7);
	while (menu3) {



		if (old_move != move) {
			for (int h = 0; h < 10; h++) {
				for (int k = 0; k < 5; k++) {
					Color(0, 0);
					GotoXY(50 + k, 10 + 2 * h);
					cout << " ";
				}
			}
			for (int j = 0; j < 20; j++) {
				Color(8, 8);
				GotoXY(65, 10 + j);
				cout << " ";
			}
			for (int j = 0; j < 10; j++) {
				Color(7, 7);
				GotoXY(65, 10 + j + move);
				cout << " ";
			}
			old_move = move;
		}

		Color(0, 7);
		for (int i = 0; i < 10; i++) {
			GotoXY(10, 10 + 2 * i);
			cout << Player[i + move].Name;
			for (unsigned int j = 0; j < (40 - Player[i + move].Name.size()); j++) cout << " ";
			cout << Player[i + move].ScoreSaved << endl;
		}


		if (!Mouse_On) {
			switch (_getch())
			{
			case 83:
			case 115:
				move = min(move++, 10);
				break;
			case 87:
			case 119:
				move = max(move--, 0);
				break;
			case 27:
				menu3 = false;
				drawmenubox();
			default:
				break;
			}
		}


		if (Mouse_On) {
			bool right_pos = false;


			GetCursorPos(&real_mouse);
			mouse.x = real_mouse.x / 16;
			mouse.y = max((real_mouse.y - 23), 0) / 16;
			if ((mouse.x == 65) && (mouse.y > 10 + move) && (mouse.y < 10 + move + 9)) {
				right_pos = true;
				position old_mouse = mouse;
				while (GetAsyncKeyState(VK_RBUTTON)) {
					GetCursorPos(&real_mouse);
					mouse.y = max((real_mouse.y - 23), 0) / 16;
					if (mouse.y < old_mouse.y) move = max(0, move - 1);
					if (mouse.y > old_mouse.y) move = min(10, move + 1);
					old_mouse = mouse;
					if (old_move != move) {
						for (int h = 0; h < 10; h++) {
							for (int k = 0; k < 5; k++) {
								Color(0, 0);
								GotoXY(50 + k, 10 + 2 * h);
								cout << " ";
							}
						}
						for (int j = 0; j < 20; j++) {
							Color(8, 8);
							GotoXY(65, 10 + j);
							cout << " ";
						}
						for (int j = 0; j < 10; j++) {
							Color(7, 7);
							GotoXY(65, 10 + j + move);
							cout << " ";
						}
						old_move = move;
					}

					Color(0, 7);
					for (int i = 0; i < 10; i++) {
						GotoXY(10, 10 + 2 * i);
						cout << Player[i + move].Name;
						for (unsigned int j = 0; j < (40 - Player[i + move].Name.size()); j++) cout << " ";
						cout << Player[i + move].ScoreSaved << endl;
					}
				}
			}
			if ((mouse.x > 1 && mouse.x < 10) && (mouse.y > 1 && mouse.y < 7)) {
				right_pos = true;
				drawbox({ 2,2 }, 8, 4, 3);
				Color(0, 3);
			}
			else {
				drawbox({ 2,2 }, 8, 4, 7);
				right_pos = false;
			}

			GotoXY(4, 4);
			cout << "MENU";
			Color(0, 7);
			if (GetAsyncKeyState(VK_RBUTTON)) {
				if (right_pos) {
					menu3 = false;
					drawmenubox();
				}
			}
		}

	}
	Color(0, 0);
	GotoXY(4, 4);
	cout << "     ";
	Color(0, 7);
}


void setting_menu(int move_speed) {

	int music_temp, sound_temp, mouse_temp;
	if (Music_on) music_temp = 1;
	else music_temp = 0;
	if (SoundEffect_on) sound_temp = 1;
	else sound_temp = 0;
	if (Mouse_On) mouse_temp = 1;
	else mouse_temp = 0;

	bool Menu4 = true;
	int old_speed = (150 - setup_speed) / 10;
	int setting_choice = 0;

	bool out_choice = true;


	drawmenubox();
	drawmenubox();
	GotoXY(WIDTH / 2 + 1, 3);
	Color(0, 4);
	cout << "SETTING";
	Color(0, 7);

	while (Menu4) {


		if (setting_choice == 0) Color(0, 3);
		GotoXY(WIDTH / 2 + 2, 8);
		cout << "SPEED";

		GotoXY(WIDTH / 2 - 6, 10);
		for (int i = 0; i < 22; i++) {
			Color(8, 8);
			cout << " ";
		}
		GotoXY(WIDTH / 2 - 6 + 2 * move_speed, 10);
		Color(7, 7);
		cout << "  ";
		Color(0, 7);
		if (move_speed == 0) Color(0, 2);
		GotoXY(WIDTH / 2 - 10, 10);
		cout << "MIN";
		Color(0, 7);
		if (move_speed == 10) Color(0, 2);
		GotoXY(WIDTH / 2 + 17, 10);
		cout << "MAX";
		Color(0, 7);



		if (setting_choice == 1) Color(0, 3);
		GotoXY(WIDTH / 2 - 6, 13);
		cout << "MUSIC";
		GotoXY(WIDTH / 2 + 11, 13);
		Color(8, 8);
		cout << "  ";
		GotoXY(WIDTH / 2 + 11 + music_temp, 13);
		Color(7, 7);
		cout << " ";
		Color(0, 7);
		if (!music_temp) Color(0, 2);
		GotoXY(WIDTH / 2 + 7, 13);
		cout << "OFF";
		Color(0, 7);
		if (music_temp) Color(0, 2);
		GotoXY(WIDTH / 2 + 14, 13);
		cout << "ON";
		Color(0, 7);




		if (setting_choice == 2) Color(0, 3);
		GotoXY(WIDTH / 2 - 6, 15);
		cout << "SOUND";
		GotoXY(WIDTH / 2 + 11, 15);
		Color(8, 8);
		cout << "  ";
		GotoXY(WIDTH / 2 + 11 + sound_temp, 15);
		Color(7, 7);
		cout << " ";
		Color(0, 7);
		if (!sound_temp) Color(0, 2);
		GotoXY(WIDTH / 2 + 7, 15);
		cout << "OFF";
		Color(0, 7);
		if (sound_temp) Color(0, 2);
		GotoXY(WIDTH / 2 + 14, 15);
		cout << "ON";
		Color(0, 7);



		GotoXY(WIDTH / 2 + 1, 18);
		if (setting_choice == 3) Color(0, 3);
		cout << "CONTROL";
		Color(0, 7);




		if (!mouse_temp) {
			drawbox({ WIDTH / 2 - 9 ,20 }, 11, 10, 2);
			Color(0, 2);
		}
		else drawbox({ WIDTH / 2 - 9,20 }, 11, 10, 7);
		GotoXY(WIDTH / 2 - 8, 32);
		cout << "KEYBOARD";
		GotoXY(WIDTH / 2 - 4, 24);
		cout << "W";
		GotoXY(WIDTH / 2 - 4, 26);
		cout << "S";
		GotoXY(WIDTH / 2 - 6, 26);
		cout << "A";
		GotoXY(WIDTH / 2 - 2, 26);
		cout << "D";
		Color(0, 7);

		int c;
		if (mouse_temp) {
			drawbox({ WIDTH / 2 + 7 ,20 }, 11, 10, 2);
			Color(0, 2);
			c = 2;
		}
		else {
			drawbox({ WIDTH / 2 + 7,20 }, 11, 10, 7);
			c = 7;
		}
		GotoXY(WIDTH / 2 + 10, 32);
		cout << "MOUSE";
		Color(c, c);
		GotoXY(WIDTH / 2 + 12, 23);
		cout << "X";
		GotoXY(WIDTH / 2 + 11, 24);
		cout << "XXX";
		GotoXY(WIDTH / 2 + 10, 25);
		cout << "XXXXX";
		GotoXY(WIDTH / 2 + 12, 26);
		cout << "X";
		GotoXY(WIDTH / 2 + 12, 27);
		cout << "X";
		Color(0, 7);




		if (setting_choice == 4) {
			drawbox({ 3,29 }, 9, 4, 3);
			Color(0, 3);
		}
		else drawbox({ 3,29 }, 9, 4, 7);
		GotoXY(5, 31);
		cout << "APPLY";
		Color(0, 7);



		if (setting_choice == 5) {
			drawbox({ 56,29 }, 10, 4, 4);
			Color(0, 4);
		}
		else drawbox({ 56,29 }, 10, 4, 7);
		GotoXY(58, 31);
		cout << "CANCEL";
		Color(0, 7);



		if (Mouse_On) {
			bool right_pos = false;
			GetCursorPos(&real_mouse);
			mouse.x = real_mouse.x / 16;
			mouse.y = max(0, real_mouse.y - 23) / 16;
			position old_mouse = mouse;
			if ((mouse.x > WIDTH / 2 + 1 && mouse.x < WIDTH / 2 + 7) && mouse.y == 8) setting_choice = 0;
			if ((mouse.x > (WIDTH / 2 - 6 + 2 * move_speed - 1) && mouse.x < (WIDTH / 2 - 6 + 2 * move_speed + 2)) && mouse.y == 10) {
				setting_choice = 0;
				position old_mouse = mouse;
				while (GetAsyncKeyState(VK_RBUTTON)) {
					GetCursorPos(&real_mouse);
					mouse.x = real_mouse.x / 16;
					if (mouse.x < old_mouse.x) move_speed = max(move_speed - 1, 0);
					if (mouse.x > old_mouse.x) move_speed = min(move_speed + 1, 10);
					old_mouse = mouse;
					Sleep(50);
					GotoXY(WIDTH / 2 - 6, 10);
					for (int i = 0; i < 22; i++) {
						Color(8, 8);
						cout << " ";
					}
					GotoXY(WIDTH / 2 - 6 + 2 * move_speed, 10);
					Color(7, 7);
					cout << "  ";
					Color(0, 7);
					if (move_speed == 0) Color(0, 2);
					GotoXY(WIDTH / 2 - 10, 10);
					cout << "MIN";
					Color(0, 7);
					if (move_speed == 10) Color(0, 2);
					GotoXY(WIDTH / 2 + 17, 10);
					cout << "MAX";
					Color(0, 7);
				}
			}
			if (mouse.y == 13) {
				if (mouse.x > WIDTH / 2 - 7 && mouse.x < WIDTH / 2 - 1) {
					setting_choice = 1;
					right_pos = true;
				}
				if (mouse.x > WIDTH / 2 + 6 && mouse.x < WIDTH / 2 + 16) {
					setting_choice = 1;
					right_pos = true;
				}
			}
			if (mouse.y == 15) {
				if (mouse.x > WIDTH / 2 - 7 && mouse.x < WIDTH / 2 - 1) {
					setting_choice = 2;
					right_pos = true;
				}
				if (mouse.x > WIDTH / 2 + 6 && mouse.x < WIDTH / 2 + 16) {
					setting_choice = 2;
					right_pos = true;
				}
			}
			if (mouse.y == 18 && mouse.x > WIDTH / 2 && mouse.x < WIDTH / 2 + 8) { setting_choice = 3, right_pos = true; }
			if ((mouse.x > WIDTH / 2 - 10 && mouse.x < WIDTH / 2 + 2) && (mouse.y > 19 && mouse.y < 31) || ((mouse.y == 32) && (mouse.x > WIDTH / 2 - 9 && mouse.x < WIDTH / 2))) {
				setting_choice = 3;
				if (GetAsyncKeyState(VK_RBUTTON)) mouse_temp = 0;

			}
			if ((mouse.x > WIDTH / 2 + 6 && mouse.x < WIDTH / 2 + 18) && (mouse.y > 19 && mouse.y < 31) || ((mouse.y == 32) && (mouse.x > WIDTH / 2 + 9 && mouse.x < WIDTH / 2 + 15))) {
				setting_choice = 3;
				if (GetAsyncKeyState(VK_RBUTTON)) mouse_temp = 1;
			}
			if ((mouse.x > 1 && mouse.x < 10) && (mouse.y > 1 && mouse.y < 7)) {
				drawbox({ 2,2 }, 8, 4, 3);
				Color(0, 3);
				right_pos = true;
				out_choice = true;
			}
			else {
				drawbox({ 2,2 }, 8, 4, 7);
				out_choice = false;
			}
			GotoXY(4, 4);
			cout << "MENU";
			Color(0, 7);
			if ((mouse.x > 2 && mouse.x < 12) && (mouse.y > 28 && mouse.y < 34)) {
				setting_choice = 4;
				right_pos = true;
			}
			if ((mouse.x > 55 && mouse.x < 66) && (mouse.y > 28 && mouse.y < 34)) {
				setting_choice = 5;
				right_pos = true;
			}
			if (GetAsyncKeyState(VK_RBUTTON) && right_pos) {
				Sleep(200);
				if (out_choice) {
					Menu4 = false;
					drawmenubox();
				}
				switch (setting_choice)
				{
				case 0:
					move_speed = min(move_speed + 1, 10);
					break;
				case 1:
					music_temp = 1 - music_temp;
					break;
				case 2:
					sound_temp = 1 - sound_temp;
					break;
				case 3:
					mouse_temp = 1 - mouse_temp;
					break;
				case 4:
					Menu4 = false;
					drawmenubox();
					break;
				case 5:
					drawmenubox();
					return;
				default:
					break;
				}
			}

		}
		else {
			switch (_getch())
			{
			case 68:
			case 100: {
				switch (setting_choice)
				{
				case 0: {
					move_speed = min(move_speed + 1, 10);
					break;
				}
				case 1: {
					music_temp = 1;
					break;
				}
				case 2: {
					sound_temp = 1;
					break;
				}
				case 3:
					mouse_temp = 1;
					break;
				case 4:
					setting_choice++;
				default:
					break;
				}
				break;
			}
			case 65:
			case 97: {
				switch (setting_choice)
				{
				case 0: {
					move_speed = max(move_speed - 1, 0);
					break;
				}
				case 1: {
					music_temp = 0;
					break;
				}
				case 2: {
					sound_temp = 0;
					break;
				}
				case 3:
					mouse_temp = 0;
					break;
				case 5:
					setting_choice--;;
				default:
					break;
				}
				break;
			}
			case 87:
			case 119: {
				if (setting_choice == 5) setting_choice--;
				setting_choice = max(0, setting_choice - 1);
				break;
			}
			case 83:
			case 115: {
				setting_choice = min(setting_choice + 1, 5);
				break;
			}
			case 13:
				switch (setting_choice)
				{
				case 1: {
					music_temp = 1 - music_temp;
					break;
				}
				case 2: {
					sound_temp = 1 - sound_temp;
					break;
				}
				case 3:
					mouse_temp = 1 - mouse_temp;
					break;
				case 4:
					Menu4 = false;
					drawmenubox();
					break;
				case 5:
					drawmenubox();
					return;
				default:
					break;
				}
				break;
			case 27:
				Menu4 = false;
				drawmenubox();
				break;
			default:
				break;
			}
		}
		if (music_temp) {
			PlaySound(TEXT("realsoundrtack.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC | SND_NOSTOP);
		}
		else {
			PlaySound(NULL, NULL, 0);
		}
		Color(0, 7);

	}


	Color(0, 7);
	drawmenubox();
	if (move_speed != old_speed) {
		if (sure_board("All data will be change!!!", { 15,10 }, 40, 15)) {
			speed = 150 - move_speed * 10;
		}
		else
		{
			setting_menu(move_speed);
		}
	}
	if (music_temp) Music_on = true;
	else Music_on = false;

	if (sound_temp) SoundEffect_on = true;
	else SoundEffect_on = false;

	if (mouse_temp) Mouse_On = true;
	else Mouse_On = false;

	setup_speed = speed;
	drawmenubox();
}


void introduction() {
	bool menu5 = true;
	int intro_choice = 0;
	drawblack({ 29,9 }, 12, 15);
	GotoXY(WIDTH / 2 - 2, 4);
	Color(0, 4);
	cout << "INTRODUCTION";
	Color(0, 7);
	drawbox({ 4,8 }, 29, 24, 7);
	drawbox({ 36,8 }, 29, 24, 7);
	drawbox({ 32,8 }, 5, 6, 7);
	drawblack({ 32,9 }, 4, 4);

	Color(0, 5);
	GotoXY(9, 10);
	cout << "Snake Project - Programming Technique - class:21CTT4";
	GotoXY(13, 12);
	cout << "Supporting teacher: Mr.Truong Toan Thinh.";
	Color(0, 2);
	GotoXY(15, 14);
	cout << "TEAM 15";
	Color(0, 7);


	GotoXY(40, 15);
	cout << "Music: Pvz-Ost-Remix.";
	GotoXY(46, 18);
	Color(0, 1);
	cout << "COPYRIGHT";
	Color(0, 15);
	GotoXY(38, 20);
	cout << "Do not use any information";
	GotoXY(38, 22);
	cout << "of our project without";
	GotoXY(38, 24);
	cout << "asking permission.";
	Color(0, 14);
	GotoXY(42, 27);
	cout << "copyright @ team15.";
	GotoXY(42, 29);
	cout << "All right reserved.";

	char team[5][50] = { "   21120429 - Luu Kien Dat     \0",
						"   21120433 - Nguyen Quang Dinh\0",
						"   21120434 - Bui Tien Dung    \0",
						"   21120436 - Chau Quy Duong   \0",
						"   21120451 - Le Bao Hieu      \0" };
	int len[5] = { 0 };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; team[i][j] != '\0'; j++, len[i]++);
	}
	int run[5] = { 4,4,4,4,4 };
	while (menu5) {
		for (int i = 0; i < 5; i++) {
			GotoXY(9, 17 + 3 * i);
			if (i == intro_choice) Color(0, 3);
			for (int k = 0; k < 20; k++) {
				cout << team[i][(run[i] + k) % len[i]];
			}
			run[i] = min(run[i] + 1, len[i]);
			if (run[i] == len[i]) run[i] = 0;
			Color(0, 7);
		}
		Sleep(300);



		if (!Mouse_On) {
			if (_kbhit()) {
				switch (_getch()) {
				case 87:
				case 119: {
					intro_choice = (intro_choice + 4) % 5;
					break;
				}
				case 83:
				case 115: {
					intro_choice = (intro_choice + 1) % 5;
					break;
				}
				case 13: {
					int n = intro_choice;
					while (n != -1) {
						n = member(n);
					}

					introduction();
					menu5 = false;
					break;
				}

				case 27:
					menu5 = false;
					drawmenubox();
					break;
				}
			}
		}
		else {
			bool back;
			bool right_pos = false;
			GetCursorPos(&real_mouse);
			mouse.x = real_mouse.x / 16;
			mouse.y = max(0, real_mouse.y - 23) / 16;

			if ((mouse.x > 1 && mouse.x < 10) && (mouse.y > 1 && mouse.y < 7)) {
				back = true;
				right_pos = true;
				drawbox({ 2,2 }, 8, 4, 3);
				Color(0, 3);
			}
			else {
				drawbox({ 2,2 }, 8, 4, 7);
				right_pos = false;
				back = false;
			}
			GotoXY(4, 4);
			cout << "BACK";
			Color(0, 7);





			for (int i = 0; i < 5; i++) {
				if (mouse.x > 8 && mouse.x < 28 && mouse.y == 17 + 3 * i) {
					intro_choice = i;
					right_pos = true;
				}
			}
			if (GetAsyncKeyState(VK_RBUTTON) && right_pos) {
				if (back && right_pos) {
					intro_choice = -2;
					drawmenubox();
				}
				int n = intro_choice;
				while (n >= 0) {
					n = member(n);
				}
				if (n == -1) {
					introduction();
				}
				menu5 = false;
				break;
			}

		}


	}
}


int member(int member_choice, bool Member_check) {
	int next_member = -1;
	drawblack({ 4, 6 }, 31, 26);
	drawblack({ 36,6 }, 29, 26);
	bool Member_menu = true;
	bool sound_check = false;
	MEMBER team[5] = { {"21120429\0","Luu Kien Dat\0","Developer, Tester\0","With his own ideas, Dat discussed with the team and developed the game in a new way by adding new modes to help players get more excited (Boss,...).\0"},
						{"21120433\0","Nguyen Quang Dinh\0","Developer, Tester\0","Dinh is the one who combined the members's code with his own code. In addition, Dinh also gave very interesting ideas on how to develop the game.\0"},
						{"21120434\0","Bui Tien Dung\0","Tester, Designer, Developer\0","As an artistic person, Dung designed the map himself and together with Duong designed the game.\0"},
						{"21120436\0","Chau Quy Duong\0","Leader, Designer, Developer\0","Being a person with artistic vision, Duong has designed the game to be more amazing.At the same time, Duong is also a great team leader, when dividing the work effectively.\0"},
						{"21120451\0","Le Bao Hieu\0","Bussiness Analyst,Developer, Tester\0","Hieu is someone who has a deep understanding of new technologies, so Hieu has helped other members a lot.\0"} };
	int len[5][4] = { 0 };
	for (int i = 0; i < 5; i++) {
		for (int j = 0; (team[i].id)[j] != '\0'; j++, len[i][0]++);
		for (int j = 0; (team[i].Name)[j] != '\0'; j++, len[i][1]++);
		for (int j = 0; (team[i].pos)[j] != '\0'; j++, len[i][2]++);
		for (int j = 0; (team[i].prop)[j] != '\0'; j++, len[i][3]++);
	}
	int a = 0, b = 0, c = 0, d = 0;
	int n = 0, m = 0;
	Color(0, 14);
	GotoXY(10, 10); cout << "Student ID:";
	GotoXY(10, 13); cout << "Name:";
	GotoXY(10, 16); cout << "Position on team:";
	GotoXY(10, 19); cout << "Properties:";
	Color(0, 7);
	PlaySound(TEXT("keyboard.wav"), NULL, SND_ASYNC | SND_LOOP);
	while (Member_menu) {

		GotoXY(29, 32);
		for (int i = 0; i < 5; i++) {
			if (i == member_choice) Color(0, 3);
			cout << ".  ";
			Color(0, 7);
		}



		if (!Member_check)Sleep(150);
		else Sleep(1);
		if (a < len[member_choice][0]) {
			GotoXY(22 + a, 10);
			cout << (team[member_choice].id)[a];
			a = min(a + 1, len[member_choice][0]);
		}
		if (a == len[member_choice][0] && b < len[member_choice][1]) {
			GotoXY(15 + b, 13);
			cout << (team[member_choice].Name)[b];
			b = min(b + 1, len[member_choice][1]);
		}
		if (b == len[member_choice][1] && c < len[member_choice][2]) {
			GotoXY(27 + c, 16);
			cout << (team[member_choice].pos)[c];
			c = min(c + 1, len[member_choice][2]);
		}
		if (c == len[member_choice][2] && d < len[member_choice][3]) {
			if (n < ((12 + d) / 50) && (team[member_choice].prop)[d] == ' ') {
				n++;
				m = 0;
			}
			if (n < 1) {
				if (m <= (12 + d) % 50)m = (12 + d) % 50;


			}
			GotoXY(9 + m, 19 + 2 * n);
			cout << (team[member_choice].prop)[d];
			d = min(d + 1, len[member_choice][3]);
			m++;
		}
		if (d == len[member_choice][3]) {
			if (!sound_check) {
				PlaySound(TEXT("enter.wav"), NULL, SND_ASYNC);
				sound_check = true;
			}
		}


		if (!Mouse_On) {

			if (_kbhit()) {
				switch (_getch())
				{
				case 68:
				case 100: {
					return ((member_choice + 1) % 5);
				}
				case 65:
				case 97: {
					return ((member_choice + 4) % 5);
				}
				case 13: {
					Member_check = true;
					break;
				}
				case 27:
					Member_menu = false;
					break;
				default:
					break;
				}
			}
		}
		else {
			if (next_member == 0) Color(3, 3);
			else Color(7, 7);
			GotoXY(5, 14);
			cout << "X";
			GotoXY(4, 15);
			cout << "XX";
			GotoXY(3, 16);
			cout << "XXX";
			GotoXY(4, 17);
			cout << "XX";
			GotoXY(5, 18);
			cout << "X";
			if (next_member == 1) Color(3, 3);
			else Color(7, 7);
			GotoXY(63, 14);
			cout << "X";
			GotoXY(63, 15);
			cout << "XX";
			GotoXY(63, 16);
			cout << "XXX";
			GotoXY(63, 17);
			cout << "XX";
			GotoXY(63, 18);
			cout << "X";







			bool back;
			bool right_pos = false;
			GetCursorPos(&real_mouse);
			mouse.x = real_mouse.x / 16;
			mouse.y = max(0, real_mouse.y - 23) / 16;



			if ((mouse.x > 1 && mouse.x < 10) && (mouse.y > 1 && mouse.y < 7)) {
				back = true;
				right_pos = true;
				drawbox({ 2,2 }, 8, 4, 3);
				Color(0, 3);
			}
			else {
				drawbox({ 2,2 }, 8, 4, 7);
				right_pos = false;
				back = false;
			}
			GotoXY(4, 4);
			cout << "BACK";
			Color(0, 7);




			for (int i = 0; i < 3; i++) {
				if ((mouse.y == 14 + i || mouse.y == 18 - i)) {
					if (mouse.x > 4 - i && mouse.x < 6) {
						right_pos = true;
						next_member = 0;
					}
					else {
						if (mouse.x > 62 && mouse.x < 64 + i && (mouse.y == 14 + i || mouse.y == 18 - i)) {
							right_pos = true;
							next_member = 1;
						}
						else {

							next_member = -1;
						}
					}
				}
				else next_member = -1;
			}

			if (GetAsyncKeyState(VK_RBUTTON)) {
				if (back && right_pos) Member_menu = false;
				switch (next_member)
				{
				case 0:
				case 1:
					Member_menu = false;
					break;
				default:
					Member_check = true;
					break;
				}

			}
			Color(0, 7);

		}

	}
	PlaySound(NULL, NULL, 0);
	drawmenubox();
	if (Music_on) PlaySound(TEXT("realsoundrtack.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC | SND_NOSTOP);
	switch (next_member)
	{
	case 0:
		return ((member_choice + 4) % 5);
	case 1:
		return ((member_choice + 1) % 5);
	case -1:
		return -1;
	default:
		break;
	}
	return -1;
}


bool save_slot_out(bool check) {

	bool wanna_save;
	int save_choice_out = 0;
	bool save_quit = false;

	Color(7, 7);
	drawbox({ 13,9 }, 32, 19, 7);
	Color(0, 0);
	for (int i = 1; i < 31; i++) {
		for (int j = 1; j < 18; j++) {
			GotoXY(13 + i, 9 + j);
			cout << " ";
		}
	}
	Color(0, 7);


	string name[5] = { "0.txt","1.txt","2.txt","3.txt","4.txt" };

	int number_choice[5];

	GotoXY(15, 13 + 2 * 5);
	Color(0, 2);
	cout << "Pay 5$ to buy this save_slot";
	Color(0, 7);


	while (!save_quit) {
		for (int i = 0; i < 5; i++) {
			GotoXY(24, 13 + 2 * i);
			if (i == save_choice_out) Color(0, 3);
			cout << "Save_Slot " << i;
			Color(0, 7);
		}
		switch (_getch())
		{
		case 87:
		case 119:
			save_choice_out = (save_choice_out + 4) % 5;
			break;
		case 83:
		case 115:
			save_choice_out = (save_choice_out + 1) % 5;
			break;
		case 13:
			save_quit = true;
			wanna_save = true;
			break;
		case 27:
			save_quit = true;
			wanna_save = false;
			break;
		default:
			break;
		}
	}

	if (wanna_save) {
		ifstream ContinueBySave(name[save_choice_out]);
		int Saved; ContinueBySave >> Saved;
		ContinueBySave.close();
		if (Saved == 1) {
			if (!sure_board("Old data will be replaced?", { 14,12 }, 30, 15)) {
				wanna_save = false;
				save_slot_out(check);
				check = true;
			}
		}
		if (!check) {
			savedetail(name[save_choice_out]);
			return true;
		}

	}
	if (!wanna_save) return false;
}



void save_slot_in() {

	int save_choice_in = 0;
	string name[5] = { "0.txt","1.txt","2.txt","3.txt","4.txt" };
	int number_choice[5];
	string useful;

	//Để đưa con trỏ đến vị trí file đầu tiên được lưu
	for (int i = 0; i < 5; i++) {
		ifstream ContinueBySave(name[i]);
		int Saved; ContinueBySave >> Saved;
		ContinueBySave.close();
		if (Saved != 0) {
			save_choice_in = i;
			break;
		}
	}

	drawbox({ 18,8 }, 32, 19, 7);
	drawblack({ 19,9 }, 29, 17);
	GotoXY(19, 10);
	Color(0, 1);
	cout << "Choose your save to continue!";


	GotoXY(20, 13 + 2 * 5);
	Color(0, 2);
	cout << "Pay 5$ to buy this save_slot";
	Color(0, 7);

	int len[6];

	while (!Continue) {
		for (int i = 0; i < 5; i++) {
			ifstream ContinueBySave(name[i]);
			int Saved; ContinueBySave >> Saved;
			if (Saved == 1) {
				ContinueBySave >> useful;
				len[i] = useful.size();
			}
			else len[i] = 10;
			ContinueBySave.close();
			GotoXY(34 - len[i] / 2, 13 + 2 * i);
			if (i == save_choice_in) Color(0, 3);
			if (Saved == 0) {
				Color(0, 8);
				number_choice[i] = i;
			}
			else {
				number_choice[i] = -1;
			}
			if (Saved == 1) {
				cout << useful;
			}
			else
			{
				cout << "Save slot " << i;
			}
			Color(0, 7);
		}

		if (!Mouse_On) {
			switch (_getch())
			{
			case 87:
			case 119: {
				save_choice_in = (save_choice_in + 4) % 5;
				for (int i = 0; i < 5; i++) {
					if (save_choice_in == number_choice[i]) {
						save_choice_in = (save_choice_in + 4) % 5;
						i = -1;
					}
				}
				break;
			}
			case 83:
			case 115: {
				save_choice_in = (save_choice_in + 1) % 5;
				for (int i = 0; i < 5; i++) {
					if (save_choice_in == number_choice[i]) {
						save_choice_in = (save_choice_in + 1) % 5;
						i = -1;
					}
				}
				break;
			}
			case 27: {
				system("cls");
				drawmenubox();
				return;
				break;
			}
			case 13: {
				Continue = true;
			}
			default:
				break;
			}
		}
		else {
			bool right_pos = false;
			GetCursorPos(&real_mouse);
			mouse.x = real_mouse.x / 16;
			mouse.y = max(0, real_mouse.y - 23) / 16;
			for (int i = 0; i < 5; i++) {
				if (abs(mouse.x - 33) < len[i] / 2 && mouse.y == 13 + 2 * i) {
					save_choice_in = i;
					right_pos = true;
				}
			}
			if (GetAsyncKeyState(VK_RBUTTON) && right_pos) {
				Continue = true;
			}


		}
	}

	Loaddetail(name[save_choice_in]);

	menuend = true;
	gameover = false;
	snakedead = false;
	Continue = true;
}


// set up các giá trị cơ bản
void setup() {
	speed = setup_speed;

	gameend = false;
	gameover = false;
	snakedead = true;
	Continue = false;

	meteoralive = false;
	delcount = 0;
	for (int i = 0; i < meteorsize; i++) {
		meteorTail[i].x = 0;
		meteorTail[i].y = 0;
	}
	lastmeteortail.x = 0;
	lastmeteortail.y = 0;
	lastmeteortail2.x = 0;
	lastmeteortail2.y = 0;
	meteor_check = false;



	score = 0;
	Snake_long = 2;
	/*Snake.x = 10;
	Snake.y = 10;*/
	Snake.x = WIDTH / 2;
	Snake.y = HEIGHT / 2;
	snake_move = STOP;

	Food.x = rand() % (WIDTH - 3) + 1;
	Food.y = rand() % (HEIGHT - 2) + 1;
	Menu_choice = 0;
	pause_choice = 0;

	index_start = 1;
	index_end = 1;
	level = 0;

	gate_into = false;
	gate_on = false;
	gate_out = false;

	Gate.x = rand() % (WIDTH - 6) + 3;
	Gate.y = rand() % (HEIGHT - 5) + 3;
	while (!(Gate_direction = rand() % 5 - 2));

	for (int i = 0; i < 500; i++) {
		Wall[i] = { 0,0 };
	}
	index_wall = 0;
}

// vẽ tường
void drawWALL() {
	// Vẽ tường trên dưới
	drawbox({ 0,0 }, WIDTH - 1, HEIGHT, 7);
}

// Vẽ hộp
void drawbox(position start_point, int Width, int height, int color) {
	// Vẽ 1 hộp thoại bên cạnh map game
	//Vẽ 2 cạnh ngang trên dưới
	Color(color, color);
	for (int i = 0; i < Width; i++) {
		GotoXY(start_point.x + i, start_point.y); cout << WALL;
		GotoXY(start_point.x + i, start_point.y + height); cout << WALL;
	}
	for (int i = 0; i < height; i++) {
		GotoXY(start_point.x, start_point.y + i); cout << WALL;
		GotoXY(start_point.x + Width - 1, start_point.y + i); cout << WALL;
	}
	Color(0, 7);
}

void drawdetail() {



	// Ve diem
	GotoXY(WIDTH + 6, HEIGHT / 6);
	cout << "Diem:" << score;
	GotoXY(WIDTH + 6, HEIGHT / 6 + 2);
	cout << "level:" << level + 1;
	if (!Mouse_On) {
		GotoXY(WIDTH + 6, HEIGHT / 6 + 18);
		cout << "GUIDE:";
		GotoXY(WIDTH + 6, HEIGHT / 6 + 20);
		cout << "W: UP";
		GotoXY(WIDTH + 6, HEIGHT / 6 + 22);
		cout << "A: LEFT";
		GotoXY(WIDTH + 6, HEIGHT / 6 + 24);
		cout << "D: RIGHT";
		GotoXY(WIDTH + 6, HEIGHT / 6 + 26);
		cout << "S: DOWN";

	}
	else {
		GotoXY(WIDTH + 6, HEIGHT / 6 + 18);
		cout << "GUIDE:";
		Color(7, 7);
		GotoXY(WIDTH + 9, HEIGHT / 6 + 20);
		cout << "X";
		GotoXY(WIDTH + 8, HEIGHT / 6 + 21);
		cout << "XXX";
		GotoXY(WIDTH + 7, HEIGHT / 6 + 22);
		cout << "XXXXX";
		GotoXY(WIDTH + 9, HEIGHT / 6 + 23);
		cout << "X";
		GotoXY(WIDTH + 9, HEIGHT / 6 + 24);
		cout << "X";

		Color(0, 7);
		GotoXY(WIDTH + 4, HEIGHT / 6 + 26);
		cout << "RIGHT CLICK";
	}
}

void drawmap(bool sleep_right) {
	Color(0, 0);
	system("cls");
	Color(7, 7);
	drawWALL();
	drawbox({ WIDTH + 2,0 }, 14, HEIGHT, 7);
	drawwall(sleep_right);
	Color(0, 7);
	drawdetail();
}

void drawsnake() {
	Color(0, 7);
	GotoXY(Snake.x, Snake.y);
	if (!gate_into) cout << 2;
	if (gate_out) cout << 2;
	for (int i = index_start; i < Snake_long; i++) {
		GotoXY(Tail[i].x, Tail[i].y);
		if (snake_move != 0) {
			if (i >= index_end - 1) Color(0, 0);
			cout << name[(i) % 40];
			Color(0, 7);
		}
	}
	GotoXY(lasttail.x, lasttail.y);
	if (snake_move != 0 || gate_out) {
		cout << " ";
	}
	if (gate_into && !gate_out) {
		if (index_start < Snake_long) index_start++;
		if (index_start == Snake_long) {
			snake_move = STOP;
			newword();
			drawmap(false);
			drawWALL();
		}
	}
	if (!gate_out) return;
	index_end = min(index_end + 1, Snake_long);
	if (index_end == Snake_long) {
		gate_out = false;
		gate_into = false;
		gate_on = false;
		Color(0, 0);
		GotoXY(Gate.x, Gate.y);
		cout << " ";
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				GotoXY(Block[i].x, Block[j].y);
				cout << " ";
			}
		}
		Color(0, 7);
		spawnfood();
		drawwall(false);
		drawdetail();
	}

}

void drawfood() {
	if (gate_on) return;
	Color(0, 1);
	GotoXY(Food.x, Food.y);
	cout << "F";
	Color(0, 7);
}

void drawgate() {
	switch (Gate_direction)
	{
	case -1:
	case 1: {
		Block[0].x = Gate.x + 1;
		Block[1].x = Gate.x - 1;
		Block[0].y = Gate.y;
		Block[1].y = Gate.y + Gate_direction;
		break;
	}
	default: {
		Block[0].x = Gate.x;
		Block[1].x = Gate.x + Gate_direction / 2;
		Block[0].y = Gate.y + 1;
		Block[1].y = Gate.y - 1;
		break;
	}
	}
	while (!(col = rand() % 16));
	Color(col, col);
	GotoXY(Gate.x, Gate.y);
	cout << ' ';
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			GotoXY(Block[i].x, Block[j].y);
			cout << ' ';
		}
	}
	Color(0, 7);
}

void Gate_collision() {
	// chỉ khi bật gate_on mới kích hoạt sự va chạm gate, vì gate vẫn tồn tại dù vô hình
	if (!gate_on) return;
	// Rắn chạm thành cổng
	if ((Snake.y == Block[0].y || Snake.y == Block[1].y) && (Snake.x == Block[0].x || Snake.x == Block[1].x)) {
		gameover = true;
		snakedead = true;
		level = 0;
	}
	//rắn chạm vào cổng
	if (Snake.y == Gate.y && Snake.x == Gate.x) {
		int i = Gate_direction;
		// nếu chạm từ phía sau sẽ chết
		if ((Tail[1].y) == (Gate.y - (i % 2)) && (Tail[1].x == (Gate.x - (i * i - abs(i)) / i))) {
			gameover = true;
			snakedead = true;
			level = 0;
		}
		// nếu không thì sẽ đi vào cổng 
		gate_into = true;
	}
}

void newword() {

	level++;
	if (level == 5) {
		win();
	}
	else {

		drawgate();
		char a[5][40] = { "X  XXX X   X XXX X   X  X XXX    X  ",
						  "X  X   X   X X   X   X  X X X   XXX ",
						  "X  XXX  X X  XXX X   X  X XXX  X X X",
						  "X  X    X X  X   X   X  X X      X  ",
						  "XX XXX   X   XXX XX  XXXX X      X  " };
		GotoXY(12, 8);
		cout << "Speed Up";
		for (int i = 1; i < 5; i++) {
			int c = i + rand() % 11;
			for (int j = 0; j < 5; j++) {
				GotoXY(13, 10 + j);
				for (int k = 0; k < sizeof(a[j]); k++) {
					Color(c, c);
					if (a[j][k] == ' ') Color(0, 0);
					cout << a[j][k];
				}
			}
			Color(0, 0);
			Sleep(100);
		}

		gate_out = true;
		gate_into = false;
		speed -= 30;
		if (speed <= 20) speed = 20;
		index_start = 1;
		index_end = 1;

		snake_move = STOP;

		switch (level)
		{
		case 1: {
			int x[10] = { 5,7,9,13,50,42,40,17,47,55 };
			int y[10] = { 3,5,7,9,11,13,15,17,19,21 };
			int i = rand() % 10;
			Gate = { x[i],y[i] };
			while (!(Gate_direction = rand() % 5 - 2));
			if (Gate.x > 49) Gate_direction = -2;
			if (Gate.x < 7) Gate_direction = 1;
			if (Gate.y < 6) Gate_direction = -1;
			break;
		}
		case 2: {
			int x[10] = { 5,7,9,50,24,50,40,52,30,35 };
			int y[10] = { 3,5,7,9,11,13,15,17,19,21 };
			int i = rand() % 10;
			Gate = { x[i],y[i] };
			while (!(Gate_direction = rand() % 5 - 2));
			if (Gate.x > 49) Gate_direction = -2;
			if (Gate.x < 7) Gate_direction = 1;
			if (Gate.y < 6) Gate_direction = -1;
			if (Gate.x > 19 && Gate.x < 36) Gate_direction = 1;
			break;
		}
		case 3:
			Gate = { 3,30 };
			Gate_direction = -1;
			break;
		case 4:
			Gate = { 5,HEIGHT / 2 - 2 };
			Gate_direction = 2;
			bossfight = true;
		default:
			break;
		}
		Snake = Gate;

		switch (level)
		{
		case 0: {
			for (int i = 0; i < 200; i++) {
				Wall[i] = { 0,0 };
			}
			index_wall = 0;
			break;
		}
		case 1: {
			Before_mapD2();
			break;
		}
		case 2: {
			Before_mapD3();
			break;
		}
		case 3: {
			Before_mapD4();
			break;
		}
		case 4:
			mapboss();
			index_wall = 0;
			for (int i = 0; i < WIDTH / 2 - 2; i++) {
				Wall[index_wall] = { i,HEIGHT / 2 };
				index_wall++;
				Wall[index_wall] = { WIDTH / 2 + 1 + i,HEIGHT / 2 };
				index_wall++;
			}
			for (int i = 0; i < HEIGHT / 2 - 2; i++) {
				Wall[index_wall] = { WIDTH / 2, 3 +i };
				index_wall++;
				Wall[index_wall] = { WIDTH / 2,HEIGHT / 2 + 1 + i };
				index_wall++;
			}

		}



		switch (Gate_direction)
		{
		case -2: {
			snake_move = LEFT;
			break;
		}
		case -1: {
			snake_move = UP;
			break;
		}
		case 1: {
			snake_move = DOWN;
			break;
		}
		case 2: {
			snake_move = RIGHT;
			break;
		}
		}
	}
}


void spawnfood() {
	Food.x = rand() % (WIDTH - 3) + 1;
	Food.y = rand() % (HEIGHT - 2) + 1;
	for (int i = 0; i < index_wall; i++) {
		if (Food.x == Wall[i].x && Food.y == Wall[i].y) {
			Food.x = rand() % (WIDTH - 3) + 1;
			Food.y = rand() % (HEIGHT - 2) + 1;
			i = 0;
		}
	}
	for (int i = 0; i < Snake_long; i++) {
		if (Food.x == Tail[i].x && Food.y == Tail[i].y) {
			spawnfood();
		}
	}

	for (int i = 0; i < index_warning; i++) {
		if (Food.x == Warning[i].x && Food.y == Warning[i].y) {
			Food.x = rand() % (WIDTH - 3) + 1;
			Food.y = rand() % (HEIGHT - 2) + 1;
			i = 0;
		}
	}

	for (int i = 0; i < meteorsize; i++) {
		if (direction == 1 || direction == 2) {
			for (int j = 0; j < 10; j++) {
				if (Food.x == meteorTail[i].x && Food.y == meteorTail[i].y + j) {
					Food.x = rand() % (WIDTH - 3) + 1;
					Food.y = rand() % (HEIGHT - 2) + 1;
				}
			}
		}
		if (direction == 3 || direction == 4) {
			for (int j = 0; j < 10; j++) {
				if (Food.x == meteorTail[i].x + j && Food.y == meteorTail[i].y) {
					Food.x = rand() % (WIDTH - 3) + 1;
					Food.y = rand() % (HEIGHT - 2) + 1;
				}
			}
		}
	}
	drawfood();

}





void Input() {
	if (Mouse_On) {
		GetCursorPos(&real_mouse);
		mouse.x = real_mouse.x / 16;
		mouse.y = max(0, real_mouse.y - 23) / 16;
		int a = (mouse.x - Snake.x);
		int b = (mouse.y - Snake.y);

		bool pause_pos = false;
		if (mouse.x > WIDTH + 4 && mouse.x < WIDTH + 13 && mouse.y > HEIGHT / 6 + 3 && mouse.y < HEIGHT / 6 + 9) {
			Color(0, 3);
			pause_pos = true;
		}

		GotoXY(WIDTH + 7, HEIGHT / 6 + 6);
		cout << "PAUSE";
		Color(0, 7);

		if (GetAsyncKeyState(VK_RBUTTON)) {
			if (pause_pos) {
				pausegame = true;
				pause();
				return;
			}
			int a = (mouse.x - Snake.x);
			int b = (mouse.y - Snake.y);
			switch (snake_move)
			{
			case STOP:
				GotoXY(WIDTH / 2, HEIGHT / 2);
				Color(0, 0);
				cout << " ";
				Color(0, 7);
				if (abs(a) > abs(b)) {
					if (a > 0) snake_move = RIGHT;
					if (a < b) snake_move = LEFT;
				}
				else {
					if (b > 0) snake_move = DOWN;
					if (b < 0) snake_move = UP;
				}
				break;
			case LEFT:
			case RIGHT:
				if (b > 0) snake_move = DOWN;
				else { if (b < 0) snake_move = UP; }
				break;
			case UP:
			case DOWN:
				if (a > 0) snake_move = RIGHT;
				else { if (a < b) snake_move = LEFT; }
				break;
			default:
				break;
			}
		}


	}
	else {
		if (_kbhit())
		{
			switch (_getch())
			{
			case 65:
			case 97: {
				if (snake_move != RIGHT) snake_move = LEFT;
				break;
			}
			case 68:
			case 100: {
				if (snake_move != LEFT) snake_move = RIGHT;
				break;
			}
			case 87:
			case 119: {
				if (snake_move != DOWN) snake_move = UP;
				break;
			}
			case 83:
			case 115: {
				if (snake_move != UP) snake_move = DOWN;
				break;
			}
			case 80:
			case 112: {
				pausegame = true;
				pause();
				break;
			}
			case 27: {
				if (save_slot_out(false)) {
					menuend = false;
					gameover = true;
					snakedead = false;
				}
				else {
					drawmap(true);
					drawfood();
					Continue = true;
				}

				break;
			}
			}
		}
	}

}

void reasonofdead() {
	if (gate_into && !gate_out) return;
	if (Snake.x <= 0 || Snake.x >= WIDTH - 2 || Snake.y <= 0 || Snake.y >= HEIGHT) {
		gameover = true;
		snakedead = true;
		level = 0;
	}
	for (int i = 2; i < Snake_long - 1; i++) {
		if (Snake.x == Tail[i].x && Snake.y == Tail[i].y) {
			gameover = true;
			snakedead = true;
			level = 0;
		}
	}
	for (int i = 0; i < index_wall; i++) {
		if (Snake.x == Wall[i].x && Snake.y == Wall[i].y) {
			gameover = true;
			snakedead = true;
			level = 0;
		}
	}
	for (int i = 0; i < meteorsize; i++) {
		for (int k = 0; k < 10; k++) {
			if (direction == 1 || direction == 2) {
				if (meteorTail[i].x == Snake.x && meteorTail[i].y + k == Snake.y) {
					gameover = true;
					snakedead = true;
				}
			}
			if (direction == 3 || direction == 4) {
				if (meteorTail[i].x + k == Snake.x && meteorTail[i].y == Snake.y) {
					gameover = true;
					snakedead = true;
				}
			}
		}
	}
	for (int i = 0; i < meteorsize; i++) {
		for (int k = 0; k < 10; k++) {
			for (int j = 2; j < Snake_long - 1; j++) {
				if (direction == 1 || direction == 2) {
					if (meteorTail[i].x == Tail[j].x && meteorTail[i].y + k == Tail[j].y) {
						gameover = true;
						snakedead = true;
					}
				}
				if (direction == 3 || direction == 4) {
					if (meteorTail[i].x + k == Tail[j].x && meteorTail[i].y == Tail[j].y) {
						gameover = true;
						snakedead = true;
					}
				}
			}
		}
	}

}

void tailxy() {
	int prevX = Tail[0].x;
	int prevY = Tail[0].y;
	int prev2X, prev2Y;
	Tail[0].x = Snake.x;
	Tail[0].y = Snake.y;
	for (int i = 1; i < Snake_long; i++)
	{
		prev2X = Tail[i].x;
		prev2Y = Tail[i].y;
		Tail[i].x = prevX;
		Tail[i].y = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	lasttail.x = Tail[Snake_long - 1].x;
	lasttail.y = Tail[Snake_long - 1].y;
}

void move() {
	switch (snake_move)
	{
	case LEFT:
		Snake.x--;
		break;
	case RIGHT:
		Snake.x++;
		break;
	case UP:
		Snake.y--;
		break;
	case DOWN:
		Snake.y++;
		break;
	default:
		break;
	}
}

void Logic() {
	Gate_collision();
	if (!gate_into || gate_out) Input();
	move();
	if (!gate_on) {
		if (Snake.x == Food.x && Snake.y == Food.y) {
			if (SoundEffect_on) {
				PlaySound(TEXT("sfx_point.wav"), NULL, SND_FILENAME | SND_ASYNC);
			}
			score += 10;
			Snake_long++;
			if (!gate_out) index_end = max(Snake_long, index_end);
			if ((score + 10) % 80 == 0) gate_on = true;
			if (!gate_on) spawnfood();
		}
	}
	reasonofdead();
	tailxy();

}

void pause() {
	while (pausegame) {
		Color(0, 0);
		GotoXY(WIDTH + 7, HEIGHT / 6 + 6);
		cout << "PAUSE";
		Color(0, 7);
		pausemenu();

		if (!Mouse_On) {


			switch (_getch()) {
			case 87:
			case 119: {
				pause_choice = (pause_choice + 3) % 4;
				break;
			}
			case 83:
			case 115: {
				pause_choice = (pause_choice + 1) % 4;
				break;
			}
			case 13:
				switch (pause_choice)
				{
				case 0:
					pausegame = false;
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 10; j++) {
							GotoXY(WIDTH + 4 + j, HEIGHT / 4 + 2 * i);
							cout << " ";
						}
					}
					break;
				case 1:
					pausegame = false;
					setup();
					drawmap(true);
					drawfood();
					break;
				case 2:
					save_slot_out(false);
					gameover = true;
					pausegame = false;
					snakedead = false;
					menuend = false;
					break;
				default:
					break;
				}
				break;
			}
		}
		else {
			bool right_pos = false;
			GetCursorPos(&real_mouse);
			mouse.x = real_mouse.x / 16;
			mouse.y = max(0, real_mouse.y - 23) / 16;
			for (int i = 0; i < 4; i++) {
				if (mouse.x > WIDTH + 3 && mouse.x < WIDTH + 14 && mouse.y == HEIGHT / 4 + 6 + 2 * i) {
					pause_choice = i;
					right_pos = true;
				}
			}

			if (GetAsyncKeyState(VK_RBUTTON) && right_pos) {
				switch (pause_choice)
				{
				case 0:
					pausegame = false;
					for (int i = 0; i < 10; i++) {
						for (int j = 0; j < 10; j++) {
							GotoXY(WIDTH + 5 + j, HEIGHT / 4 + 2 * i);
							cout << " ";
						}
					}
					break;
				case 1:
					pausegame = false;
					setup();
					drawmap(true);
					drawfood();
					break;
				case 2:
					save_slot_out(false);
					gameover = true;
					pausegame = false;
					snakedead = false;
					menuend = false;
					break;
				default:
					break;
				}
			}








		}
	}

}


void pausemenu() {
	int n = 2;
	if (Mouse_On) n = 6;
	string word[3] = { "Continue","Reset Game","Menu" };
	int size_word;
	for (int i = 0; i < 3; i++) {
		size_word = word[i].size();
		GotoXY(WIDTH + 9 - size_word / 2, HEIGHT / 4 + n + 2 * i);
		if (i == pause_choice) {
			Color(0, 3);
			cout << word[i];
			Color(0, 7);
		}
		else cout << word[i];
	}

}

void setting_in_game() {






}




void Dead() {
	if (SoundEffect_on) {
		PlaySound(TEXT("sfx_hit.wav"), NULL, SND_FILENAME | SND_ASYNC);
	}
	for (int i = 1; i < Snake_long; i++) {
		if (gate_on) drawgate();
		GotoXY(Tail[i].x, Tail[i].y);
		cout << " ";
		Sleep(300 / Snake_long);

	}
	Sleep(speed);

}

void Afterdead() {
	snake_move = STOP;
	GotoXY(WIDTH / 2 - 14, HEIGHT / 2);
	if (sure_board("Do you want to play again?", { 15,13 }, 30, 15)) {
		setup();
		drawmap(false);
		for (int i = 0; i < 500; i++) {
			Wall[i] = { 0,0 };
		}
		index_wall = 0;
	}
	else {
		gameover = true;
		menuend = false;
		snakedead = true;
	}
}

void savedetail(string text) {
	ofstream SaveToContinue(text);
	SaveToContinue << 1 << endl;

	vector<char> temp_name;
	char a;
	int a_ascii;
	int check = 0;
	bool enter_name = true;


	Color(0, 0);
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < 21; j++) {
			GotoXY(13 + i, 9 + j);
			cout << " ";
		}
	}

	drawbox({ WIDTH / 2 - 24,HEIGHT / 2 - 3 }, 46, 8, 7);

	Color(0, 7);
	GotoXY(WIDTH / 2 - 20, HEIGHT / 2);
	cout << "Enter your name:";
	while (enter_name) {
		a = _getch();
		a_ascii = a;
		GotoXY(WIDTH / 2 - 12, HEIGHT / 2 + 3);
		cout << "                                ";
		switch (a_ascii)
		{
		case 13: {
			if (check == 0) {
				GotoXY(WIDTH / 2 - 6, HEIGHT / 2 + 3);
				cout << "TOO SHORT";
			}
			else {
				enter_name = false;
			}
			break;
		}
		case 8:
			if (temp_name.size() > 0) {
				temp_name.pop_back();
				check--;
			}
			break;
		case 32: {
			GotoXY(WIDTH / 2 - 10, HEIGHT / 2 + 3);
			cout << "CAN'T CONTAIN SPACE";
			break;
		}
		default: {
			check++;
			temp_name.push_back(a);
			break;
		}
		}
		GotoXY(WIDTH / 2 - 4, HEIGHT / 2);
		Color(0, 0);
		for (int i = 0; i < 21; i++) {
			cout << " ";
		}
		Color(0, 7);
		GotoXY(WIDTH / 2 - 4, HEIGHT / 2);
		for (int i = 0; i < temp_name.size(); i++) {
			cout << temp_name[i];
		}
		if (temp_name.size() > 20) {
			GotoXY(WIDTH / 2 - 6, HEIGHT / 2 + 3);
			cout << "TOO LONG";
			temp_name.pop_back();
			check = 20;
		}
	}
	for (int i = 0; i < temp_name.size(); i++) {
		SaveToContinue << temp_name[i];
	}
	SaveToContinue << '\0' << '\n';
	SaveToContinue << level << endl;
	SaveToContinue << Snake_long << endl;
	SaveToContinue << Snake.x << " " << Snake.y << endl;
	for (int i = 0; i < Snake_long; i++) {
		SaveToContinue << Tail[i].x << " " << Tail[i].y << endl;
	}
	SaveToContinue << lasttail.x << " " << lasttail.y << endl;
	SaveToContinue << Food.x << " " << Food.y << endl;
	switch (snake_move)
	{
	case RIGHT:
		SaveToContinue << 1;
		break;
	case LEFT:
		SaveToContinue << 2;
		break;
	case UP:
		SaveToContinue << 3;
		break;
	case DOWN:
		SaveToContinue << 4;
		break;
	}
	SaveToContinue << endl << score << endl;
	SaveToContinue << gate_on << endl;
	SaveToContinue << Gate.x << " " << Gate.y << endl;
	SaveToContinue << Gate_direction;

	SaveToContinue.close();
}

void Loaddetail(string text) {
	ifstream ContinueBySave(text);
	int Saved;
	ContinueBySave >> Saved;
	string useless;
	ContinueBySave >> useless;
	ContinueBySave >> level;
	ContinueBySave >> Snake_long;
	ContinueBySave >> Snake.x >> Snake.y;
	for (int i = 0; i < Snake_long; i++) {
		ContinueBySave >> Tail[i].x;
		ContinueBySave >> Tail[i].y;
	}
	ContinueBySave >> lasttail.x >> lasttail.y;
	ContinueBySave >> Food.x >> Food.y;
	int i;
	ContinueBySave >> i;
	switch (i)
	{
	case 1:
		snake_move = RIGHT;
		break;
	case 2:
		snake_move = LEFT;
		break;
	case 3:
		snake_move = UP;
		break;
	case 4:
		snake_move = DOWN;
		break;
	}
	meteoralive = false;
	delcount = 0;
	for (int i = 0; i < meteorsize; i++) {
		meteorTail[i].x = 0;
		meteorTail[i].y = 0;
	}
	lastmeteortail.x = 0;
	lastmeteortail.y = 0;
	lastmeteortail2.x = 0;
	lastmeteortail2.y = 0;
	stop_ticks_count = 0;
	meteor_check = false;
	ContinueBySave >> score;
	ContinueBySave >> gate_on;
	ContinueBySave >> Gate.x >> Gate.y;
	ContinueBySave >> Gate_direction;
	ContinueBySave.close();
	ofstream SaveToContinue(text);
	SaveToContinue << 0;
	SaveToContinue.close();
}

void SaveScore() {
	ofstream Savetofile("highscore.txt", ios::app);

	vector<char> temp_name;
	char a;
	int a_ascii;
	int check = 0;
	bool enter_name = true;
	char* real_name = new char[22];




	if (sure_board("Do you want to save your score?", { 12,12 }, 35, 15)) {
		drawmap(false);
		drawbox({ WIDTH / 2 - 24,HEIGHT / 2 - 3 }, 46, 6, 7);
		Color(0, 7);
		GotoXY(WIDTH / 2 - 20, HEIGHT / 2);
		cout << "Enter your name:";
		while (enter_name) {
			if (!GetAsyncKeyState(VK_RBUTTON)) {
				a = _getch();
				a_ascii = a;
				GotoXY(WIDTH / 2 - 6, HEIGHT / 2 + 5);
				cout << "                            ";
				switch (a_ascii)
				{
				case 13:
					if (check == 0) {
						GotoXY(WIDTH / 2 - 6, HEIGHT / 2 + 5);
						cout << "TOO SHORT";
					}
					else {
						enter_name = false;
					}
					break;
				case 8:
					if (temp_name.size() > 0) {
						temp_name.pop_back();
						check--;
					}
					break;
				case 32: {
					GotoXY(WIDTH / 2 - 6, HEIGHT / 2 + 5);
					cout << "CAN'T CONTAIN SPACE";
					break;
				}
				default:
					check++;
					temp_name.push_back(a);
					break;
				}
				GotoXY(WIDTH / 2 - 4, HEIGHT / 2);
				Color(0, 0);
				for (int i = 0; i < 21; i++) {
					cout << " ";
				}
				Color(0, 7);
				GotoXY(WIDTH / 2 - 4, HEIGHT / 2);
				for (int i = 0; i < temp_name.size(); i++) {
					cout << temp_name[i];
				}
				if (temp_name.size() > 20) {
					check = 20;
					GotoXY(WIDTH / 2 - 6, HEIGHT / 2 + 5);
					cout << "TOO LONG";
					temp_name.pop_back();
				}
			}
		}
		for (int i = 0; i < temp_name.size(); i++) {
			real_name[i] = temp_name[i];
			Savetofile << real_name[i];
		}
		for (int i = 0; i < 100 - sizeof(temp_name); i++) {
			Savetofile << " ";
		}
		Savetofile << score << endl;
	}
	delete[] real_name;
	Savetofile.close();
	Sort_Score();
}

int Down(const void* a, const void* b) {
	return (int)((PLAYER*)b)->ScoreSaved - (int)((PLAYER*)a)->ScoreSaved;
}

void Sort_Score() {
	File_To_Player();

	ofstream SortScoreIn("highscore.txt");
	for (int i = 0; i < 20; i++) {
		SortScoreIn << Player[i].Name;
		for (unsigned int j = 0; j < 100 - Player[i].Name.size(); j++) SortScoreIn << " ";
		SortScoreIn << Player[i].ScoreSaved << endl;
	}
	SortScoreIn.close();
}

void File_To_Player() {
	ifstream SortScoreOut("highscore.txt");
	for (int i = 0; i < 21; i++) {
		SortScoreOut >> Player[i].Name;
		SortScoreOut >> Player[i].ScoreSaved;
	}
	qsort(Player, 21, sizeof(PLAYER), Down);
	SortScoreOut.close();
}

bool sure_board(string word, position start_point, int width, int height) {
	bool sure_board_quit = false;
	int YesNo = 0;

	drawbox(start_point, width, height, 7);
	Color(0, 0);
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height; j++) {
			GotoXY(start_point.x + i, start_point.y + j);
			cout << " ";
		}
	}
	Color(0, 7);
	GotoXY(start_point.x + (width - word.size()) / 2, start_point.y + 3);
	cout << word;

	while (!sure_board_quit)
	{
		GotoXY(start_point.x + width / 2 - word.size() / 4, start_point.y + 9);
		if (YesNo == 0) Color(0, 3);
		else Color(0, 7);
		cout << "YES";
		GotoXY(start_point.x + width / 2 + word.size() / 4, start_point.y + 9);
		if (YesNo == 1) Color(0, 4);
		else Color(0, 7);
		cout << "NO";

		if (!Mouse_On) {
			switch (_getch())
			{

			case 65:
			case 97:
			case 68:
			case 100:
				YesNo = (YesNo + 1) % 2;
				break;
			case 13:
				sure_board_quit = true;
			default:
				break;
			}
		}
		else {
			bool right_pos = false;
			GetCursorPos(&real_mouse);
			mouse.x = real_mouse.x / 16;
			mouse.y = max(0, real_mouse.y - 23) / 16;
			if ((mouse.x > (start_point.x + width / 2 - word.size() / 4 - 1) && mouse.x < (start_point.x + width / 2 - word.size() / 4 + 3)) && mouse.y == start_point.y + 9) {
				YesNo = 0;
				right_pos = true;
			}
			if ((mouse.x > (start_point.x + width / 2 + word.size() / 4 - 1) && mouse.x < (start_point.x + width / 2 + word.size() / 4 + 2)) && mouse.y == start_point.y + 9) {
				YesNo = 1;
				right_pos = true;
			}
			if (GetAsyncKeyState(VK_RBUTTON) && right_pos) {
				sure_board_quit = true;
			}
		}

	}

	// bỏ các dấu vết cũ
	Color(0, 0);
	for (int i = 0; i < width; i++) {
		GotoXY(start_point.x + i, start_point.y); cout << WALL;
		GotoXY(start_point.x + i, start_point.y + height); cout << WALL;
	}
	for (int i = 0; i < height; i++) {
		GotoXY(start_point.x, start_point.y + i); cout << WALL;
		GotoXY(start_point.x + width - 1, start_point.y + i); cout << WALL;
	}
	Color(0, 0);
	for (int i = 1; i < width - 1; i++) {
		for (int j = 1; j < height - 1; j++) {
			GotoXY(start_point.x + i, start_point.y + j);
			cout << " ";
		}
	}
	// xử lí biến
	Color(0, 7);
	if (YesNo == 0) return true;
	else return false;
}

void Before_mapD2() {
	for (int i = 27; i < 33; i++)
	{
		for (int j = 15; j < 21; j++)
		{

			if ((i + j > 43) && (i - j < 16) && (i - j > 8) && (i + j < 51)) {

				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;

			}
		}
	}

	for (int i = 1; i < 21; i++)
	{

		Wall[index_wall].x = i;
		Wall[index_wall].y = 34;
		index_wall++;
		if (i < 20) {
			Wall[index_wall].x = i;
			Wall[index_wall].y = 33;
			index_wall++;
		}

	}
	for (int i = 39; i < 59; i++)
	{

		Wall[index_wall].x = i;
		Wall[index_wall].y = 1;
		index_wall++;
		if (i > 39) {

			Wall[index_wall].x = i;
			Wall[index_wall].y = 2;
			index_wall++;
		}
	}
	for (int i = 1; i < 4; i++)
	{
		for (int j = 30; j < 33; j++)
		{
			if (i - j < -28) {

				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;
			}
		}
	}
	for (int i = 56; i < 59; i++)
	{
		for (int j = 3; j < 6; j++)
		{
			if (i - j > 52) {

				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;
			}
		}
	}

	for (int i = 12; i < 17; i++)
	{
		for (int j = 1; j < 6; j++)
		{
			if (i - j > 10)
			{
				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;
			}
		}
	}


	for (int i = 43; i < 48; i++)
	{
		for (int j = 30; j < 35; j++)
		{
			if (i - j < 14)
			{
				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;
			}
		}
	}
	for (int i = 17; i < 21; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			if (i + j < 22)
			{
				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;
			}
		}
	}
	for (int i = 39; i < 43; i++)
	{
		for (int j = 31; j < 35; j++)
		{
			if (i + j > 72) {
				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;/*
				GotoXY(i, j);
				cout << WALL;*/
			}
		}
	}

	//Color(0, 35);
}

//map2
void Before_mapD3() {
	index_wall = 0;
	for (int i = 1; i < 4; i++)
	{
		for (int j = 1; j < 4; j++)
		{
			if (i + j < 5)
			{
				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;
			}
		}

	}

	for (int i = 56; i < 59; i++)
	{
		for (int j = 32; j < 35; j++)
		{
			if (i + j > 89)
			{
				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;
			}
		}
	}

	for (int i = 8; i < 28; i++)
	{
		Wall[index_wall].x = 21;
		Wall[index_wall].y = i;
		index_wall++;
		Wall[index_wall].x = 38;
		Wall[index_wall].y = i;
		index_wall++;
		if (i == 9)
		{
			for (int j = 39; j < 51; j++)
			{
				Wall[index_wall].x = j;
				Wall[index_wall].y = i;
				index_wall++;
			}
		}
		if (i == 26)
		{
			for (int j = 9; j < 21; j++)
			{
				Wall[index_wall].x = j;
				Wall[index_wall].y = i;
				index_wall++;
			}
		}

	}
	for (int i = 39; i < 42; i++)
	{
		for (int j = 24; j < 27; j++)
		{
			if (i + j < 66) {
				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;
			}
		}
	}
	for (int i = 18; i < 21; i++)
	{
		for (int j = 9; j < 12; j++)
		{
			if (i + j > 28) {
				Wall[index_wall].x = i;
				Wall[index_wall].y = j;
				index_wall++;
			}
		}
	}


	for (int i = 29; i < 31; i++) {
		for (int j = 17; i < 19; j++) {
			Wall[index_wall].x = i;
			Wall[index_wall].y = j;
			index_wall++;
		}
	}


}


void Before_mapD4() {
	index_wall = 0;
	//1
	for (int i = 1; i < 44; i++)
	{
		Wall[index_wall].x = i + 7;
		Wall[index_wall].y = 4;
		index_wall++;
		Wall[index_wall].x = i + 7;
		Wall[index_wall].y = HEIGHT - 4;
		index_wall++;
	}
	// Vẽ trái phải
	for (int i = 5; i < 28; i++)
	{
		Wall[index_wall].x = 8;
		Wall[index_wall].y = i;
		index_wall++;
		Wall[index_wall].x = 50;
		Wall[index_wall].y = HEIGHT - i;
		index_wall++;
	}

	//2
	for (int i = 1; i < 29; i++)
	{
		Wall[index_wall].x = i + 12;
		Wall[index_wall].y = 8;
		index_wall++;
		Wall[index_wall].x = 46 - i;
		Wall[index_wall].y = HEIGHT - 8;
		index_wall++;
	}
	// Vẽ trái phải
	for (int i = 5; i < 27; i++)
	{
		Wall[index_wall].x = 13;
		Wall[index_wall].y = i + 4;
		index_wall++;
		Wall[index_wall].x = 45;
		Wall[index_wall].y = HEIGHT - i - 4;
		index_wall++;
	}

	//3
	for (int i = 1; i < 19; i++)
	{
		Wall[index_wall].x = i + 17;
		Wall[index_wall].y = 12;
		index_wall++;
		Wall[index_wall].x = 41 - i;
		Wall[index_wall].y = HEIGHT - 12;
		index_wall++;
	}
	// Vẽ trái phải
	for (int i = 5; i < 19; i++)
	{
		Wall[index_wall].x = 18;
		Wall[index_wall].y = i + 8;
		index_wall++;
		Wall[index_wall].x = 40;
		Wall[index_wall].y = HEIGHT - i - 8;
		index_wall++;
	}

	//4
	for (int i = 1; i < 10; i++)
	{
		Wall[index_wall].x = i + 22;
		Wall[index_wall].y = 16;
		index_wall++;
		Wall[index_wall].x = 36 - i;
		Wall[index_wall].y = HEIGHT - 16;
		index_wall++;
	}
	// Vẽ trái phải
	for (int i = 5; i < 11; i++)
	{
		Wall[index_wall].x = 23;
		Wall[index_wall].y = i + 12;
		index_wall++;
		Wall[index_wall].x = 35;
		Wall[index_wall].y = HEIGHT - i - 12;
		index_wall++;
	}
}

void drawwall(bool check) {
	Color(7, 7);
	for (int i = 0; i < index_wall; i++) {
		GotoXY(Wall[i].x, Wall[i].y);
		cout << WALL;
		if (check) Sleep(10);
	}
	Color(0, 7);
}


bool Gate_near_wall() {
	for (int i = 0; i < index_wall; i++) {
		if (abs(Wall[i].x - Gate.x) < 3 || abs(Wall[i].y - Gate.y) < 3) {
			return true;
		}
	}
	return false;
}


void mapboss() {
	bossfight = true;
}

void drawwarning() {
	index_warning = 0;
	direction = rand() % 4 + 1;
	switch (direction) {
	case 1: {
		int meteor_start = rand() % 22 + 2;
		for (int i = 2; i < 4; i++) {
			for (int j = meteor_start; j < meteor_start + 10; j++) {
				if (j != meteor_start + 7) {
					Warning[index_warning].x = i;
					Warning[index_warning].y = j;
					index_warning++;
				}
			}
		}
		break;
	}
	case 2: {
		int meteor_start = rand() % 22 + 2;
		for (int i = WIDTH - 4; i < WIDTH - 2; i++) {
			for (int j = meteor_start; j < meteor_start + 10; j++) {
				if (j != meteor_start + 7) {
					Warning[index_warning].x = i;
					Warning[index_warning].y = j;
					index_warning++;
				}
			}
		}
		break;
	}
	case 3: {
		int meteor_start = rand() % 47 + 2;
		for (int i = meteor_start; i < meteor_start + 10; i++) {
			for (int j = 2; j < 4; j++) {
				if (i != meteor_start + 7) {
					Warning[index_warning].x = i;
					Warning[index_warning].y = j;
					index_warning++;
				}
			}
		}
		break;
	}
	case 4: {
		int meteor_start = rand() % 47 + 2;
		for (int i = meteor_start; i < meteor_start + 10; i++) {
			for (int j = HEIGHT - 4; j < HEIGHT - 2; j++) {
				if (i != meteor_start + 7) {
					Warning[index_warning].x = i;
					Warning[index_warning].y = j;
					index_warning++;
				}
			}
		}
		break;
	}
	}
	if (direction == 1 || direction == 2) {
		if (Warning[0].x > 6) {
			meteor.x = Warning[0].x + 1;
			meteor.y = Warning[0].y;
		}
		if (Warning[0].x < 6) {
			meteor.x = Warning[0].x - 1;
			meteor.y = Warning[0].y;
		}
	}
	if (direction == 3 || direction == 4) {
		if (Warning[0].y > 6) {
			meteor.x = Warning[0].x;
			meteor.y = Warning[0].y + 1;
		}
		if (Warning[0].y < 6) {
			meteor.x = Warning[0].x;
			meteor.y = Warning[0].y - 1;
		}
	}
	Color(7, 7);
	for (int i = 0; i < index_warning; i++) {
		GotoXY(Warning[i].x, Warning[i].y);
		cout << WALL;
	}
	Color(0, 7);
}

void deletewarning() {
	Color(0, 0);
	for (int i = 0; i < index_warning; i++) {
		GotoXY(Warning[i].x, Warning[i].y);
		cout << WALL;
	}
	Color(0, 7);
}

void meteormove() {
	switch (meteor_move)
	{
	case LEFT:
		meteor.x--;
		break;
	case RIGHT:
		meteor.x++;
		break;
	case UP:
		meteor.y--;
		break;
	case DOWN:
		meteor.y++;
		break;
	default:
		break;
	}
}

void meteortailxy() {
	int prevX = meteorTail[0].x;
	int prevY = meteorTail[0].y;
	int prev2X, prev2Y;
	meteorTail[0].x = meteor.x;
	meteorTail[0].y = meteor.y;
	for (int i = 1; i < meteorsize; i++)
	{
		prev2X = meteorTail[i].x;
		prev2Y = meteorTail[i].y;
		meteorTail[i].x = prevX;
		meteorTail[i].y = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	lastmeteortail.x = meteorTail[meteorsize - 2].x;
	lastmeteortail.y = meteorTail[meteorsize - 2].y;
	lastmeteortail2.x = meteorTail[meteorsize - 1].x;
	lastmeteortail2.y = meteorTail[meteorsize - 1].y;
}

void deletemeteor(int part) {
	switch (direction) {
	case 1: {
		for (int i = 0; i < meteorsize; i++) {
			GotoXY(meteorTail[i].x, meteorTail[i].y + part);
			Color(0, 0);
			cout << " ";
			Color(0, 7);
		}
		break;
	}
	case 2: {
		for (int i = 0; i < meteorsize; i++) {
			GotoXY(meteorTail[i].x, meteorTail[i].y + part);
			Color(0, 0);
			cout << " ";
			Color(0, 7);
		}
		break;
	}
	case 3: {
		for (int i = 0; i < meteorsize; i++) {
			GotoXY(meteorTail[i].x + part, meteorTail[i].y);
			Color(0, 0);
			cout << " ";
			Color(0, 7);
		}
		break;
	}
	case 4: {
		for (int i = 0; i < meteorsize; i++) {
			GotoXY(meteorTail[i].x + part, meteorTail[i].y);
			Color(0, 0);
			cout << " ";
			Color(0, 7);
		}
		break;
	}
	}
}


void drawmeteor_ht(int part) {
	mtx.lock();
	if (randomstop > 0) {
		randomstop--;
		meteor_index_end = min(meteor_index_end + 1, meteorsize);
		GotoXY(meteor.x, meteor.y + part);
		for (int i = 0; i < meteorsize; i++) {
			GotoXY(meteorTail[i].x, meteorTail[i].y + part);
			if (meteor_move != 0) {
				if (meteormodel[part][i] == '/') {
					Color(0, 0);
					cout << meteormodel[part][i];
					Color(0, 7);
				}
				if (meteormodel[part][i] == 'X') {
					Color(4, 4);
					cout << meteormodel[part][i];
					Color(0, 7);
				}
				if (meteormodel[part][i] == 'O') {
					Color(6, 6);
					cout << meteormodel[part][i];
					Color(0, 7);
				}
			}
		}
		GotoXY(lastmeteortail.x, lastmeteortail.y + part);
		if (meteor_move != 0) {
			Color(0, 12);
			cout << "-";
			Color(0, 7);
		}
		GotoXY(lastmeteortail2.x, lastmeteortail2.y + part);
		if (meteor_move != 0) {
			cout << " ";
		}
		if (Food.x == meteorTail[0].x && Food.y == meteorTail[0].y + part) {
			spawnfood();
		}
		if (part == 9) meteor_check = true;
	}
	
	if (randomstop <= 0 && meteor_check) {
		meteor_move = STOP;
		stop_ticks_count++;
		if (stop_ticks_count >= 150) {
			deletemeteor(part);
			delcount++;
			if (delcount >= 30) {
				meteoralive = false;
				delcount = 0;
				for (int i = 0; i < meteorsize; i++) {
					meteorTail[i].x = 0;
					meteorTail[i].y = 0;
				}
				stop_ticks_count = 0;
				lastmeteortail.x = 0;
				lastmeteortail.y = 0;
				lastmeteortail2.x = 0;
				lastmeteortail2.y = 0;
				meteor_check = false;
			}
		}
	}
	mtx.unlock();
}

void drawmeteor_vt(int part) {
	mtx.lock();
	if (randomstop > 0) {
		randomstop--;
		meteor_index_end = min(meteor_index_end + 1, meteorsize);
		GotoXY(meteor.x + part, meteor.y);
		for (int i = 0; i < meteorsize; i++) {
			GotoXY(meteorTail[i].x + part, meteorTail[i].y);
			if (meteor_move != 0) {
				if (meteor_move != 0) {
					if (meteormodel[part][i] == '/') {
						Color(0, 0);
						cout << meteormodel[part][i];
						Color(0, 7);
					}
					if (meteormodel[part][i] == 'X') {
						Color(4, 4);
						cout << meteormodel[part][i];
						Color(0, 7);
					}
					if (meteormodel[part][i] == 'O') {
						Color(6, 6);
						cout << meteormodel[part][i];
						Color(0, 7);
					}
				}
			}
		}
		GotoXY(lastmeteortail.x + part, lastmeteortail.y);
		if (meteor_move != 0) {
			Color(0, 12);
			cout << "|";
			Color(0, 7);
		}
		GotoXY(lastmeteortail2.x + part, lastmeteortail2.y);
		if (meteor_move != 0) {
			cout << " ";
		}
		if (Food.x == meteorTail[0].x + part && Food.y == meteorTail[0].y) {
			spawnfood();
		}
		if (part == 9) meteor_check = true;
	}
	
	if (randomstop <= 0 && meteor_check) {
		meteor_move = STOP;
		stop_ticks_count++;
		if (stop_ticks_count >= 150) {
			deletemeteor(part);
			delcount++;
			if (delcount >= 30) {
				meteoralive = false;
				delcount = 0;
				for (int i = 0; i < meteorsize; i++) {
					meteorTail[i].x = 0;
					meteorTail[i].y = 0;
				}
				stop_ticks_count = 0;
				lastmeteortail.x = 0;
				lastmeteortail.y = 0;
				lastmeteortail2.x = 0;
				lastmeteortail2.y = 0;
				meteor_check = false;
			}
		}
	}
	mtx.unlock();
}

void spawnmeteor() {
	if (meteor_move != 0) {
		meteormove();
		meteortailxy();
	}
	switch (direction) {
	case 1: {
		GotoXY(meteor.x, meteor.y);
		meteor_move = RIGHT;
		thread meteor1(drawmeteor_ht, 0);
		thread meteor2(drawmeteor_ht, 1);
		thread meteor3(drawmeteor_ht, 2);
		thread meteor4(drawmeteor_ht, 3);
		thread meteor5(drawmeteor_ht, 4);
		thread meteor6(drawmeteor_ht, 5);
		thread meteor7(drawmeteor_ht, 6);
		thread meteor8(drawmeteor_ht, 7);
		thread meteor9(drawmeteor_ht, 8);
		thread meteor10(drawmeteor_ht, 9);
		meteor1.join();
		meteor2.join();
		meteor3.join();
		meteor4.join();
		meteor5.join();
		meteor6.join();
		meteor7.join();
		meteor8.join();
		meteor9.join();
		meteor10.join();
		break;
	}
	case 2: {
		GotoXY(meteor.x, meteor.y);
		meteor_move = LEFT;
		thread meteor1(drawmeteor_ht, 0);
		thread meteor2(drawmeteor_ht, 1);
		thread meteor3(drawmeteor_ht, 2);
		thread meteor4(drawmeteor_ht, 3);
		thread meteor5(drawmeteor_ht, 4);
		thread meteor6(drawmeteor_ht, 5);
		thread meteor7(drawmeteor_ht, 6);
		thread meteor8(drawmeteor_ht, 7);
		thread meteor9(drawmeteor_ht, 8);
		thread meteor10(drawmeteor_ht, 9);
		meteor1.join();
		meteor2.join();
		meteor3.join();
		meteor4.join();
		meteor5.join();
		meteor6.join();
		meteor7.join();
		meteor8.join();
		meteor9.join();
		meteor10.join();
		break;
	}
	case 3: {
		GotoXY(meteor.x, meteor.y);
		meteor_move = DOWN;
		thread meteor1(drawmeteor_vt, 0);
		thread meteor2(drawmeteor_vt, 1);
		thread meteor3(drawmeteor_vt, 2);
		thread meteor4(drawmeteor_vt, 3);
		thread meteor5(drawmeteor_vt, 4);
		thread meteor6(drawmeteor_vt, 5);
		thread meteor7(drawmeteor_vt, 6);
		thread meteor8(drawmeteor_vt, 7);
		thread meteor9(drawmeteor_vt, 8);
		thread meteor10(drawmeteor_vt, 9);
		meteor1.join();
		meteor2.join();
		meteor3.join();
		meteor4.join();
		meteor5.join();
		meteor6.join();
		meteor7.join();
		meteor8.join();
		meteor9.join();
		meteor10.join();
		break;
	}
	case 4: {
		GotoXY(meteor.x, meteor.y);
		meteor_move = UP;
		thread meteor1(drawmeteor_vt, 0);
		thread meteor2(drawmeteor_vt, 1);
		thread meteor3(drawmeteor_vt, 2);
		thread meteor4(drawmeteor_vt, 3);
		thread meteor5(drawmeteor_vt, 4);
		thread meteor6(drawmeteor_vt, 5);
		thread meteor7(drawmeteor_vt, 6);
		thread meteor8(drawmeteor_vt, 7);
		thread meteor9(drawmeteor_vt, 8);
		thread meteor10(drawmeteor_vt, 9);
		meteor1.join();
		meteor2.join();
		meteor3.join();
		meteor4.join();
		meteor5.join();
		meteor6.join();
		meteor7.join();
		meteor8.join();
		meteor9.join();
		meteor10.join();
		break;
	}
	}
}

void boss() {
	if (bossfight) {
		if (warning_exist == false && meteoralive == false) {
			drawwarning();
			ticks_count = 0;
			warning_exist = true;
		}
		ticks_count++;
		if (ticks_count == 30 && warning_exist == true) {
			deletewarning();
			warning_exist = false;
			meteoralive = true;
			if (direction == 1 || direction == 2) randomstop = 10 * (rand() % 33 + 20);
			else randomstop = 10 * (rand() % 10 + 22);
			meteor_check = false;
		}
		if (warning_exist == false && meteoralive == true) {
			spawnmeteor();
			drawWALL();
			drawwall(false);
		}
	}
}



void drawintro(int part) {
	mtx.lock();
	intro_index_end = min(intro_index_end + 1, introsize);
	GotoXY(intro.x, intro.y + part);
	for (int i = 0; i < introsize; i++) {
		GotoXY(introTail[i].x, introTail[i].y + part);
		if (intromodel[part][i] == '/') {
			Color(0, 0);
			cout << intromodel[part][i];
			Color(0, 7);
		}
		if (intromodel[part][i] == 'B') {
			Color(7, 7);
			cout << intromodel[part][i];
			Color(0, 7);
		}
		if (intromodel[part][i] == 'G') {
			Color(10, 10);
			cout << intromodel[part][i];
			Color(0, 7);
		}
		if (intromodel[part][i] == 'Y') {
			Color(6, 6);
			cout << intromodel[part][i];
			Color(0, 7);
		}
		if (intromodel[part][i] == 'L') {
			Color(14, 14);
			cout << intromodel[part][i];
			Color(0, 7);
		}
		if (intromodel[part][i] == 'D') {
			Color(2, 2);
			cout << intromodel[part][i];
			Color(0, 7);
		}
	}
	GotoXY(lastintrotail.x, lastintrotail.y + part);
	if (intro_move != 0) {
		cout << " ";
	}
	if (introTail[0].x == 1) {
		intro_move = STOP;
		if (part == 30) {
			Sleep(2000);
			introplayed = true;
		}
	}
	mtx.unlock();
}

void intromove() {
	switch (intro_move)
	{
	case LEFT:
		intro.x--;
		break;
	case RIGHT:
		intro.x++;
		break;
	case UP:
		intro.y--;
		break;
	case DOWN:
		intro.y++;
		break;
	default:
		break;
	}
}

void introtailxy() {
	int prevX = introTail[0].x;
	int prevY = introTail[0].y;
	int prev2X, prev2Y;
	introTail[0].x = intro.x;
	introTail[0].y = intro.y;
	for (int i = 1; i < introsize; i++)
	{
		prev2X = introTail[i].x;
		prev2Y = introTail[i].y;
		introTail[i].x = prevX;
		introTail[i].y = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	lastintrotail.x = introTail[introsize - 1].x;
	lastintrotail.y = introTail[introsize - 1].y;
}

void snakeintro() {
	if (intro_move != STOP) {
		intromove();
		introtailxy();
	}
	GotoXY(intro.x, intro.y);
	intro_move = LEFT;
	thread intro1(drawintro, 0);
	thread intro2(drawintro, 1);
	thread intro3(drawintro, 2);
	thread intro4(drawintro, 3);
	thread intro5(drawintro, 4);
	thread intro6(drawintro, 5);
	thread intro7(drawintro, 6);
	thread intro8(drawintro, 7);
	thread intro9(drawintro, 8);
	thread intro10(drawintro, 9);
	thread intro11(drawintro, 10);
	thread intro12(drawintro, 11);
	thread intro13(drawintro, 12);
	thread intro14(drawintro, 13);
	thread intro15(drawintro, 14);
	thread intro16(drawintro, 15);
	thread intro17(drawintro, 16);
	thread intro18(drawintro, 17);
	thread intro19(drawintro, 18);
	thread intro20(drawintro, 19);
	thread intro21(drawintro, 20);
	thread intro22(drawintro, 21);
	thread intro23(drawintro, 22);
	thread intro24(drawintro, 23);
	thread intro25(drawintro, 24);
	thread intro26(drawintro, 25);
	thread intro27(drawintro, 26);
	thread intro28(drawintro, 27);
	thread intro29(drawintro, 28);
	thread intro30(drawintro, 29);
	thread intro31(drawintro, 30);
	intro1.join();
	intro2.join();
	intro3.join();
	intro4.join();
	intro5.join();
	intro6.join();
	intro7.join();
	intro8.join();
	intro9.join();
	intro10.join();
	intro11.join();
	intro12.join();
	intro13.join();
	intro14.join();
	intro15.join();
	intro16.join();
	intro17.join();
	intro18.join();
	intro19.join();
	intro20.join();
	intro21.join();
	intro22.join();
	intro23.join();
	intro24.join();
	intro25.join();
	intro26.join();
	intro27.join();
	intro28.join();
	intro29.join();
	intro30.join();
	intro31.join();
}


void win() {
	PlaySound(TEXT("Everyone-Joins-the-battle-song_1.wav"), NULL, SND_ASYNC | SND_LOOP);
	bool win_end = false;
	int time = 0;
	while (!win_end) {
		int m = rand() % (WIDTH-20);
		int n = rand() % HEIGHT;
		char win[5][30] = { "X       X  XXXXX  XX    X",
							"X   X   X    X    X X   X",
							"X   X   X    X    X  X  X",
							"X   X   X    X    X   X X",
							" XXX XXX   XXXXX  X    XX" };
		int i = rand() % 15 +1;
		int c = i + rand() % 2 + 1;
		for (int j = 0; j < 5; j++) {
			GotoXY(m, j + n);
			for (int k = 0; k < sizeof(win[j]); k++) {
				Color(c, c);
				if (win[j][k] == ' ') Color(0, 0);
				cout << win[j][k];
			}
		}
		
		Color(0, 0);
		Sleep(100);
		time++;
		if (time >= 20) {
			win_end = true;
		}


		if(_kbhit()) {
			switch (_getch())
			{
			default:
				win_end = true;
				break;
			}
		}
	}
	SaveScore();
	gameover = true;
	menuend = false;
	snakedead = false;
}