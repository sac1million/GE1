#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "FountainEffect.h"
#include "Sphere.h"
#include "ObstacleAvoidanceScenario.h"


using namespace std;

namespace BGE
{
	class Cosmic :
		public Game
	{
	public:
		Cosmic(void);
		~Cosmic(void);

		shared_ptr<Sphere> sphere[13];
		shared_ptr<SteeringController> starController;
		shared_ptr<FountainEffect> shootingStar;

		float speed;
		float elapsed;
		bool Initialise();
		void Update(float timeDelta);
		long double score;
		std::string s;

		glm::vec3 force;
		glm::vec3 tempforce;
		float mass;
		glm::vec3 accel;
		glm::vec3 velocity;
		float max; 
		glm::vec3 newPos;

	};
}



