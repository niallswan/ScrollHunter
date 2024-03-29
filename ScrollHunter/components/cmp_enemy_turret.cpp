#include "cmp_enemy_turret.h"
#include "cmp_bullet.h"
#include "cmp_hurt_player.h"
#include "cmp_spritesheet.h"
#include "engine.h"
#include <SFML/Graphics/CircleShape.hpp>
#include "cmp_bullet_physics.h"
#include <LevelSystem.h>
#include "cmp_aimed_bullet_physics.h"
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;

auto walls = ls::findTiles(ls::WALL);
static Texture tex;
static shared_ptr<Entity> player;

static int bossBullet = 1;
static int amount = 0;

static Vector2f bossPosOrigin = Vector2f(2460, 510);

//sounds
static Sound cast;
static SoundBuffer castB;

//fire Bullets
void EnemyTurretComponent::update(double dt) 
{                   
    //while player is close to enemies
    if (_parent->getPosition().x > player->getPosition().x - 960 && _parent->getPosition().x < player->getPosition().x + 960)
    {
        //Fire every 2 seconds
        _firetime -= dt;
        if (_firetime <= 0.f)
        {
            fire();

            //If boss, make him fire faster depending on position
            if (_parent->getType() == "boss")
            {
                if (length(bossPosOrigin - _parent->getPosition()) < 10.0)
                {
                    _firetime = 0.1f;
                }
                else
                {
                    _firetime = 0.5f;
                }
            }
            else
            {
                _firetime = 2.0f;
            }
        }
    }

  static float angle = 0.f;
  angle += 1.f * dt;

  _parent->setRotation(270.f + sin(angle) * 45.f);
}

void EnemyTurretComponent::fire() const 
{
	cast.stop();
	cast.play();

    //Create new bullet, get position from parent that fired it and add hurt component, tag it as bullet also
	auto bullet = _parent->scene->makeEntity();
	bullet->setPosition(_parent->getPosition());
	bullet->addComponent<HurtComponent>();
	bullet->addTag("bullet");

    //If bullets parent is an archer set up it's spritesheet
    if (_parent->getType() == "archer")
    {
        bullet->addComponent<AimedBulletComponent>();
	      auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
	      tex.loadFromFile("res/archerAttack.png");
	      s->setSpritesheet(tex);
	      s->setFrameCount(8);
	      s->setFrameTime(0.05f);
    }
    //If bullets parent is an chief set up it's spritesheet
    else if(_parent->getType() == "chief")
    {
        bullet->addComponent<BulletPhysicsComponent>();
	      auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
	      tex.loadFromFile("res/chiefAttack.png");
	      s->setSpritesheet(tex);
	      s->setFrameCount(8);
	      s->setFrameTime(0.05f);
    }
    //If bullets parent is a boss set up it's spritesheet
    else if (_parent->getType() == "boss")
    {
        shared_ptr<SpriteSheetComponent> s;

        //Fire first bullet
        if (bossBullet == 1)
        {
            //If boss at original pos, fire string of aimed bullets, else do normal attack
            if (length(bossPosOrigin - _parent->getPosition()) < 10.0)
            {
                bullet->addComponent<AimedBulletComponent>();
                s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
                tex.loadFromFile("res/archerAttack.png");
            }
            else
            {
                bullet->addComponent<BulletPhysicsComponent>();
                s = bullet->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
                tex.loadFromFile("res/chiefAttack.png");
            }
            
            s->setSpritesheet(tex);
            s->setFrameCount(8);
            s->setFrameTime(0.05f);
            amount++;
            
        }
        //Fire second bullet
        if (bossBullet == 2)
        {
            bullet->addComponent<AimedBulletComponent>();
            s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
            tex.loadFromFile("res/archerAttack.png");
            s->setSpritesheet(tex);
            s->setFrameCount(8);
            s->setFrameTime(0.05f);
            amount++;
            
        }
        //Fire third bullet
        if (bossBullet == 3)
        {
            //If boss at original pos, fire string of aimed bullets, else do normal attack
            if (length(bossPosOrigin - _parent->getPosition()) < 10.0)
            {
                bullet->addComponent<AimedBulletComponent>();
                s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
                tex.loadFromFile("res/archerAttack.png");
                s->setFrameCount(8);
                s->setFrameTime(0.05f);
            }
            else
            {
                bullet->addComponent<BulletPhysicsComponent>();
                s = bullet->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
                tex.loadFromFile("res/spell3SheetBoss.png");
                s->setFrameCount(91);
                s->setFrameTime(0.03f);
                bullet->setPosition(bullet->getPosition() + Vector2f(0, -150));
            }
            s->setSpritesheet(tex);         
            
            amount++;
            
        }
        //Fire fourth bullet
        if (bossBullet == 4)
        {
            //If boss at original pos, fire string of aimed bullets, else do normal attack
            if (length(bossPosOrigin - _parent->getPosition()) < 10.0)
            {
                bullet->addComponent<AimedBulletComponent>();
                s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
                tex.loadFromFile("res/archerAttack.png");
            }
            else
            {
                bullet->addComponent<BulletPhysicsComponent>();
                s = bullet->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
                tex.loadFromFile("res/chiefAttack.png");
            }
            s->setSpritesheet(tex);
            s->setFrameCount(8);
            s->setFrameTime(0.05f);
            amount++;
            
        }
        //Fire fifth bullet
        if (bossBullet == 5)
        {
            //If boss at original pos, fire string of aimed bullets, else do normal attack
            if (length(bossPosOrigin - _parent->getPosition()) < 10.0)
            {
                bullet->addComponent<AimedBulletComponent>();
                s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
                tex.loadFromFile("res/archerAttack.png");
                s->setFrameCount(8);
                s->setFrameTime(0.05f);
            }
            else
            {
                bullet->addComponent<BulletPhysicsComponent>();
                s = bullet->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
                tex.loadFromFile("res/spell3SheetBoss.png");
                s->setFrameCount(91);
                s->setFrameTime(0.03f);
                bullet->setPosition(bullet->getPosition() + Vector2f(0, 150));
            }
            s->setSpritesheet(tex);                    
            amount++;          
        }

        //Increase bullet fired by the amount that has been fired
        if (amount == 2)
        {
            bossBullet++;
        }
        if (amount == 3)
        {
            bossBullet++;
        }
        if (amount == 4)
        {
            bossBullet++;
        }
        if (amount == 6)
        {
            bossBullet++;
        }
        if (amount == 7)
        {
            bossBullet = 1;
            amount = 0;
        }
                   
        cout << "AMOUNT: " << amount << endl;
        cout << "BOSSBULLET: " << bossBullet << endl;
    }
    //else bullets parent is a skeleton, so set up it's spritesheet
    else
    {
	      bullet->addComponent<BulletPhysicsComponent>();
	      auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
	      tex.loadFromFile("res/skeletonAttack.png");
	      s->setSpritesheet(tex);
	      s->setFrameCount(8);
	      s->setFrameTime(0.05f);
    }
  
  
}

EnemyTurretComponent::EnemyTurretComponent(Entity* p)
    : Component(p), _firetime(2.f) 
{
	castB.loadFromFile("res/sounds/shootEnemy.wav");
	cast.setBuffer(castB);

    //Reference player
	player = _parent->scene->ents.find("player")[0];

    //Make boss fire faster
    if (_parent->getType() == "boss")
    {        
        if (length(bossPosOrigin - _parent->getPosition()) < 10.0)
        {
            _firetime = 0.1f;
        }
        else
        {
            _firetime = 0.5f;
        }
    }
    else
    {
        _firetime = 2.0f;
    }
}
