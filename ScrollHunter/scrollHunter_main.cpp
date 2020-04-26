#include "engine.h"
#include "game.h"
#include "scenes/scene_menu.h"

using namespace std;

MenuScene menu;
GameOver gameOver;
VictoryScene victory;
HelpScene help;
Level1Scene level1;
Level2Scene level2;
Level3Scene level3;
Level4Scene finalScene;

int main() {
  Engine::Start(1920, 1080, "Scroll Hunter",&menu);
}