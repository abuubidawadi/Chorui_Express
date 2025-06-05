#include "iGraphics.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 600

int GameState=0, bg_music=1, bird_sound=1;

void HomePage(){
    iShowImage(0, 0, "HomePage.jpg");
    iSetColor(0, 0, 0);
    iText(350, 100, "Press Enter to go to Main Menu", GLUT_BITMAP_TIMES_ROMAN_24);
}

void MainMenuPage(){
    iShowImage(0, 0, "MainMenuPage.jpg");
}

void GameStart(){
    iShowImage(0, 0, "HomePage.jpg");
}

void Instructions(){
    iShowImage(0, 0, "Instructions.jpg");
}

void Settings(){
    if(bg_music==1 && bird_sound==1){
        iShowImage(0, 0, "Settings_on_on.jpg");
    }
    else if(bg_music==0 && bird_sound==1){
        iShowImage(0, 0, "Settings_off_on.jpg");
    }
    else if(bg_music==1 && bird_sound==0){
        iShowImage(0, 0, "Settings_on_off.jpg");
    }
    else if(bg_music==0 && bird_sound==0){
        iShowImage(0, 0, "Settings_off_off.jpg");
    }
}

void HighScore(){
    iShowImage(0, 0, "HomePage.jpg");
}

void Exit(){
    iShowImage(0, 0, "HomePage.jpg");
}

/*
function iDraw() is called again and again by the system.
*/
void iDraw()
{
    iClear();

    if(GameState==0){
        HomePage();
    }
    else if(GameState==1){
        MainMenuPage();
    }
    else if(GameState==2){
        GameStart();
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
    // place your codes here
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
                GameState = 2;      //start game
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
void iKeyboard(unsigned char key)
{
    switch (key)
    {
    case '\r':
        GameState = 1;
        break;
    // place your codes for other keys here
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
void iSpecialKeyboard(unsigned char key)
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
    iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Chorui Express");
    return 0;
}