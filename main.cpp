#include <iostream>
#include <raylib.h>

using namespace std;

static int playerWins = 0;
static int aiWins = 0;

typedef struct Window_Config {
    const int WIDTH = 600;
    const int HEIGHT = 400;
    const string TITLE = "Pong Game";
    Color bgColor = {38,185,154,255};
} Config;

class Ball {
public:
    float speed_x;
    float speed_y;
    float radius;
    float centerX;
    float centerY;

    Color ballColor = {255,0,0,255};

    Ball(const float speed_x, const float speed_y, const float radius, const float centerX, const float centerY) {
        this->speed_x = speed_x;
        this->speed_y = speed_y;
        this->radius = radius;
        this->centerX = centerX;
        this->centerY = centerY;
    }

    void Draw() {
        DrawCircle(centerX, centerY,radius, ballColor);
    }
    void setCenterX(float centerX) {
        this->centerX = centerX;
    }
    void setCenterY(float centerY) {
        this->centerY = centerY;
    }

    void moveBall() {
        centerX += speed_x;
        centerY += speed_y;

        if (centerY + radius >= GetScreenHeight() || centerY - radius <= 0) {
            speed_y *= -1;
        }

        if (centerX + radius >= GetScreenWidth()) {
            playerWins++;
            resetBall();
        }

        if (centerX - radius <= 0) {
            aiWins++;
            resetBall();
        }
    }
    void resetBall() {
        centerX = GetScreenWidth() / 2;
        centerY = GetScreenHeight() / 2;

        int choices[] = {1,-1};
        speed_x *= choices[GetRandomValue(0,1)];
        speed_y *= choices[GetRandomValue(0,1)];
    }
};

class Paddle {
public:
    const float width = 15;
    const float height = 75;
    float posX;
    float posY;
    const float paddleSpeed = 7;
    Color paddleColor= WHITE;

    Paddle(float posX, float posY) : posX(posX), posY(posY) {
        this->posX = posX;
        this->posY = posY;
    }

    void Draw() {
        DrawRectangle(posX,posY,width, height, paddleColor);

    }
    void movePaddle(const int WINDOW_HEIGHT) {
        if (IsKeyDown(KEY_DOWN)) {
            posY+=paddleSpeed;
        }
        else if (IsKeyDown(KEY_UP)) {
            posY-=paddleSpeed;
        }

        //check for collision between paddle and frame
        if (posY < 0) {
            posY = 0;
        }
        if (posY + height >= GetScreenHeight()) {
            posY = GetScreenHeight() - height;
        }
    }

    void displayWin() {
        DrawText(TextFormat("%i", playerWins), GetScreenWidth() /2 - 100, 10, 20, GRAY);
    }

};

class AiPaddle : public Paddle {
public:
    AiPaddle(float posX, float posY) : Paddle(posX,posY) {}
    void movePaddle(const float ballY) {
        if (posY + height / 2 <= ballY) {
            posY += paddleSpeed;
        }

        if (posY + height / 2  > ballY) {
            posY -= paddleSpeed;
        }
    }

    void displayWin() {
        DrawText(TextFormat("%i", aiWins), GetScreenWidth() / 2 + 100, 10, 20, GRAY);
    }
};
int main() {
    //Init Window
    Config c;
    InitWindow(c.WIDTH,c.HEIGHT,c.TITLE.c_str());
    SetTargetFPS(60);

    Color Dark_Green = {20,160,133,255};
    Color Light_Green = {129,204,184,255};

    const int ballSpeed = 3; // 7m/s
    const int radius = 15; // 15m
    Ball b(ballSpeed,ballSpeed, radius, c.WIDTH/2, c.HEIGHT/2);

    const int paddleWidth = 15;
    const int paddleHeight = 75;
    const int spacing = 10;

    Color circleDesign = {150,204,184,200};

    Paddle playerPaddle(c.WIDTH - paddleWidth  - spacing, c.HEIGHT/2 - paddleHeight / 2);
    AiPaddle aiPaddle(0 + spacing, c.HEIGHT /2 - paddleHeight / 2);
    //Game LOOP
    while (!WindowShouldClose()) {
        // 1. Event Handling
        //check for collision between paddle and window frame
        playerPaddle.movePaddle(c.HEIGHT);
        aiPaddle.movePaddle(b.centerY);

        // 2. Updating Position
        b.moveBall();
        //check collision between ball and paddle - player
        if (CheckCollisionCircleRec(Vector2{b.centerX,b.centerY},b.radius, Rectangle{playerPaddle.posX,playerPaddle.posY,playerPaddle.width,playerPaddle.height})) {
            b.speed_x *= -1;

        }
        //check collision between ball and paddle - ai
        if (CheckCollisionCircleRec(Vector2{b.centerX,b.centerY},b.radius, Rectangle{aiPaddle.posX,aiPaddle.posY,aiPaddle.width,aiPaddle.height})) {
            b.speed_x *= -1;
        }

        // 3. Drawing Objects
        BeginDrawing();
        ClearBackground(c.bgColor); // ensure last changes is updated
        //DESIGN
        DrawLine(c.WIDTH/2,0,c.WIDTH/2,c.HEIGHT, WHITE);
        DrawRectangle(0,0,c.WIDTH/2,c.HEIGHT,Dark_Green);
        DrawRectangle(c.WIDTH/2,0,c.WIDTH/2,c.HEIGHT,Light_Green);
        DrawCircle(c.WIDTH/2,c.HEIGHT/2, 80,circleDesign);

        // 1. DRAW BALL
        b.Draw();
        // 2. DRAW PADDLE
            //i. Draw playerPaddle
        playerPaddle.Draw();
            //ii. Draw aiPaddle
        aiPaddle.Draw();

        // DRAW WINS
        playerPaddle.displayWin();
        aiPaddle.displayWin();


        EndDrawing();
    }
    //Close Window
    CloseWindow();
    return 0;
}