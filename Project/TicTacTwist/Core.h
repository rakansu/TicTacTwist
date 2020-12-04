#ifndef CORE_H
#define CORE_H
#include <vector>
#include <irrklang/irrKlang.h>
using namespace irrklang;


extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern ISoundEngine* Audio;

extern class Event;
extern class eListener;
extern class Test;

extern class Mouse;
extern class Input;
extern class GameObject;

extern Input input;

extern unsigned int PLAYER1_PIECE_TEXTURE;
extern unsigned int PLAYER2_PIECE_TEXTURE;

extern std::vector<GameObject*> render_buffer;
extern std::vector<GameObject*> render_buffer2;

extern unsigned int* text_textures;
extern unsigned int* card_textures;
extern unsigned int* misc_textures;


#endif 
