//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "math.h"

struct Window {
    HWND hWnd;//����� ����
    HDC device_context, context;// ��� ��������� ���������� (��� �����������)
    int width, height;//���� �������� ������� ���� ������� ������� ���������
} window;


void ShowBitmap(int x, int y, int width, int height, HBITMAP hBitmapBall, bool alpha = false);

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

    void Load(const char* name)
    {
        hBitmap = LoadBMP(name);
    }

};
const int enemycout = 22;
Enemycco enemy1[enemycout];

struct GameState {
    int score, lives;//���������� ��������� ����� � ���������� "������"
    bool action = false;//��������� - �������� (����� ������ ������ ������) ��� ����
} game;
 
enum class GameMode { map, battle, loot, terminal };
GameMode game_mode = GameMode::map;

void InitGame() { //TODO
     srand(0);

    int i = 0;
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
    game.lives = 9;

}

/*void ProcessSound(const char* name)//������������ ���������� � ������� .wav, ���� ������ ������ � ��� �� ����� ��� � ���������
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//���������� name �������� ��� �����. ���� ASYNC ��������� ����������� ���� ���������� � ����������� ���������
}*/

void ShowBitmap(int x, int y, int width, int height, HBITMAP hBitmap, bool alpha) {
    HDC hMemDC = CreateCompatibleDC(window.context);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    BITMAP bm;

    if (hOldBitmap) {
        GetObject(hBitmap, sizeof(BITMAP), &bm);
        if (alpha) {
            TransparentBlt(window.context, x, y, width, height, hMemDC, NULL, NULL, width, height, RGB(0, 0, 0));
        }
        else {
            StretchBlt(window.context, x, y, width, height, hMemDC, NULL, NULL, bm.bmWidth, bm.bmHeight, SRCCOPY);
        }
        SelectObject(hMemDC, hOldBitmap);
    }
    DeleteDC(hMemDC);
}

bool CheckCollisionMouse(Enemycco& coll) {
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
    
    window.hWnd = CreateWindow("edit", NULL, WS_POPUPWINDOW |WS_VISIBLE , 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), NULL, NULL, NULL, NULL);

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



int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

    InitWindow();
    InitGame();
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