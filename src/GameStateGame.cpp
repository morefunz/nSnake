#include "GameStateGame.hpp"
#include "Input.hpp"
#include "Ncurses.hpp"

GameStateGame::GameStateGame():
    player(NULL),
    board(NULL),
    gameTimer(NULL),
    foods(NULL)
{ }
GameStateGame::~GameStateGame()
{ }
void GameStateGame::load(int stack)
{
    UNUSED(stack);

    this->board = new Board(80, 23);
//    this->board->setBorders(true);
    this->board->loadFile("levels/00.nsnake");

    this->foods = new FoodManager(this->board);
    this->foods->addAtRandom();

    this->player = new Snake(this->board);

    // Again, this is the game speed.
    // This is the timer that says when the snake will be
    // updated.
    this->gameTimer = new TimerCounter(100);
    this->gameTimer->startCounting();
}
int GameStateGame::unload()
{
// Time to delete!
// This macro deletes a thing only if it's non-NULL,
// making sure it won't double-delete things.
#define safe_delete(x) \
{                      \
    if (x)             \
    {                  \
        delete (x);    \
        x = NULL;      \
    }                  \
}

    safe_delete(this->player);
    safe_delete(this->board);
    return 0;
}
GameState::StateCode GameStateGame::update(float dt)
{
    UNUSED(dt);

    Input* input = Input::getInstance();
    input->update();

    if (input->isKeyDown('q')) // quit the game
        return GameState::QUIT;

    if (input->isKeyDown('r')) // restart the game!
        return GameState::GAME_START;

    if (input->isKeyDown('i')) // increase the player
        this->player->eatFood();

//    this->foods->update();
    this->player->update();

    // If the game speed time has passed, we'll force
    // the snake to move.
    //
    // Note that we always check for input on Snake::update(),
    // so don't worry about that.
    if (this->gameTimer->isDone())
    {
        this->player->move();
        this->player->checkCollision();
        this->foods->update();
        this->gameTimer->startCounting();
    }

    return GameState::CONTINUE;
}
void GameStateGame::render()
{
    Ncurses::setStyle(Color::pair("cyan"));
    Ncurses::print("nSnake v2.0", 0, 0);

    Ncurses::print("score:", 20, 0);
    Ncurses::print(Ncurses::intToString(this->player->getScore()),
                   28, 0);

    Ncurses::setStyle(Color::pair("magenta"));
    Ncurses::print("| <q> quit | <r> restart |", 50, 0);

    this->board->render(0, 1);
}

