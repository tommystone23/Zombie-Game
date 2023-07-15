#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "Entity.h"
#include <string>
#include "Human.h"

class Zombie : public Entity
{
public:
    Zombie();
    ~Zombie();

    void init(float speed, glm::vec2 position);

    virtual void update(const std::vector<std::string> &level_data,
                        std::vector<Human*> &humans, std::vector<Zombie*> &zombies,
                        float delta_time, AudioEngine &audio) override;

private:
    Human* get_nearest_human(std::vector<Human*>& humans);
};

#endif