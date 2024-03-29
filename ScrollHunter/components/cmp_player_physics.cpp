#include "cmp_player_physics.h"
#include "system_physics.h"
#include <LevelSystem.h>
#include <SFML/Window/Keyboard.hpp>
#include "cmp_actor_movement.h"
#include "cmp_spritesheet.h"
#include "cmp_bullet.h"
#include "cmp_bullet_physics.h"
#include "cmp_player_bullet_physics.h"
#include "cmp_familiar.h"
#include <LevelSystem.h>
#include <engine.h>
#include <iostream>
#include "cmp_hurt_enemy.h"
#include "ecm.h"
#include <SFML/Audio.hpp>


using namespace std;
using namespace sf;
using namespace Physics;

//shield
static shared_ptr<Entity> shield;
static float shieldDuration = 0.f;
static bool shieldActive = false;

//familiar
static shared_ptr<Entity> familiar;
static float familiarDuration = 0.f;
static bool familiarActive = false;

//increased defualt attack
static shared_ptr<Entity> powerup;
static float increaseDuration = 0.f;
static bool increaseActive = false;
static shared_ptr<Entity> increaseBullet;
static shared_ptr<Entity> increaseBullet2;



static shared_ptr<Entity> bullet;
static shared_ptr<Entity> bullet2;
static shared_ptr<Entity> bullet3;

//ability cooldowns
static float qCooldown = 0.f;
static float wCooldown = 0.f;
static float eCooldown = 0.f;

static Texture qAttack;
static Texture wAttack;
static Texture familiarSheet;
static Texture shieldSheet;
static Texture castSheet;

static float timer = 2.f;

//sounds
static Sound cast;
static SoundBuffer castB;
static Sound cast2;
static SoundBuffer castB2;
static Sound cast3;
static SoundBuffer castB3;

static Sound powerupS;
static SoundBuffer powerupB;


bool PlayerPhysicsComponent::isGrounded() const {
  auto touch = getTouching();
  const auto& pos = _body->GetPosition();
  const float halfPlrHeigt = _size.y * .5f;
  const float halfPlrWidth = _size.x * .52f;
  b2WorldManifold manifold;
  for (const auto& contact : touch) {
    contact->GetWorldManifold(&manifold);
    const int numPoints = contact->GetManifold()->pointCount;
    bool onTop = numPoints > 0;
    // If all contacts are below the player.
    for (int j = 0; j < numPoints; j++) {
      onTop &= (manifold.points[j].y < pos.y - halfPlrHeigt);
    }
    if (onTop) {
      return true;
    }
  }

  return false;
}

void PlayerPhysicsComponent::update(double dt) {

  const auto pos = _parent->getPosition();
  


  setVelocity(Vector2f(0.f, 0.f));

  if (Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::Down)) {
	  if (Keyboard::isKeyPressed(Keyboard::Right))
	  {
		  if (getVelocity().x < _maxVelocity.x)
		  {
			  impulse({ 10.f, 0 });
		  }
	  }
	  if (Keyboard::isKeyPressed(Keyboard::Left))
	  {
		  if (getVelocity().x > -_maxVelocity.x)
		  {
			  impulse({ -10.f, 0 });
		  }
	  }
	  if (Keyboard::isKeyPressed(Keyboard::Up))
	  {
		  if (getVelocity().y > -_maxVelocity.y)
		  {
			  impulse({ 0, -10.f });
		  }
	  }
	  if (Keyboard::isKeyPressed(Keyboard::Down))
	  {
		  if (getVelocity().y < _maxVelocity.y)
		  {
			  impulse({ 0, 10.f });
		  }
	  }
  }
  else
  {
	  // Dampen X axis movement
	  dampen({ 0.9f, 1.0f });
  }


  //ABILITIES
  //Q ability cooldown
  if (qCooldown > 0.f)
  {
	  qCooldown -= dt;
  }

  //W ability cooldown
  if (wCooldown > 0.f)
  {
	  wCooldown -= dt;
  }

  //E ability cooldown
  if (eCooldown > 0.f)
  {
	  eCooldown -= dt;
  }


  //Q Ability
  if (Keyboard::isKeyPressed(Keyboard::Q) && qCooldown <= 0.f)
  {
	  cast.stop();
	  cast.play();
	  bullet = _parent->scene->makeEntity();
	  bullet->setPosition(_parent->getPosition() + Vector2f(50, 0));
	  bullet->addComponent<EnemyHurtComponent>(25);
	  auto s = bullet->addComponent<SpriteSheetComponent>(Vector2f(55.f, 55.f));
	  qAttack.loadFromFile("res/attackOrbSheet.png");
	  s->setSpritesheet(qAttack);
	  s->setFrameCount(8);
	  s->setFrameTime(0.05f);
	  auto p = bullet->addComponent<PlayerBulletPhysicsComponent>();
	  bullet->addComponent<BulletComponent>();

	  if (increaseActive == true)
	  {
		  increaseBullet = _parent->scene->makeEntity();
		  increaseBullet->setPosition(_parent->getPosition() + Vector2f(50, 50));
		  increaseBullet->addComponent<EnemyHurtComponent>(20);
		  auto s = increaseBullet->addComponent<SpriteSheetComponent>(Vector2f(55.f, 55.f));
		  s->setSpritesheet(qAttack);
		  s->setFrameCount(8);
		  s->setFrameTime(0.05f);
		  auto p = increaseBullet->addComponent<PlayerBulletPhysicsComponent>();
		  increaseBullet->addComponent<BulletComponent>();

		  increaseBullet2 = _parent->scene->makeEntity();
		  increaseBullet2->setPosition(_parent->getPosition() + Vector2f(50, -50));
		  increaseBullet2->addComponent<EnemyHurtComponent>(20);
		  auto s2 = increaseBullet2->addComponent<SpriteSheetComponent>(Vector2f(55.f, 55.f));
		  s2->setSpritesheet(qAttack);
		  s2->setFrameCount(8);
		  s2->setFrameTime(0.05f);
		  auto p2 = increaseBullet2->addComponent<PlayerBulletPhysicsComponent>();
		  increaseBullet2->addComponent<BulletComponent>();
	  }


	  qCooldown = 0.8f;
  }

  
  


  //W Ability
  if (Keyboard::isKeyPressed(Keyboard::W) && wCooldown <= 0.f && _parent->getEssence() > 0)
  {
	  cast2.stop();
	  cast2.play();
	  bullet2 = _parent->scene->makeEntity();
	  bullet2->setPosition(_parent->getPosition() + Vector2f(80, 10));
	  bullet2->addComponent<EnemyHurtComponent>(15);

	  auto s = bullet2->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
	  wAttack.loadFromFile("res/beamSheet.png");
	  s->setSpritesheet(wAttack);
	  s->setFrameCount(8);
	  s->setFrameTime(0.05f);

	  auto p = bullet2->addComponent<PlayerBulletPhysicsComponent>();
	  bullet2->addComponent<BulletComponent>();
	  _parent->setEssence(_parent->getEssence() - 5);
	  wCooldown = 0.06f;
  }


  //E ability
  if (Keyboard::isKeyPressed(Keyboard::E) && eCooldown <= 0.f)
  {
	  cast3.stop();
	  cast3.play();
	  bullet3 = _parent->scene->makeEntity();
	  bullet3->setPosition(_parent->getPosition() + Vector2f(80, 0));
	  bullet3->addComponent<EnemyHurtComponent>(100);
	  auto s = bullet3->addComponent<ShapeComponent>();

	  //s->setShape<sf::CircleShape>(40.f);
	  //s->getShape().setFillColor(Color::Green);
	  //s->getShape().setOrigin(8.f, 8.f);

	  auto sp = bullet3->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
	  familiarSheet.loadFromFile("res/spell3Sheet.png");
	  sp->setSpritesheet(familiarSheet);
	  sp->setFrameCount(91);
	  sp->setFrameTime(0.03f);

	  auto p = bullet3->addComponent<PlayerBulletPhysicsComponent>();
	  bullet3->addComponent<BulletComponent>();

	  eCooldown = 5.f;
  }

  //CASTABLE POWERUPS
  //1 powerup Shield


  if (Keyboard::isKeyPressed(Keyboard::Num2) && shieldActive == false && _parent->getEssence() > 0)
  {
	  powerupS.stop();
	  powerupS.play();
	  shield = _parent->scene->makeEntity();
	  auto s = shield->addComponent<SpriteSheetComponent>(Vector2f(150.f, 150.f));
	  shieldSheet.loadFromFile("res/shieldSheet.png");
	  s->setSpritesheet(shieldSheet);
	  s->setFrameCount(8);
	  s->setFrameTime(0.09f);
	  shieldDuration = 7.f;
	  shieldActive = true;
	  _parent->setShield(shieldActive);
	  _parent->setEssence(_parent->getEssence() - 20);
  }

  if (shieldActive == true)
  {
	  shield->setPosition(_parent->getPosition());
	  shieldDuration -= dt;
  }

  if (shieldDuration < 0.f)
  {
	  shieldActive = false;
	  _parent->setShield(shieldActive);
	  shield->setForDelete();
  }






  //2 powerup Familiar
  if (Keyboard::isKeyPressed(Keyboard::Num1) && familiarActive == false && _parent->getEssence() > 0)
  {
	  powerupS.stop();
	  powerupS.play();
	  familiar = _parent->scene->makeEntity();
	  //auto s = familiar->addComponent<ShapeComponent>();
	  //s->setShape<sf::CircleShape>(25.f);
	  auto sp = familiar->addComponent<SpriteSheetComponent>(Vector2f(100.f, 100.f));
	  familiarSheet.loadFromFile("res/familiarSheet.png");
	  sp->setSpritesheet(familiarSheet);
	  sp->setFrameCount(61);
	  sp->setFrameTime(0.05f);
	  auto t = familiar->addComponent<FamiliarComponent>();
	  familiarDuration = 12.f;
	  familiarActive = true;
	  _parent->setEssence(_parent->getEssence() - 20);
  }

  if (familiarActive == true)
  {
	  familiar->setPosition(_parent->getPosition() - Vector2f(50.f, 50.f));
	  familiarDuration -= dt;
  }

  if (familiarDuration< 0.f)
  {
	  familiarActive = false;
	  familiar->setForDelete();
  }


  //3 powerup Increase Default attack
  if (Keyboard::isKeyPressed(Keyboard::Num3) && increaseActive == false && _parent->getEssence() > 0)
  {
	  powerupS.stop();
	  powerupS.play();
	  powerup = _parent->scene->makeEntity();
	  auto sp = powerup->addComponent<SpriteSheetComponent>(Vector2f(50.f, 50.f));
	  familiarSheet.loadFromFile("res/powerup3.png");
	  sp->setSpritesheet(familiarSheet);
	  sp->setFrameCount(8);
	  sp->setFrameTime(0.05f);
	  increaseDuration = 7.f;
	  increaseActive = true;
	  _parent->setEssence(_parent->getEssence() - 20);
  }

  if (increaseActive == true)
  {
	  powerup->setPosition(_parent->getPosition() - Vector2f(10.f,50.f));
	  increaseDuration -= dt;
  }

  if (increaseDuration < 0.f)
  {
	  increaseActive = false;
	  powerup->setForDelete();
  }


  // Clamp velocity.
  auto v = getVelocity();
  v.x = copysign(min(abs(v.x), _maxVelocity.x), v.x);
  v.y = copysign(min(abs(v.y), _maxVelocity.y), v.y);
  setVelocity(v);

  PhysicsComponent::update(dt);
}

PlayerPhysicsComponent::PlayerPhysicsComponent(Entity* p, const Vector2f& size) : PhysicsComponent(p, true, size) 
{
  _size = sv2_to_bv2(size, true);
  _maxVelocity = Vector2f(400.f, 400.f);
  _groundspeed = 30.f;
  _grounded = false;
  _body->SetSleepingAllowed(false);
  _body->SetFixedRotation(true);
  //Bullet items have higher-res collision detection
  _body->SetBullet(true);

  castB.loadFromFile("res/sounds/shootPlayer.wav");
  cast.setBuffer(castB);
  castB2.loadFromFile("res/sounds/shootPlayer2.wav");
  cast2.setBuffer(castB2);
  castB3.loadFromFile("res/sounds/shootPlayer3.wav");
  cast3.setBuffer(castB3);

  powerupB.loadFromFile("res/sounds/powerup.wav");
  powerupS.setBuffer(powerupB);
}
