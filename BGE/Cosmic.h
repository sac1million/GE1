#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "FountainEffect.h"
#include "Sphere.h"


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
		shared_ptr<GameComponent> earth;
		shared_ptr<FountainEffect> starFountain;
	    float fountainTheta;
		float speed;
		float elapsed;
		bool Initialise();
		void Update(float timeDelta);
		


		glm::vec3 force;
		glm::vec3 tempforce;
		float mass;
		glm::vec3 accel;
		glm::vec3 velocity;
		glm::vec3 bouncePosition;

	};
}



