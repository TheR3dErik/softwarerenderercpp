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
#include <chrono>

#include "Bitmap.h"
#include "Vec3d.h"

int main( int argc, char** argv )
{
    SDL_Init( SDL_INIT_EVERYTHING );
    atexit( SDL_Quit );

    SDL_Window* window = SDL_CreateWindow
            (
                    "Starfield",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    640*4, 480*4,
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

    Bitmap bitmap{ 640*2, 480*2, renderer };

    constexpr double max_dist { 1000.0 };
    constexpr int num_stars { 20000 };
    const double tanHalfFOV { std::tan(3.14159 / 360 * 120.0 )};
    const double tanHalf150 { std::tan(3.14159 / 360 * 150.0 )};

    std::mt19937 rng;
    //rng.seed(std::random_device()());
    rng.seed(0); //just for testing
    std::uniform_real_distribution<> dist_x(-max_dist*tanHalfFOV, max_dist*tanHalfFOV);
    std::uniform_real_distribution<> dist_y(-max_dist*tanHalfFOV, max_dist*tanHalfFOV);
    std::uniform_real_distribution<> dist_z(0.001,max_dist);
    std::uniform_real_distribution<> zero_one(0,1);

    std::vector<Vec3d> stars;
    stars.reserve(num_stars);
    for (int i = 0; i < num_stars; ++i)
    {
        auto z = dist_z(rng);
        auto y = dist_y(rng);
        auto x = dist_x(rng);
        while (y/z > tanHalfFOV || x/z > tanHalfFOV)
        {
            y = dist_y(rng);
            x = dist_x(rng);
        }
        stars.push_back(Vec3d{ dist_x(rng), dist_y(rng), dist_z(rng)});
    }
    std::sort(stars.begin(), stars.end(), [](Vec3d i, Vec3d j) { return i.z > j.z; });

    SDL_Event event;
    bool running = true;
    auto last_frame = std::chrono::steady_clock::now();
    while( running )
    {
        auto frame_start = std::chrono::steady_clock::now();
        double time_diff = std::chrono::duration_cast<std::chrono::microseconds>(frame_start-last_frame).count() / 1000000.0;
        last_frame = frame_start;

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
            star.z -= 100 * (time_diff);
            if (star.z > 0)
            {
                auto projected_x = star.x * bitmap.get_width() / (2*tanHalfFOV*star.z);
                auto projected_y = star.y * bitmap.get_height() / (2*tanHalfFOV*star.z);

                if (projected_x >= -bitmap.get_width() / 2 && projected_x < bitmap.get_width() / 2 &&
                    projected_y >= -bitmap.get_height() / 2 && projected_y < bitmap.get_height() / 2)
                {
                    double luminosity = 1 - star.z / max_dist;
                    luminosity = luminosity*luminosity;
                    auto i = static_cast<std::uint8_t>(luminosity*255);
                    bitmap.set_pixel(static_cast<int>(projected_x) + bitmap.get_width() / 2,
                                     static_cast<int>(projected_y) + bitmap.get_height() / 2, 255, i,i,i);
                }
                /*
                else
                {
                    auto z = max_dist;
                    star = Vec3d{ dist_x(rng), dist_y(rng), max_dist};
                }
                 */
            } else
            {
                auto z = max_dist;
                star = Vec3d{ dist_x(rng), dist_y(rng), max_dist};
            }
        }
        //this line is very performance heavy and doesn't have that many benefits
        //remove if necessary
        //std::sort(stars.begin(), stars.end(), [](Vec3d i, Vec3d j) { return i.z > j.z; });

        bitmap.update_texture();
        bitmap.render_copy();

        SDL_RenderPresent( renderer );

        std::cout << "Frame time: " << time_diff * 1000.0 << "ms" << std::endl;
    }

    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();
}