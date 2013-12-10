#include "Sphere.h"
#include "Model.h"
#include "Content.h"

using namespace BGE;

Sphere::Sphere(float radius)
{
	std::shared_ptr<GameComponent> model (Content::LoadModel("sphere"));
	model->drawMode = Model::draw_modes::single_material;
	model->Initialise();
	diffuse = glm::vec3(RandomFloat(),RandomFloat(),RandomFloat());
	specular = glm::vec3(1.2f,1.2f, 1.2f);
	tag = "Sphere";
	Attach(model);
	scale = glm::vec3(radius, radius, radius);
}

//overloaded the constructor to allow for textured spheres to be loaded
Sphere::Sphere(float radius,std::string name)
{
	std::shared_ptr<GameComponent> model (Content::LoadModel(name));
	model->drawMode = Model::draw_modes::textured;
	model->Initialise();
	diffuse = glm::vec3(RandomFloat(),RandomFloat(),RandomFloat());
	specular = glm::vec3(1.2f,1.2f, 1.2f);
	tag = "Sphere";
	Attach(model);
	scale = glm::vec3(radius, radius, radius);
}


Sphere::~Sphere(void)
{
}
