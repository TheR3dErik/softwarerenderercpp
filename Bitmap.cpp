//
// Created by erik on 16/09/18.
//

#include "Bitmap.h"

Bitmap::Bitmap(int width, int height, SDL_Renderer* renderer): m_width{ width }, m_height{ height }, m_pixels(width*height), m_renderer{ renderer }
{
    m_texture = SDL_CreateTexture
            (
                    renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STREAMING,
                    width, height
            );
}

void Bitmap::update_texture()
{
    SDL_UpdateTexture
            (
                    m_texture,
                    nullptr,
                    &m_pixels[0],
                    m_width * 4
            );
}

void Bitmap::render_copy()
{
    SDL_RenderCopy( m_renderer, m_texture, nullptr, nullptr );
}

Bitmap::~Bitmap()
{
    SDL_DestroyTexture(m_texture);
}

Bitmap::Bitmap(const Bitmap &other): m_width{ other.m_width }, m_height{ other.m_height }, m_pixels{ other.m_pixels }, m_renderer{ other.m_renderer }
{
    m_texture = SDL_CreateTexture
            (
                    m_renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STREAMING,
                    m_width, m_height
            );
    //Updating the texture has to be done manually!
}

Bitmap::Bitmap(Bitmap &&other) noexcept: m_width { other.m_width }, m_height{ other.m_height }, m_pixels{ std::move(other.m_pixels) }, m_renderer{ other.m_renderer }, m_texture{ other.m_texture } {}

Bitmap &Bitmap::operator=(const Bitmap &other)
{
    m_width = other.m_width;
    m_height = other.m_height;
    m_pixels = other.m_pixels;
    m_renderer = other.m_renderer;
    m_texture = SDL_CreateTexture
            (
                    m_renderer,
                    SDL_PIXELFORMAT_ARGB8888,
                    SDL_TEXTUREACCESS_STREAMING,
                    m_width, m_height
            );
    //Updating the texture has to be done manually!
    return *this;
}

Bitmap &Bitmap::operator=(Bitmap &&other) noexcept
{
    m_width = other.m_width;
    m_height = other.m_height;
    m_pixels = std::move(other.m_pixels);
    m_renderer = other.m_renderer;
    m_texture = other.m_texture;
    return *this;
}

