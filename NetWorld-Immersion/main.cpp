//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "math.h"

struct Window {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
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
    int score, lives;//количество набранных очков и оставшихся "жизней"
    bool action = false;//состояние - ожидание (игрок должен нажать пробел) или игра
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

/*void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
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
    window.device_context = GetDC(window.hWnd);//из хэндла окна достаем хэндл контекста устройства для рисования
    window.width = r.right - r.left;//определяем размеры и сохраняем
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//второй буфер
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//привязываем окно к контексту
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
// Нужно максимально сократить повторы, загнать вызовы функций в переменные и вызывать их локально из переменных, а не создавать по новой, со switch'ами сделать то же самое, за счет чего увеличиваем общую читаемость и оптимизируем код
// Максимально рефакторим, ПОСЛЕ оптимизируем. Все API'шные штуки выносим в отдельный слой и в дальнейшем вызываем функциями, а функции выносим в переменные и вызываем переменными.(например опрос мышки, опрос клавиатуры).
//Когда в игре наносится урон игра крашится,выявить закономерность и поправить, когда кликаем на chest'ы игра крашится, когда кликаем на terminal'ы? тоже крашится. UPD: Краш рандомный, то подряд несколько раз окно не открывается игры, то открывается, то открывается и крашится, то открывается и дает поиграть.
// Прежде чем пересадимся на новую либу, чистим код от лишних сущностей, переписываем всё всё всё под нашу игру, что бы не было переменных из пинг-понга/арканоида. Не забываем что кодяра у нас в X86 разрядности, хотя я бы перевел на X64, нахрен 32-х битку)))).
//TODO - везде писал так, ктрл+ф и радуемся жизни

//Нужно с выделением памяти поработать, он сначала берет 8мб, потом падает в 2 и статично держит их на основном экране игры.