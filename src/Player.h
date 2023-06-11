#ifndef PLAYER_H
#define PLAYER_H

#include "Human.h"
#include "Engine/InputManager.h"
#include <string>
#include "Engine/Camera2D.h"
#include "Bullet.h"
#include "Gun.h"

class Player : public Human
{
public:
    Player();
    ~Player();

    void init(int speed, glm::vec2 position, InputManager *input_manager, Camera2D *camera, std::vector<Bullet> *bullets);
    virtual void update(const std::vector<std::string> &level_data,
                        std::vector<Human*> &humans, std::vector<Zombie*> &zombies);

    void add_gun(Gun *gun);
private:
    InputManager *_input_manager;
    std::vector<Gun*> _guns;
    Camera2D *_camera;
    std::vector<Bullet> *_bullets;

    int _cur_gun_index;
};

#endif