
#include "LTextures.h"

LTextures::LTextures()
{
  pf = 0;
  ren = NULL;
}

LTextures::LTextures(Uint32 pixelFormat, SDL_Renderer *renderer)
{
  pf = pixelFormat;
  ren = renderer;

  texture[0] = SDL_LoadBMP("./assets/tex1.bmp");
  texture[1] = SDL_LoadBMP("./assets/tex2.bmp");
  texture[2] = SDL_LoadBMP("./assets/tex3.bmp");
  texture[3] = SDL_LoadBMP("./assets/tex4.bmp");
  texture[4] = SDL_LoadBMP("./assets/tex5.bmp");
  texture[5] = SDL_LoadBMP("./assets/tex6.bmp");
  texture[6] = SDL_LoadBMP("./assets/tex7.bmp");
  texture[7] = SDL_LoadBMP("./assets/tex8.bmp");

  defaultTex = SDL_LoadBMP("./assets/tex1.bmp");
}

// void LTextures::generateTextures() {

//   array<array<u_int32_t, 4096>, 8> tmp;
//   //generate some textures
//   for(int x = 0; x < texWidth; x++)
//   for(int y = 0; y < texHeight; y++)
//   {
//     int xorcolor = (x * 256 / texWidth) ^ (y * 256 / texHeight);
//     //int xcolor = x * 256 / texWidth;
//     int ycolor = y * 256 / texHeight;
//     int xycolor = y * 128 / texHeight + x * 128 / texWidth;
//     tmp[0][texWidth * y + x] = 65536 * 254 * (x != y && x != texWidth - y); //flat red texture with black cross
//     tmp[1][texWidth * y + x] = xycolor + 256 * xycolor + 65536 * xycolor; //sloped greyscale
//     tmp[2][texWidth * y + x] = 256 * xycolor + 65536 * xycolor; //sloped yellow gradient
//     tmp[3][texWidth * y + x] = xorcolor + 256 * xorcolor + 65536 * xorcolor; //xor greyscale
//     tmp[4][texWidth * y + x] = 256 * xorcolor; //xor green
//     tmp[5][texWidth * y + x] = 65536 * 192 * (x % 16 && y % 16); //red bricks
//     tmp[6][texWidth * y + x] = 65536 * ycolor; //red gradient
//     tmp[7][texWidth * y + x] = 128 + 256 * 128 + 65536 * 128; //flat grey texture
//   }

//   texture.push_back(tmp[0]);
//   texture.push_back(tmp[1]);
//   texture.push_back(tmp[2]);
//   texture.push_back(tmp[3]);
//   texture.push_back(tmp[4]);
//   texture.push_back(tmp[5]);
//   texture.push_back(tmp[6]);
//   texture.push_back(tmp[7]);
// }

// array<u_int8_t, 3> LTextures::textureToWall(int mapValue, int x, int y) {

//     u_int8_t cr,cg,cb;

//     if (x > 64) return {0,0,0};
//     if (y > 64) return {0,0,0};

//     cr = texture[mapValue][x + y * 64] >> 24 & 0xFF;
//     cg = texture[mapValue][x + y * 64] >> 16 & 0xFF;
//     cb = texture[mapValue][x + y * 64] >> 8 & 0xFF;

//     return {cr,cg,cb};
// }

// int LTextures::getTextureCount() {
//     return texture.size();
// }

/*
Copies the pixels from a SDL2 surface.
You should free() the returned pixels when you're done with it.
Taken from https://stackoverflow.com/questions/23367098/getting-the-sdl-color-of-a-single-pixel-in-a-sdl-texture
*/
Uint8 *LTextures::CopySurfacePixels(
    SDL_Surface *surface, // surface to take pixels from
    int *width,           // stores result width
    int *height,          // stores result height
    int *pitch)           // stores result pitch
{
  Uint8 *pixels = 0;
  SDL_Surface *tmpSurface = 0;
  SDL_Texture *texture = 0;
  int sizeInBytes = 0;
  void *tmpPixels = 0;
  int tmpPitch = 0;

  tmpSurface = SDL_ConvertSurfaceFormat(surface, pf, 0);
  if (!tmpSurface)
  {
    tmpSurface = SDL_ConvertSurfaceFormat(defaultTex, pf, 0);
    texture = SDL_CreateTexture(ren, pf,
                                SDL_TEXTUREACCESS_STREAMING,
                                tmpSurface->w, tmpSurface->h);
  }
  if (tmpSurface)
  {
    texture = SDL_CreateTexture(ren, pf,
                                SDL_TEXTUREACCESS_STREAMING,
                                tmpSurface->w, tmpSurface->h);
  }
  else
  {
    cout << "Error converting surface: " << SDL_GetError() << endl;
    return NULL;
  }

  if (texture)
  {
    if (width)
    {
      *width = tmpSurface->w;
    }
    if (height)
    {
      *height = tmpSurface->h;
    }
    if (pitch)
    {
      *pitch = tmpSurface->pitch;
    }
    sizeInBytes = tmpSurface->pitch * tmpSurface->h;
    pixels = (Uint8 *)malloc(sizeInBytes);
    SDL_LockTexture(texture, 0, &tmpPixels, &tmpPitch);
    memcpy(pixels, tmpSurface->pixels, sizeInBytes);
    SDL_UnlockTexture(texture);
  }
  else
  {
    cout << "Error creating texture" << endl;
  }

  // Cleanup
  if (texture)
  {
    SDL_DestroyTexture(texture);
  }
  if (tmpSurface)
  {
    SDL_FreeSurface(tmpSurface);
  }

  return pixels;
}

SDL_Color LTextures::textureToWall(int mapValue, int x, int y)
{

  int bpp = 3;
  try
  {
    SDL_PixelFormat *fmt = texture[mapValue]->format;
    bpp = fmt->BytesPerPixel;  

  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
  

  /* Here p is the address to the pixel we want to retrieve */
  Uint8 *p = (Uint8 *)texture[mapValue]->pixels + y * texture[mapValue]->pitch + x * bpp;

  SDL_Color rgb;
  switch (bpp)
  {
  case 1:
    SDL_GetRGB(*p, texture[mapValue]->format, &rgb.r, &rgb.g, &rgb.b);
    return rgb;
    break;

  case 2:
    SDL_GetRGB(*(Uint16 *)p, texture[mapValue]->format, &rgb.r, &rgb.g, &rgb.b);
    return rgb;
    break;
  case 3:
      if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        Uint32 color = p[0] << 16 | p[1] << 8 | p[2];
        SDL_GetRGB(color, texture[mapValue]->format, &rgb.r, &rgb.g, &rgb.b);
        return rgb;
      }
      else {
          Uint32 color = p[0] | p[1] << 8 | p[2] << 16;
          SDL_GetRGB(color, texture[mapValue]->format, &rgb.r, &rgb.g, &rgb.b);
          return rgb;
      }

    case 4:
      SDL_GetRGB(*(Uint32 *)p, texture[mapValue]->format, &rgb.r, &rgb.g, &rgb.b);
      return rgb;
      break;
  default:
    return rgb; /* shouldn't happen, but avoids warnings */
  }

  return rgb;

  // int w=0, h=0, p=0;
  // Uint8* pixels = CopySurfacePixels(texture[mapValue], &w, &h, &p);
  // if (pixels)
  // {
  //     // Assuming BGRA format
  //     int b = pixels[4 * (y * w + x) + 0]; // Blue
  //     int g = pixels[4 * (y * w + x) + 1]; // Green
  //     int r = pixels[4 * (y * w + x) + 2]; // Red
  //     int a = pixels[4 * (y * w + x) + 3]; // Alpha

  //     free(pixels);
  //     // printf("Pixel at (%d,%d) has RGBA color (%d,%d,%d,%d)\n", x, y, r, g, b, a);

  //     return {r, g, b};
  // }
  // else { return {0,0,0}; }
}