
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
    void ShowHealth(int Health) { //TODO как будто бы можно сделать менее вырвиглазно в самой игре, да и код€ру оптимизировать
        ShowBitmap(x, y, width, height, hBitmapBack);
        ShowBitmap(x, y, Health / (float)player.HealthMax * width, height, hBitmapFront);
    }
    void ShowShield(int Shield) {
        ShowBitmap(x, y, width, height, hBitmapBack);
        ShowBitmap(x, y, Shield / (float)player.ShieldMax * width, height, hBitmapFront);
    }
    bool CheckCollisionMouse() {
        return Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y;
    }
};
Bar Health_bar, HealthEnemy_bar, Shield_bar, ShieldEnemy_bar; //TODO ¬џ–¬»√Ћјјјј«