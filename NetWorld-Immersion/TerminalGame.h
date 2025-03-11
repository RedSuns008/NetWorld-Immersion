
bool InitTerminalFlag = false;

void InitTerminal() {
    if (!InitTerminalFlag) {
        Terminalphon1_bmp = LoadBMP("Terminalphon1.bmp");
        Exit.Load("Exit_butt.bmp", "Exit_butt_glow.bmp", 12, -16, .04, .03);
        InitTerminalFlag = true;
    }
}
 
void ShowTerminal() { 
    ShowBitmap(0, 0, window.width, window.height, TerminalhBack);//задний фон
    bool exit = Exit.Show();
}

void TerminalGame() { //TODO
    InitTerminal();
    ShowTerminal();
    if (Mouse.L_butt) {
        if (Exit.CheckCollisionMouse()) {
            game_mode = GameMode::map;
        }
    }
}