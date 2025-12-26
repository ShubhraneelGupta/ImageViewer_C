#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
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
    //Open ppm file
    image = fopen(argv[1], "rb");
    if (image == NULL) 
    {
        perror("fopen");
        return 1;
    }

    //Read PPM header
    char magic[3];
    int width = 0, height = 0, maxval = 0;

    //Read Magic number
    if(fscanf(image, "%2s", magic) != 1)
    {
        fprintf(stderr, "Invalid PPM file\n");
        fclose(image);
        return 1;
    }
    // Check if it's P6 format (binary PPM)
    if (strcmp(magic, "P6\0") != 0) {
        fprintf(stderr, "Only P6 format PPM files are supported\n");
        fclose(image);
        return 1;
    }

    //Skip whitespace and comments
    int c;
    while((c = fgetc(image)) != EOF && isspace(c));

    //Handle comments
    while(c == '#')
    {
        while((c = fgetc(image)) != EOF && c != '\n');
        while((c = fgetc(image)) != EOF && isspace(c));
    }

    //Put back the non-whitespace character
    ungetc(c, image);

    //Read width, height, and maxval
    if(fscanf(image, "%d %d %d", &width, &height, &maxval) != 3)
    {
        fprintf(stderr, "Invalid PPM header\n");
        fclose(image);
        return 1;
    }

    //Skip the whitespace after the maxval chaacter
    fgetc(image);

    const int HEIGHT = height;
    const int WIDTH = width;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) 
    {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }


    //Window creation.
    SDL_Window *window = SDL_CreateWindow(
        "Image Viewer",
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
    SDL_Rect pixel = (SDL_Rect)
    {
            0, 0,
            1, 1
    };
    for(int y = 0; y < HEIGHT; y++)
    {
        for(int x = 0; x < WIDTH; x++)
        {
            Uint8 r = fgetc(image);
            Uint8 g = fgetc(image);
            Uint8 b = fgetc(image);

            //If maxval is not 255
            if(maxval != 255)
            {
                r = (r*255) / maxval;
                g = (g*255) / maxval;
                b = (b*255) / maxval;
            }

            Uint32 color = SDL_MapRGB(surface->format, r, g, b);

            pixel.x = x;
            pixel.y = y;

            SDL_FillRect(surface, &pixel, color);
        }
    }
    SDL_UpdateWindowSurface(window);

    // Event polling is required on Wayland to keep the surface mapped.
    // on X11 the window may still appear without it, but polling is still recommended.
    SDL_bool running = SDL_TRUE;

    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)
            {
                running = SDL_FALSE;
            }
            else if (e.type == SDL_QUIT)
            {
                running = SDL_FALSE;
            }
        }

        SDL_Delay(16);
    }


    fclose(image);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
