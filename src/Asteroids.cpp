#include "Asteroids.h"
#include "framework/GameObjectRect.h"
#include <raymath.h>

namespace AsteroidsSettings
{
constexpr int PLAYER_WIDTH = 20;
constexpr int PLAYER_HEIGHT = 40;
constexpr int PLAYER_SPEED = 180;
constexpr int PLAYER_ACCELERATION = 60;
constexpr int BULLETS_POOL_SIZE = 10;
constexpr int BULLET_SPEED = 600;
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

void Bullet::Shoot(Vector2 position, Vector2 direction)
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
}

void Asteroids::UpdateGame(float deltaTime)
{
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

    // Shoot bullet
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        ShootBullet(playerPosition, direction);
    }

    // Update bullets
    for (int i = 0; i < AsteroidsSettings::BULLETS_POOL_SIZE; ++i)
    {
        m_bullets[i].Update(deltaTime);
    }
}

void Asteroids::DrawGame()
{
    BeginDrawing();

    // Draw playground
    ClearBackground(RAYWHITE);

    // Draw the game objects
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

void Asteroids::ShootBullet(Vector2 position, Vector2 direction)
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
        freeBullet->Shoot(position, direction);
    }
}
