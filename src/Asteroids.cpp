#include "Asteroids.h"
#include "framework/GameObjectRect.h"
#include <raymath.h>

namespace AsteroidsSettings
{
constexpr int PLAYER_WIDTH = 25;
constexpr int PLAYER_HEIGHT = 25;
constexpr int PLAYER_SPEED = 180;
constexpr int PLAYER_ACCELERATION = 60;
constexpr int BULLETS_POOL_SIZE = 10;
constexpr int BULLET_SPEED = 600;
constexpr int ROCKS_POOL_SIZE = 10;
constexpr int ROCKS_START_COUNT = 5;
constexpr int ROCKS_SPAWN_INTERVAL = 2;
constexpr int ROCK_SPEED = 100;
}  // namespace AsteroidsSettings

// Bullet implementation
void Bullet::Init(const Color& color, float radius, float screenWidth, float screenHeight)
{
    SetActive(false);
    SetColor(color);
    SetRadius(radius);

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}

void Bullet::Spawn(Vector2 position, Vector2 direction)
{
    SetActive(true);
    SetPosition(position);
    m_direction = direction;
}

void Bullet::Update(float deltaTime)
{
    if (IsActive())
    {
        Vector2 bulletPosition = GetPosition();
        bulletPosition = Vector2Add(bulletPosition, Vector2Scale(m_direction, AsteroidsSettings::BULLET_SPEED * deltaTime));
        SetPosition(bulletPosition);

        if (bulletPosition.x > m_screenWidth || bulletPosition.x < 0 || bulletPosition.y > m_screenHeight || bulletPosition.y < 0)
        {
            SetActive(false);
        }
    }
}

// Rock implementation
void Rock::Init(const Color& color, float screenWidth, float screenHeight)
{
    SetActive(false);
    SetColor(color);

    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
}

void Rock::Spawn(int size, Vector2 position, Vector2 direction)
{
    SetActive(true);

    m_size = size;
    SetRadius(size * 10);

    SetPosition(position);
    m_direction = direction;
}

void Rock::Update(float deltaTime)
{
    if (IsActive())
    {
        Vector2 rockPosition = GetPosition();
        rockPosition = Vector2Add(rockPosition, Vector2Scale(m_direction, AsteroidsSettings::ROCK_SPEED * deltaTime));

        // Wrap around screen
        const float radius = GetRadius();
        if (rockPosition.x - radius > m_screenWidth)
        {
            rockPosition.x = 0;
        }
        else if (rockPosition.x + radius < 0)
        {
            rockPosition.x = m_screenWidth;
        }

        if (rockPosition.y - radius > m_screenHeight)
        {
            rockPosition.y = 0;
        }
        else if (rockPosition.y + radius < 0)
        {
            rockPosition.y = m_screenHeight;
        }

        SetPosition(rockPosition);
    }
}

void Rock::Hit()
{
    m_size -= 1;

    if (m_size <= 0)
    {
        SetActive(false);
    }
    else
    {
        SetRadius(m_size * 10);
    }
}

// Game implementation
void Asteroids::InitGame()
{
    m_screenWidth = GetScreenWidth();
    m_screenHeight = GetScreenHeight();

    m_player = new GameObjectRect(ORANGE, AsteroidsSettings::PLAYER_WIDTH, AsteroidsSettings::PLAYER_HEIGHT);
    m_player->SetPosition({m_screenWidth / 2.f - AsteroidsSettings::PLAYER_WIDTH / 2.f, m_screenHeight / 2.f});

    m_bullets = new Bullet[AsteroidsSettings::BULLETS_POOL_SIZE];
    for (int i = 0; i < AsteroidsSettings::BULLETS_POOL_SIZE; ++i)
    {
        m_bullets[i].Init(BLUE, 6, m_screenWidth, m_screenHeight);
    }

    m_rocks = new Rock[AsteroidsSettings::ROCKS_POOL_SIZE];
    for (int i = 0; i < AsteroidsSettings::ROCKS_POOL_SIZE; ++i)
    {
        m_rocks[i].Init(DARKGRAY, m_screenWidth, m_screenHeight);
    }

    for (int i = 0; i < AsteroidsSettings::ROCKS_START_COUNT; ++i)
    {
        SpawnRock();
    }
}

void Asteroids::UpdateGame(float deltaTime)
{
    m_time += deltaTime;

    if (m_isGameOver)
    {
        const float currentTime = GetTime();
        if (currentTime - m_startTimer >= m_startDelay)
        {
            ResetGame();
        }
    }

    // Rotate player towards mouse
    Vector2 mousePosition = GetMousePosition();
    Vector2 playerPosition = m_player->GetPosition();
    Vector2 direction = Vector2Normalize(Vector2Subtract(mousePosition, playerPosition));
    float angle = atan2f(direction.y, direction.x);
    m_player->SetRotation(angle * RAD2DEG - 90);

    // Update player movement and speed
    Vector2 movementDirection = GetMovementDirection();
    if (Vector2Length(movementDirection) > 0)
    {
        m_lastMovementDirection = movementDirection;
        m_isMoving = true;
    }
    else
    {
        m_isMoving = false;
    }

    if (m_isMoving)
    {
        m_currentSpeed += AsteroidsSettings::PLAYER_ACCELERATION * deltaTime;
        if (m_currentSpeed > AsteroidsSettings::PLAYER_SPEED)
        {
            m_currentSpeed = AsteroidsSettings::PLAYER_SPEED;
        }
    }
    else
    {
        m_currentSpeed -= AsteroidsSettings::PLAYER_ACCELERATION * deltaTime;
        if (m_currentSpeed < 0)
        {
            m_currentSpeed = 0;
        }
    }

    // Update player position
    Vector2 playerMovement = Vector2Scale(m_lastMovementDirection, m_currentSpeed);
    Vector2 newPosition = Vector2Add(playerPosition, Vector2Scale(playerMovement, deltaTime));

    // Wrap around screen
    if (newPosition.x > m_screenWidth)
    {
        newPosition.x = 0;
    }
    else if (newPosition.x < 0)
    {
        newPosition.x = m_screenWidth;
    }

    if (newPosition.y > m_screenHeight)
    {
        newPosition.y = 0;
    }
    else if (newPosition.y < 0)
    {
        newPosition.y = m_screenHeight;
    }

    m_player->SetPosition(newPosition);

    // Update rocks
    if (m_time - m_lastSpawnTime > AsteroidsSettings::ROCKS_SPAWN_INTERVAL)
    {
        m_lastSpawnTime = m_time;
        SpawnRock();
    }

    for (int i = 0; i < AsteroidsSettings::ROCKS_POOL_SIZE; ++i)
    {
        m_rocks[i].Update(deltaTime);
    }

    // Update bullets
    if (m_player->IsActive() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        SpawnBullet(playerPosition, direction);
    }

    for (int i = 0; i < AsteroidsSettings::BULLETS_POOL_SIZE; ++i)
    {
        m_bullets[i].Update(deltaTime);
    }

    // Handle collisions
    for (int i = 0; i < AsteroidsSettings::ROCKS_POOL_SIZE; ++i)
    {
        Rock& rock = m_rocks[i];

        if (rock.IsActive())
        {
            if (CheckCollisionCircleRec(rock.GetPosition(), rock.GetRadius(), m_player->GetRectangle()))
            {
                GameOver();
            }

            for (int j = 0; j < AsteroidsSettings::BULLETS_POOL_SIZE; ++j)
            {
                Bullet& bullet = m_bullets[j];

                if (bullet.IsActive() && CheckCollisionCircles(rock.GetPosition(), rock.GetRadius(), bullet.GetPosition(), bullet.GetRadius()))
                {
                    rock.Hit();

                    if (rock.GetSize() == 0)
                    {
                        ++m_score;
                    }

                    bullet.SetActive(false);
                }
            }
        }
    }
}

void Asteroids::DrawGame()
{
    BeginDrawing();

    // Draw playground
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("%d", m_score), m_screenWidth / 2, 30, 40, LIGHTGRAY);

    // Draw the game objects
    for (int i = 0; i < AsteroidsSettings::ROCKS_POOL_SIZE; ++i)
    {
        m_rocks[i].Draw();
    }

    for (int i = 0; i < AsteroidsSettings::BULLETS_POOL_SIZE; ++i)
    {
        m_bullets[i].Draw();
    }

    m_player->Draw();

    EndDrawing();
}

void Asteroids::UnloadGame()
{
    delete m_player;
    delete[] m_bullets;
    delete[] m_rocks;
}

Vector2 Asteroids::GetMovementDirection() const
{
    Vector2 direction = {0, 0};

    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        direction.y = -1;
    }

    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        direction.y = 1;
    }

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        direction.x = -1;
    }

    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        direction.x = 1;
    }

    return Vector2Normalize(direction);
}

void Asteroids::SpawnBullet(Vector2 position, Vector2 direction)
{
    Bullet* freeBullet = nullptr;

    for (int i = 0; i < AsteroidsSettings::BULLETS_POOL_SIZE; ++i)
    {
        if (!m_bullets[i].IsActive())
        {
            freeBullet = &m_bullets[i];
            break;
        }
    }

    if (freeBullet != nullptr)
    {
        freeBullet->Spawn(position, direction);
    }
}

void Asteroids::SpawnRock()
{
    Rock* freeRock = nullptr;

    for (int i = 0; i < AsteroidsSettings::ROCKS_POOL_SIZE; ++i)
    {
        if (!m_rocks[i].IsActive())
        {
            freeRock = &m_rocks[i];
            break;
        }
    }

    if (freeRock != nullptr)
    {
        const int size = GetRandomValue(1, 3);
        const Vector2 position = {static_cast<float>(GetRandomValue(0, static_cast<int>(m_screenWidth))), static_cast<float>(GetRandomValue(0, static_cast<int>(m_screenHeight)))};
        const Vector2 direction = {static_cast<float>(GetRandomValue(-1, 1)), static_cast<float>(GetRandomValue(-1, 1))};
        freeRock->Spawn(size, position, Vector2Normalize(direction));
    }
}

void Asteroids::GameOver()
{
    m_isGameOver = true;
    m_player->SetActive(false);
    m_startTimer = GetTime();
}

void Asteroids::ResetGame()
{
    m_isGameOver = false;
    m_score = 0;
    m_lastMovementDirection = {0.f, 0.f};
    m_player->SetActive(true);
    m_player->SetPosition({m_screenWidth / 2.f - AsteroidsSettings::PLAYER_WIDTH / 2.f, m_screenHeight / 2.f});
}