class Mouse_ {
public:
    float x, y;
    bool L_butt, R_butt;

    void Update() { //done
        POINT p;
        GetCursorPos(&p);
        ScreenToClient(window.hWnd, &p);
        x = static_cast<float>(p.x);
        y = static_cast<float>(p.y);
        L_butt = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
        R_butt = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
    }
};
Mouse_ Mouse;