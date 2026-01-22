#include "raylib.h"

struct Brick {
    Rectangle rect;
    bool active;
};

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Simple Breakout");
    SetTargetFPS(60);

    Rectangle paddle = { 350, 550, 100, 20 };
    float paddleSpeed = 10.0f;
    Vector2 ball = { 400, 450 };
    Vector2 ballSpeed = { 5, -5 };
    float ballRadius = 10;

    bool gameOver = false;
    bool gameStarted = false;
    int score = 0;
    
    Rectangle startButton = { screenWidth/2 - 50, screenHeight/2 - 25, 100, 50 };

    const int ROWS = 5;
    const int COLS = 10;
    Brick bricks[ROWS][COLS];

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            bricks[i][j].rect.x = j * 80.0f;
            bricks[i][j].rect.y = i * 40.0f + 50;
            bricks[i][j].rect.width = 75.0f;
            bricks[i][j].rect.height = 30.0f;
            bricks[i][j].active = true;
        }
    }

    while (!WindowShouldClose()) {
        if (!gameStarted) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(GetMousePosition(), startButton)) {
                    gameStarted = true;
                }
            }
        }
        else if (!gameOver) {
            if (IsKeyDown(KEY_LEFT)) paddle.x -= paddleSpeed;
            if (IsKeyDown(KEY_RIGHT)) paddle.x += paddleSpeed;

            if (paddle.x < 0) paddle.x = 0;
            if (paddle.x > screenWidth - paddle.width) paddle.x = screenWidth - paddle.width;

            ball.x += ballSpeed.x;
            ball.y += ballSpeed.y;

            if (ball.x >= screenWidth || ball.x <= 0) ballSpeed.x *= -1;
            if (ball.y <= 0) ballSpeed.y *= -1;

            if (ball.y >= screenHeight) {
                gameOver = true;
            }

            if (CheckCollisionCircleRec(ball, ballRadius, paddle)) {
                ballSpeed.y *= -1;
            }

            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (bricks[i][j].active) {
                        if (CheckCollisionCircleRec(ball, ballRadius, bricks[i][j].rect)) {
                            bricks[i][j].active = false;
                            ballSpeed.y *= -1;
                            score += 10;
                        }
                    }
                }
            }
        }
        else {
            if (IsKeyPressed(KEY_R)) {
                gameOver = false;
                gameStarted = false;
                ball = { 400, 450 };
                ballSpeed = { 5, -5 };
                score = 0;
                paddle.x = 350;

                for (int i = 0; i < ROWS; i++) {
                    for (int j = 0; j < COLS; j++) {
                        bricks[i][j].active = true;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!gameStarted) {
            DrawRectangleRec(startButton, BLACK);
            DrawText("START", startButton.x + 20, startButton.y + 15, 20, WHITE);
        }
        else if (!gameOver) {
            for (int i = 0; i < ROWS; i++) {
                for (int j = 0; j < COLS; j++) {
                    if (bricks[i][j].active) {
                        Color color = (i % 2 == 0) ? BLACK : GRAY;
                        DrawRectangleRec(bricks[i][j].rect, color);
                    }
                }
            }

            DrawRectangleRec(paddle, BLACK);
            DrawCircleV(ball, ballRadius, BLACK);
            
            DrawText(TextFormat("Score: %i", score), 10, 10, 20, DARKGRAY);
        }
        else {
            const char* text = "GAME OVER";
            int textWidth = MeasureText(text, 40);
            DrawText(text, screenWidth/2 - textWidth/2, screenHeight/2 - 50, 40, BLACK);
            
            const char* subText = "Press 'R' to Restart";
            int subWidth = MeasureText(subText, 20);
            DrawText(subText, screenWidth/2 - subWidth/2, screenHeight/2 + 50, 20, DARKGRAY);

            const char* scoreText = TextFormat("Final Score: %i", score);
            int scoreWidth = MeasureText(scoreText, 30);
            DrawText(scoreText, screenWidth/2 - scoreWidth/2, screenHeight/2, 30, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}