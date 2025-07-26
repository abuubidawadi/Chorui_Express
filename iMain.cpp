#include "iGraphics.h"
#include "iSound.h"
#include "iFont.h"
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

#define MAX_LEADERS 10
#define MAX_NAME_LEN 30

// Path for save file
#define SAVE_FILE "savegame.dat"

int GameState = 0, level, pause = 0, bg_music = 1, bird_sound = 1, BGmusic, CoinSound, IsGameOver = 0, PausePossible = 1, NameInput = 1, LevelHighScore = 0;
Image BirdImage[6], ObstacleImage[1], EnemyImage[6], CoinImage[6];
Sprite BirdSprite, ObstacleSprite, EnemySprite, CoinSprite;

int SpriteTimer, BirdFallTimer, ObstacleMoveTimer, EnemyMoveTimer, CoinMoveTimer;

int score = 0;
int highScore = 0;
int obstacleSpeed;
int CoinSpeed;
int enemySpeed;
int gravity;
int jumpSpeed = 50;

int typingName = 0;

char playerName[100] = "";
int nameLength = 0;
int difficulty = 1; // 1 = Easy, 2 = Medium, 3 = Hard

#define MAX_ENTRIES 100
char names[MAX_ENTRIES][100];
int scores[MAX_ENTRIES];
int entryCount = 0;

void HomePage(){
    iShowImage(0, 0, "assets/images/pages/HomePage.jpg");
}

void MainMenuPage(){
    iShowImage(0, 0, "assets/images/pages/MainMenuPage.jpg");
}

void GameStart(){
    iShowImage(0, 0, "assets/images/pages/Difficulty.jpg");
}

void PauseWindow(){
    iShowImage(0, 0, "assets/images/pages/PauseWindow.png");
}

void LoadSprite(){
    iInitSprite(&BirdSprite);
	iLoadFramesFromFolder(BirdImage, "assets/images/Sprites");
	iChangeSpriteFrames(&BirdSprite, BirdImage, 6);
    iSetSpritePosition(&BirdSprite, 150, 250);

    iInitSprite(&ObstacleSprite);
	iLoadFramesFromFolder(ObstacleImage, "assets/images/Obstacles");
	iChangeSpriteFrames(&ObstacleSprite, ObstacleImage, 1);
    iSetSpritePosition(&ObstacleSprite, -200, 0);

    iInitSprite(&CoinSprite);
	iLoadFramesFromFolder(CoinImage, "assets/images/coin");
	iChangeSpriteFrames(&CoinSprite, CoinImage, 6);
    iSetSpritePosition(&CoinSprite, -200, 0);

    iInitSprite(&EnemySprite);
    iLoadFramesFromFolder(EnemyImage, "assets/images/enemy");
    iChangeSpriteFrames(&EnemySprite, EnemyImage, 6);
    iSetSpritePosition(&EnemySprite, 550, 250);
}

void SetDifficultyParameters() {
    if(level == 1){         // Easy
        obstacleSpeed = 5;
        CoinSpeed = obstacleSpeed;
        enemySpeed = 8;
        gravity = 5;
        jumpSpeed = 50;
    }
    else if(level == 2){    // Medium
        obstacleSpeed = 7;
        CoinSpeed = obstacleSpeed;
        enemySpeed = 10;
        gravity = 7;
        jumpSpeed = 50;
    }
    else if(level == 3){    // Hard
        obstacleSpeed = 7;
        CoinSpeed = obstacleSpeed;
        enemySpeed = 10;
        gravity = 7;
        jumpSpeed = 50;
    }
}


void iAnim(){
	iAnimateSprite(&BirdSprite);
    iAnimateSprite(&CoinSprite);
    if(level == 3){
        iAnimateSprite(&EnemySprite);
    }
}

int GetHighScoreForLevel(int level) {
    char *filename;
    if (level == 1) filename = "ezScore.txt";
    else if (level == 2) filename = "medScore.txt";
    else if (level == 3) filename = "hardScore.txt";
    else return 0;

    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    int highScore = 0;
    char name[100];
    int score;

    while (fscanf(fp, "%s %d", name, &score) == 2) {
        if (score > highScore) {
            highScore = score;
        }
    }

    fclose(fp);
    return highScore;
}

void InsertScore(char names[][MAX_NAME_LEN], int scores[], int max_leaders, char *newName, int newScore) {
    for (int i = 0; i < max_leaders; i++) {
        if (newScore > scores[i]) {
            // Shift down from the end to i
            for (int j = max_leaders - 1; j > i; j--) {
                scores[j] = scores[j - 1];
                strncpy(names[j], names[j - 1], MAX_NAME_LEN);
            }
            // Insert new score & name at i
            scores[i] = newScore;
            strncpy(names[i], newName, MAX_NAME_LEN - 1);
            names[i][MAX_NAME_LEN - 1] = '\0';
            break;
        }
    }
}



void AddScore(int level, char name[], int score) {
    char *filename;

    if (level == 1) filename = "ezScore.txt";
    else if (level == 2) filename = "medScore.txt";
    else filename = "hardScore.txt";

    FILE *fp = fopen(filename, "a");
    if (fp) {
        fprintf(fp, "%s %d\n", name, score);
        fclose(fp);
    }
}

int totalScores = 0;

void LoadScores(int level) {
    char *filename;

    if (level == 1) filename = "ezScore.txt";
    else if (level == 2) filename = "medScore.txt";
    else filename = "hardScore.txt";

    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    totalScores = 0;
    char tempName[100];
    int tempScore;

    while (fscanf(fp, "%s %d", tempName, &tempScore) == 2) {
        // Check for duplicate
        int duplicate = 0;
        for (int i = 0; i < totalScores; i++) {
            if (strcmp(names[i], tempName) == 0) {
                if (tempScore > scores[i]) scores[i] = tempScore;
                duplicate = 1;
                break;
            }
        }
        if (!duplicate && totalScores < MAX_ENTRIES) {
            strcpy(names[totalScores], tempName);
            scores[totalScores] = tempScore;
            totalScores++;
        }
    }
    fclose(fp);

    // Sort by score descending
    for (int i = 0; i < totalScores - 1; i++) {
        for (int j = i + 1; j < totalScores; j++) {
            if (scores[i] < scores[j]) {
                int tmpScore = scores[i];
                scores[i] = scores[j];
                scores[j] = tmpScore;

                char tmpName[100];
                strcpy(tmpName, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], tmpName);
            }
        }
    }
}

void ShowLeaderboard() {
    iSetColor(0, 0, 0);
    
    for (int i = 0; i < totalScores && i < 10; i++) {
        char buf[200];
        sprintf(buf, "%2d. %-20s %5d", i + 1, names[i], scores[i]);
        iShowText(360, 390 - i * 30, buf, "assets/fonts/englishfont.ttf", 18);
    }
    
    if (totalScores == 0) {
        iShowText(350, 300, "No high scores yet!", "assets/fonts/englishfont.ttf", 18);
    }
}


void InitializeScoreFiles() {
    FILE *f;
    
    // Check and create files if they don't exist
    if ((f = fopen("ezScore.txt", "r")) == NULL) {
        f = fopen("ezScore.txt", "w");
        fclose(f);
    }
    
    if ((f = fopen("medScore.txt", "r")) == NULL) {
        f = fopen("medScore.txt", "w");
        fclose(f);
    }
    
    if ((f = fopen("hardScore.txt", "r")) == NULL) {
        f = fopen("hardScore.txt", "w");
        fclose(f);
    }
}


// starting a new game (or pausing mid-play) e call korbo
void SaveGame() {
    FILE *fp = fopen(SAVE_FILE, "wb");
    if (!fp) return;

    
    /*fwrite(&GameState, sizeof(GameState), 1, fp);*/
    fwrite(&level, sizeof(level), 1, fp);
    fwrite(&pause, sizeof(pause), 1, fp);
    fwrite(&score, sizeof(score), 1, fp);
    fwrite(&highScore, sizeof(highScore), 1, fp);
    
    
    fwrite(&BirdSprite.x, sizeof(int), 1, fp);
    fwrite(&BirdSprite.y, sizeof(int), 1, fp);

    
    fwrite(&ObstacleSprite.x, sizeof(int), 1, fp);
    fwrite(&ObstacleSprite.y, sizeof(int), 1, fp);

    fwrite(&CoinSprite.x, sizeof(int), 1, fp);
    fwrite(&CoinSprite.y, sizeof(int), 1, fp);
    
    // Enemy position only for level 3
    if (level == 3) {
        fwrite(&EnemySprite.x, sizeof(int), 1, fp);
        fwrite(&EnemySprite.y, sizeof(int), 1, fp);
    }

    // Save player name length + name
    int nameLen = strlen(playerName);
    fwrite(&nameLen, sizeof(int), 1, fp);
    fwrite(playerName, sizeof(char), nameLen + 1, fp);

    // Save audio preferences
    fwrite(&bg_music, sizeof(bg_music), 1, fp);
    fwrite(&bird_sound, sizeof(bird_sound), 1, fp);

    fclose(fp);
}

// load an existing save (if any)
void LoadGame() {
    FILE *fp = fopen(SAVE_FILE, "rb");
    if (!fp) return; // No save file yet

    /*fread(&GameState, sizeof(GameState), 1, fp);*/
    fread(&level, sizeof(level), 1, fp);
    fread(&pause, sizeof(pause), 1, fp);
    fread(&score, sizeof(score), 1, fp);
    fread(&highScore, sizeof(highScore), 1, fp);

    fread(&BirdSprite.x, sizeof(int), 1, fp);
    fread(&BirdSprite.y, sizeof(int), 1, fp);

    fread(&ObstacleSprite.x, sizeof(int), 1, fp);
    fread(&ObstacleSprite.y, sizeof(int), 1, fp);

    fread(&CoinSprite.x, sizeof(int), 1, fp);
    fread(&CoinSprite.y, sizeof(int), 1, fp);

    if (level == 3) {
        fread(&EnemySprite.x, sizeof(int), 1, fp);
        fread(&EnemySprite.y, sizeof(int), 1, fp);
    }

    int nameLen;
    fread(&nameLen, sizeof(int), 1, fp);
    fread(playerName, sizeof(char), nameLen + 1, fp);

    fread(&bg_music, sizeof(bg_music), 1, fp);
    fread(&bird_sound, sizeof(bird_sound), 1, fp);

    fclose(fp);

    // Re-initialize sprites and timers
    //LoadSprite();
    SetDifficultyParameters();

    // Resume timers
    iResumeTimer(SpriteTimer);
    iResumeTimer(BirdFallTimer);
    iResumeTimer(ObstacleMoveTimer);
    iResumeTimer(CoinMoveTimer);
    iResumeTimer(EnemyMoveTimer);
}

void GameOver() {
    int currentLevelHighScore = GetHighScoreForLevel(level);
    if(score > highScore) highScore = score;
    iShowImage(0, 0, "assets/images/pages/GameOverWindow.png");

    iSetColor(118, 88, 71);
    char scoreText[20];
    sprintf(scoreText, "Avcbvi †¯‹vit %d", score);
    iShowText(395, 369, scoreText, "assets/fonts/banglafont2.ttf", 40);

    char highScoreText[30];
    sprintf(highScoreText, "m‡e©v”P †¯‹vi: %d", currentLevelHighScore);
    iShowText(398, 313, highScoreText, "assets/fonts/banglafont2.ttf", 40);

    iPauseTimer(SpriteTimer);
    iPauseTimer(BirdFallTimer);
    iPauseTimer(ObstacleMoveTimer);
    iPauseTimer(CoinMoveTimer);
    iPauseTimer(EnemyMoveTimer);
    PausePossible = 0;

    AddScore(level, playerName, score);
    
}



void SpriteFall(){
    if(level > 0 && pause % 2 == 0){
        BirdSprite.y -= gravity;
        if (BirdSprite.y < -50) {
            IsGameOver = 1;
        }
        else if (BirdSprite.y > SCREEN_HEIGHT - 50) {
            BirdSprite.y = SCREEN_HEIGHT - 50;
        }
    }
}

void ObstacleMove(){
    if(level > 0 && pause % 2 == 0){
        ObstacleSprite.x -= obstacleSpeed;
        CoinSprite.x -= CoinSpeed;
            if(ObstacleSprite.x < -15276) ObstacleSprite.x = 1000;
            if(CoinSprite.x < -15276) CoinSprite.x = 1000;
    }
}

void CoinMove(){
    if(level > 0 && pause % 2 == 0){
        CoinSprite.x -= CoinSpeed;
            if(CoinSprite.x < -15276) CoinSprite.x = 1000;
    }
}

void EnemyMove() {
    if (level > 0 && pause % 2 == 0) {
        EnemySprite.x -= enemySpeed;
        if (EnemySprite.x < -300) {
            EnemySprite.x = SCREEN_WIDTH + 500;
            EnemySprite.y = rand() % (SCREEN_HEIGHT - 150);
        }
    }
}

void GamePlay(){
    iShowSprite(&BirdSprite);
    iShowSprite(&ObstacleSprite);
    iShowSprite(&CoinSprite);
    if(level == 3) iShowSprite(&EnemySprite);

    int count = iCheckCollision(&BirdSprite, &ObstacleSprite);
    int visibleCount = iGetVisiblePixelsCount(&BirdSprite);
    if (count / (20.0 * visibleCount) > 0.01) {
        IsGameOver = 1;   
    }

    if(level == 3){
        int count = iCheckCollision(&BirdSprite, &ObstacleSprite) + iCheckCollision(&BirdSprite, &EnemySprite);
        int visibleCount = iGetVisiblePixelsCount(&BirdSprite);
        if (count / (20.0 * visibleCount) > 0.01) {
            IsGameOver = 1;   
    }
    }

    int coin_count = iCheckCollision(&BirdSprite, &CoinSprite);
    int coin_visibleCount = iGetVisiblePixelsCount(&BirdSprite);
    if (coin_count / (5.0 * coin_visibleCount) > 0.01) {
        score++;
        if(bird_sound == 1){
            iResumeSound(CoinSound);
            }
    }
    else{
        iPauseSound(CoinSound);
    }

    if(IsGameOver == 1){
        GameOver();
    }

    if(IsGameOver == 0){
            iResumeTimer(SpriteTimer);
            iResumeTimer(BirdFallTimer);
            iResumeTimer(ObstacleMoveTimer);
            iResumeTimer(EnemyMoveTimer);
    }

    int currentLevelHighScore = GetHighScoreForLevel(level);


    //score and name
    iSetColor(55, 33, 21);
    char scoreText[20];
    sprintf(scoreText, "†¯‹vi: %d", score);
    iShowText(20, 50, scoreText, "assets/fonts/banglafont.ttf", 20);

    char highScoreText[30];
    sprintf(highScoreText, "m‡e©v”P †¯‹vi: %d", currentLevelHighScore);
    iShowText(20, 20, highScoreText, "assets/fonts/banglafont.ttf", 20);

    iShowText(20, 80, playerName, "assets/fonts/englishfont.ttf", 20);

}

void NameInputWindow(){
    iShowImage(0, 0, "assets/images/pages/NameInput.jpg");
    iSetColor(0, 0, 0);
    iText(530, 210, playerName, GLUT_BITMAP_HELVETICA_18);

    if (typingName) {
        iText(530 + 10 * strlen(playerName), 210, "|", GLUT_BITMAP_HELVETICA_18);  // cursor
    }

}

void LevelEasy(){
    iShowImage(0, 0, "assets/images/pages/EasyBG.jpg");
    GamePlay();
        if(pause%2!=0){
            PauseWindow();
        }
        else{}
}

void LevelMedium(){
    iShowImage(0, 0, "assets/images/pages/MediumBG.jpg");
    GamePlay();
        if(pause%2!=0){
            PauseWindow();
        }
        else{}
}

void LevelHard(){
    iShowImage(0, 0, "assets/images/pages/HardBG.jpg");
    GamePlay();
        if(pause%2!=0){
            PauseWindow();
        }
        else{}
}

void Instructions(){
    iShowImage(0, 0, "assets/images/pages/Instructions.jpg");
}

void ClearAllScores() {
    FILE *fp;

    // Easy
    fp = fopen("ezScore.txt", "w");
    if(fp) fclose(fp);

    // Medium
    fp = fopen("medScore.txt", "w");
    if(fp) fclose(fp);

    // Hard
    fp = fopen("hardScore.txt", "w");
    if(fp) fclose(fp);
}



void Settings(){
    if(bg_music==1 && bird_sound==1){
        iShowImage(0, 0, "assets/images/pages/Settings_on_on.jpg");
    }
    else if(bg_music==0 && bird_sound==1){
        iShowImage(0, 0, "assets/images/pages/Settings_off_on.jpg");
    }
    else if(bg_music==1 && bird_sound==0){
        iShowImage(0, 0, "assets/images/pages/Settings_on_off.jpg");
    }
    else if(bg_music==0 && bird_sound==0){
        iShowImage(0, 0, "assets/images/pages/Settings_off_off.jpg");
    }

    iSetColor(0, 0, 0);
    iText(465, 215, playerName, GLUT_BITMAP_HELVETICA_18);

    if(typingName){
        iText(465 + 10 * strlen(playerName), 215, "|", GLUT_BITMAP_HELVETICA_18); // cursor
    }
}

void HighScoreEasy(){
    iShowImage(0, 0, "assets/images/pages/EasyHighScore.jpg");
    LoadScores(1);  
    ShowLeaderboard();
    
}

void HighScoreMedium(){
    iShowImage(0, 0, "assets/images/pages/MediumHighScore.jpg");
    LoadScores(2);  
    ShowLeaderboard();
}

void HighScoreHard(){
    iShowImage(0, 0, "assets/images/pages/HardHighScore.jpg");
    LoadScores(3); 
    ShowLeaderboard();
}

void HighScore(){
    if(LevelHighScore==0){
        iShowImage(0, 0, "assets/images/pages/DifficultyHighScore.jpg");
    }
    else if(LevelHighScore==1){
        HighScoreEasy();
    }
    else if(LevelHighScore==2){
        HighScoreMedium();
    }
    else if(LevelHighScore==3){
        HighScoreHard();
    }
}

void Credit(){
    iShowImage(0, 0, "assets/images/pages/Credit.jpg");
}

void Exit(){
    SaveGame(); // Save current game state before exiting
    exit(0);
}

/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    iClear();

    if(bg_music == 1) iResumeSound(BGmusic);
    else if(bg_music == 0) iPauseSound(BGmusic);

    /*if(bird_sound == 1) iResumeSound(CollisionSound);
    else if(bird_sound == 0) iPauseSound(CollisionSound);*/

    if(GameState==0){
        HomePage();
        iSetColor(0, 0, 0);

    }
    else if(GameState==1){
        MainMenuPage();
    }
    else if(GameState==2){
        if(level==0 && NameInput == 1){
            NameInputWindow();
        }
        else if(level==0 && NameInput == 0){
            GameStart();
        }
        else if(level==0 && PausePossible == 1){
            GamePlay();
            if(pause%2!=0){
                PauseWindow();
            }
        }
        else if(level==0 && PausePossible == 0){
            GamePlay();
            if(pause%2!=0){
                PauseWindow();
            }
        }
        else if(level==1){
            LevelEasy();
        }
        else if(level==2){
            LevelMedium();
        }
        else if(level==3){
            LevelHard();
            }
    }
    else if(GameState==3){
        Instructions();
    }
    else if(GameState==4){
        Settings();
    }
    else if(GameState==5){
        HighScore();
    }
    else if(GameState==6){
        Exit();
    }
    else if(GameState==7){
        Credit();
    }
}

/*
function iMouseMove() is called when the user moves the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    printf("x = %d, y= %d\n",mx,my);
}

/*
function iMouseDrag() is called when the user presses and drags the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouseDrag(int mx, int my)
{
    // place your codes here
}

/*
function iMouse() is called when the user presses/releases the mouse.
(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my){

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){

        if(GameState==1){       //in main menu        
            if (mx > 387 && mx < 692 && my > 357 && my < 405){
                // Reset game variables when starting a new game
                GameState = 2;      //start game
                NameInput = 1;
                level = 0;
                pause = 0;
                IsGameOver = 0;
                PausePossible = 1;
                score = 0;
                BirdSprite.y = 250;
                BirdSprite.x = 150;
                ObstacleSprite.x = 800;
                CoinSprite.x = 800;
                ObstacleSprite.y = 0;
                CoinSprite.y = 0;
                EnemySprite.x = 1200;
                EnemySprite.y = rand() % (SCREEN_HEIGHT - 150);
            }
            else if (mx > 387 && mx < 692 && my > 292 && my < 346){
                GameState = 3;      //instructions
            }
            else if (mx > 387 && mx < 692 && my > 227 && my < 279){
                GameState = 4;      //settings
            }
            else if (mx > 387 && mx < 692 && my > 163 && my < 224){
                GameState = 5;      //high score
            }
            else if (mx > 387 && mx < 692 && my > 98 && my < 148){
                GameState = 6;      //exit
            }
            else if(mx > 92 && mx < 177 && my > 76 && my < 208){
                GameState = 7;      //credit
            }
        }

        else if(GameState==3){      //in instructions
            if(mx > 845 && mx < 980 && my > 470 && my < 577){
                GameState = 1;      //back to main menu
            }
        }

        else if(GameState==4){      //in settings
            if(mx > 845 && mx < 980 && my > 470 && my < 577){
                GameState = 1;      //back to main menu
            }
            else if(mx > 633 && mx < 698 && my > 341 && my < 387){
                bg_music = 0;
            }
            else if(mx > 559 && mx < 633 && my > 341 && my < 387){
                bg_music = 1;
            }
            else if(mx > 492 && mx < 557 && my > 269 && my < 317){
                bird_sound = 0;
            }
            else if(mx > 418 && mx < 492 && my > 269 && my < 317){
                bird_sound = 1;
            }

            // Check click in name area
            else if(mx > 465 && mx < 700 && my > 215 && my < 235){
                typingName = 1;
            }

            else if(mx > 215 && mx < 400 && my > 150 && my < 170){   //reset all scores
                ClearAllScores();
               
            }

        }

        else if(GameState==5){      //in high score
            if(LevelHighScore==0){
                if(mx > 845 && mx < 980 && my > 470 && my < 577){
                    GameState = 1;      //back to main menu
                }
                else if(mx > 356 && mx < 645 && my > 296 && my < 371){  //easy
                    LevelHighScore = 1;
                }
                else if(mx > 356 && mx < 645 && my > 192 && my < 265){  //medium
                    LevelHighScore = 2;
                }
                else if(mx > 356 && mx < 645 && my > 94 && my < 166){       //hard
                    LevelHighScore = 3;
                }
            }
            else if(LevelHighScore==1){
                if(mx > 845 && mx < 980 && my > 470 && my < 577){
                    LevelHighScore = 0;  
                }
            }
            else if(LevelHighScore==2){
                if(mx > 845 && mx < 980 && my > 470 && my < 577){
                    LevelHighScore = 0;  
                }
            }
            else if(LevelHighScore==3){
                if(mx > 845 && mx < 980 && my > 470 && my < 577){
                    LevelHighScore = 0;  
                }
            }
        }

        else if(GameState==7){
            if(mx > 845 && mx < 980 && my > 470 && my < 577){
                GameState = 1;      //back to main menu
            }
        }

        else if(GameState==6){      //in exit
            Exit();
        }

        else if(GameState==2 && NameInput == 1){   //name input
            if(mx > 530 && mx < 700 && my > 210 && my < 235){   //click in name input area
                typingName = 1;
            }
                if(mx > 845 && mx < 980 && my > 470 && my < 577){
                GameState = 1;      //back to main menu
                }
                else if(mx > 455 && mx < 620 && my > 348 && my < 395){   //new game start
                    NameInput = 0;
                    IsGameOver = 0;
                    typingName = 0;
                    level = 0;
                }
                else if(mx > 443 && mx < 633 && my > 273 && my < 321){   //resume game
                    LoadGame();
                    GameState = 2;
                    NameInput = 0;
                    IsGameOver = 0;
                    typingName = 0;
                }
        }

        else if(GameState==2 && level==0 && NameInput==0){      //in difficulty page
            if(mx > 845 && mx < 980 && my > 470 && my < 577){
                GameState = 1;      //back to main menu
            }
            else if(mx > 356 && mx < 645 && my > 296 && my < 371){
                level = 1; BirdSprite.y = 250; ObstacleSprite.x = 1200; CoinSprite.x = 1200;      //easy
                IsGameOver = 0;
                SetDifficultyParameters();      
            }
            else if(mx > 356 && mx < 645 && my > 192 && my < 265){
                IsGameOver = 0;
                level = 2; BirdSprite.y = 250; ObstacleSprite.x = 1000; CoinSprite.x = 1000;     //medium
                SetDifficultyParameters();
            }
            else if(mx > 356 && mx < 645 && my > 94 && my < 166){
                IsGameOver = 0;
                level = 3; BirdSprite.y = 250; ObstacleSprite.x = 800; CoinSprite.x = 800;     //hard
                SetDifficultyParameters();
            }
        }

        else if(GameState==2 && level>0 && PausePossible==1){      //in any level
                if(mx > 924 && mx < 974 && my > 530 && my < 574){
                    pause++;
                    SaveGame();
                }
                if(pause%2!=0){
                            if(mx > 385 && mx < 694 && my > 339 && my < 395){       //continue
                                pause++;
                            }
                            else if(mx > 385 && mx < 694 && my > 277 && my < 327){      //restart
                                pause++;
                                score = 0;
                                BirdSprite.y = 250;
                                ObstacleSprite.x = 800;
                                CoinSprite.x = 800;
                                EnemySprite.x = 1200;
                                EnemySprite.y = rand() % (SCREEN_HEIGHT - 150);
                            }
                            else if(mx > 385 && mx < 694 && my > 210 && my < 252){      //exit
                                GameState = 1; level = 0; pause++;
                            }
                        }
        }

        else if(GameState==2 && level>0 && PausePossible==0){   //after game over
            if(mx > 385 && mx < 694 && my > 243 && my < 297){   //restart
                IsGameOver = 0;
                PausePossible = 1;
                score = 0;
                BirdSprite.y = 250;
                ObstacleSprite.x = 800;
                CoinSprite.x = 800;
                EnemySprite.x = 1200;
                EnemySprite.y = rand() % (SCREEN_HEIGHT - 150);
            }
            else if(mx > 385 && mx < 694 && my > 180 && my < 222){      //exit
                GameState = 1; level = 0;
                IsGameOver = 0;
                PausePossible = 1;
            }
        }
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        // place your codes here
    }
}

/*
function iMouseWheel() is called when the user scrolls the mouse wheel.
dir = 1 for up, -1 for down.
*/
void iMouseWheel(int dir, int mx, int my)
{
    // place your code here
}

/*
function iKeyboard() is called whenever the user hits a key in keyboard.
key- holds the ASCII value of the key pressed.
*/
void iKeyPress(unsigned char key) 
{
    // Name input handling 
    if (typingName && (GameState == 4 || (GameState == 2 && level == 0 && NameInput == 1))) {
        if (key == '\r') { // Enter finishes typing
            typingName = 0;
        }
        else if (key == 8) { // Backspace
            int len = strlen(playerName);
            if (len > 0) {
                playerName[len - 1] = '\0';
            }
        }
        else if (key == 27) { // ESC
            typingName = 0;
        }
        else if ((key >= 32 && key <= 126) && strlen(playerName) < 28) {
            int len = strlen(playerName);
            playerName[len] = key;
            playerName[len + 1] = '\0';
        }
        return; // Don't continue to main switch
    }

    // Other keys (game control)
    switch (key)
    {
    case '\r':
        GameState = 1;
        break;
    case ' ':
        if (GameState == 2 && level > 0 && pause % 2 == 0 && IsGameOver == 0) {
            BirdSprite.y += jumpSpeed; 
        }
        break;
    case 'p':
        if (GameState == 2 && level > 0) {
            bird_sound = 0;
            bg_music = 0;
        }
        break;
    case 'r':
        if (GameState == 2 && level > 0) {
            bird_sound = 1;
            bg_music = 1;
        }
        break;
    default:
        break;
    }
}


/*
function iSpecialKeyboard() is called whenver user hits special keys likefunction
keys, home, end, pg up, pg down, arraows etc. you have to use
appropriate constants to detect them. A list is:
GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11,
GLUT_KEY_F12, GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN,
GLUT_KEY_PAGE_UP, GLUT_KEY_PAGE_DOWN, GLUT_KEY_HOME, GLUT_KEY_END,
GLUT_KEY_INSERT */
void iSpecialKeyPress(unsigned char key)
{
    switch (key)
    {
    case GLUT_KEY_END:
        // do something
        break;
    // place your codes for other keys here
    default:
        break;
    }
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    // place your own initialization codes here.
    
    //LoadGame(); // Load existing game state if available
    InitializeScoreFiles();
    LoadSprite();
    SpriteTimer = iSetTimer(80, iAnim);

    BirdFallTimer = iSetTimer(30, SpriteFall);

    ObstacleMoveTimer = iSetTimer(10, ObstacleMove);

    EnemyMoveTimer = iSetTimer(10, EnemyMove);

    iInitializeFont();

    iInitializeSound();
    BGmusic = iPlaySound("assets/sounds/BGmusic.wav", true, 50);
    CoinSound = iPlaySound("assets/sounds/coin.wav", true, 50);

    iPauseSound(CoinSound);

    iOpenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chorui Express");
    return 0;
}