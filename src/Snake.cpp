#include "Snake.h"
#include "framework/GameObjectCircle.h"
#include "framework/GameObjectRect.h"
#include <raygui.h>
#include <raymath.h>

namespace SnakeSettings
{
constexpr int PLAYER_RADIUS = 20;
constexpr int PLAYER_SPEED = 200;
constexpr int TRAIL_POOL_SIZE = 100;
constexpr int START_TRAIL_COUNT = 0;
}

Snake::Snake(ArenaAllocator& arena) : GameBase(arena)
{
    m_player = m_arena.Allocate<GameObjectCircle>(SKYBLUE, SnakeSettings::PLAYER_RADIUS);
    m_trail = m_arena.AllocateArray<GameObjectCircle>(SnakeSettings::TRAIL_POOL_SIZE, SKYBLUE, SnakeSettings::PLAYER_RADIUS - 3.f);
    m_pickup = m_arena.Allocate<GameObjectRect>(RED, 25.0f, 25.0f);

    ResetGame();
}

void Snake::UpdateGame(float deltaTime)
{
    m_time += deltaTime;

    if (m_isGameOver)
    {
        const float currentTime = GetTime();
        if (currentTime - m_startTimer >= m_startDelay)
        {
            ResetGame();
        }
        else
        {
            return;
        }
    }

    Vector2 mousePosition = GetMousePosition();
    Vector2 playerPosition = m_player->GetPosition();
    Vector2 direction = Vector2Normalize(Vector2Subtract(mousePosition, playerPosition));
    Vector2 offset = Vector2Scale(direction, SnakeSettings::PLAYER_SPEED * deltaTime);

    playerPosition = Vector2Add(playerPosition, offset);
    if (Vector2Distance(mousePosition, playerPosition) > 5.f)
    {
        m_player->SetPosition(playerPosition);
    }

    GameObjectCircle* prevPart = m_player;
    for (size_t i = 0; i < SnakeSettings::TRAIL_POOL_SIZE; ++i)
    {
        if (m_trail[i].IsActive())
        {
            Vector2 prevPosition = prevPart->GetPosition();
            Vector2 currPosition = m_trail[i].GetPosition();
            Vector2 dir = Vector2Normalize(Vector2Subtract(prevPosition, currPosition));
            currPosition = Vector2Add(currPosition, Vector2Scale(dir, SnakeSettings::PLAYER_SPEED * deltaTime));
            if (Vector2Distance(prevPosition, currPosition) > SnakeSettings::PLAYER_RADIUS * 2.f)
            {
                m_trail[i].SetPosition(currPosition);
            }

            prevPart = &m_trail[i];
        }
        else
        {
            break;
        }
    }

    // Check collisions
    if (CheckCollisionCircleRec(playerPosition, SnakeSettings::PLAYER_RADIUS, m_pickup->GetRectangle()))
    {
        CollectPickup();
    }

    // Check if player collides with the trail, ignore the first part
    for (size_t i = 1; i < SnakeSettings::TRAIL_POOL_SIZE; ++i)
    {
        if (m_trail[i].IsActive() && CheckCollisionCircles(m_player->GetPosition(), m_player->GetRadius(), m_trail[i].GetPosition(), m_trail[i].GetRadius()))
        {
            GameOver();
            break;
        }
    }
}

void Snake::DrawGame()
{
    BeginDrawing();

    // Draw playground
    ClearBackground(RAYWHITE);
    DrawText(TextFormat("Score: %d", m_score), 10, 10, 30, LIGHTGRAY);

    // Draw game objects
    m_player->Draw();

    for (size_t i = 0; i < SnakeSettings::TRAIL_POOL_SIZE; ++i)
    {
        m_trail[i].Draw();
    }

    m_pickup->Draw();

    // Draw close button
    if (GuiButton({m_screenWidth - 50, 20, 30, 30}, "x"))
    {
        m_shouldClose = true;
    }

    EndDrawing();
}

Vector2 Snake::GetRandomPosition() const
{
    return {static_cast<float>(GetRandomValue(0, static_cast<int>(m_screenWidth))), static_cast<float>(GetRandomValue(0, static_cast<int>(m_screenHeight)))};
}

void Snake::ResetGame()
{
    m_isGameOver = false;
    m_score = 0;

    m_player->SetPosition({m_screenWidth / 2.f, m_screenHeight / 2.f});
    m_player->SetColor(SKYBLUE);

    m_trailCount = SnakeSettings::START_TRAIL_COUNT;
    for (size_t i = 0; i < SnakeSettings::TRAIL_POOL_SIZE; ++i)
    {
        if (i < m_trailCount)
        {
            m_trail[i].SetActive(true);
            m_trail[i].SetPosition({m_screenWidth / 2.f, m_screenHeight / 2.f - (i * 2.f * SnakeSettings::PLAYER_RADIUS)});
        }
        else
        {
            m_trail[i].SetActive(false);
        }
    }

    m_pickup->SetPosition(GetRandomPosition());
    m_pickup->SetRotation(45.f);
    m_pickup->SetActive(true);
}

void Snake::CollectPickup()
{
    m_trailCount++;

    if (m_trailCount % 10 == 0)
    {
        m_score += 1100;
    }
    else
    {
        m_score += 100;
    }

    if (m_trailCount > SnakeSettings::TRAIL_POOL_SIZE)
    {
        m_trailCount = SnakeSettings::TRAIL_POOL_SIZE;
    }

    for (size_t i = 0; i < SnakeSettings::TRAIL_POOL_SIZE; ++i)
    {
        if (i < m_trailCount)
        {
            m_trail[i].SetActive(true);
            m_trail[i].SetColor(SKYBLUE);
        }
        else
        {
            m_trail[i].SetActive(false);
        }

        if (i == m_trailCount - 1)
        {
            if (i > 0)
            {
                m_trail[i].SetPosition(m_trail[i - 1].GetPosition());
            }
            else
            {
                m_trail[i].SetPosition(m_player->GetPosition());
            }
        }
    }

    m_pickup->SetPosition(GetRandomPosition());
}

void Snake::GameOver()
{
    m_isGameOver = true;

    m_player->SetColor(BLACK);

    for (size_t i = 0; i < SnakeSettings::TRAIL_POOL_SIZE; ++i)
    {
        m_trail[i].SetColor(BLACK);
    }

    m_startTimer = GetTime();
}