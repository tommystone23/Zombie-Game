#ifndef SPRITE_BATCH_H
#define SPRITE_BATCH_H
#include <GL/glew.h>
#include "Vertex.h"
#include <glm/glm.hpp>
#include <vector>

enum class GlyphSortType
{
    NONE,
    FRONT_TO_BACK,
    BACK_TO_FRONT,
    TEXTURE
};

struct Glyph
{
    GLuint texture;
    float depth;

    Vertex top_left;
    Vertex bottom_left;
    Vertex top_right;
    Vertex bottom_right;
};

class RenderBatch
{
public:
    RenderBatch(GLuint Offset, GLuint Num_Vertices, GLuint Texture) : offset(Offset),
                num_vertices(Num_Vertices), texture(Texture) {}
    GLuint offset;
    GLuint num_vertices;
    GLuint texture;
};

class SpriteBatch
{
public:
    SpriteBatch();
    ~SpriteBatch();

    void init();

    void begin(GlyphSortType sort_type = GlyphSortType::TEXTURE);
    void end();

    void draw(const glm::vec4 &dest_rect, const glm::vec4 &uv_rect, GLuint texture, float depth, const Color &color);
    void render_batch();
private:
    GLuint _vbo;
    GLuint _vao;

    GlyphSortType _sort_type;

    static bool compare_front_to_back(Glyph *a, Glyph *b);
    static bool compare_back_to_front(Glyph *a, Glyph *b);
    static bool compare_texture(Glyph *a, Glyph *b);

    void create_render_batches();
    void create_vao();
    void sort_glyphs();

    std::vector<Glyph*> _glyphs;
    std::vector<RenderBatch> _render_batches;
};

#endif