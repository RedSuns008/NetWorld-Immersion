//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "math.h"

struct {
    HWND hWnd;//хэндл окна
    HDC device_context, context;// два контекста устройства (для буферизации)
    int width, height;//сюда сохраним размеры окна которое создаст программа
} window;

#include "WinApiUtils.h"
#include "utils.h"
#include "mouse.h"
#include "timer.h"
#include "player.h"
#include "enemy.h"


enum class Entity { empty, enemy, lootchest, terminal };
int random = 1;

HBITMAP enemycco_bmp;
HBITMAP lootchest_bmp;
HBITMAP terminal_bmp;
HBITMAP raketka_bmp;
HBITMAP phon1_bmp;
HBITMAP Battlephon1_bmp;
HBITMAP Inventoryphon1_bmp;
HBITMAP Terminalphon1_bmp;

struct Enemycco {
    float x, y, width, height;
    HBITMAP hBitmap;
    Entity type;

    void Load(const char* name)//TODO лоады как будто бы можно сократить до одного
    {
        hBitmap = LoadBMP(name);
    }

};
const int enemycout = 22;
Enemycco enemy1[enemycout];

struct { //TODO
    int score, balls;//количество набранных очков и оставшихся "жизней"
    bool action = false;//состояние - ожидание (игрок должен нажать пробел) или игра
} game;


enum class GameMode { map, battle, loot, terminal };
GameMode game_mode = GameMode::map;

void ShowBitmap(int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);

DWORD AttackStartTime = 0;
DWORD AttackTime = 1000;
DWORD AttackcurrentTime = 0;

//TODO кнопки расположены абсолютно хаотично, если сделать это менее вырвиглазно и более структурированно и писать игру и рефакторить станет легче, как будто бы на стадии написания для удобства можно отрисовать текст под нашими картиночками
class Button {
public:

    float x, y, width, height;
    HBITMAP hBitmap;//хэндл к спрайту шарика
    HBITMAP hBitmapGlow;

    bool CheckCollisionMouse() {
        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
    }
    void Load(const char* imagename, const char* imagenameglow, float x_, float y_, float w, float h) {
        x = x_; y = y_;
        hBitmap = LoadBMP(imagename);
        hBitmapGlow = LoadBMP(imagenameglow);
        height = h * window.height;
        width = w * window.width;
        x = window.width / 2 - width * x;
        y = window.height / 2 + height * y;
    }
    void LoadInv(const char* imagename, float x_, float y_, float w, float h) {
        x = x_; y = y_;
        hBitmap = LoadBMP(imagename);
        height = h * window.height;
        width = w * window.width;
        x = window.width / 2 - width * x;
        y = window.height / 2 + height * y;
    }

    bool Show() {
        bool pw_collision = CheckCollisionMouse();
        float offset = (float)pw_collision * sinf(timeGetTime() * .008);

        if ((currentTime < healStartTime + healTime) || (AttackcurrentTime < AttackStartTime + AttackTime))
        {
            offset = 0;
        }

        ShowBitmap(x, y + offset, width, height, pw_collision ? hBitmapGlow : hBitmap);
        return pw_collision;
    }
    bool ShowInv() {
        bool pw_collision = CheckCollisionMouse();
        ShowBitmap(x, y, width, height, hBitmap);
        return pw_collision;
    }

    bool CheckCollisionMouseHeal() {
        if (Mouse.L_butt) {
            if (Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y)
            {
                if (currentTime > healStartTime + healTime) //todo
                {
                    player.adjustHeal();
                    return true;
                }

            }
        }
        return false;
    }
};

class Bar { //done and todo
public:
    float x, y, width, height, Health, Shield;
    HBITMAP hBitmapBack;
    HBITMAP hBitmapFront;
    void Load(const char* imagenameBack, const char* imagenameFront, float x_, float y_, float w, float h) {
        x = x_; y = y_;
        hBitmapBack = LoadBMP(imagenameBack);
        hBitmapFront = LoadBMP(imagenameFront);
        height = h * window.height;
        width = w * window.width;
        x = window.width / 2 - width * x;
        y = window.height / 2 + height * y;
    }
    void ShowHealth(int Health) { //TODO как будто бы можно сделать менее вырвиглазно в самой игре, да и кодяру оптимизировать
        ShowBitmap(x, y, width, height, hBitmapBack);
        ShowBitmap(x, y, Health / (float)player.HealthMax * width, height, hBitmapFront);
    }
    void ShowShield(int Shield) {
        ShowBitmap(x, y, width, height, hBitmapBack);
        ShowBitmap(x, y, Shield / (float)player.ShieldMax * width, height, hBitmapFront);
    }
    bool CheckCollisionMouse()
    {
        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
    }
};

Bar Health_bar, HealthEnemy_bar, Shield_bar, ShieldEnemy_bar; //TODO ВЫРВИГЛААААЗ

Button PrimWeapon, SpecWeapon, DestructiveWeapon, EnemyB, Exit, Heal_butt, Inventory_butt, Boots__inventory_butt;
HBITMAP hBack;// хэндл для фонового изображения
HBITMAP hBattleBack;
HBITMAP InventoryhBack;
HBITMAP TerminalhBack;

void InitGame() //TODO
{
    //в этой секции загружаем спрайты с помощью функций gdi
    //пути относительные - файлы должны лежать рядом с .exe 
    //результат работы LoadImageA сохраняет в хэндлах битмапов, рисование спрайтов будет произовдиться с помощью этих хэндлов

    PrimWeapon.Load("pw_butt.bmp", "pw_butt_glow.bmp", 1.65, 4.13, .09, .09);
    SpecWeapon.Load("sw_butt.bmp", "sw_butt_glow.bmp", 0.5, 4.13, .09, .09);
    DestructiveWeapon.Load("dw_butt.bmp", "dw_butt_glow.bmp", -0.7, 4.13, .09, .09);
    EnemyB.Load("Enemy_butt.bmp", "Enemy_butt_glow.bmp", 0.43, -0.56, .25, .60);
    Exit.Load("Exit_butt.bmp", "Exit_butt_glow.bmp", 12, -16, .04, .03);
    Heal_butt.Load("Heal_butt.bmp", "Heal_butt.bmp", -2.65, 5.12, .07, .07);
    Inventory_butt.Load("Heal_butt.bmp", "Heal_butt.bmp", 3.65, 5.12, .07, .07);
    Boots__inventory_butt.LoadInv("Boots__inventory_butt.bmp", 0.5, 3., .4, .07);

    Health_bar.Load("Health_bar_back.bmp", "Health_bar_front.bmp", 0.45, 33.7, .28, .01);
    Shield_bar.Load("Shield_bar_back.bmp", "Shield_bar_front.bmp", 0.45, 31.7, .28, .01);
    ShieldEnemy_bar.Load("Shield_bar_back.bmp", "Shield_bar_front.bmp", 0.4, -42.4, .28, .01);
    HealthEnemy_bar.Load("Health_bar_back.bmp", "Health_bar_front.bmp", 0.4, -40.4, .28, .01);

    enemycco_bmp = LoadBMP("enemycco.bmp");
    lootchest_bmp = LoadBMP("lootchest.bmp");
    terminal_bmp = LoadBMP("terminal.bmp");
    raketka_bmp = LoadBMP("raketka.bmp");
    phon1_bmp = LoadBMP("phon1.bmp");
    Battlephon1_bmp = LoadBMP("Battlephon1.bmp");
    Inventoryphon1_bmp = LoadBMP("Inventoryphon1.bmp");
    Terminalphon1_bmp = LoadBMP("Terminalphon1.bmp");

    srand(0);

    int i = 0;//TODO может тоже пересесть на свитчкейс и потом загнать в будущем это в переменку? Но тут я не уверен как можно переписать, пробовал, нарываюсь лицом на вилы ерроров
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

void ProcessSound(const char* name)//проигрывание аудиофайла в формате .wav, файл должен лежать в той же папке где и программа
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//переменная name содежрит имя файла. флаг ASYNC позволяет проигрывать звук паралельно с исполнением программы
}

void ShowScore()
{
    return;
    //поиграем шрифтами и цветами
    SetTextColor(window.context, RGB(160, 160, 160));
    SetBkColor(window.context, RGB(0, 0, 0));
    auto hFont = CreateFont(70, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "GEORGIA");
    auto hTmp = (HFONT)SelectObject(window.context, hFont);

    char txt[32];//буфер для текста
    _itoa_s(game.score, txt, 10);//преобразование числовой переменной в текст. текст окажется в переменной txt
    TextOutA(window.context, 10, 10, "Score", 5);
    TextOutA(window.context, 200, 10, (LPCSTR)txt, strlen(txt));

    _itoa_s(game.balls, txt, 10);
    TextOutA(window.context, 10, 100, "Balls", 5);
    TextOutA(window.context, 200, 100, (LPCSTR)txt, strlen(txt));
}

/*void ProcessInput() //TODO
{
    if (!game.action && GetAsyncKeyState(VK_SPACE))
    {
        game.action = true;
        ProcessSound("knopka-voda-vyisokii-rezkii.wav");
    }
}*/ 

void ShowBitmap(int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha) //TODO
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(window.context); // Создаем контекст памяти, совместимый с контекстом отображения
    hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmapBall);// Выбираем изображение bitmap в контекст памяти

    if (hOldbm) // Если не было ошибок, продолжаем работу
    {
        GetObject(hBitmapBall, sizeof(BITMAP), (LPSTR)&bm); // Определяем размеры изображения

        if (alpha)
        {
            TransparentBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, x1, y1, RGB(0, 0, 0));//все пиксели черного цвета будут интепретированы как прозрачные
        }
        else
        {
            StretchBlt(window.context, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // Рисуем изображение bitmap
        }

        SelectObject(hMemDC, hOldbm);// Восстанавливаем контекст памяти
    }

    DeleteDC(hMemDC); // Удаляем контекст памяти
}

void ShowInventory() {
    ShowBitmap(0, 0, window.width / 2., window.height / 2., InventoryhBack);//задний фон
    bool BootsInventory = Boots__inventory_butt.ShowInv();
}

void ShowLoot() {
    ShowBitmap(0, 0, window.width, window.height, InventoryhBack);//задний фон
    bool exit = Exit.Show();
    bool BootsInventory = Boots__inventory_butt.Show();
}

void ShowTerminal() {
    ShowBitmap(0, 0, window.width, window.height, TerminalhBack);//задний фон
    bool exit = Exit.Show();
}





bool CheckCollisionMouse(Enemycco& coll)//TODO
{
    return sqrt(pow(Mouse.x - coll.x, 2) + pow(Mouse.y - coll.y, 2)) < coll.height;
}
#include "MapGame.h"


void ProcessRoom()
{
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

void InitWindow()
{
    SetProcessDPIAware();
    
    window.hWnd = CreateWindow("edit", 0, WS_POPUPWINDOW |WS_VISIBLE , 0, 0, GetSystemMetrics(SM_CXSCREEN)/2, GetSystemMetrics(SM_CYSCREEN), 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//из хэндла окна достаем хэндл контекста устройства для рисования
    window.width = r.right - r.left;//определяем размеры и сохраняем
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//второй буфер
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//привязываем окно к контексту
    GetClientRect(window.hWnd, &r);

}

#include "BattleGame.h"

void TerminalGame() { //TODO
    ShowTerminal();
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
    Sleep(16);
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }
}

void LootGame() {
    ShowLoot();

    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//копируем буфер в окно
    Sleep(20);//ждем 16 милисекунд (1/количество кадров в секунду)
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }//done
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    InitWindow();//здесь инициализируем все что нужно для рисования в окне
    InitGame();//здесь инициализируем переменные игры

    while (!GetAsyncKeyState(VK_ESCAPE))
    {
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