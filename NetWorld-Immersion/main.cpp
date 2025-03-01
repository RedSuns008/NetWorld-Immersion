//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "math.h"

struct {
    HWND hWnd;//����� ����
    HDC device_context, context;// ��� ��������� ���������� (��� �����������)
    int width, height;//���� �������� ������� ���� ������� ������� ���������
} window;


void ShowBitmap(int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);

#include "WinApiUtils.h"
#include "utils.h"
#include "mouse.h"
#include "timer.h"
#include "player.h"
#include "enemy.h"
#include "HBITMAPS.h"
#include "Button.h"
#include "Bar.h"

enum class Entity { empty, enemy, lootchest, terminal };
int random = 1;

struct Enemycco {
    float x, y, width, height;
    HBITMAP hBitmap;
    Entity type;

    void Load(const char* name)//TODO ����� ��� ����� �� ����� ��������� �� ������
    {
        hBitmap = LoadBMP(name);
    }

};
const int enemycout = 22;
Enemycco enemy1[enemycout];

struct { //TODO
    int score, balls;//���������� ��������� ����� � ���������� "������"
    bool action = false;//��������� - �������� (����� ������ ������ ������) ��� ����
} game;
 
enum class GameMode { map, battle, loot, terminal };
GameMode game_mode = GameMode::map;

void InitGame() { //TODO
    //� ���� ������ ��������� ������� � ������� ������� gdi
    //���� ������������� - ����� ������ ������ ����� � .exe 
    //��������� ������ LoadImageA ��������� � ������� ��������, ��������� �������� ����� ������������� � ������� ���� �������
    srand(0);

    int i = 0;//TODO ����� ���� ��������� �� ��������� � ����� ������� � ������� ��� � ���������? �� ��� � �� ������ ��� ����� ����������, ��������, ��������� ����� �� ���� �������
    float cellsize = 50;
    for (int x = 0; x < window.width / cellsize; x++) {
        for (int y = 0; y < window.height / cellsize; y++) {
            if (i >= enemycout) {
                continue;
            }
            Entity Etype;
            auto rnd = (rand() % 100);
            if (rnd < 97) {
                Etype = Entity::empty;
            }
            else {
                rnd = 100 - rnd;
                Etype = (Entity)rnd;
            }
            if (Etype == Entity::empty) {
                continue;
            }
            enemy1[i].type = Etype;

            switch (enemy1[i].type) {
            case Entity::enemy:
                enemy1[i].hBitmap = enemycco_bmp;
                break;
            case Entity::lootchest:
                enemy1[i].hBitmap = lootchest_bmp;
                break;
            case Entity::terminal:
                enemy1[i].hBitmap = terminal_bmp;
                break;
            }

            enemy1[i].width = cellsize;
            enemy1[i].height = cellsize;
            float screen_X = enemy1[i].width * x;
            float screen_Y = enemy1[i].height * y;
            enemy1[i].x = screen_X;
            enemy1[i].y = screen_Y;
            i++;
        }
    }

    hBack = LoadBMP("phon1.bmp");
    hBattleBack = LoadBMP("Battlephon1.bmp");
    InventoryhBack = LoadBMP("Lootphon1.bmp");
    TerminalhBack = LoadBMP("Terminalphon1.bmp");

    game.score = 0;
    game.balls = 9;

}

void ProcessSound(const char* name)//������������ ���������� � ������� .wav, ���� ������ ������ � ��� �� ����� ��� � ���������
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//���������� name �������� ��� �����. ���� ASYNC ��������� ����������� ���� ���������� � ����������� ���������
}
/*void ProcessInput()  { //TODO
    if (!game.action && GetAsyncKeyState(VK_SPACE))
    {
        game.action = true;
        ProcessSound("knopka-voda-vyisokii-rezkii.wav");
    }
}*/ 

void ShowBitmap(int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha) { //TODO
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(window.context); // ������� �������� ������, ����������� � ���������� �����������
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);// �������� ����������� bitmap � �������� ������

    if (hOldbm) // ���� �� ���� ������, ���������� ������
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm); // ���������� ������� �����������

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//��� ������� ������� ����� ����� ��������������� ��� ����������
        }
        else
        {
            StretchBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // ������ ����������� bitmap
        }

        SelectObject(hMemDC, hOldbm);// ��������������� �������� ������
    }

    DeleteDC(hMemDC); // ������� �������� ������
}

bool CheckCollisionMouse(Enemycco& coll) { //TODO
    return sqrt(pow(Mouse.x - coll.x, 2) + pow(Mouse.y - coll.y, 2)) < coll.height;
}

void ProcessRoom() {
    for (int i = 0; i < enemycout; i++) {
        if (Mouse.L_butt && CheckCollisionMouse(enemy1[i]) == true) {
            switch (enemy1[i].type) {
            case Entity::lootchest:
                game_mode = GameMode::loot;
                break;
            case Entity::enemy:
                game_mode = GameMode::battle;
                break;
            case Entity::terminal:
                game_mode = GameMode::terminal;
                break;
            }
        }
    }
}//done

void InitWindow() {
    SetProcessDPIAware();
    
    window.hWnd = CreateWindow("edit", 0, WS_POPUPWINDOW |WS_VISIBLE , 0, 0, GetSystemMetrics(SM_CXSCREEN)/2, GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//�� ������ ���� ������� ����� ��������� ���������� ��� ���������
    window.width = r.right - r.left;//���������� ������� � ���������
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//������ �����
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//����������� ���� � ���������
    GetClientRect(window.hWnd, &r);

}

#include "Inventory.h"
#include "MapGame.h"
#include "BattleGame.h"
#include "TerminalGame.h"
#include "LootGame.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow) {

    InitWindow();//����� �������������� ��� ��� ����� ��� ��������� � ����
    InitGame();//����� �������������� ���������� ����
    InitMap();

    while (!GetAsyncKeyState(VK_ESCAPE))  {
        currentTime = timeGetTime();
        AttackcurrentTime = timeGetTime();
        Mouse.Update();

        switch (game_mode) {
        case GameMode::map: MapGame(); break;
        case GameMode::battle: BattleGame(); break;
        case GameMode::loot: LootGame(); break;
        case GameMode::terminal: TerminalGame(); break;
        }
    }

}
// ����� ����������� ��������� �������, ������� ������ ������� � ���������� � �������� �� �������� �� ����������, � �� ��������� �� �����, �� switch'��� ������� �� �� �����, �� ���� ���� ����������� ����� ���������� � ������������ ���
// ����������� ����������, ����� ������������. ��� API'���� ����� ������� � ��������� ���� � � ���������� �������� ���������, � ������� ������� � ���������� � �������� �����������.(�������� ����� �����, ����� ����������).
//����� � ���� ��������� ���� ���� ��������,������� �������������� � ���������, ����� ������� �� chest'� ���� ��������, ����� ������� �� terminal'�? ���� ��������. UPD: ���� ���������, �� ������ ��������� ��� ���� �� ����������� ����, �� �����������, �� ����������� � ��������, �� ����������� � ���� ��������.
// ������ ��� ����������� �� ����� ����, ������ ��� �� ������ ���������, ������������ �� �� �� ��� ���� ����, ��� �� �� ���� ���������� �� ����-�����/���������. �� �������� ��� ������ � ��� � X86 �����������, ���� � �� ������� �� X64, ������ 32-� �����)))).
//TODO - ����� ����� ���, ����+� � �������� �����

//����� � ���������� ������ ����������, �� ������� ����� 8��, ����� ������ � 2 � �������� ������ �� �� �������� ������ ����.