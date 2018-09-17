//
// Created by erik on 16/09/18.
//

#ifndef SDLSOFTWARERENDERER_BITMAP_H
#define SDLSOFTWARERENDERER_BITMAP_H

#include <cstdint>
#include <vector>

#include <SDL2/SDL.h>

using Pixel = std::uint32_t;

class Bitmap {
private:
    int m_width;
    int m_height;

    std::vector<Pixel> m_pixels;

    SDL_Texture* m_texture;
public:
    Bitmap(int width, int height, SDL_Renderer* renderer);

    inline int get_width() const { return m_width; }
    inline int get_height() const { return m_height; }

    void clear(Pixel color = 0xFF000000);
    void set_pixel(int x, int y, Pixel p);
    Pixel get_pixel(int x, int y) const;

    void update_texture(SDL_Renderer* renderer);
};


#endif //SDLSOFTWARERENDERER_BITMAP_H
