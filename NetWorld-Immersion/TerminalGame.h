
void InitTerminal() {
    Terminalphon1_bmp = LoadBMP("Terminalphon1.bmp");
    Exit.Load("Exit_butt.bmp", "Exit_butt_glow.bmp", 12, -16, .04, .03);
}

void ShowTerminal() {
    ShowBitmap(0, 0, window.width, window.height, TerminalhBack);//задний фон
    bool exit = Exit.Show();
}

void TerminalGame() { //TODO
    InitTerminal();
    ShowTerminal();
    BitBlt(window.device_context, 0, 0, window.width, window.height, window.context, 0, 0, SRCCOPY);
    Sleep(16);
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }
}