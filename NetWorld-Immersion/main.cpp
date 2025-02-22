//linker::system::subsystem  - Windows(/ SUBSYSTEM:WINDOWS)
//configuration::advanced::character set - not set
//linker::input::additional dependensies Msimg32.lib; Winmm.lib

#include "windows.h"
#include "math.h"

HBITMAP LoadBMP(const char* name)
{
    return (HBITMAP)LoadImageA(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

int clamp(int x, int minX, int maxX)
{
    return max(min(maxX, x), minX);
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

int random = 1;


DWORD healStartTime = 0;
DWORD healTime = 2000;
DWORD currentTime = 0;


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

void ShowBitmap(int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha = false);

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

struct player_ {
    int Health = 46000;
    int HealthMin = 0;
    int HealthMax = 50000;
    int Shield = 46000;
    int ShieldMin = 0;
    int ShieldMax = 50000;
    int Shield_bonus = 2; // rework shield_bonus system

    void adjustHealth(int v) {
        srand(random);
        random = rand() % v;
        Health = Health - random;
        Health = clamp(Health, HealthMin, HealthMax);
    }
    void adjustShield(int s) {
        srand(random);
        random = (rand() % s) * 2;
        Shield = Shield - random;
        Shield = clamp(Shield, ShieldMin, ShieldMax);
    }
    void adjustHeal() {
        Health += 1500;
        Health = clamp(Health, HealthMin, HealthMax);
        Shield = (Shield + 1500) * Shield_bonus;
        Shield = clamp(Shield, ShieldMin, ShieldMax);
        healStartTime = currentTime;
    }

};

player_ player;

struct Enemy_ {
    int HealthEnemy = 40000;
    int HealthEnemyMin = 0;
    int HealthEnemyMax = 50000;
    int ShieldEnemy = 40000;
    int ShieldEnemyMin = 0;
    int ShieldEnemyMax = 50000;
    int ShieldEnemy_bonus = 20; // rework shield_bonus system

    void adjustHealth(int v) {
        srand(random);
        int random = rand() % v;
        HealthEnemy = HealthEnemy - random;
        HealthEnemy = clamp(HealthEnemy, HealthEnemyMin, HealthEnemyMax);
    }
    void adjustShield(int s) {
        srand(random);
        int random = (rand() % s) * 2;
        ShieldEnemy = ShieldEnemy - random;
        ShieldEnemy = clamp(ShieldEnemy, ShieldEnemyMin, ShieldEnemyMax);
    }
   
};

Enemy_ Enemy;


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


Bar Health_bar, HealthEnemy_bar, Shield_bar, ShieldEnemy_bar; //TODO ������������

button PrimWeapon, SpecWeapon, DestructiveWeapon, EnemyB, Exit, Heal_butt,  Inventory_butt, Boots__inventory_butt;
HBITMAP hBack;// ����� ��� �������� �����������
HBITMAP hBattleBack;
HBITMAP InventoryhBack;
HBITMAP TerminalhBack;

void InitGame() //TODO
{
    //� ���� ������ ��������� ������� � ������� ������� gdi
    //���� ������������� - ����� ������ ������ ����� � .exe 
    //��������� ������ LoadImageA ��������� � ������� ��������, ��������� �������� ����� ������������� � ������� ���� �������
    racket.Load("raketka.bmp");

    //DW_1     
    PrimWeapon.Load("pw_butt.bmp", "pw_butt_glow.bmp", 1.65, 4.13, .09, .09);
    SpecWeapon.Load("sw_butt.bmp", "sw_butt_glow.bmp", 0.5, 4.13, .09, .09);
    DestructiveWeapon.Load("dw_butt.bmp", "dw_butt_glow.bmp", -0.7, 4.13, .09, .09);
    EnemyB.Load("Enemy_butt.bmp", "Enemy_butt_glow.bmp", 0.43, -0.56, .25, .60 );
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

    hBack = LoadBMP("phon1.bmp");
    hBattleBack = LoadBMP("Battlephon1.bmp");
    InventoryhBack = LoadBMP("Inventoryphon1.bmp");
    TerminalhBack = LoadBMP("Terminalphon1.bmp");

    racket.width = 50;
    racket.height = 50;
    racket.speed = 17;//�������� ����������� �������
    racket.x = window.width / 2.;//������� ���������� ����
    racket.y = window.height - racket.height;//���� ���� ���� ������ - �� ������ �������       //TODO//TODO//TODO ��������� �� ���������, ������ �� ���� ��� ����������

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

void ShowBitmap(int x, int y, int x1, int y1, HBITMAP hBitmapBall, bool alpha) //TODO
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
    ShowBitmap(0, 0, window.width / 2., window.height / 2., InventoryhBack);//������ ���
    bool BootsInventory = Boots__inventory_butt.ShowInv();
}

void ShowLoot() {
    ShowBitmap( 0, 0, window.width, window.height, InventoryhBack);//������ ���
    bool exit = Exit.Show();
    bool BootsInventory = Boots__inventory_butt.Show();
    ShowBitmap(racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);
}

void ShowTerminal() {
    ShowBitmap(0, 0, window.width, window.height, TerminalhBack);//������ ���
    bool exit = Exit.Show();
    ShowBitmap(racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);
}


void ShowBattle() {
  
    ShowBitmap(0, 0, window.width, window.height, hBattleBack);//������ ���

    bool pw = PrimWeapon.Show();
    bool sw = SpecWeapon.Show();
    bool dw = DestructiveWeapon.Show();
    bool exit = Exit.Show();
    bool enemy = EnemyB.Show();
    bool HealButt = Heal_butt.Show();
    bool InventoryButt = Inventory_butt.Show();

    Health_bar.ShowHealth(player.Health);
    Shield_bar.ShowShield(player.Shield);
    ShieldEnemy_bar.ShowShield(Enemy.ShieldEnemy);
    HealthEnemy_bar.ShowHealth(Enemy.HealthEnemy);


 }

void ShowRacketAndBall() //TODO
{
    ShowBitmap(0, 0, window.width, window.height, hBack);//������ ���

    ShowBitmap(racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);// ������� ������
    
    for (int i = 0; i < enemycout; i++) {
        ShowBitmap(enemy1[i].x - enemy1[i].width / 2., enemy1[i].y, enemy1[i].width, enemy1[i].height, enemy1[i].hBitmap);
    }
    
    ShowBitmap(Mouse.x, Mouse.y, 1,1, raketka_bmp);
}


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

void BattleGame() {//TODO ??????

    ShowBattle();//������ ��� 
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(20);//���� 16 ���������� (1/���������� ������ � �������)
    if (Mouse.L_butt)
    {
        if (PrimWeapon.CheckCollisionMouse()) {
            if (AttackcurrentTime > AttackStartTime + AttackTime) {
             
                player.adjustHealth(4000);
                player.adjustShield(6500);
                Enemy.adjustHealth(5000);
                Enemy.adjustShield(6500);
                AttackStartTime = currentTime;
            }                       
        }
        if (SpecWeapon.CheckCollisionMouse()) {
            if (AttackcurrentTime > AttackStartTime + AttackTime) {
                player.adjustHealth(7500);
                player.adjustShield(9000);
                Enemy.adjustHealth(8000);
                Enemy.adjustShield(12000);
                AttackStartTime = currentTime;
            }
        }

        if (DestructiveWeapon.CheckCollisionMouse()) {
            if (AttackcurrentTime > AttackStartTime + AttackTime)  {
                player.adjustHealth(10000);
                player.adjustShield(15000);
                Enemy.adjustHealth(17000);
                Enemy.adjustShield(23000);
                AttackStartTime = currentTime;
            }
        }
        if (Heal_butt.CheckCollisionMouseHeal()) { //TO DOO/QUESTION?
           
        }
        if (Inventory_butt.CheckCollisionMouse()) {
            ShowInventory();
        }

        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }

    }
    ProcessInput();//����� ���������� //TODO ���� � �� ������ ������, �� �� �������� � ������ ������
}

void TerminalGame() { //TODO
    ShowTerminal();
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
    Sleep(20);
    ProcessInput();
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }
}

void LootGame() {
    ShowLoot();
    
    ShowBitmap(racket.x - racket.width / 2., racket.y, racket.width, racket.height, racket.hBitmap);
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);//�������� ����� � ����
    Sleep(20);//���� 16 ���������� (1/���������� ������ � �������)
    ProcessInput();//����� ����������
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
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