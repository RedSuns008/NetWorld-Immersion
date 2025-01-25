//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "math.h"

// ������ ������ ����
typedef struct {
    float x, y, width, height, speed;
    HBITMAP hBitmap;
} racket_type;

racket_type racket;
enum class entity { empty, enemy, lootchest, terminal };

typedef struct {
    float x, y, width, height;
    HBITMAP hBitmap;
    entity type;
} enemycco;
const int enemycout = 22;
enemycco enemy1[enemycout];

typedef struct {
    float x, y, width, height;
    HBITMAP hBitmap;
} sprite;
const int splittedEnemyI = 3;
const int splittedEnemyj = 3;
sprite splittedEnemy[3][3];// �������� ��������

struct {
    float x;
    HBITMAP hBitmap;//����� � ������� ������� ����������
} enemy;

struct {
    float x, y, rad, dx, dy, speed;
    HBITMAP hBitmap;//����� � ������� ������
} ball;

struct {
    int score, balls;//���������� ��������� ����� � ���������� "������"
    bool action = false;//��������� - �������� (����� ������ ������ ������) ��� ����
} game;

struct {
    HWND hWnd;//����� ����
    HDC device_context, context;// ��� ��������� ���������� (��� �����������)
    int width, height;//���� �������� ������� ���� ������� ������� ���������
} window;


enum class GameMode { map, battle, loot, terminal };
GameMode game_mode = GameMode::battle;

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);

class  Mouse_ {
public:
    float x, y;
    bool L_butt, R_butt;

    void  Update() {
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(window.hWnd, &p);
        x = p.x;
        y = p.y;

        L_butt = GetAsyncKeyState(VK_LBUTTON);
        R_butt = GetAsyncKeyState(VK_RBUTTON);
    }


};

Mouse_ Mouse;




class button {
public:
    float x, y, width, height;
    HBITMAP hBitmap;//����� � ������� ������
    HBITMAP hBitmapGlow;

    bool CheckCollisionMouse()
    {
        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
    }

    void Load(const char* imagename, const char* imagenameglow, float x_, float y_, float w, float h) {
        x = x_; y = y_;
        hBitmap = (HBITMAP)LoadImageA(NULL, imagename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hBitmapGlow = (HBITMAP)LoadImageA(NULL, imagenameglow, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        height = w * window.height;
        width = h * window.width;
        x = window.width / 2 - width * x;
        y = window.height / 2 + height * y;
    }

    bool Show()
    {
        bool pw_collision = CheckCollisionMouse();
        float offset = (float)pw_collision * sinf(timeGetTime() * .01);
        ShowBitmap(window.context, x, y + offset, width, height, pw_collision ? hBitmapGlow : hBitmap);
        return pw_collision;
    }

};


class Bar {
public:
    float x, y, width, height, Health, Shield;
    HBITMAP hBitmapBack;
    HBITMAP hBitmapFront;
   

    void Load(const char* imagenameBack, const char* imagenameFront, float x_, float y_, float w, float h) {
        x = x_; y = y_;
        hBitmapBack = (HBITMAP)LoadImageA(NULL, imagenameBack, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        hBitmapFront = (HBITMAP)LoadImageA(NULL, imagenameFront, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        height = h * window.height;
        width = w * window.width;
        x = window.width / 2 - width * x;
        y = window.height / 2 + height * y;
        Health = 100;
     }
    void Show() {
        ShowBitmap(window.context, x, y, width, height, hBitmapBack);
        ShowBitmap(window.context, x, y, Health / 100. * width, height, hBitmapFront);
    }
    bool CheckCollisionMouse()
    {
        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
    }

   
};


Bar Health_bar, HealthEnemy_bar, Shield_bar, ShieldEnemy_bar;

button PW_butt, SW_butt, DW_butt, Enemy_butt, Exit_butt, Heal_butt;
HBITMAP hBack;// ����� ��� �������� �����������
HBITMAP hBattleBack;




//c����� ����





void InitGame()
{
    //� ���� ������ ��������� ������� � ������� ������� gdi
    //���� ������������� - ����� ������ ������ ����� � .exe 
    //��������� ������ LoadImageA ��������� � ������� ��������, ��������� �������� ����� ������������� � ������� ���� �������
    ball.hBitmap = (HBITMAP)LoadImageA(NULL, "shar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    racket.hBitmap = (HBITMAP)LoadImageA(NULL, "raketka.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    // ��������
    
    PW_butt.Load("pw_butt.bmp", "pw_butt_glow.bmp", 1.65, 4.13, .09, .09);
    SW_butt.Load("sw_butt.bmp", "sw_butt_glow.bmp", 0.5, 4.13, .09, .09);
    DW_butt.Load("dw_butt.bmp", "dw_butt_glow.bmp", -0.7, 4.13, .09, .09);
    Enemy_butt.Load("Enemy_butt.bmp", "Enemy_butt_glow.bmp", 0.43, -0.56, .60, .25);
    Exit_butt.Load("Exit_butt.bmp", "Exit_butt_glow.bmp", 12, -24, .02, .04);
    Heal_butt.Load("Heal_butt.bmp", "Heal_butt.bmp", -2.65, 5.12, .07, .07);
    Health_bar.Load("Health_bar_back.bmp", "Health_bar_front.bmp", 0.45, 33.7, .28, .01);

    //splittedEnemy[1][1].hBitmap = (HBITMAP)LoadImageA(NULL, "enemy1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    srand(0);


    int i = 0;
    float cellsize = 50;
    for (int x = 0; x < window.width / cellsize; x++) {
        for (int y = 0; y < window.height / cellsize; y++) {
            if (i >= enemycout) {
                continue;
            }
            entity Etype;
            auto rnd = (rand() % 100);
            if (rnd < 97) {
                Etype = entity::empty;
            }
            else {
                rnd = 100 - rnd;
                Etype = (entity)rnd;
            }
            if (Etype == entity::empty) {
                continue;
            }
            enemy1[i].type = Etype;
            if (enemy1[i].type == entity::enemy)
            {
                enemy1[i].hBitmap = (HBITMAP)LoadImageA(NULL, "enemycco.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            }
            if (enemy1[i].type == entity::lootchest)
            {
                enemy1[i].hBitmap = (HBITMAP)LoadImageA(NULL, "lootchest.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            }
            if (enemy1[i].type == entity::terminal)
            {
                enemy1[i].hBitmap = (HBITMAP)LoadImageA(NULL, "terminal.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
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


    enemy.hBitmap = (HBITMAP)LoadImageA(NULL, "raketka.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBack = (HBITMAP)LoadImageA(NULL, "phon1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    hBattleBack = (HBITMAP)LoadImageA(NULL, "Battlephon1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    //------------------------------------------------------

   // splittedEnemy[3][3].width = 150;
   // splittedEnemy[3][3].height = 100;

    racket.width = 50;
    racket.height = 50;
    racket.speed = 17;//�������� ����������� �������
    racket.x = window.width / 2.;//������� ���������� ����
    racket.y = window.height - racket.height;//���� ���� ���� ������ - �� ������ �������

    enemy.x = racket.x;//� ���������� �������� ������ � �� �� ����� ��� � ������

    ball.dy = (rand() % 65 + 35) / 100.;//��������� ������ ������ ������
    ball.dx = -(1 - ball.dy);//��������� ������ ������ ������
    ball.speed = 30;
    ball.rad = 40;
    ball.x = racket.x;//x ���������� ������ - �� ������� �������
    ball.y = racket.y - ball.rad;//����� ����� ������ �������

    game.score = 0;
    game.balls = 9;

}

void ProcessSound(const char* name)//������������ ���������� � ������� .wav, ���� ������ ������ � ��� �� ����� ��� � ���������
{
    PlaySound(TEXT(name), NULL, SND_FILENAME | SND_ASYNC);//���������� name �������� ��� �����. ���� ASYNC ��������� ����������� ���� ���������� � ����������� ���������
}

void ShowScore()
{
    return;
    //�������� �������� � �������
    SetTextColor(window.context, RGB(160, 160, 160));
    SetBkColor(window.context, RGB(0, 0, 0));
    auto hFont = CreateFont(70, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, "GEORGIA");
    auto hTmp = (HFONT)SelectObject(window.context, hFont);

    char txt[32];//����� ��� ������
    _itoa_s(game.score, txt, 10);//�������������� �������� ���������� � �����. ����� �������� � ���������� txt
    TextOutA(window.context, 10, 10, "Score", 5);
    TextOutA(window.context, 200, 10, (LPCSTR)txt, strlen(txt));

    _itoa_s(game.balls, txt, 10);
    TextOutA(window.context, 10, 100, "Balls", 5);
    TextOutA(window.context, 200, 100, (LPCSTR)txt, strlen(txt));
}

void ProcessInput()
{
    if (GetAsyncKeyState(VK_LEFT)) racket.x -= racket.speed;
    if (GetAsyncKeyState(VK_RIGHT)) racket.x += racket.speed;
    if (GetAsyncKeyState(VK_UP)) racket.y -= racket.speed;
    if (GetAsyncKeyState(VK_DOWN)) racket.y += racket.speed;

    if (!game.action && GetAsyncKeyState(VK_SPACE))
    {
        game.action = true;
        ProcessSound("knopka-voda-vyisokii-rezkii.wav");
    }
}

void ShowBitmap(HDC hDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha)
{
    HBITMAP hbm, hOldbm;
    HDC hMemDC;
    BITMAP bm;

    hMemDC = CreateCompatibleDC(hDC); // ������� �������� ������, ����������� � ���������� �����������
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
            StretchBlt(hDC, x, y, x1, y1, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY); // ������ ����������� bitmap
        }

        SelectObject(hMemDC, hOldbm);// ��������������� �������� ������
    }

    DeleteDC(hMemDC); // ������� �������� ������
}


void ShowLoot() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);//������ ���
    ShowBitmap(window.context, racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);
}

void ShowTerminal() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);//������ ���
    ShowBitmap(window.context, racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);
}







void ShowBattle() {
  
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBattleBack);//������ ���

    bool pw = PW_butt.Show();
    bool sw = SW_butt.Show();
    bool dw = DW_butt.Show();
    bool exit = Exit_butt.Show();
    bool enemy = Enemy_butt.Show();
    bool HealButt = Heal_butt.Show();
    Health_bar.Show();
 

    if (Mouse.L_butt)
    {
        if (pw)
        {

        }
    }
   
 }

void ShowRacketAndBall()
{
    ShowBitmap(window.context, 0, 0, window.width, window.height, hBack);//������ ���

    ShowBitmap(window.context, racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);// ������� ������
    for (int i = 0; i < enemycout; i++) {
        ShowBitmap(window.context, enemy1[i].x - enemy1[i].width / 2., enemy1[i].y, enemy1[i].width, enemy1[i].height, enemy1[i].hBitmap);
    }
    if (ball.dy < 0 && (enemy.x - racket.width / 4 > ball.x || ball.x > enemy.x + racket.width / 4))
    {
        enemy.x = ball.x * .1 + enemy.x * .9;
    }
    ShowBitmap(window.context, Mouse.x, Mouse.y, ball.rad, ball.rad, ball.hBitmap);
}

void LimitRacket()
{
    racket.x = max(racket.x, racket.width / 2.);//���� ��������� ������ ���� ������� ������ ����, �������� �� ����
    racket.x = min(racket.x, window.width - racket.width / 2.);//���������� ��� ������� ����
    racket.y = max(racket.y, 0.);
    racket.y = min(racket.y, window.height - racket.height);
}


void CheckWalls()
{
    if (ball.x < ball.rad || ball.x > window.width - ball.rad)
    {
        ball.dx *= -1;
        ProcessSound("knopka-voda-vyisokii-rezkii.wav");
    }
}

void CheckRoof()
{
    if (ball.y < ball.rad + racket.height)
    {
        ball.dy *= -1;
        ProcessSound("knopka-voda-vyisokii-rezkii.wav");
    }
}

bool tail = false;

bool CheckCollision(racket_type r, enemycco e)
{
    auto dx = r.x - e.x;
    auto dy = r.y - e.y;
    auto dxy = sqrt(dx * dx + dy * dy);

    if (dxy < (r.width + e.height) / 2.)
    {
        return true;
    }
    return false;
}

bool CheckCollisionMouse(enemycco e)
{
    auto dx = Mouse.x - e.x;
    auto dy = Mouse.y - e.y;
    auto dxy = sqrt(dx * dx + dy * dy);

    if (dxy < (dxy + e.height) / 2.)
    {
        return true;
    }
    return false;
}





// �������� �� ����� �������� ��
bool CheckCollisionMouseBar(Bar br)
{
    if (Mouse.x < br.x + br.width && Mouse.x > br.x && Mouse.x < br.y + br.height && Mouse.y > br.y)
    {
        return true;
    }
    return false;
}

void CheckFloor()
{
    if (ball.y > window.height - ball.rad - racket.height)//����� ������� ����� ������� - ����������� �������
    {
        if (!tail && ball.x >= racket.x - racket.width / 2. - ball.rad && ball.x <= racket.x + racket.width / 2. + ball.rad)//����� �����, � �� �� � ������ ��������� ������
        {
            game.score++;//�� ������ ������� ���� ���� ����
            ball.speed += 5. / game.score;//�� ����������� ��������� - ���������� �������� ������
            ball.dy *= -1;//������
            racket.width -= 10. / game.score;//������������� ��������� ������ ������� - ��� ���������
            ProcessSound("knopka-voda-vyisokii-rezkii.wav");//������ ���� �������
        }
        else
        {//����� �� �����

            tail = true;//����� ������ ������ ���� �������

            if (ball.y - ball.rad > window.height)//���� ����� ���� �� ������� ����
            {
                game.balls--;//��������� ���������� "������"

                ProcessSound("fail.wav");//������ ����

                if (game.balls < 0) { //�������� ������� ��������� "������"

                    MessageBoxA(window.hWnd, "game over", "", MB_OK);//������� ��������� � ���������
                    InitGame();//������������������ ����
                }

                ball.dy = (rand() % 65 + 35) / 100.;//������ ����� ��������� ������ ��� ������
                ball.dx = -(1 - ball.dy);
                ball.x = racket.x;//�������������� ���������� ������ - ������ ��� �� �������
                ball.y = racket.y - ball.rad;
                game.action = false;//���������������� ����, ���� ����� �� ������ ������
                tail = false;
            }
        }
    }
}





void ProcessRoom()
{
    //������������ �����, ������� � ���. ������� - ���� ������� ����� ���� ���������, � ������, ��� ������� �� ����� ������ ������������� ����� ������� �������� ������
    CheckWalls();
    CheckRoof();
    CheckFloor();

    for (int i = 0; i < enemycout; i++) {
        if (CheckCollision(racket, enemy1[i]) == true) {
            if (enemy1[i].type == entity::lootchest) {
                game_mode = GameMode::loot;
            }
            if (enemy1[i].type == entity::enemy) {
                game_mode = GameMode::battle;
            }

            if (enemy1[i].type == entity::terminal) {
                game_mode = GameMode::terminal;
            }
        }
    }
}



void InitWindow()
{
    SetProcessDPIAware();
    window.hWnd = CreateWindow("edit", 0, WS_POPUP | WS_VISIBLE | WS_MAXIMIZE, 0, 0, 0, 0, 0, 0, 0, 0);

    RECT r;
    GetClientRect(window.hWnd, &r);
    window.device_context = GetDC(window.hWnd);//�� ������ ���� ������� ����� ��������� ���������� ��� ���������
    window.width = r.right - r.left;//���������� ������� � ���������
    window.height = r.bottom - r.top;
    window.context = CreateCompatibleDC(window.device_context);//������ �����
    SelectObject(window.context, CreateCompatibleBitmap(window.device_context, window.width, window.height));//����������� ���� � ���������
    GetClientRect(window.hWnd, &r);

}
void BattleGame() {

    ShowBattle();//������ ��� 
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(20);//���� 16 ���������� (1/���������� ������ � �������)
    if (Mouse.L_butt)
    {
        if (PW_butt.CheckCollisionMouse()) {

            game_mode = GameMode::map;

        }
        if (SW_butt.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }

        if (DW_butt.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
        if (Exit_butt.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }

    }
    ProcessInput();//����� ����������
    LimitRacket();



}

void TerminalGame() {
    ShowTerminal();
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
    Sleep(20);
    ProcessInput();
}

void LootGame() {
    ShowLoot();
    ShowBitmap(window.context, racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(20);//���� 16 ���������� (1/���������� ������ � �������)
    ProcessInput();//����� ����������

}


void MapGame() {
    ShowRacketAndBall();//������ ���, ������� � �����
    ShowScore();
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(16);//���� 16 ���������� (1/���������� ������ � �������)
    for (int i = 0; i < enemycout; i++) {
        if (Mouse.L_butt) {
            if (CheckCollisionMouse(enemy1[i]))
            {
                if (enemy1[i].type == entity::lootchest) {
                    game_mode = GameMode::loot;
                }
                if (enemy1[i].type == entity::enemy) {
                    game_mode = GameMode::battle;
                }

                if (enemy1[i].type == entity::terminal) {
                    game_mode = GameMode::terminal;
                }
            }
        }
    }
    ProcessInput();//����� ����������
    LimitRacket();//���������, ����� ������� �� ������� �� �����
    // ProcessBall();//���������� �����
    ProcessRoom();//������������ ������� �� ���� � �������, ��������� ������ � ��������
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    InitWindow();//����� �������������� ��� ��� ����� ��� ��������� � ����
    InitGame();//����� �������������� ���������� ����

    while (!GetAsyncKeyState(VK_ESCAPE))
    {

      
        Mouse.Update();
        
        switch (game_mode) {
        case GameMode::map: MapGame(); break;
        case GameMode::battle: BattleGame(); break;
        case GameMode::loot: LootGame(); break;
        case GameMode::terminal: TerminalGame(); break;
        }


    }

}
