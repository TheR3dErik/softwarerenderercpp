//
// Created by erik on 16/09/18.
//

#include "Bitmap.h"

Bitmap::Bitmap(int width, int height, SDL_Renderer* renderer): m_width{ width }, m_height{ height }, m_pixels(width*height)
{
    m_texture = SDL_CreateTexture
            (
                    renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STREAMING,
                    width, height
            );
}

void Bitmap::clear(Pixel color)
{
    std::fill(m_pixels.begin(), m_pixels.end(), color);
}

void Bitmap::set_pixel(int x, int y, Pixel p)
{
    m_pixels[y*m_width + x] = p;
}

Pixel Bitmap::get_pixel(int x, int y) const
{
    return m_pixels[y*m_width + x];
}

void Bitmap::update_texture(SDL_Renderer *renderer)
{
    SDL_UpdateTexture
            (
                    m_texture,
                    nullptr,
                    &m_pixels[0],
                    m_width * 4
            );
    SDL_RenderCopy( renderer, m_texture, nullptr, nullptr );
}
