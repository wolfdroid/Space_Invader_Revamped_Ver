#include "raylib.h"

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define NUM_SHOOTS 1000
#define NUM_MAX_ENEMIES 1000
#define FIRST_WAVE 10
#define SECOND_WAVE 25
#define THIRD_WAVE 50
#define FOURTH_WAVE 75
#define FINAL_WAVE 10
#define BONUS_WAVE 100

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum { FIRST = 0, SECOND, THIRD, FOURTH, FINAL, BONUS } EnemyWave;

typedef struct Player{
    Rectangle rec;
    Vector2 speed;
    int life;
    bool awakening;
    Texture tex;
    Texture awake;
    Texture bg;
    Sound ready;
    Sound woken;
    Sound hit;
    Sound die;
    Music bgm;
} Player;

typedef struct Enemy{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Texture tex;
    Sound die;
} Enemy;

typedef struct Boss{
    Rectangle rec;
    Vector2 speed;
    bool active;
    int life;
    Texture tex;
    Sound warning;
    Sound hit;
    Sound die;
} Boss;

typedef struct Shoot{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
    Sound blast;
} Shoot;

typedef struct Blast{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Color color;
    Texture text;
    Sound blast;
} Blast;

typedef struct Power{
    Rectangle rec;
    Vector2 speed;
    bool active;
    Texture tex;
    Color color;
} Power;

//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

static bool gameOver;
static bool pause;
static int score;
static bool victory;

static Player player;
static Enemy enemy[NUM_MAX_ENEMIES];
static Shoot shoot[NUM_SHOOTS];
static Blast blast;
static Boss boss;
static Power power;
static EnemyWave wave;

static Texture2D enemyTex;

static int shootRate;
static float alpha;

static int activeEnemies;
static int enemiesKill;
static bool smooth;

int screenWidth = 350;
int screenHeight = 500;

//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
static void InitGame(void);         // Initialize game
static void UpdateGame(void);       // Update game (one frame)
static void DrawGame(void);         // Draw game (one frame)
static void UnloadGame(void);       // Unload game
static void UpdateDrawFrame(void);  // Update and Draw (one frame)

int main()
{
    // Menu Initialization
    //-------------------------------------------------------------------------------------
    int framesCounter = 0;                                                                 //Variable Penghitung Frames
    InitWindow(screenWidth, screenHeight, "SPACE INVADER");                                //Judul
    int framecounter = 0;                                                                  //Variable Penghitung Frames untuk Animasi Text
    InitAudioDevice();                                                                     //Inisialisasi Audio
    Image WI = LoadImage("textures/WI.png");                                                //Inisialisasi Windows Image
    SetWindowIcon(WI);                                                                     // Set icon for window (only PLATFORM_DESKTOP)
    
    //Buttons Variables
    bool buttonBackb = false;
    bool buttonHelpx = false;
    bool buttonDer_over = false;                                                                            //Inisialisasi Button Keluar Permainan
    bool buttonIzq_over = false;                                                                            //Inisialisasi Button Bermain
    Vector2 buttonLeft = {(screenWidth/4)-(screenWidth/10),(screenHeight*0.75)-(screenHeight*0.0625)};      //Inisialisasi peletakkan Button yang berada di Kiri
    Vector2 buttonRight = {(screenWidth/2),(screenHeight*0.75)-(screenHeight*0.0625)};                      //Inisialisasi peletakkan Button yang berada di Kanan
    Vector2 buttonHelp = {0,0};                                                                             //Inisilaisasi peletakkan Button Help
    Vector2 buttonBack = {0,390};
    
    //Mouse Variables
    Vector2 mousePosition = {0,0};                                                         //Inisialisasi Posisi Mouse Awal
    
    //Screen Variables
    typedef enum {LOGO,TRIBUTE,TITLE,HELP,GAMING} GameScreen;                              //Membuat Definisi 
    GameScreen gameScreen = LOGO;                                                          //Men-set screen awal = screen LOGO
    int state = 0;                                                                         //Variable untuk Black Screen
    float alpha = 0.0;                                                                     //Variable Switch dari Black Screen ke Texture
    bool animation = false;                                                                //Variable Penggantian Image
    bool quitGame = false;                                                                 //Inisialisasi Fungsi Quit Game 
    
    //Logo Enveriesage
    Texture2D BL = LoadTexture("textures/1.png");                                          //Inisialisasi Background Logo
    Texture2D BS = LoadTexture("textures/BS.png");                                         //Inisialisasi Black Screen
    const char desc[25] = "Decipher your Envisage";                                        //Animasi Text
    Sound LS = LoadSound("sound/E.ogg");                                                   //Inisialisasi suara Logo
    
    //Logo Raylib
    Texture2D RL = LoadTexture("textures/raylib_logo.png");                                //Inisialisasi Logo Raylib
    
    //Title
    Texture2D BT1 = LoadTexture("textures/t1.png");                                        //Inisialisasi Gambar Title 1
    Texture2D BT2 = LoadTexture("textures/t2.png");                                        //Inisialisasi Gambar Title 2
    Texture2D BHh = LoadTexture("textures/BG.png");
    Sound TS = LoadSound("sound/sountrack.ogg");                                           //Inisialisasi Backsound pada Title
    Texture2D BP = LoadTexture("textures/p.png");                                          //Inisialisasi Play Button
    Texture2D BE = LoadTexture("textures/E.png");                                          //Inisialisasi Exit Button
    Texture2D BH = LoadTexture("textures/H.png");                                          //Inisialisasi Help Button
    Texture2D BI = LoadTexture("textures/BI.png");                                         //Inisialisasi Boss Button
    Texture2D EI = LoadTexture("textures/EI.png");                                         //Inisialisasi Enemy Button
    Texture2D BB = LoadTexture("textures/BB.png");                                         //Inisialisasi Back Button
    
    // Initialize Game Resources
    //-------------------------------------------------------------------------------------
    // Load Images
    Image bg = LoadImage("textures/bg.png");
    ImageResize(&bg, 350, 500);
    Image imagePlayer = LoadImage("textures/player.png");
    ImageResize(&imagePlayer, 20, 20);
    Image imageAwake = LoadImage("textures/player_awake.png");
    ImageResize(&imageAwake, 20, 20);
    Image imageBoss = LoadImage("textures/boss.png");
    ImageResize(&imageBoss, 100, 48);
    Image imageEnemy_red = LoadImage("textures/enemy_red.png");
    ImageResize(&imageEnemy_red, 35, 25); 
    Image b_i = LoadImage("textures/Laser.png");
    ImageResize(&b_i,100,300);
    Image p_i = LoadImage("textures/PUI.png");
    ImageResize(&p_i,50,50);

    
    // Initialize Main Textures 
    Texture2D bgtex = LoadTextureFromImage(bg);
    Texture2D playerTexture = LoadTextureFromImage(imagePlayer);
    Texture2D playerAwake = LoadTextureFromImage(imageAwake);
    Texture2D bossTexture = LoadTextureFromImage(imageBoss);
    Texture2D powerUp = LoadTextureFromImage(p_i);
    Texture2D blastImage = LoadTextureFromImage(b_i);
    enemyTex = LoadTextureFromImage(imageEnemy_red);

    // Unload unused images
    UnloadImage(bg);
    UnloadImage(imagePlayer);
    UnloadImage(imageAwake);
    UnloadImage(imageBoss);
    UnloadImage(imageEnemy_red);
    UnloadImage(b_i);
    UnloadImage(p_i);
    
    // Initialize Sounds
    Music bgm = LoadMusicStream("sounds/bgm.ogg");
    Sound playerReady = LoadSound("sounds/Ready.ogg");
    Sound playerWoken = LoadSound("sounds/Awakening.ogg");
    Sound playerHit = LoadSound("sounds/Player_Hit.ogg");
    Sound playerDie = LoadSound("sounds/Player_Explode.ogg");
    Sound bossWarning = LoadSound("sounds/Warning.ogg");
    SetSoundVolume(bossWarning,0.5);
    Sound bossHit = LoadSound("sounds/Boss_Hit.ogg");
    Sound bossDie = LoadSound("sounds/Boss_Explode.ogg");
    Sound shootBlast = LoadSound("sounds/Shoot_Blast.ogg");     
    SetSoundVolume(shootBlast,0.3);
    Sound blastBlast = LoadSound("sounds/Shoot_Blast.ogg");
    
    // Resource to Struct    
    player.bg = bgtex;
    player.tex = playerTexture;
    player.awake = playerAwake;
    player.ready = playerReady;
    player.woken = playerWoken;
    player.hit = playerHit;
    player.die = playerDie;
    player.bgm = bgm;
    boss.tex = bossTexture;
    boss.warning = bossWarning;
    boss.hit = bossHit;
    boss.die = bossDie;
    blast.blast = blastBlast;
    blast.text = blastImage;
    power.tex = powerUp;
    
    InitGame();
    
    SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()&&!quitGame)                                                // Looping hingga ditekan tombol close pada window atau fungsi keluar yang terdapat didalamnya
    {
        // Update
        //----------------------------------------------------------------------------------
        
        //General Updates
        framecounter +=4;                          //Pengatur Kecepatan Text
        framesCounter ++;                          //Pengatur Transisi Antar Layar
        mousePosition.x = GetMouseX();             //Mendapatkan posisi mouse pada sumbu x
        mousePosition.y = GetMouseY();             //Mendapatkan posisi mouse pada sumbu y
        
        //GameScreen Updates
        switch(gameScreen){
            case LOGO   : {
                //Black Screen before image
                if(state == 0) {
                    if(alpha<1.0)alpha += 0.05;                         //Pengatur Kecepatan Black Screen
                    else state = 1;
                }
                else if (state == 1){
                    alpha = 1.0;
                    if(framesCounter % 150==0) state = 2;               //Durasi untuk Menampilkan Layer Logo
                }
                else if (state == 2){
                    if(alpha>0.0)alpha -= 0.05;                         //Pengatur Kecepatan Black Screen
                    else{
                        gameScreen = TRIBUTE;
                        state = 0;
                    }
                }
            }break;
            case TRIBUTE: {
                //Black Screen before image
                if(state == 0) {
                    if(alpha<1.0)alpha += 0.05;              //Pengatur Kecepatan Black Screen
                    else state = 1;
                }
                else if (state == 1){
                    alpha = 1.0;
                    if(framesCounter % 150==0) state = 2;    //Lamanya Durasi Logo Raylib Ditampilkan
                }
                else if (state == 2){
                    if(alpha>0.0)alpha -= 0.05;              //Pengatur Kecepatan Black Screen
                    else{
                        gameScreen = TITLE;
                        state = 0;
                    }
                }
            }break;
            case TITLE  : {
                //title have background and two button
                if(state == 0){                                 //doing transition
                    if(alpha<1.0)alpha += 0.05;                 //pengatur waktu transisisi perpindahan
                    else state = 1;
                }
                else if(state == 1){                            
                    alpha = 1.0;
                    if( ( framesCounter % 15) == 0) animation = !animation;             //Setiap kelipatan x count maka gambar title 1 dan title 2 akan bergantian muncul
                    
                    //Jarak Terjauh Button untuk Ditekan
                    //Karena button dianggap berbentuk dot maka diperlukan limit
                    if( mousePosition.x >= buttonRight.x+25 && mousePosition.x <= (buttonRight.x+95)&& mousePosition.y >= buttonRight.y+40 && mousePosition.y <= (buttonRight.y+90)){
                        buttonIzq_over = true;
                        if(IsMouseButtonDown(0)) quitGame = true; 
                    }
                    else buttonIzq_over = false;
                    
                    if( mousePosition.x >= buttonLeft.x+25 && mousePosition.x <= (buttonLeft.x+95)&& mousePosition.y >= buttonLeft.y+35 && mousePosition.y <= (buttonLeft.y+90)){
                        buttonDer_over = true;
                        if(IsMouseButtonDown(0)) {
                            PlaySound(player.ready);
                            state = 2;
                        }
                    }
                    else buttonDer_over = false;
                    
                    if( mousePosition.x >= (buttonHelp.x+15) && mousePosition.x <= (buttonHelp.x+40)&& mousePosition.y >= (buttonHelp.y+10) && mousePosition.y <= (buttonHelp.y+40)){
                        buttonHelpx = true;
                        if(IsMouseButtonDown(0)) state = 3;
                    }
                    else buttonHelpx = false;
                }
                else if(state == 2){
                    if(alpha > 0.0) alpha -= 0.05;                  //Transisi Black Screen
                    else gameScreen = GAMING, state = 0;
                }
                else if (state == 3){
                    if(alpha > 0.0) alpha -= 0.05;
                    else gameScreen = HELP, state = 0;
                }
            }break;
            case HELP : {
                if(state == 0) {                                 //doing transition
                    if(alpha<1.0)alpha += 0.05;                 //pengatur waktu transisisi perpindahan
                    else state = 1;
                }
                else if(state == 1){
                    alpha = 1.0;
                    if( mousePosition.x >= (buttonBack.x+15) && mousePosition.x <= (buttonBack.x+65)&& mousePosition.y >= (buttonBack.y+15) && mousePosition.y <= (buttonBack.y+65)){
                            buttonBackb = true;
                            if(IsMouseButtonDown(0)) state = 3;
                        }
                    else buttonBackb = false;
                }
                else if(state == 3){
                    gameScreen = TITLE, state = 1;
                }
            }break;
            case GAMING : {
                //Start Game
                //----------------------------------------------------------------

                    UpdateDrawFrame();
            }break;
        }
        
        
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);
                switch(gameScreen){
                //Untuk Layer Logo
                case LOGO   : {
                    switch(state){
                        case 0 :{DrawTexture(BS,0,0,Fade(BLACK,alpha));
                                 PlaySound(LS);
                        }
                                 break;
                        case 1 :{
                            DrawText(SubText(desc,0,framecounter/13)                                                                       //Penulisan Animasi Text(dimulai dari index text nomor 0 dan mulai hingga setiap framecounter/13)
                                     ,screenWidth/2-screenWidth/2.8,screenHeight/2+screenHeight/12,20,BLACK);
                            DrawTexture(BL,screenWidth/2-screenWidth/2.8,screenHeight/2-screenHeight/3.5,RAYWHITE);}                       
                                 break;
                        case 2 :{DrawTexture(BS,0,0,Fade(BLACK,alpha));}
                                 break;
                    }
                }break;
                //Untuk Layer Tribute
                case TRIBUTE : {
                    switch(state){
                        case 1 :{
                           DrawText("Powered by",screenWidth/2-RL.width/2,screenHeight/5,20,BLACK);
                           DrawTexture(RL,screenWidth/2-RL.width/2,screenHeight/2-RL.height/2+screenHeight/35,RAYWHITE);}                       //Draw the texture with all color (alpha = 1.0).
                                 break;
                        case 2 :{DrawTexture(BS,0,0,Fade(BLACK,alpha));}
                                 break;
                    }
                }break;
                case TITLE  : {
                       switch(state){
                       case 0 : {
                           DrawTexture(BT1,0,0,Fade(BLACK,alpha));
                           PlaySound(TS);
                        }break;
                       case 1 : {
                           if(animation){
                               DrawTexture(BT1,0,0,RAYWHITE);                                           //Gambar Screen Title 1 
                           }
                           else{
                               DrawTexture(BT2,0,0,RAYWHITE);                                           //Gambar Screen Title 2
                           }
                           
                           if (buttonDer_over) {
                               DrawTexture(BP,buttonLeft.x,buttonLeft.y,BLUE);         //Menggambar Button Normal
                               DrawText("Play",buttonLeft.x+40,buttonLeft.y+90, 20,RAYWHITE);
                           }
                           else DrawTexture(BP,buttonLeft.x,buttonLeft.y+3,RAYWHITE);                        //Mewarnai Button dengan warna Biru
                       
                           if (buttonIzq_over) {
                               DrawTexture(BE,buttonRight.x,buttonRight.y,BLUE);       //Menggambar Button Normal
                               DrawText("Exit",buttonRight.x+45,buttonRight.y+90, 20,RAYWHITE);
                           }
                           else DrawTexture(BE,buttonRight.x,buttonRight.y+3,RAYWHITE);                      //Mewarnai Button dengan warna Biru
                            
                           if (buttonHelpx){ 
                               DrawTexture(BH,buttonHelp.x-10,buttonHelp.y-15,BLUE);       //Menggambar Button Normal
                               DrawText("Help",buttonHelp.x+57,buttonHelp.y+25, 20,RAYWHITE);
                           }
                           else DrawTexture(BH,buttonHelp.x-10,buttonHelp.y-12,RAYWHITE);                      //Mewarnai Button dengan warna Biru
                            
                       }break;
                       case 2 : {
                           DrawTexture(BT1,0,0,Fade(BLACK,alpha));
                           }break; //Transition when Clicked    
                   }
                }break;
                case HELP   : {
                    DrawTexture(BHh,0,0,RAYWHITE);
                    DrawText("Help",screenWidth/3+20, screenHeight/10, 40, Fade(GREEN, alpha));
                    DrawTexture(powerUp,(screenWidth/4)-20,screenHeight/5+10,Fade(RAYWHITE, alpha));
                    DrawText("POWER UP",(screenWidth*5)/8-30,(screenHeight/4)+5,15, Fade(YELLOW, alpha));
                    DrawTexture(BI,screenWidth/25,(screenHeight*1)/5+12,Fade(RAYWHITE, alpha));
                    DrawText("ENEMY BOSS",(screenWidth*5)/8-30,(screenHeight*1)/4+60, 15, Fade(YELLOW, alpha));
                    DrawTexture(EI,screenWidth/25-8,(screenHeight*1)/5+70,Fade(RAYWHITE, alpha));
                    DrawText("ORDINARY ENEMY",(screenWidth*5)/8-30,(screenHeight*1)/4+125, 15, Fade(YELLOW, alpha));
                    DrawText("Press [P] to Pause the Game",(screenWidth/5),(screenHeight*7)/12+10, 18, Fade(YELLOW, alpha));
                    DrawText("Hold [Space] to Fire",(screenWidth/5)+35,((screenHeight*7)/12)+40, 18, Fade(YELLOW, alpha));
                    DrawText("Press any Arrow Keys to move the Plane",(screenWidth/5)-55,((screenHeight*7)/12)+70, 18, Fade(YELLOW, alpha));
                    DrawText("Press [B] to use the Power Up",(screenWidth/5),((screenHeight*7)/12)+95, 18, Fade(YELLOW, alpha));
                    if (buttonBackb) {
                        DrawTexture(BB,buttonBack.x,buttonBack.y+3,BLUE);                         //Menggambar Button Normal
                        DrawText("Back",buttonBack.x+80,buttonBack.y+43, 18,RAYWHITE);
                    }
                    else DrawTexture(BB,buttonBack.x,buttonBack.y,RAYWHITE);                      //Mewarnai Button dengan warna Biru
                    
                }break;
                case GAMING : {
                    StopSound(TS);
                    UpdateDrawFrame();
                    PlayMusicStream(bgm);
                }break;
            }

            EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    UnloadGame();         // Unload loaded data (textures, sounds, models...)
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//------------------------------------------------------------------------------------
// GAME MODULAR FUNCTION
//------------------------------------------------------------------------------------

// Initialize game variables
void InitGame(void)
{
    //Initialize Looping Resources
    Sound enemyDie = LoadSound("sounds/Enemy_Explode.ogg");  
    SetSoundVolume(enemyDie,0.5);
    Sound shootBlast = LoadSound("sounds/Shoot_Blast.ogg");     
    SetSoundVolume(shootBlast,0.3);
   
    // Initialize game variables
    shootRate = 0;
    pause = false;
    gameOver = false;
    victory = false;
    smooth = false;
    wave = FIRST;
    activeEnemies = FIRST_WAVE;
    enemiesKill = 0;
    score = 0;
    alpha = 0;

    // Initialize player
    player.rec.x =  screenWidth/2;
    player.rec.y = screenHeight*3/4;
    player.rec.width = 20;
    player.rec.height = 20;
    player.speed.x = 3;
    player.speed.y = 3;
    player.life = 3;
    
    
    // Initialize enemies
    for (int i = 0; i < NUM_MAX_ENEMIES; i++)
    {
        enemy[i].rec.width = 35;
        enemy[i].rec.height = 25;
        enemy[i].rec.x = GetRandomValue(0, screenWidth - enemy[i].rec.width);
        enemy[i].rec.y = GetRandomValue(0, 0 - 500);
        enemy[i].speed.x = 2;
        enemy[i].speed.y = 2;
        enemy[i].active = true;
        enemy[i].tex = enemyTex;
        enemy[i].die = enemyDie;
        
    }
    
    // Initialize Boss Wave 5
    boss.rec.x =  screenWidth/2;
    boss.rec.y = 0;
    boss.rec.width = 100;
    boss.rec.height = 48;
    boss.speed.x = 3;
    boss.speed.y = 3;
    boss.active = false;
    boss.life = 35;
    
    // Initialize shoots
    for (int i = 0; i < NUM_SHOOTS; i++)
    {
        shoot[i].rec.x = player.rec.x + player.rec.width*3/8;
        shoot[i].rec.y = player.rec.y - player.rec.height/2;
        shoot[i].rec.width = 3;
        shoot[i].rec.height = 3;
        shoot[i].speed.x = 0;
        shoot[i].speed.y = -3;
        shoot[i].active = false;
        shoot[i].color = SKYBLUE;
        shoot[i].blast = shootBlast;
    }
    
    // Initialize Blaster
    blast.rec.width = 70;
    blast.rec.height = 300;
    blast.rec.x = player.rec.x + player.rec.width/2 - blast.rec.width/2;
    blast.rec.y = player.rec.y - blast.rec.height;
    blast.speed.x = 0;
    blast.speed.y = -7;
    blast.active = false;
    blast.color = BLUE;
    
    // Initialize Power Ups
    power.rec.x =  screenWidth/2;
    power.rec.y = 0;
    power.rec.width = 50;
    power.rec.height = 50;
    power.speed.x = 0;
    power.speed.y = 3;
    power.color = PINK;
    boss.active = false;
}

// Update game (one frame)
void UpdateGame(void)
{
    if (!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            UpdateMusicStream(player.bgm);
            
            switch (wave)
            {
                case FIRST:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if (enemiesKill == activeEnemies+20)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }
                        
                        activeEnemies = SECOND_WAVE;
                        wave = SECOND;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case SECOND:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if (enemiesKill == activeEnemies/2)
                    {
                        power.active = true;
                    }
                    
                    if (enemiesKill == activeEnemies+50)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }
                        
                        activeEnemies = THIRD_WAVE;
                        wave = THIRD;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case THIRD:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if (enemiesKill == activeEnemies/2)
                    {
                        power.active = true;
                    }
                    
                    if (enemiesKill == activeEnemies+75)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }
                        
                        activeEnemies = FOURTH_WAVE;
                        wave = FOURTH;
                        smooth = false;
                        alpha = 0.0f;
                    }
                } break;
                case FOURTH:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if (enemiesKill == activeEnemies/2)
                    {
                        power.active = true;
                    }
                    
                    if (enemiesKill == activeEnemies+100)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < activeEnemies; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }
                        
                        if (!boss.active) boss.active = true;
                        activeEnemies = FINAL_WAVE;
                        wave = FINAL;
                        PlaySound(boss.warning);
                        smooth = false;
                        alpha = 0.0f;
                    }   
                } break;
                case FINAL:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if ( boss.life == 0) 
                    {
                        PlaySound(boss.die);
                        victory = true;
                        boss.active = false;
                        activeEnemies = BONUS_WAVE;
                        wave = BONUS;
                        smooth = false;
                        alpha = 0.0f;
                    }                        
                } break;
                case BONUS:
                {
                    if (!smooth)
                    {
                        alpha += 0.02f;
                        
                        if (alpha >= 1.0f) smooth = true;
                    }
                    
                    if (smooth) alpha -= 0.02f;
                    
                    if (enemiesKill % 15 == 0)
                    {
                        power.active = true;
                    }
                    
                    if (victory == true)
                    {
                        enemiesKill = 0;

                        for (int i = 0; i < BONUS_WAVE; i++)
                        {
                            if (!enemy[i].active) enemy[i].active = true;
                        }
                    }
                } break;
                default: break;
            }

            // Player movement
            if (IsKeyDown(KEY_RIGHT)) player.rec.x += player.speed.x;
            if (IsKeyDown(KEY_LEFT)) player.rec.x -= player.speed.x;
            if (IsKeyDown(KEY_UP)) player.rec.y -= player.speed.y;
            if (IsKeyDown(KEY_DOWN)) player.rec.y += player.speed.y;

            // Player collisions
            for (int i = 0; i < activeEnemies; i++)
            {
                if (CheckCollisionRecs(player.rec, enemy[i].rec)) 
                {
                    player.life -= 1;
                    PlaySound(player.hit);
                    enemy[i].rec.x = GetRandomValue(0, screenWidth - enemy[i].rec.width);
                    enemy[i].rec.y = GetRandomValue(0, 0 - 500);
                }
            }
            
            if(boss.active)
            {
                if (CheckCollisionRecs(player.rec, boss.rec)) player.life -= 1;
            }
            
            if (player.life == 0) 
            {
                gameOver = true;
                player.awakening = false;
                PlaySound(player.die);
            }

             // Enemy behaviour
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    enemy[i].rec.y += enemy[i].speed.y;

                    if (enemy[i].rec.y > screenHeight)
                    {
                        enemy[i].rec.x = GetRandomValue(0, screenWidth - enemy[i].rec.width);
                        enemy[i].rec.y = GetRandomValue(0, 0 - 500);
                    }
                }
            }
            
             // Boss behaviour
            if (boss.active)
            {
                boss.rec.y += boss.speed.y;
                boss.rec.x += boss.speed.x;

                if (boss.rec.y + boss.rec.height > screenHeight || boss.rec.y < 0)
                {
                    boss.speed.y *= -1;
                }
                if (boss.rec.x + boss.rec.width > screenWidth || boss.rec.x < 0)
                {
                    boss.speed.x *= -1;
                }
            }

                // Wall behaviour
            if (player.rec.x <= 0) player.rec.x = 0;
            if (player.rec.x + player.rec.width >= screenWidth) player.rec.x = screenWidth - player.rec.width;
            if (player.rec.y <= 0) player.rec.y = 0;
            if (player.rec.y + player.rec.height >= screenHeight) player.rec.y = screenHeight - player.rec.height;

            //Shoot initialization
            if (IsKeyDown(KEY_SPACE))
            {
                shootRate += 2 ;

                for (int i = 0; i < NUM_SHOOTS; i++)
                {
                    if (!shoot[i].active && shootRate%20 == 0)
                    {
                        PlaySound(shoot[i].blast);
                        shoot[i].rec.x = player.rec.x + player.rec.width*3/8;
                        shoot[i].rec.y = player.rec.y ;
                        shoot[i].active = true;
                        break;
                    }
                }
            }

            // Shoot logic
            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot[i].active)
                {
                    // Movement
                    shoot[i].rec.y += shoot[i].speed.y;
                    
                    // Collision with enemy
                    for (int j = 0; j < activeEnemies; j++)
                    {
                        if (enemy[j].active)
                        {
                            if (CheckCollisionRecs(shoot[i].rec, enemy[j].rec))
                            {
                                shoot[i].active = false;
                                enemy[j].rec.x = GetRandomValue(0, screenWidth - enemy[j].rec.width);
                                enemy[j].rec.y = GetRandomValue(0, 0 - 500);
                                shootRate = 0;
                                PlaySound(enemy[j].die);
                                enemiesKill++;
                                score += 10;
                            }
                            
                            if (shoot[i].rec.y + shoot[i].rec.height <= 0)
                            {
                                shoot[i].active = false;
                                shootRate = 0;
                            }
                        }
                        if (boss.active)
                        {
                            if (CheckCollisionRecs(shoot[i].rec, boss.rec))
                            {
                                shoot[i].active = false;
                                shootRate = 0;
                                boss.life--;
                                PlaySound(boss.hit);
                                score += 10;
                            }
                            
                            if (shoot[i].rec.y + shoot[i].rec.height <= 0)
                            {
                                shoot[i].active = false;
                                shootRate = 0;
                            }
                        }
                    }
                }
            }
            
            // Power Up Behaviour
            if (power.active)
                {
                    power.rec.y += power.speed.y;
                    
                    if (CheckCollisionRecs(player.rec, power.rec)) 
                    {
                        player.awakening = true;   
                        PlaySound(player.woken);
                        power.active = false;
                        power.rec.x = GetRandomValue(0, screenWidth - power.rec.width);
                        power.rec.y = GetRandomValue(0, 0 - 500);
                    }    

                    if (power.rec.y > screenHeight)
                    {
                        power.rec.x = GetRandomValue(0, screenWidth - power.rec.width);
                        power.rec.y = GetRandomValue(0, 0 - 500);
                        power.active = false;
                    }
                }
            
            //Blast Initialization
            if (player.awakening)
            {
                if (!blast.active)
                {
                    if (IsKeyPressed('B'))
                    {
                        PlaySound(blast.blast);
                        blast.rec.x = player.rec.x + player.rec.width/2 - blast.rec.width/2;
                        blast.rec.y = player.rec.y - blast.rec.height;
                        blast.active = true;
                        player.awakening = false;
                    }
                }
            }
            
            //Blast Logic
            if (blast.active)
                {
                    // Movement
                    blast.rec.y += blast.speed.y;
                    
                    // Collision with enemy
                    for (int j = 0; j < activeEnemies; j++)
                    {
                        if (enemy[j].active)
                        {
                            if (CheckCollisionRecs(blast.rec, enemy[j].rec))
                            {
                                enemy[j].rec.x = GetRandomValue(0, screenWidth - enemy[j].rec.width);
                                enemy[j].rec.y = GetRandomValue(0, 0 - 500);
                                PlaySound(enemy[j].die);
                                enemiesKill++;
                                score += 10;
                            }
                            
                            if (blast.rec.y + blast.rec.height <= 0)
                            {
                                blast.active = false;
                            }
                        }
                        if (boss.active)
                        {
                            if (CheckCollisionRecs(blast.rec, boss.rec))
                            {
                                blast.active = false;
                                boss.life = boss.life - 5;
                                PlaySound(boss.hit);
                                score += 100;
                            }
                            if (blast.rec.y + blast.rec.height <= 0)
                            {
                                blast.active = false;
                            }
                        }
                    }
                }
        }
    }
    else
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            PlaySound(player.ready);
            InitGame();
            gameOver = false;
        }
    }
}

// Draw game (one frame)
void DrawGame(void)
{
        ClearBackground(DARKGRAY);
        
        if (!gameOver)
        {
            DrawTexture(player.bg, 0, 0 ,WHITE);
            DrawTexture(player.tex, player.rec.x, player.rec.y, WHITE);

            if (wave == FIRST)
            {
                 DrawText("FIRST WAVE", screenWidth/2 - MeasureText("FIRST WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(GRAY, alpha));
                 DrawText("FIRST", screenWidth - 20 - MeasureText("FIRST", 40), 20, 40, YELLOW);
            }
            else if (wave == SECOND) 
            {
                DrawText("SECOND WAVE", screenWidth/2 - MeasureText("SECOND WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(GRAY, alpha));
                DrawText("SECOND", screenWidth - 20 - MeasureText("SECOND", 40), 20, 40, YELLOW);
            }
            else if (wave == THIRD) 
            {
                DrawText("THIRD WAVE", screenWidth/2 - MeasureText("THIRD WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(GRAY, alpha));
                DrawText("THIRD", screenWidth - 20 - MeasureText("THIRD", 40), 20, 40, YELLOW);
            }
            else if (wave == FOURTH) 
            {
                DrawText("FOURTH WAVE", screenWidth/2 - MeasureText("FOURTH WAVE", 40)/2, screenHeight/2 - 40, 40, Fade(GRAY, alpha));
                DrawText("FOURTH", screenWidth - 20 - MeasureText("FOURTH", 40), 20, 40, YELLOW);
            }else if (wave == FINAL) 
            {
                DrawText("WARNING", screenWidth/2 - MeasureText("WARNING", 60)/2, screenHeight/2 - 60, 60, Fade(ORANGE, alpha));
                DrawText("FINAL", screenWidth - 20 - MeasureText("FINAL", 40), 20, 40, YELLOW);
            }else if (wave == BONUS) 
            {
                DrawText("BONUS WAVE", screenWidth/2 - MeasureText("BONUS WAVE", 40)/2, screenHeight/2 - 20, 40, Fade(GRAY, alpha));
                DrawText("BONUS", screenWidth - 20 - MeasureText("BONUS", 40), 20, 40, YELLOW);
            }
            
            if (player.life == 3)
            {
                DrawText("LIFE: 3", screenWidth/2-MeasureText("Life: 3",40)/2, screenHeight*7/8, 40, GREEN);
            }else if (player.life == 2)
            {
                DrawText("LIFE: 2", screenWidth/2-MeasureText("Life: 2",40)/2, screenHeight*7/8, 40, GREEN);
            }else if (player.life == 1)
            {
                DrawText("LIFE: 1", screenWidth/2-MeasureText("Life: 1",40)/2, screenHeight*7/8, 40, GREEN); 
            }
            
            for (int i = 0; i < activeEnemies; i++)
            {
                if (enemy[i].active)
                {
                    DrawTexture(enemy[i].tex, enemy[i].rec.x, enemy[i].rec.y, WHITE);  
                } 
            }
            
            if (boss.active) 
            {
                DrawTexture(boss.tex, boss.rec.x, boss.rec.y, WHITE);
            }

            for (int i = 0; i < NUM_SHOOTS; i++)
            {
                if (shoot[i].active) DrawRectangleRec(shoot[i].rec, shoot[i].color);
            }
            
            if (power.active) DrawTexture(power.tex,power.rec.x,power.rec.y,WHITE);
            
            if (blast.active) DrawTexture(blast.text,blast.rec.x,blast.rec.y,WHITE);
            
            if (player.awakening) DrawTexture(player.awake, player.rec.x, player.rec.y, WHITE);
            
            DrawText(FormatText("%04i", score), 20, 20, 40, YELLOW);
        
            if (victory) DrawText("YOU WIN", screenWidth/2 - MeasureText("YOU WIN", 40)/2, screenHeight/2 - 60, 40, Fade(GREEN, alpha));
        
            if (pause) DrawText("GAME PAUSED", screenWidth/2 - MeasureText("GAME PAUSED", 40)/2, screenHeight/2 - 40, 40, GRAY);
        }
        else {
            DrawText("YOU LOSE", GetScreenWidth()/2 - MeasureText("YOU LOSE", 20)/2, GetScreenHeight()/2 - 40, 20, GRAY);
            DrawText("PRESS [ENTER] TO PLAY AGAIN", GetScreenWidth()/2 - MeasureText("PRESS [ENTER] TO PLAY AGAIN", 20)/2, GetScreenHeight()/2 - 20, 20, GRAY);
        }
}

// Unload game variables
void UnloadGame(void)
{
    // Unload all dynamic loaded data (textures, sounds, models...)
    UnloadTexture(player.tex);
    UnloadTexture(player.awake);
    UnloadTexture(player.bg);
    UnloadTexture(boss.tex);
    UnloadTexture(blast.text);
    UnloadTexture(enemyTex);
    UnloadTexture(power.tex);
    UnloadSound(player.ready);
    UnloadSound(player.woken);
    UnloadSound(player.hit);
    UnloadSound(player.die);
    UnloadMusicStream(player.bgm);
    UnloadSound(boss.warning);
    UnloadSound(boss.hit);
    UnloadSound(boss.die); 
    CloseAudioDevice();
}

// Update and Draw (one frame)
void UpdateDrawFrame(void)
{
    UpdateGame();
    DrawGame();
}