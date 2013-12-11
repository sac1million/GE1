#include "Cosmic.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"
#include "FountainEffect.h"
#include "ObstacleAvoidanceScenario.h"
#include "Params.h"
#include "SteeringGame.h"

using namespace BGE;


struct obsParam
{
public:
	glm::vec3 pos;
	float radius;
	vector<obsParam> sphere;

	obsParam(glm::vec3 pos, float radius)
	{
		this->pos = pos;
		this->radius = radius;
	}
};


Cosmic::Cosmic(void)
{
	elapsed = 10000;
	speed = 5.0f;
	score = 0;
}

Cosmic::~Cosmic(void)
{
}

bool Cosmic::Initialise()
{
	
	// sphere for shooting star
	shared_ptr<Sphere> fountainSphere = make_shared<Sphere>(0.5);
	fountainSphere->diffuse = glm::vec3(1.0f,1.0f,0.0f);

	Params::Load("default");
	shootingStar = make_shared<FountainEffect>(200);
	shootingStar->diffuse = glm::vec3(1.0f,1.0f,0.0f);
	shootingStar->tag = "Steerable";
	starController = make_shared<SteeringController>();
	starController->position = glm::vec3(0, 10, 10);      
	starController->TurnOn(SteeringController::behaviour_type::arrive);
	starController->TurnOn(SteeringController::behaviour_type::obstacle_avoidance);
	starController->targetPos = glm::vec3(0, 0, -100);
	shootingStar->Attach(starController);
	
	shootingStar->Attach(fountainSphere);
	shootingStar->scale = glm::vec3(2,2,2);
	Attach(shootingStar);

	// declare obstacles for shooting star obstacle avoidance.
	vector<obsParam> obstacle;
	obstacle.push_back(obsParam(glm::vec3(0, 2, 10), 2));
	obstacle.push_back(obsParam(glm::vec3(11, 2, -10), 2));
	obstacle.push_back(obsParam(glm::vec3(-10, 2, -10), 2));
	obstacle.push_back(obsParam(glm::vec3(3, 2, -25), 2));
	obstacle.push_back(obsParam(glm::vec3(0, 10, -10), 2));
	obstacle.push_back(obsParam(glm::vec3(0, -10, -10), 2));
	obstacle.push_back(obsParam(glm::vec3(-6, 10, -20), 2));
	obstacle.push_back(obsParam(glm::vec3(-6, 2, -20), 2));
	obstacle.push_back(obsParam(glm::vec3(6, 2, 10), 2));
	obstacle.push_back(obsParam(glm::vec3(0, 8, 10), 2));
	obstacle.push_back(obsParam(glm::vec3(0, -4, 10), 2));
	obstacle.push_back(obsParam(glm::vec3(6, -6, -20), 2));
	//obstacle.push_back(obsParam(glm::vec3(20, 10, -50), 2)); --> Moon (Image wouldn't load)
	obstacle.push_back(obsParam(glm::vec3(2, 4, -10), 2)); // earth

	// assign spheres as obstacles
	for (int i = 0 ; i < obstacle.size(); i ++)
	{
		if(i == obstacle.size() - 1)
		{
			sphere[i] = make_shared<Sphere>(obstacle[i].radius,"earth"); //Creating a new sphere with texture earth
			sphere[i]->tag = "Obstacle";
			sphere[i]->position = obstacle[i].pos;
			Attach(sphere[i]);
		}

		/* Moon code 
		if(i == obstacle.size() - 2)
		{
			sphere[i] = make_shared<Sphere>(obstacle[i].radius,"moon"); //Creating a new sphere with texture moon
			sphere[i]->tag = "Obstacle";
			sphere[i]->position = obstacle[i].pos;
			Attach(sphere[i]);
		}
		*/

		else
		{
			sphere[i] = make_shared<Sphere>(obstacle[i].radius);
			sphere[i]->tag = "Obstacle";
			sphere[i]->position = obstacle[i].pos;
			Attach(sphere[i]);
		}

		if(i == 0) // pinball
		{
			sphere[i]->diffuse = glm::vec3(1.0f,1.0f,1.0f);
		}
	}

	riftEnabled = false;
	fullscreen = false;
	width = 800;
	height = 600;

	mass = 1.0f;

	velocity = glm::vec3(0,0,0);

	Game::Initialise();

	camera->GetController()->position = glm::vec3(0, 4, 30);
	return true;
}

void Cosmic::Update(float timeDelta)
{	

	// Forces on Cosmic
	float newtons = 30.0f;
	float epsilon = glm::epsilon<float>();

/*	if (keyState[SDL_SCANCODE_F])
	{
		force = force + (sphere[0]->look * newtons);
	}
	else if (keyState[SDL_SCANCODE_N])
	{
		force = force - (sphere[0]->look * newtons);
	}
	else if (keyState[SDL_SCANCODE_LEFT])
	{
		force = force - (sphere[0]->right * newtons);
	}
	else if (keyState[SDL_SCANCODE_RIGHT])
	{
		force = force + (sphere[0]->right * newtons);
	}
	else if (keyState[SDL_SCANCODE_DOWN])
	{
		force = force - (sphere[0]->up * newtons);
	}
	else if (keyState[SDL_SCANCODE_UP])
	{
		force = force + (sphere[0]->up * newtons);
	}*/
	if (keyState[SDL_SCANCODE_Z]) // stop sphere
	{
		force = glm::vec3(0,0,0);
		sphere[0]->position = glm::vec3(0,2,10);
		sphere[0]->velocity = glm::vec3(0,0,0);		
	}

	if (keyState[SDL_SCANCODE_SPACE]) //prepare to release sphere[0].
	{
		score = 0; // zero score
		sphere[0]->velocity = glm::vec3(0,0,0);
		sphere[0]->position = glm::vec3(0, 2, 10);
		force += (sphere[0]->look * newtons * 2.0f); // build up starting force
	}

	if (keyState[SDL_SCANCODE_TAB]) // operate 'flipper' spheres
	{
		sphere[8]->position = glm::vec3(-4, 2, 10);
		sphere[9]->position = glm::vec3(4, 2, 10);
		sphere[10]->position = glm::vec3(0, 6, 10);
		sphere[11]->position = glm::vec3(0, -2, 10);
	}
	else // return 'flipper' spheres
	{
		sphere[8]->position = glm::vec3(-6, 2, 10);
		sphere[9]->position = glm::vec3(6, 2, 10);
		sphere[10]->position = glm::vec3(0, 8, 10);
		sphere[11]->position = glm::vec3(0, -4, 10);
	}
	
	if (keyState[SDL_SCANCODE_RETURN]) // operate movable spheres
	{
		sphere[1]->position = glm::vec3(6, 2, -10);
		sphere[2]->position = glm::vec3(-6, 2, -10);
		sphere[3]->position = glm::vec3(0, 6, -30);
	}
	else // return movable spheres
	{
		sphere[1]->position = glm::vec3(10, 2, -10);
		sphere[2]->position = glm::vec3(-10, 2, -10);
		sphere[3]->position = glm::vec3(0, 2, -30);
	}
	
	// create 'gravity' back towards start position.
	if(sphere[0]->position.z < 10)
	{
		force.z += 4;
	}
	if(sphere[0]->position.x < 0)
	{
		force.x += 4;
	}
	else if(sphere[0]->position.x > 0)
	{
		force.x -= 4;
	}
	if(sphere[0]->position.y < 2)
	{
		force.y += 4;
	}
	else if(sphere[0]->position.y > 2)
	{
		force.y -= 4;
	}
	
	// acceleration, velocity, position
	accel = force/mass;

	velocity += accel * timeDelta;

	for(int i=1; i<13; i++) // check for collisions of sphere[0] with other spheres
	{
		if(glm::length(sphere[i]->position - sphere[0]->position) <= (sphere[i]->scale.x + sphere[0]->scale.x)) // check for collision with specific spheres.
		{
			score += 5;
			soundSystem->PlaySound("Ding", glm::vec3(0,0,0));
			velocity -= (sphere[i]->position - sphere[0]->position);

			if(i == 8 || i == 9 || i == 10 || i == 11 ) // if sphere[0] collides with one of the flipper spheres.
			{
				if (keyState[SDL_SCANCODE_TAB])
				{
					velocity = velocity * 1.05f; // boost velocity of sphere[0].
				}
			}
			//sphere[0]->position = bouncePosition; // restore sphere[0] to pre-collision position.
		}
		else
		{
			//bouncePosition = sphere[0]->position; // store sphere[0] position just before any collision.

		}
	}// end for

	if(glm::length(glm::vec3(0,2,-10) - sphere[0]->position) >= 50) // check for collision with virtual sphere boundary
	{
			velocity += (glm::vec3(0,2,-10) - sphere[0]->position);
			
			//sphere[0]->position = bouncePosition; // restore sphere[0] to pre-collision position.
	}

	if(glm::length(glm::vec3(0,2,-40) - sphere[0]->position) >= 50) // to prevent sphere[0] going behind flipper spheres except at centre.
	{
		// check if sphere[0] near centre
		if((sphere[0]->position.x < 4 && sphere[0]->position.x > -4) && (sphere[0]->position.y < 6 && sphere[0]->position.y > -2) )
		{
			if(sphere[0]->position.z > 11) // check if sphere[0] back at start position.
			{
				force = glm::vec3(0,0,0);
				sphere[0]->velocity = glm::vec3(0,0,0);
				sphere[0]->position = glm::vec3(0, 2, 11);
			}
		}
		else // if away from centre - bounce back
		{
			velocity += ((glm::vec3(0,2,-40) - sphere[0]->position) * 1.01f);
		}
	}

	sphere[0]->position += velocity * timeDelta;

	// Check if the velocity length is > epsilon and if so create the look vector from the velocity
	if (glm::length(velocity) > epsilon)
	{
		sphere[0]->look = glm::normalize(velocity);
	}
	// Now check to see if the |look - basis| > epsilon
	if ((glm::length(sphere[0]->look) - glm::length(basisLook)) > epsilon)
	{

	// And if so calculate the quaternion
		float theta = 0.0f;
		theta = glm::acos(glm::dot(GameComponent::basisLook, glm::normalize(sphere[0]->look)));
	
		glm::vec3 cross = glm::cross(GameComponent::basisLook, glm::normalize(sphere[0]->look));
		cross = glm::normalize(cross);

		glm::quat squat = glm::angleAxis(glm::degrees(theta), cross);

		sphere[0]->orientation = squat;

	} // end if

	velocity = velocity * 0.99f;


	// Reset the force accumulator
	force = glm::vec3(0.0f,0.0f,0.0f);
	
	//check if the negative target position (-Z) is reached and set a new target position. 
	if(shootingStar->position.z < starController->targetPos.z + 10)
	{
		starController->TurnOn(SteeringController::behaviour_type::arrive); //Using the arrive behaviour to send the shooting star to a particular point
		starController->TurnOn(SteeringController::behaviour_type::obstacle_avoidance); // Using the Obstacle avoidance so the shooting star avoids the spheres (Planets). 
		newPos = glm::vec3(RandomFloat() * 30, RandomFloat() * 15, 20); // Create a new position, randomising X and Y to an extent. Z remains fixed as we want the shooting star to navigate around the game between Z = 20 and Z = -70
		starController->targetPos = newPos; // Sets the new target position
	}
	
	//check if the positive target position (+Z) is reached and a set a new target position
	if((shootingStar->position.z > starController->targetPos.z - 10))
	{
		starController->TurnOn(SteeringController::behaviour_type::arrive);
		starController->TurnOn(SteeringController::behaviour_type::obstacle_avoidance);
		newPos = glm::vec3(RandomFloat() * -30, RandomFloat() * 5, -70); // Create a new position, randomising X and Y to an extent. Z remains fixed as we want the shooting star to navigate around the game between Z = 20 and Z = -70
		starController->targetPos = newPos; // Sets the new target position
	}

		s = to_string(score);
		PrintText("Score: " + s);

	Game::Update(timeDelta);
	
}
