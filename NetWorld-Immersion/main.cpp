//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "math.h"

HBITMAP LoadBMP(const char* name)
{
    return (HBITMAP)LoadImageA(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

// ������ ������ ����
struct racket_type {
    float x, y, width, height, speed;
    HBITMAP hBitmap;

    void Load(const char* name)//TODO
    {
        hBitmap = LoadBMP(name);
    }

};

racket_type racket;
enum class entity { empty, enemy, lootchest, terminal };
//TODO
HBITMAP enemycco_bmp;
HBITMAP lootchest_bmp;
HBITMAP terminal_bmp;
HBITMAP raketka_bmp;
HBITMAP phon1_bmp;
HBITMAP Battlephon1_bmp;
HBITMAP Inventoryphon1_bmp;
HBITMAP Terminalphon1_bmp;

struct enemycco {
    float x, y, width, height;
    HBITMAP hBitmap;
    entity type;

    void Load(const char* name)//TODO                                                                                                                         //TODO//TODO//TODO//TODO//TODO ����� ��� ����� �� ����� ��������� �� ������
    {
        hBitmap = LoadBMP(name);
    }

};
const int enemycout = 22;
enemycco enemy1[enemycout];

struct sprite {
    float x, y, width, height, dx,dy, rad,speed;
    HBITMAP hBitmap;


    void Load(const char* name)//TODO
    {
        hBitmap = LoadBMP(name);
    }

};

const int splittedEnemyI = 3;
const int splittedEnemyj = 3;
sprite splittedEnemy[3][3];// �������� ��������

sprite enemy;
sprite ball;//TODO

struct { //TODO
    int score, balls;//���������� ��������� ����� � ���������� "������"
    bool action = false;//��������� - �������� (����� ������ ������ ������) ��� ����
} game;

struct {
    HWND hWnd;//����� ����
    HDC device_context, context;// ��� ��������� ���������� (��� �����������)
    int width, height;//���� �������� ������� ���� ������� ������� ���������
} window;


enum class GameMode { map, battle, loot, terminal };
GameMode game_mode = GameMode::map;

void ShowBitmap(HDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);

class  Mouse_ { //TODO
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
//TODO
Mouse_ Mouse;
int Health = 6000;
int HealthMin = 0;
int HealthMax = 50000;
int Shield = 6000;
int ShieldMin = 0;
int ShieldMax = 50000;
int Shield_bonus = 2; // rework shield_bonus system

int HealthEnemy = 40000;
int HealthEnemyMin = 0;
int HealthEnemyMax = 50000;
int ShieldEnemy = 40000;
int ShieldEnemyMin = 0;
int ShieldEnemyMax = 50000;
int ShieldEnemy_bonus = 20; // rework shield_bonus system

int clamp(int x, int minX, int maxX)
{
    return max(min(maxX, x), minX);
}
DWORD healStartTime = 0;
DWORD healTime = 2000;
DWORD currentTime = 0;


DWORD AttackStartTime = 0;
DWORD AttackTime = 1000;
DWORD AttackcurrentTime = 0;



//TODO ������ ����������� ��������� ��������, ���� ������� ��� ����� ����������� � ����� ���������������� � ������ ���� � ����������� ������ �����, ��� ����� �� �� ������ ��������� ��� �������� ����� ���������� ����� ��� ������ ������������
class button {
public:

    float x, y, width, height;
    HBITMAP hBitmap;//����� � ������� ������
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

        ShowBitmap(window.context, x, y + offset, width, height, pw_collision ? hBitmapGlow : hBitmap);
        return pw_collision;
    }
    bool ShowInv() {
        bool pw_collision = CheckCollisionMouse();
        ShowBitmap(window.context, x, y, width, height, hBitmap);
        return pw_collision;
    }

    bool CheckCollisionMouseHeal() {
        if (Mouse.L_butt) {
            if (Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y)
            {
                if (currentTime > healStartTime + healTime)
                {
                    Health = Health + 1500;
                    Health = clamp(Health, HealthMin, HealthMax);
                    Shield = (Shield + 1500) * Shield_bonus;
                    Shield = clamp(Shield, ShieldMin, ShieldMax);
                    healStartTime = currentTime;

                    return true;
                }

            }
        }
        return false;
    }
};

class Bar { //TODO
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

   
    void ShowHealth(int Health) { //TODO ��� ����� �� ����� ������� ����� ����������� � ����� ����, �� � ������ ��������������
        ShowBitmap(window.context, x, y, width, height, hBitmapBack);
        ShowBitmap(window.context, x, y, Health / (float)HealthMax * width, height, hBitmapFront);
    }
    void ShowShield(int Shield) {
        ShowBitmap(window.context, x, y, width, height, hBitmapBack);
        ShowBitmap(window.context, x, y, Shield / (float)ShieldMax * width, height, hBitmapFront);
    }
    bool CheckCollisionMouse()
    {
        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
    }

   
};

/*class  DW { //TODO �� ����� �� ������� ������� ���� � ����� ��� ���������� ������� ������?
public:
    float x, y, width, height, Damage, Crit_Chance, Crit_Damge;
   
    void  Nay() {
       
    }


};
*/


Bar Health_bar, HealthEnemy_bar, Shield_bar, ShieldEnemy_bar; //TODO ������������

button PW_butt, SW_butt, DW_butt, Enemy_butt, Exit_butt, Heal_butt,  Inventory_butt, BackPack__inventory_butt, Boots__inventory_butt;
HBITMAP hBack;// ����� ��� �������� �����������
HBITMAP hBattleBack;
HBITMAP InventoryhBack;
HBITMAP TerminalhBack;
//TODO

//c����� ����




void InitGame() //TODO
{
    //� ���� ������ ��������� ������� � ������� ������� gdi
    //���� ������������� - ����� ������ ������ ����� � .exe 
    //��������� ������ LoadImageA ��������� � ������� ��������, ��������� �������� ����� ������������� � ������� ���� �������
    ball.Load("shar.bmp");
    racket.Load("raketka.bmp");

    //DW_1     
    PW_butt.Load("pw_butt.bmp", "pw_butt_glow.bmp", 1.65, 4.13, .09, .09);
    SW_butt.Load("sw_butt.bmp", "sw_butt_glow.bmp", 0.5, 4.13, .09, .09);
    DW_butt.Load("dw_butt.bmp", "dw_butt_glow.bmp", -0.7, 4.13, .09, .09);
    Enemy_butt.Load("Enemy_butt.bmp", "Enemy_butt_glow.bmp", 0.43, -0.56, .25, .60 );
    Exit_butt.Load("Exit_butt.bmp", "Exit_butt_glow.bmp", 12, -16, .04, .03);
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


    int i = 0;//TODO ����� ���� ��������� �� ��������� � ����� ������� � ������� ��� � ���������?
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

            
            switch (enemy1[i].type) // � ���� ��������� �� ��������� ������� ��������� � ������� ���� //TODO
            {
            case entity::enemy:
                enemy1[i].hBitmap = enemycco_bmp;
                break;
            case entity::lootchest:
                enemy1[i].hBitmap = lootchest_bmp;
                break;
            case entity::terminal:
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

    enemy.Load("raketka.bmp");
    hBack = LoadBMP("phon1.bmp");
    hBattleBack = LoadBMP("Battlephon1.bmp");
    InventoryhBack = LoadBMP("Inventoryphon1.bmp");
    TerminalhBack = LoadBMP("Terminalphon1.bmp");

    racket.width = 50;
    racket.height = 50;
    racket.speed = 17;//�������� ����������� �������
    racket.x = window.width / 2.;//������� ���������� ����
    racket.y = window.height - racket.height;//���� ���� ���� ������ - �� ������ �������       //TODO//TODO//TODO ��������� �� ���������, ������ �� ���� ��� ����������

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

void ShowScore() //TODO
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

void ProcessInput() //TODO
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

void ShowBitmap(HDC, int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha) //TODO
{
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




void ShowInventory() {
    ShowBitmap(window.context, 0, 0, window.width / 2., window.height / 2., InventoryhBack);//������ ���
    bool BootsInventory = Boots__inventory_butt.ShowInv();
}

void ShowLoot() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, InventoryhBack);//������ ���
    bool exit = Exit_butt.Show();
    bool BootsInventory = Boots__inventory_butt.Show();
    ShowBitmap(window.context, racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);
}

void ShowTerminal() {
    ShowBitmap(window.context, 0, 0, window.width, window.height, TerminalhBack);//������ ���
    bool exit = Exit_butt.Show();
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
    bool InventoryButt = Inventory_butt.Show();

    Health_bar.ShowHealth(Health);
    Shield_bar.ShowShield(Shield);
    ShieldEnemy_bar.ShowShield(ShieldEnemy);
    HealthEnemy_bar.ShowHealth(HealthEnemy);


 }

void ShowRacketAndBall() //TODO
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

void LimitRacket() //TODO
{
    racket.x = max(racket.x, racket.width / 2.);//���� ��������� ������ ���� ������� ������ ����, �������� �� ����
    racket.x = min(racket.x, window.width - racket.width / 2.);//���������� ��� ������� ����
    racket.y = max(racket.y, 0.);
    racket.y = min(racket.y, window.height - racket.height);
}


void CheckWalls() //TODO
{
    if (ball.x < ball.rad || ball.x > window.width - ball.rad)
    {
        ball.dx *= -1;
        ProcessSound("knopka-voda-vyisokii-rezkii.wav");
    }
}

void CheckRoof() //TODO
{
    if (ball.y < ball.rad + racket.height)
    {
        ball.dy *= -1;
        ProcessSound("knopka-voda-vyisokii-rezkii.wav");
    }
}

bool tail = false; //TODO

bool CheckCollision(racket_type r, enemycco e) //TODO
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

bool CheckCollisionMouse(enemycco e) //TODO
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



void ProcessRoom() //TODO
{
    //������������ �����, ������� � ���. ������� - ���� ������� ����� ���� ���������, � ������, ��� ������� �� ����� ������ ������������� ����� ������� �������� ������
    CheckWalls();
    CheckRoof();

    for (int i = 0; i < enemycout; i++) {  // � ���� ��������� �� ���������
        if (CheckCollision(racket, enemy1[i]) == true) 
        {
            switch (enemy1[i].type) {
                case entity::lootchest:
                    game_mode = GameMode::loot;
                     break;
                case entity::enemy:
                    game_mode = GameMode::battle;
                        break;
                case entity::terminal:
                    game_mode = GameMode::terminal;
                        break;
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
double random = 1; 
void BattleGame() {//TODO ??????

    ShowBattle();//������ ��� 
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(20);//���� 16 ���������� (1/���������� ������ � �������)
    if (Mouse.L_butt)
    {
        if (PW_butt.CheckCollisionMouse()) {
            if (AttackcurrentTime > AttackStartTime + AttackTime) {
                //game_mode = GameMode::map;
                srand(random);
                random = rand() % 500;
                Health = Health - random;
                Health = clamp(Health, HealthMin, HealthMax);
                random = (rand() % 600) * 2;
                Shield = Shield - random;
                Shield = clamp(Shield, ShieldMin, ShieldMax);

                random = (rand() % 4000) * 2;
                HealthEnemy = HealthEnemy - random;
                HealthEnemy = clamp(HealthEnemy, HealthEnemyMin, HealthEnemyMax);
                random = (rand() % 6000) * 2;
                ShieldEnemy = ShieldEnemy - random;
                ShieldEnemy = clamp(ShieldEnemy, ShieldEnemyMin, ShieldEnemyMax);
                AttackStartTime = currentTime;
            }                       
        }
        if (SW_butt.CheckCollisionMouse()) {
            if (AttackcurrentTime > AttackStartTime + AttackTime) {
                //game_mode = GameMode::map;
                srand(random);
                random = rand() % 700;
                Health = Health - random;
                Health = clamp(Health, HealthMin, HealthMax);
                random = (rand() % 1500) * 2;
                Shield = Shield - random;
                Shield = clamp(Shield, ShieldMin, ShieldMax);

                random = (rand() % 8000) * 2;
                HealthEnemy = HealthEnemy - random;
                HealthEnemy = clamp(HealthEnemy, HealthEnemyMin, HealthEnemyMax);
                random = (rand() % 12000) * 2;
                ShieldEnemy = ShieldEnemy - random;
                ShieldEnemy = clamp(ShieldEnemy, ShieldEnemyMin, ShieldEnemyMax);
                AttackStartTime = currentTime;
            }
        }

        if (DW_butt.CheckCollisionMouse()) {
            if (AttackcurrentTime > AttackStartTime + AttackTime)  {
                //game_mode = GameMode::map;
                srand(random);
                random = rand() % 1500;
                Health = Health - random;
                Health = clamp(Health, HealthMin, HealthMax);
                random = (rand() % 3000) * 2;
                Shield = Shield - random;
                Shield = clamp(Shield, ShieldMin, ShieldMax);

                random = (rand() % 20000) * 2;
                HealthEnemy = HealthEnemy - random;
                HealthEnemy = clamp(HealthEnemy, HealthEnemyMin, HealthEnemyMax);
                random = (rand() % 10000) * 2;
                ShieldEnemy = ShieldEnemy - random;
                ShieldEnemy = clamp(ShieldEnemy, ShieldEnemyMin, ShieldEnemyMax);

                AttackStartTime = currentTime;
            }
        }
        if (Heal_butt.CheckCollisionMouseHeal()) { //TO DOO/QUESTION?
           
        }
        if (Inventory_butt.CheckCollisionMouse()) {
            ShowInventory();
        }

        if (Exit_butt.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }

    }
    ProcessInput();//����� ���������� //TODO ���� � �� ������ ������, �� �� �������� � ������ ������
    LimitRacket();//TODO

}

void TerminalGame() { //TODO
    ShowTerminal();
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
    Sleep(20);
    ProcessInput();
    if (Mouse.L_butt) {
        if (Exit_butt.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }
}

void LootGame() {
    ShowLoot();
    
    ShowBitmap(window.context, racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(20);//���� 16 ���������� (1/���������� ������ � �������)
    ProcessInput();//����� ����������
    if (Mouse.L_butt) {
        if (Exit_butt.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }//TODO
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
                switch (enemy1[i].type) {
                    case entity::lootchest:
                        game_mode = GameMode::loot;
                        break;
                    case entity::enemy:
                        game_mode = GameMode::battle;
                        break;
                    case entity::terminal:
                        game_mode = GameMode::terminal;
                        break;
                }
            }
        }
    } //TODO
    ProcessInput();//����� ����������//TODO
    LimitRacket();//���������, ����� ������� �� ������� �� �����//TODO
    // ProcessBall();//���������� ����� //TODO
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
//����� � ���� ��������� ���� ���� ��������,������� �������������� � ���������, ����� ������� �� chest'� ���� ��������, ����� ������� �� terminal'�? ���� ��������.
// ������ ��� ����������� �� ����� ����, ������ ��� �� ������ ���������, ������������ �� �� �� ��� ���� ����, ��� �� �� ���� ���������� �� ����-�����/���������. �� �������� ��� ������ � ��� � X86 �����������, ���� � �� ������� �� X64, ������ 32-� �����)))).
//TODO - ����� ����� ���, ����+� � �������� �����