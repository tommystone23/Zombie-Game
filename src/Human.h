#ifndef HUMAN_H
#define HUMAN_H

#include "Entity.h"
#include <string>

class Human : public Entity
{
public:
    Human();
    virtual ~Human();

    void init(float speed, glm::vec2 position);

    virtual void update(const std::vector<std::string> &level_data,
                        std::vector<Human*> &humans, std::vector<Zombie*> &zombies,
                        float delta_time, AudioEngine &audio) override;
};

#endif