#ifndef __j1TEXT_H__
#define __j1TEXT_H__

#include "j1Module.h"
#include "p2List.h"

#include "SDL/include/SDL.h"
#include "SDL2_ttf-2.0.14\include\SDL_ttf.h"
#pragma comment(lib, "SDL2_ttf-2.0.14/lib/x86/SDL2_ttf.lib")


struct SDL_Texture;
struct SDL_Surface;

class Text;

enum Alignment
{
	left,
	middle,
	right
};

struct TexAndLen
{
	TexAndLen(SDL_Texture* _texture, uint _lenght)
	{
		texture = _texture;
		lenght = _lenght;
	}
	TexAndLen(){}

	SDL_Texture* texture;
	uint lenght;
};

// -------------------------------

class j1Text : public j1Module
{
public:

	j1Text();

	// Destructor
	virtual ~j1Text();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before quitting
	bool CleanUp();

	int LoadFont(char* path, int size);

public:
	p2List<TTF_Font*> fonts;

};

class Text
{
public:
	// Constructors
	Text(char* _text, int x, int y, int font, int _spacing = 10, uint _r = 255, uint _g = 255, uint _b = 255);
	Text(int x, int y, int _font, int _spacing = 10, uint _r = 255, uint _g = 255, uint _b = 255);
	Text(Text* text);

	// Destructor
	~Text();

	// Functions
	void PrintText();
	void SetFont(TTF_Font* _font);
	void SetText(char* _text, Alignment _alignment = left);

private:
	void SetUpText();
	void SetLonguestTexture(SDL_Surface* surface);
	void CleanTextures();
	uint GetSpaces();

public:
	SDL_Color			  color;
	char*				  text;
	iPoint				  pos;
	int				      spacing;
	bool				  print;
	Alignment			  alignment;
	p2List<int>			  words_position;
	bool				  is_ui;

private:
	TTF_Font*			  font;
	uint				  longuest_texture;
	p2List<TexAndLen>     texture_list;
};

#endif // __j1TEXT_H__