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

#include "Bitmap.h"

int main( int argc, char** argv )
{
    SDL_Init( SDL_INIT_EVERYTHING );
    atexit( SDL_Quit );

    SDL_Window* window = SDL_CreateWindow
            (
                    "SDL2",
                    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                    1000, 1000,
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

    Bitmap bitmap{ 256, 256, renderer };

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

        //bitmap.clear(Pixel{0,0,255,255});

        // splat down some random pixels
        for( unsigned int i = 0; i < 500; i++ )
        {
            const unsigned int x = rand() % bitmap.get_width();
            const unsigned int y = rand() % bitmap.get_height();

            //bitmap.set_pixel(x, y, Pixel{ static_cast<std::uint8_t>(rand() % 256), static_cast<std::uint8_t>(rand() % 256), static_cast<std::uint8_t>(rand() % 256), SDL_ALPHA_OPAQUE });
            bitmap.set_pixel(x,y,Pixel{ 255, 0, 0, 255 });
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