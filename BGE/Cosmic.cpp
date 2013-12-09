#include "Cosmic.h"
#include "Content.h"
#include "VectorDrawer.h"
#include "LazerBeam.h"
#include "FountainEffect.h"

using namespace BGE;

Cosmic::Cosmic(void)
{
	elapsed = 10000;
	speed = 5.0f;
}


Cosmic::~Cosmic(void)
{
}

bool Cosmic::Initialise()
{
/*	earth = make_shared<GameComponent>();
	earth->Attach(Content::LoadModel("myearth"));
	earth->position = glm::vec3(0,0,0);
	earth->scale = glm::vec3(5,5,5);
	earth->diffuse = glm::vec3(1.2f, 1.2f, 1.2f);
	Attach(earth); */
	
	sphere[0] = make_shared<Sphere>(2);
	sphere[0]->position = glm::vec3(0, 2, 10);
    sphere[0]->diffuse = glm::vec3(1.0f,1.0f,1.0f);
	Attach(sphere[0]);

	sphere[1] = make_shared<Sphere>(2);
	sphere[1]->position = glm::vec3(10, 2, -10);
    sphere[1]->diffuse = glm::vec3(1.0f,0.0f,0.0f);
	Attach(sphere[1]);

	sphere[2] = make_shared<Sphere>(2);
	sphere[2]->position = glm::vec3(-10, 2, -10);
    sphere[2]->diffuse = glm::vec3(1.0f,0.0f,0.0f);
	Attach(sphere[2]);

	sphere[3] = make_shared<Sphere>(4);
	sphere[3]->position = glm::vec3(0, 2, -30);
    sphere[3]->diffuse = glm::vec3(1.0f,1.0f,0.0f);
	Attach(sphere[3]);
	
	sphere[4] = make_shared<Sphere>(2);
	sphere[4]->position = glm::vec3(6, 10, -20);
    sphere[4]->diffuse = glm::vec3(0.0f,1.0f,0.0f);
	Attach(sphere[4]);
	
	sphere[5] = make_shared<Sphere>(2);
	sphere[5]->position = glm::vec3(-6, -6, -20);
    sphere[5]->diffuse = glm::vec3(0.0f,1.0f,0.0f);
	Attach(sphere[5]);

	sphere[6] = make_shared<Sphere>(2);
	sphere[6]->position = glm::vec3(-6, 10, -20);
    sphere[6]->diffuse = glm::vec3(1.0f,0.0f,1.0f);
	Attach(sphere[6]);

	sphere[7] = make_shared<Sphere>(2);
	sphere[7]->position = glm::vec3(6, -6, -20);
    sphere[7]->diffuse = glm::vec3(1.0f,0.0f,1.0f);
	Attach(sphere[7]);

	sphere[8] = make_shared<Sphere>(2);
	sphere[8]->position = glm::vec3(-6, 2, 10);
    sphere[8]->diffuse = glm::vec3(0.0f,0.0f,1.0f);
	Attach(sphere[8]);

	sphere[9] = make_shared<Sphere>(2);
	sphere[9]->position = glm::vec3(6, 2, 10);
    sphere[9]->diffuse = glm::vec3(0.0f,0.0f,1.0f);
	Attach(sphere[9]);

	sphere[10] = make_shared<Sphere>(2);
	sphere[10]->position = glm::vec3(0, 8, 10);
    sphere[10]->diffuse = glm::vec3(0.0f,0.0f,1.0f);
	Attach(sphere[10]);

	sphere[11] = make_shared<Sphere>(2);
	sphere[11]->position = glm::vec3(0, -4, 10);
    sphere[11]->diffuse = glm::vec3(0.0f,0.0f,1.0f);
	Attach(sphere[11]);

/*	sphere[12] = make_shared<Sphere>(0.25);
    sphere[12]->diffuse = glm::vec3(1.0f,1.0f,0.0f);

	starFountain = make_shared<FountainEffect>(500);
	starFountain->Attach(sphere[12]);
	starFountain ->diffuse = glm::vec3(1.0f,1.0f, 0.0f);
	Attach(starFountain);*/

	riftEnabled = false;
	fullscreen = false;
	width = 800;
	height = 600;

	fountainTheta = 0.0f;

	mass = 1.0f;

	velocity = glm::vec3(0,0,0);

	Game::Initialise();

	camera->GetController()->position = glm::vec3(0, 4, 20);
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
		//force = glm::vec3(0,0,0);
		sphere[0]->velocity = glm::vec3(0,0,0);
		sphere[0]->position = glm::vec3(0, 2, 10);
		force += glm::vec3(0,0,-5); // build up starting force
		//force += (sphere[0]->look * newtons);
	}
/*	else if(tempforce != glm::vec3(0,0,0)) // release sphere[0].
	{
		force = tempforce;
		force += glm::vec3((RandomFloat()*5),(RandomFloat()*5),0);
		//if(force.z < 0)
		//{
			//force.z = -force.z;
		//}
		tempforce = glm::vec3(0,0,0);
	} */
	

	if (keyState[SDL_SCANCODE_TAB]) // operate 'flipper' spheres
	{
		sphere[8]->position = glm::vec3(-4, 2, 10);
		sphere[9]->position = glm::vec3(4, 2, 10);
		sphere[10]->position = glm::vec3(0, 6, 10);
		sphere[11]->position = glm::vec3(0, -2, 10);
	}
	else // return 'flipper'spheres
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
	
	
/*	// create 'gravity' back towards start position.
	if(sphere[0]->position.z < 10)
	{
		force.z += 1;
	}
	if(sphere[0]->position.x < 0)
	{
		force.x += 1;
	}
	else if(sphere[0]->position.x > 0)
	{
		force.x -= 1;
	}
	if(sphere[0]->position.y < 2)
	{
		force.y += 1;
	}
	else if(sphere[0]->position.y > 2)
	{
		force.y -= 1;
	}*/

	// acceleration, velocity, position
	accel = force/mass;

	velocity += accel * timeDelta;

/*	// detect excessive velocity.
	if(velocity.x > 100)
	{
		velocity.x = 99;
	}
	if(velocity.y > 100);
	{
		velocity.y = 99;
	}
	if(velocity.z > 100)
	{
		velocity.z = 99;
	}
	*/
	for(int i=1; i<12; i++) // check for collisions with static spheres
	{
		if(glm::length(sphere[i]->position - sphere[0]->position) <= (sphere[i]->scale.x + sphere[0]->scale.x)) // check for collision with specific spheres.
		{
			soundSystem->PlaySound("Ding", glm::vec3(0,0,0));
			velocity -= (sphere[i]->position - sphere[0]->position);
			//if((i = 8) || (i = 9) || (i = 10) || (i = 11))
			//{
				//velocity = velocity * 1.05f; // boost velocity of sphere[0].
			//}
			//sphere[0]->position = bouncePosition; // restore sphere[0] to pre-collision position.
		}
		else
		{
			//bouncePosition = sphere[0]->position; // store sphere[0] position just before any collision.

		}
	}// end for

	if(glm::length(glm::vec3(0,2,-10) - sphere[0]->position) >= 50) // check for collision with virtual sphere boundary
	{
			velocity = -velocity;
			
			//sphere[0]->position = bouncePosition; // restore sphere[0] to pre-collision position.
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

/*	fountainTheta += timeDelta;
	
	if (fountainTheta >= glm::pi<float>() * 2.0f)
	{
		fountainTheta = 0.0f;
	}
	
	starFountain->position.x += timeDelta * speed;
	starFountain->position.z += timeDelta * speed;
	starFountain->position.y += timeDelta * speed;

	if (starFountain->position.y > 30)
	{
		speed = -speed;
	}

	if (starFountain->position.y < -30)
	{
		speed = -speed;
	}

	// Reset the force accumulator
	force = glm::vec3(0.0f,0.0f,0.0f);
*/
	
	Game::Update(timeDelta);

}
