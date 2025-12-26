#include <SDL2/SDL.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    //File handling
    FILE *image;

    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    //Extracting file name and extension.
    char fileName[128];
    char fileExt[16];
    sscanf(argv[1], "%s", &fileName);

    char *dot = strrchr(fileName, '.');
    if (dot && dot[1] != '\0') 
    {
        strncpy(fileExt, dot + 1, sizeof(fileExt) - 1);
        fileExt[sizeof(fileExt) - 1] = '\0';
    } else 
    {
        fileExt[0] = '\0'; 
    }

    //Only works for .ppm file format currently.
    if(strcmp("ppm\0", fileExt))
    {
        fprintf(stderr, "Only supports .ppm for now.\n");
        return 1;
    }


//PPM
    image = fopen(argv[1], "r");
    if (image == NULL) 
    {
        perror("fopen");
        return 1;
    }
    //Finding height and width
    char height[8];
    char width[8];

    //Height and width are in 3rd line for ppm files
    char buf[256]; int line = 0;
    while(fgets(buf, sizeof buf, image))
    {
        line++;
        if(line == 3)
        {
            sscanf(buf, "%s %s", &width, &height);
            break;
        }
    }

    //converting height and width to int
    const int HEIGHT = atoi(height);
    const int WIDTH = atoi(width);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }


    //Window creation.
    SDL_Window *window = SDL_CreateWindow(
        "Test Window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        0
    );
    if (!window) 
    {
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
    Uint8 r, g, b;
    SDL_Rect pixel = (SDL_Rect)
    {
            0, 0,
            1, 1
    };
    for(int x = 0; x < WIDTH; x++)
    {
        for(int y = 0; y < HEIGHT; y++)
        {
            Uint32 color = SDL_MapRGB(surface->format, (x+y)%255, (x*y)%255, (x*x)%255);
            pixel.x = x; pixel.y = y;
            SDL_FillRect(
                surface,
                &pixel,
                color
            );
        }
    }
    SDL_UpdateWindowSurface(window);

    // Event polling is required on Wayland to keep the surface mapped.
    // on X11 the window may still appear without it, but polling is still recommended.
    Uint32 start = SDL_GetTicks();
    while (SDL_GetTicks() - start < 2000) 
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {}
        SDL_Delay(16);
    }

    fclose(image);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
