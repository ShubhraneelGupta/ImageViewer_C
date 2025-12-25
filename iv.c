#include <SDL2/SDL.h>
#include <stdio.h>

int main(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Test Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_MOUSE_GRABBED
    );
    if (!window) {
        fprintf(stderr, "CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Surface *surface = SDL_GetWindowSurface(window);
    if(!surface){
        fprintf(stderr, "Getting window surface failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
//GPU WORKFLOW
    // SDL_Renderer *renderer = SDL_CreateRenderer(
    //     window,
    //     -1,
    //     SDL_RENDERER_ACCELERATED
    // );
    // if (!renderer) {
    //     fprintf(stderr, "CreateRenderer failed: %s\n", SDL_GetError());
    //     SDL_DestroyWindow(window);
    //     SDL_Quit();
    //     return 1;
    // }

    // Uint32 start = SDL_GetTicks();
    // while (SDL_GetTicks() - start < 5000) {
    //     SDL_Event event;
    //     while (SDL_PollEvent(&event)) {
    //         /* polling only */
    //     }

    //     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //     SDL_RenderClear(renderer);
    //     SDL_RenderPresent(renderer);

    //     SDL_Delay(16);
    // }

    //SDL_DestroyRenderer(renderer);


//CPU WORKFLOW
    SDL_FillRect(
        surface,
        NULL,
        0xFFFFFF
    );

    SDL_UpdateWindowSurface(window);

    // Event polling is required on Wayland to keep the surface mapped.
    // on X11 the window may still appear without it, but polling is still recommended.
    Uint32 start = SDL_GetTicks();
    while (SDL_GetTicks() - start < 2000) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {}
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
