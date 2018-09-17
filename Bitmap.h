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
    SDL_Renderer* m_renderer;
public:
    Bitmap(int width, int height, SDL_Renderer* renderer);
    ~Bitmap();
    Bitmap(const Bitmap& other);
    Bitmap(Bitmap&& other) noexcept;
    Bitmap& operator=(const Bitmap& other);
    Bitmap& operator=(Bitmap&& other) noexcept;

    inline int get_width() const { return m_width; }
    inline int get_height() const { return m_height; }

    inline void clear(Pixel color = 0xFF000000) { std::fill(m_pixels.begin(), m_pixels.end(), color); }
    inline void clear(std::uint8_t a=255, std::uint8_t r=0, std::uint8_t g=0, std::uint8_t b=0) { std::fill(m_pixels.begin(), m_pixels.end(), (a << 24) | (r << 16) | (g << 8) | (b)); }
    inline void set_pixel(int x, int y, Pixel p) { m_pixels[y*m_width + x] = p; }
    inline void set_pixel(int x, int y, std::uint8_t a, std::uint8_t r, std::uint8_t g, std::uint8_t b) { m_pixels[y*m_width + x] = (a << 24) | (r << 16) | (g << 8) | (b); }
    inline Pixel get_pixel(int x, int y) const { return m_pixels[y*m_width + x]; }

    void update_texture();
    void render_copy();
};


#endif //SDLSOFTWARERENDERER_BITMAP_H
