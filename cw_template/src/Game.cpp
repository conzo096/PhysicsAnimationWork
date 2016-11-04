#include "game.h"
#include <algorithm>
#include <glm/gtx/transform.hpp>
using namespace glm;
using namespace std;
Component::Component(const string &token) {
	token_ = token;
	Ent_ = nullptr;
	active_ = false;
}

Component::~Component() {}

bool Component::IsActive() { return active_; }

void Component::SetActive(bool b) { active_ = b; }

void Component::SetParent(Entity *p) { Ent_ = p; }

Entity *Component::GetParent() { return Ent_; }

//############## Entity ###################

Entity::Entity() {
	visible_ = true;
	changed_ = true;
	transform.scale = vec3(1.0f, 1.0f, 1.0f);
	transform.position = vec3(0.0f, 0.0f, 0.0f);
	transform.orientation = quat();
	colour = RED;
}

Entity::~Entity() {}

const vec3 Entity::GetScale() const { return transform.scale; }

const vec3 Entity::GetPosition() const { return transform.position; }

const quat Entity::GetRotation() const { return transform.orientation; }
const vec3 Entity::GetRotationV3() const { return glm::eulerAngles(GetRotation()); }

const mat4 Entity::GetTranform() {
	if (changed_)
	{
		transform.get_transform_matrix();
		changed_ = false;
	}
	return transform.get_transform_matrix();
}

const bool Entity::IsVisible() const { return false; }

const string Entity::GetName() const { return name_; }

void Entity::SetTransform(const mat4 m4) { assert(false); }

void Entity::SetScale(const vec3 &v3)
{
	transform.scale = v3;
	changed_ = true;
}

void Entity::SetPosition(const vec3 &v3)
{
	transform.scale = v3;
	changed_ = true;
}

void Entity::SetRotation(const vec3 &v3)
{
	transform.orientation = glm::quat(v3);
	changed_ = true;
}

void Entity::SetRotation(const quat &q)
{
	transform.orientation = q;
	changed_ = true;
}

void Entity::SetVisibility(const bool b) {}

void Entity::SetName(string const &name) { name_ = name; }

void Entity::Update(const double delta) {
	for (auto &c : components_) {
		c->Update(delta);
	}
}

void Entity::Render()
{
	for (auto &c : components_)
	{
		c->Render();
	}
}

void Entity::AddComponent(unique_ptr<Component> &c)
{
	c->SetParent(this);
	components_.push_back(move(c));
}

void Entity::RemoveComponent(Component &c)
{
	// Todo: Test This
	auto position =
		find_if(components_.begin(), components_.end(), [c](unique_ptr<Component> &p) { return p.get() == &c; });
	if (position != components_.end())
		components_.erase(position);
}

// be careful what you do with this function...
vector<Component *> Entity::GetComponents(string const &name) const
{
	vector<Component *> list;
	if (components_.size() < 1)
	{
		return list;
	}
	for (auto &c : components_)
	{
		if (c->token_ == name)
		{
			list.push_back(c.get()); // It's not like we want to make safe programs anyway...
		}
	}
	return list;
}

const vector<unique_ptr<Component>> *Entity::GetComponents() const { return &components_; }

//############## Shape Renderer ###################

cShapeRenderer::cShapeRenderer() : Component("ShapeRenderer") {}

cShapeRenderer::~cShapeRenderer() {}

void cShapeRenderer::Update(double delta) {}

void cShapeRenderer::Render()
{
	phys::DrawSphere(Ent_->GetPosition(), 1.0f, RED);
}
