#include "ecm.h"

using namespace std;

Entity::Entity(Scene* const s)
    : _position({0, 0}), _rotation(0), _alive(true), _visible(true),
      scene(s), _fordeletion(false) {}

void Entity::addTag(const std::string& t) { _tags.insert(t); }
const set<string>& Entity::getTags() const { return _tags; }

void Entity::update(double dt) {
  if (!_alive) {
    return;
  }
  for (size_t i = 0; i < _components.size(); i++) {
    if (_components[i]->is_fordeletion()) {
      _components.erase(_components.begin() + i);
      --i;
    }
    _components[i]->update(dt);
  }
}

bool Entity::is_fordeletion() const { return _fordeletion; }

//Move entity towards position
void Entity::Move(Vector2f target)
{
    setDirection(normalize(target - getPosition()));

    setPosition(getPosition() + getDirection());
}

void Entity::render() {
  if (!_visible) {
    return;
  }
  for (auto& c : _components) {
    c->render();
  }
}

const sf::Vector2f& Entity::getPosition() const { return _position; }

void Entity::setPosition(const sf::Vector2f& _position) {
  Entity::_position = _position;
}

//Get and sets
int Entity::getScore() const { return _score; }

void Entity::setScore(int _score) { Entity::_score = _score; }

String Entity::getType() const { return _type; }

void Entity::setType(String _type) { Entity::_type = _type; }

Vector2f Entity::getDirection() const { return _dir; }

void Entity::setDirection(Vector2f _dir) { Entity::_dir = _dir; }

bool Entity::getShield() const { return _shieldActive; }

void Entity::setShield(bool _shieldActive) { Entity::_shieldActive = _shieldActive; }

int Entity::getHealth() const { return _health; }

void Entity::setHealth(int _health) { Entity::_health = _health; }

int Entity::getEssence() const { return _essence; }

void Entity::setEssence(int _essence) { Entity::_essence = _essence; }

float Entity::getRotation() const { return _rotation; }

void Entity::setRotation(float _rotation) { Entity::_rotation = _rotation; }

bool Entity::isAlive() const { return _alive; }

void Entity::setAlive(bool _alive) { Entity::_alive = _alive; }

void Entity::setForDelete() {
  _fordeletion = true;
  _alive = false;
  _visible = false;
}

bool Entity::isVisible() const { return _visible; }

void Entity::setVisible(bool _visible) { Entity::_visible = _visible; }

Component::Component(Entity* const p) : _parent(p), _fordeletion(false) {}

Entity::~Entity() {
  // Components can inter-depend on each other, so deleting them may take
  // multiple passes. We Keep deleting components until we can't delete any
  // more
  int deli = 0;
  while (deli != _components.size()) {
    deli = _components.size();
    _components.erase(
        remove_if(_components.begin(), _components.end(),
                  [](auto& sp) { return (sp.use_count() <= 1); }),
        _components.end());
  }

  if (_components.size() > 0) {
    throw std::runtime_error(
        "Can't delete entity, someone is grabbing a component!");
  }

  _components.clear();
}

Component::~Component() {}

bool Component::is_fordeletion() const { return _fordeletion; }

void EntityManager::update(double dt) {
  for (size_t i = 0; i < list.size(); i++) {
    if (list[i]->is_fordeletion()) {
      list.erase(list.begin() + i);
      --i;
      continue;
    }
    if (list[i]->_alive) {
      list[i]->update(dt);
    }
  }
}

void EntityManager::render() {
  for (auto& e : list) {
    if (e->_visible) {
      e->render();
    }
  }
}

vector<shared_ptr<Entity>> EntityManager::find(const string& tag) const {
  vector<shared_ptr<Entity>> ret;
  for (auto& e : list) {
    const auto tgs = e->_tags;
    if (tgs.find(tag) != tgs.end()) {
      ret.push_back(e);
    }
  }
  return ret;
}

vector<shared_ptr<Entity>>
EntityManager::find(const vector<string>& tags) const {
  vector<shared_ptr<Entity>> ret;
  for (auto& e : list) {
    const auto tgs = e->_tags;
    if (any_of(tags.begin(), tags.end(),
               [&tgs](auto t) { return tgs.find(t) != tgs.end(); })) {
      ret.push_back(e);
    }
  }
  return ret;
}
