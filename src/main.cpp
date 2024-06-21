#include <iostream>
#include <raylib.h>

int player_score;
int cpu_score;

//Global Colors
Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

class Ball{
    public:
    float x,y;
    int speed_x, speed_y;
    int radius;

void Draw()
{
    DrawCircle(x, y, radius, Yellow);
}

void Update(){
    x += speed_x;
    y += speed_y;

    if (y + radius >= GetScreenHeight() || y - radius <= 0)
    {
        speed_y *= -1;
    }

    if (x + radius >= GetScreenWidth())
    {
        cpu_score++;
        ResetBall();
    }

    if (x - radius <= 0)
    {
        player_score++;
        ResetBall();
    }
}

void ResetBall()
{
    x = GetScreenWidth()/2;
    y = GetScreenHeight()/2;
    int speed_choices[2] = {-1,1};
    speed_x *= speed_choices[GetRandomValue(0,1)];
    speed_y *= speed_choices[GetRandomValue(0,1)];
}
};

class Paddle {
    protected:
    void LimitMovement()
    {
        if (y <= 0)
        {
            y = 0;
        }

        if (y + height >= GetScreenHeight())
        {
            y = GetScreenHeight() - height;
        }
    }
    
    public:
    float x, y;
    float width, height;
    int speed;

    void Draw()
    {
        DrawRectangleRounded(Rectangle{x, y, width, height}, .8, 0, WHITE);
    }

    void Update()
    {
        if (IsKeyDown(KEY_UP))
        {
            y -= speed;
        }

        if (IsKeyDown(KEY_DOWN))
        {
            y += speed;
        }

        LimitMovement();
    }
};

class CpuPaddle : public Paddle 
{
    public:

    void Update(int ball_y)
    {
        if (y + height/2 > ball_y)
        {
            y -= speed;
        }
        if (y + height/2 <= ball_y)
        {
            y += speed;
        }

        LimitMovement();
    }
};

Ball b;
Paddle player;
CpuPaddle cpu;

int main()
{
    std::cout << "Starting Game" << std::endl;
    const int screen_width = 1280;
    const int screen_height = 800;
    InitWindow(screen_width, screen_height, "Pong");
    SetTargetFPS(60);

    b.radius = 20;
    b.x = screen_width / 2;
    b.y = screen_height / 2;
    b.speed_x = 7;
    b.speed_y = 7;

    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width -10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    cpu.width = 25;
    cpu.height = 120;
    cpu.x = 10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;


    while (WindowShouldClose() == false)
    {
        BeginDrawing();

        //Updating
        b.Update();
        player.Update();
        cpu.Update(b.y);

        //Collisions
        if (CheckCollisionCircleRec(Vector2{b.x, b.y}, b.radius, Rectangle{player.x, player.y, player.width, player.height}))
        {
            b.speed_x *= -1;
        }

        if (CheckCollisionCircleRec(Vector2{b.x, b.y}, b.radius, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}))
        {
            b.speed_x *= -1;
        }

        //Drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2, 0, screen_width/2, screen_height, Green);
        DrawCircle(screen_width/2, screen_height/2, 150, Light_Green);

        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);
        
        b.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3 * screen_width/4 - 20, 20, 80, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}