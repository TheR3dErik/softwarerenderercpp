/*
#include <iostream>
#include <SDL2/SDL.h>

int main() {
    std::std::cout << "Hello, World!" << std::std::endl;
    return 0;
}
 */
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>

#include "Bitmap.h"
#include "Vec3d.h"

int main( int argc, char** argv )
{
    SDL_Init( SDL_INIT_EVERYTHING );
    atexit( SDL_Quit );

    SDL_Window* window = SDL_CreateWindow
            (
                    "SDL2",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    640*2, 480*2,
                    SDL_WINDOW_SHOWN
            );

    SDL_Renderer* renderer = SDL_CreateRenderer
            (
                    window,
                    -1,
                    SDL_RENDERER_ACCELERATED
            );

    SDL_RendererInfo info;
    SDL_GetRendererInfo( renderer, &info );
    std::cout << "Renderer name: " << info.name << std::endl;
    std::cout << "Texture formats: " << std::endl;
    for( Uint32 i = 0; i < info.num_texture_formats; i++ )
    {
        std::cout << SDL_GetPixelFormatName( info.texture_formats[i] ) << std::endl;
    }

    Bitmap bitmap{ 640, 480, renderer };
    constexpr double max_dist { 500.0 };

    std::mt19937 rng;
    //rng.seed(std::random_device()());
    rng.seed(0);
    std::uniform_real_distribution<> dist_x(-bitmap.get_width()/2, bitmap.get_width()/2);
    std::uniform_real_distribution<> dist_y(-bitmap.get_height()/2, bitmap.get_height()/2);
    std::uniform_real_distribution<> dist_z_init(0.001,max_dist);

    constexpr int num_stars { 2000 };
    std::vector<Vec3d> stars;
    stars.reserve(num_stars);
    for (int i = 0; i < num_stars; ++i)
    {
        auto z = dist_z_init(rng);
        stars.push_back(Vec3d{ z*dist_x(rng), z*dist_y(rng), z });
    }
    std::sort(stars.begin(), stars.end(), [](Vec3d i, Vec3d j) { return i.z > j.z; });

    SDL_Event event;
    bool running = true;
    while( running )
    {
        const Uint64 start = SDL_GetPerformanceCounter();

        SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );
        SDL_RenderClear( renderer );

        while( SDL_PollEvent( &event ) )
        {
            if( ( SDL_QUIT == event.type ) ||
                ( SDL_KEYDOWN == event.type && SDL_SCANCODE_ESCAPE == event.key.keysym.scancode ) )
            {
                running = false;
                break;
            }
        }

        bitmap.clear(255,0,0,0);
        for (auto &star : stars)
        {
            star.z -= 0.1;
            if (star.z > 0)
            {
                auto projected_x = star.x / star.z;
                auto projected_y = star.y / star.z;

                if (projected_x >= -bitmap.get_width() / 2 && projected_x < bitmap.get_width() / 2 &&
                    projected_y >= -bitmap.get_height() / 2 && projected_y < bitmap.get_height() / 2)
                {
                    double luminosity = 1 - star.z / max_dist;
                    luminosity = luminosity*luminosity;
                    auto i = static_cast<std::uint8_t>(luminosity*255);
                    bitmap.set_pixel(static_cast<int>(projected_x) + bitmap.get_width() / 2,
                                     static_cast<int>(projected_y) + bitmap.get_height() / 2, 255, i,i,i);
                } else
                {
                    auto z = max_dist;
                    star = Vec3d{z*dist_x(rng), z*dist_y(rng), z };
                }
            } else
            {
                auto z = max_dist;
                star = Vec3d{z*dist_x(rng), z*dist_y(rng), z };
            }
        }

        bitmap.update_texture(renderer);

        SDL_RenderPresent( renderer );

        const Uint64 end = SDL_GetPerformanceCounter();
        const static Uint64 freq = SDL_GetPerformanceFrequency();
        const double seconds = ( end - start ) / static_cast< double >( freq );
        std::cout << "Frame time: " << seconds * 1000.0 << "ms" << std::endl;
    }

    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();
}