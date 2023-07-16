#ifndef PARTICLE_BATCH2D_H
#define PARTICLE_BATCH2D_H
#include <glm/glm.hpp>
#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

class Particle
{
public:
    Particle();
    ~Particle();

    void update(float delta_time, float decay_rate);

    friend class ParticleBatch2D;

private:
    glm::vec2 _position;
    glm::vec2 _velocity;
    Color _color;
    float _life_time;
    float _width;
};

class ParticleBatch2D
{
public:
    ParticleBatch2D(int max_particles, float decay_rate, GLTexture texture);
    ~ParticleBatch2D();

    void update(float delta_time);
    void draw(SpriteBatch &sprite_batch);

    void add_particle(const glm::vec2 &position, const glm::vec2 &velocity, 
                        const Color &color, float width);

private:
    int find_free_particle();

    float _decay_rate;

    Particle *_particles;

    int _max_particles;
    int _last_free_particle;

    GLTexture _texture;
};

#endif // PARTICLE_BATCH2D_H