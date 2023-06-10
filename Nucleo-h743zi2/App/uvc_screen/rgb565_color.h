#ifndef _GFX_COLOR_
#define _GFX_COLOR_


//При работе с такими экранами приходится иметь дело с 16-битными цветами, 
//закодированными в формате RGB565 или rrrrr:ggg ggg:bbbbb,
// 5 бит красного (в старшем байте), 6 бит зелёного и 5 бит синего (в младшем байте).
//Таким образом, красная и синяя компонента имеют 32 возможных значения, а зелёная - 64.
#define GFX_RGB(R,G,B)                (((R>>3)<<11) | ((G>>2)<<5) | (B>>3)<<0)
//#define GFX_RGB(R,G,B)                  ( ((B>>3)<<11) | ((R>>3)<<5) |  ((G>>2)<<0) )

#define UVC_INK   GFX_COLOR_GREY
#define UVC_PAPER GFX_COLOR_GREYl

typedef enum {
  GFX_COLOR_RED       = GFX_RGB(255,  0,  0), // red
  GFX_COLOR_GREEN     = GFX_RGB(  0,255,  0), // green
  GFX_COLOR_GREENl    = GFX_RGB(  0,128,  0), // green
  GFX_COLOR_BLUE      = GFX_RGB(  0,  0,255), // blue
  GFX_COLOR_BLACK     = GFX_RGB(  0,  0,  0), // black
  GFX_COLOR_GREY      = GFX_RGB(192,192,192), // grey
  GFX_COLOR_GREYl      = GFX_RGB(73,73,73), // grey
  //GFX_COLOR_GREY      = GFX_RGB(210,210,207), // grey
  GFX_COLOR_WHITE     = GFX_RGB(255,255,255), // white
  GFX_COLOR_PINK      = GFX_RGB(255,192,203), // pink
  GFX_COLOR_YELLOW    = GFX_RGB(255,255,  0), // yellow
  GFX_COLOR_GOLDEN    = GFX_RGB(255,215,  0), // golden
  GFX_COLOR_BROWN     = GFX_RGB(128, 42, 42), // brown
  GFX_COLOR_CYAN      = GFX_RGB(  0,255,255), // cyan
  GFX_COLOR_PURPLE    = GFX_RGB(160, 32,240), // purple
}GFX_Color;

#endif /* _GFX_COLOR_ */