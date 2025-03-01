
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
            if (Mouse.x < x + width && Mouse.x > x && Mouse.y < y + height && Mouse.y > y)  {
                if (currentTime > healStartTime + healTime)   { //todo
                    player.adjustHeal();
                    return true;
                }
            }
        }
        return false;
    }
};

Button PrimWeapon, SpecWeapon, DestructiveWeapon, EnemyB, Exit, Heal_butt, Inventory_butt, Boots__inventory_butt;