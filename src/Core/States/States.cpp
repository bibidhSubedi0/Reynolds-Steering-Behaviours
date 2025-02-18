#include "States.h"

void States::updateBoidsDeafult(const std::vector<std::unique_ptr<Boid>>& boids)
{
    // Can put all the checks here
    updateAccordingToNeighbours(boids);


    for (const auto& b : boids) // Iterate through each boid
    {
        glm::vec2 vel = b->getVelocity();
        b->setFacingDirection(vel);
        b->setPosition(b->getPosition() + vel);
    }
}

// Now for each boid i will have it's neighbours
// i.e for b in boids, i will have 
// b->getNeighbours() 
// which will be of type std::vector<std::unique_ptr<Boid>>


void States::updateAccordingToNeighbours(const std::vector<std::unique_ptr<Boid>>& boids)
{
    for (const auto& b : boids)
    {   
        std::vector<neighbor_info> allNeighbours = b->getNeighbors();
        glm::vec2 res_vel = { 0, 0 };
        float totalWeight = 0.0f;

        for (const auto& n : allNeighbours)
        {
            float distance = glm::length(n.position - b->getPosition());
            if (distance == 0.0f) continue;

            // Inverse distance weighting -> Farther the neighbour, lesser the influence
            float weight = 1.0f / distance; 
            res_vel += weight * n.velocity;
            totalWeight += weight;
        }

        if (totalWeight > 0.0f)
        {
            res_vel /= totalWeight; // This will just normalize the resultant velocity
            res_vel = glm::normalize(res_vel) * 1.0f; // Preserve speed of 2.0f , I think it should not be hard coded
        }
        else
        {
            res_vel = glm::normalize(b->getVelocity()) * 1.0f; // Keep current velocity if no neighbors
        }

        b->setVelocity(res_vel);
    }
}
