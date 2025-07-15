#include "iGraphics.h"
#include "iSound.h"
#include "iFont.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

int GameState = 0, level = 0, pause = 0, bg_music = 1, bird_sound = 1, BGmusic, IsGameOver = 0, PausePossible = 1, NameInput = 1;
Image BirdImage[6], ObstacleImage[1], EnemyImage[6];
Sprite BirdSprite, ObstacleSprite, EnemySprite;

int SpriteTimer, BirdFallTimer, ObstacleMoveTimer, EnemyMoveTimer;

int score = 0;
int highScore = 0;
int obstacleSpeed = 5;
int enemySpeed = 4;
int gravity = 5;
int jumpSpeed = 50;
char playerName[30] = "";
int typingName = 0;



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

    iInitSprite(&EnemySprite);
	iLoadFramesFromFolder(EnemyImage, "assets/images/enemy");
	iChangeSpriteFrames(&EnemySprite, EnemyImage, 6);
    iSetSpritePosition(&EnemySprite, 550, 250);
}

void SetDifficultyParameters() {
    if(level == 1){         // Easy
        obstacleSpeed = 4;
        enemySpeed = 8;
        gravity = 4;
        jumpSpeed = 50;
    }
    else if(level == 2){    // Medium
        obstacleSpeed = 5;
        enemySpeed = 10;
        gravity = 4;
        jumpSpeed = 50;
    }
    else if(level == 3){    // Hard
        obstacleSpeed = 6;
        enemySpeed = 12;
        gravity = 4;
        jumpSpeed = 50;
    }
}


void iAnim(){
	iAnimateSprite(&BirdSprite);
    iAnimateSprite(&EnemySprite);
}
void GameOver() {
    if(score > highScore) highScore = score;
    iShowImage(0, 0, "assets/images/pages/GameOverWindow.png");
    iPauseTimer(SpriteTimer);
    iPauseTimer(BirdFallTimer);
    iPauseTimer(ObstacleMoveTimer);
    iPauseTimer(EnemyMoveTimer);
    PausePossible = 0;
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
        score++;
            if(ObstacleSprite.x < -15276) ObstacleSprite.x = 1000;
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
    iShowSprite(&EnemySprite);

    int count = iCheckCollision(&BirdSprite, &ObstacleSprite) + iCheckCollision(&BirdSprite, &EnemySprite);
    int visibleCount = iGetVisiblePixelsCount(&BirdSprite);
    if (count / (20.0 * visibleCount) > 0.01) {
        IsGameOver = 1;
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

    //score and name
    iSetColor(0, 0, 0);
    char scoreText[20];
    sprintf(scoreText, "Score: %d", score);
    iText(20, 50, scoreText, GLUT_BITMAP_HELVETICA_18);

    char highScoreText[30];
    sprintf(highScoreText, "High Score: %d", highScore);
    iText(20, 20, highScoreText, GLUT_BITMAP_HELVETICA_18);

    iText(20, 80, playerName, GLUT_BITMAP_HELVETICA_18);

}

void NameInputWindow(){
    iShowImage(0, 0, "assets/images/pages/NameInput.jpg");
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
    //iText(50, 100, "Player Name:", GLUT_BITMAP_HELVETICA_18);
    iText(465, 215, playerName, GLUT_BITMAP_HELVETICA_18);

    if(typingName){
        iText(465 + 10 * strlen(playerName), 215, "|", GLUT_BITMAP_HELVETICA_18); // cursor
    }
}

void HighScore(){
    iShowImage(0, 0, "assets/images/pages/HomePage.jpg");
}

void Exit(){
    iShowImage(0, 0, "assets/images/pages/HomePage.jpg");
}

/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    iClear();

    if(bg_music == 1) iResumeSound(BGmusic);
    else if(bg_music == 0) iPauseSound(BGmusic);

    if(GameState==0){
        HomePage();
        iSetColor(0, 0, 0);

    }
    else if(GameState==1){
        MainMenuPage();
    }
    else if(GameState==2){
        if(level==0){
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
    else if(GameState==5){
        Exit();
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
                level = 0;
                pause = 0;
                IsGameOver = 0;
                PausePossible = 1;
                score = 0;
                BirdSprite.y = 250;
                BirdSprite.x = 150;
                ObstacleSprite.x = 800;
                ObstacleSprite.y = 0;
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

        }

        else if(GameState==2 && level==0){      //in difficulty page
            if(mx > 845 && mx < 980 && my > 470 && my < 577){
                GameState = 1;      //back to main menu
            }
            else if(mx > 356 && mx < 645 && my > 296 && my < 371){
                level = 1; BirdSprite.y = 250; ObstacleSprite.x = 800;      //easy
                IsGameOver = 0;
                SetDifficultyParameters();      
            }
            else if(mx > 356 && mx < 645 && my > 192 && my < 265){
                IsGameOver = 0;
                level = 2; BirdSprite.y = 250; ObstacleSprite.x = 800;      //medium
                SetDifficultyParameters();
            }
            else if(mx > 356 && mx < 645 && my > 94 && my < 166){
                IsGameOver = 0;
                level = 3; BirdSprite.y = 250; ObstacleSprite.x = 800;      //hard
                SetDifficultyParameters();
            }
        }

        /*else if(GameState==2 && level>0 && PausePossible==1 && NameInput == 1){   //name input
                if(mx > 455 && mx < 620 && my > 348 && my < 395){   //new game start
                    NameInput = 0;
                    IsGameOver = 0;
                }
                else if(mx > 443 && mx < 633 && my > 273 && my < 321){   //resume game
                    NameInput = 0;
                    IsGameOver = 0;
                }
        }*/

        else if(GameState==2 && level>0 && PausePossible==1){      //in any level
                if(mx > 924 && mx < 974 && my > 530 && my < 574){
                    pause++;
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
void iKeyboard(unsigned char key, int state)
{
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
    default:
        break;
    }
    //name input handling
    if(typingName && GameState == 4){
    if(key == '\r'){ // Enter finishes typing
        typingName = 0;
    }
    else if((key >= 32 && key <= 126) && strlen(playerName) < 28){
        int len = strlen(playerName);
        playerName[len] = key;
        playerName[len+1] = '\0';
    }
    }
    else if(key == 8 && typingName && GameState == 4){ // Backspace
    int len = strlen(playerName);
    if(len > 0){
        playerName[len-1] = '\0';
    }
    }
    else if(key == 27){ // ESC
            typingName = 0;
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
void iSpecialKeyboard(unsigned char key, int state)
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

    LoadSprite();
    SpriteTimer = iSetTimer(100, iAnim);

    BirdFallTimer = iSetTimer(30, SpriteFall);

    ObstacleMoveTimer = iSetTimer(1, ObstacleMove);

    EnemyMoveTimer = iSetTimer(10, EnemyMove);

    iInitializeFont();

    iInitializeSound();
    BGmusic = iPlaySound("assets/sounds/BGmusic.wav", true, 50);

    iOpenWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chorui Express");
    return 0;
}