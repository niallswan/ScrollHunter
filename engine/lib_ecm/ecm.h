#pragma once
#include "maths.h"
#include <algorithm>
#include <memory>
#include <set>
#include <typeindex>
#include <vector>

using namespace sf;

class Entity;
class Scene;

class Component {
  friend Entity;

protected:
  Entity* const _parent;
  bool _fordeletion; // should be removed
  explicit Component(Entity* const p);

  

public:
  Component() = delete;

  bool is_fordeletion() const;

  virtual void update(double dt) = 0;

  virtual void render() = 0;

  virtual ~Component();
};

struct EntityManager {
  std::vector<std::shared_ptr<Entity>> list;
  void update(double dt);
  void render();
  std::vector<std::shared_ptr<Entity>> find(const std::string& tag) const;
  std::vector<std::shared_ptr<Entity>>
  find(const std::vector<std::string>& tags) const;
};

class Entity {
  friend struct EntityManager;

protected:
  std::vector<std::shared_ptr<Component>> _components;
  sf::Vector2f _position;
  float _rotation;
  bool _alive;       // should be updated
  bool _visible;     // should be rendered
  bool _fordeletion; // should be deleted
  std::set<std::string> _tags;
  int _health; //health
  int _essence; //essence
  bool _shieldActive; //Is shield on
  Vector2f _dir; //direction to be moved
  String _type; // type of entity
  int _score; // score

  

public:
  void addTag(const std::string& t);
  const std::set<std::string>& getTags() const;
  Scene* const scene;
  Entity(Scene* const s);

  virtual ~Entity();

  virtual void update(double dt);

  virtual void render();

  //get and sets
  int getScore() const;

  void setScore(int _score);

  String getType() const;

  void setType(String _type);

  Vector2f getDirection() const;

  void setDirection(Vector2f _dir);

  bool getShield() const;

  void setShield(bool _shieldActive);

  int getHealth() const;

  void setHealth(int _health);

  int getEssence() const;

  void setEssence(int _essence);

  const sf::Vector2f& getPosition() const;

  void setPosition(const sf::Vector2f& _position);

  bool is_fordeletion() const;

  void Move(Vector2f target);

  float getRotation() const;

  void setRotation(float _rotation);

  bool isAlive() const;

  void setAlive(bool _alive);

  void setForDelete();

  bool isVisible() const;

  void setVisible(bool _visible);

  template <typename T, typename... Targs>
  std::shared_ptr<T> addComponent(Targs... params) {
    static_assert(std::is_base_of<Component, T>::value,
                  "T must be a component");
    std::shared_ptr<T> sp(std::make_shared<T>(this, params...));
    _components.push_back(sp);
    return sp;
  }

  template <typename T>
  const std::vector<std::shared_ptr<T>> get_components() const {
    static_assert(std::is_base_of<Component, T>::value,
                  "T must be a component");
    std::vector<std::shared_ptr<T>> ret;
    for (const auto c : _components) {
      if (typeid(*c) == typeid(T)) {
        ret.push_back(std::dynamic_pointer_cast<T>(c));
      }
    }
    return std::move(ret);
  }

  // Will return a T component, or anything derived from a T
  // component.
  template <typename T>
  const std::vector<std::shared_ptr<T>> GetCompatibleComponent() {
    static_assert(std::is_base_of<Component, T>::value,
                  "T must be a component");
    std::vector<std::shared_ptr<T>> ret;
    for (auto c : _components) {
      auto dd = dynamic_cast<T*>(&(*c));
      if (dd) {
        ret.push_back(std::dynamic_pointer_cast<T>(c));
      }
    }
    return ret;
  }
};
