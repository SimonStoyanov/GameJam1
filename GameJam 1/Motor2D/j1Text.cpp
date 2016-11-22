#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Text.h"
#include "j1FileSystem.h"


j1Text::j1Text() : j1Module()
{
	name.create("text");
}

// Destructor
j1Text::~j1Text()
{}

// Called before render is available
bool j1Text::Awake(pugi::xml_node& config)
{
	LOG("Loading Text Creator");
	bool ret = true;

	int init = TTF_Init();

	if (init)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", TTF_GetError());
		ret = false;
	}

	return ret;
}


// Called before the first frame
bool j1Text::Start()
{
	LOG("start text");
	bool ret = true;

	timeless_15 = LoadFont("Timeless.ttf", 15);

	return ret;
}

// Called before quitting
bool j1Text::CleanUp()
{
	LOG("Freeing Text library");

	TTF_Quit();

	return true;
}

// ------------------------------
// ------------------------------

// Loads a font a return it
int j1Text::LoadFont(char * path, int size)
{
	int ret = -1;

	TTF_Font* font = TTF_OpenFontRW(App->fs->Load(path), 1, size);

	if (font == NULL)
	{
		LOG("Could not load font with path: %s. IMG_Load: %s", path, TTF_GetError());
	}
	else
	{
		fonts.add(font);
		ret = fonts.count() - 1;
	}

	return ret;
}

// ------------------------------

// Text constructors
Text::Text(char* _text, int x, int y, int _font, int _spacing, uint _r, uint _g, uint _b)
{
	print = true;
	alignment = middle;
	longuest_texture = 0;

	text = _text;
	pos.x = x; 
	pos.y = y;
	font = App->text->fonts[_font];
	color.r = _r; color.g = _g; color.b = _b;
	spacing = _spacing;
	is_ui = false;

	SetUpText();
}

// ------------------------------

Text::Text(int x, int y, int _font, int _spacing, uint _r, uint _g, uint _b)
{
	print = true; 
	alignment = middle;
	longuest_texture = 0;

	pos.x = x;
	pos.y = y;
	font = App->text->fonts[_font];
	color.r = _r; color.g = _g; color.b = _b;
	spacing = _spacing;
	is_ui = false;
}

// ------------------------------

Text::Text(Text* _text)
{
	print = true;
	alignment = middle;
	longuest_texture = 0;

	text = _text->text;
	pos = _text->pos;
	font = _text->font;
	color = _text->color;
	spacing = _text->spacing;
	is_ui = _text->is_ui;
	
	for (int i = 0; i < _text->texture_list.count(); i++)
		texture_list.add(_text->texture_list[i]);

	if (font != NULL)
		font = _text->font;
	else
		LOG("Text constructor: Could not load font because is NULL");
}

// ------------------------------

// Destructor
Text::~Text()
{
	CleanTextures();
}

// Changes the text
void Text::SetText(char* _text, Alignment _alignment)
{
	text = _text;
	alignment = _alignment;
	SetUpText();
}

// ------------------------------

void Text::CleanTextures()
{
	for(int i = 0; i<texture_list.count(); i++)
	{
		if (texture_list[i].texture != nullptr)
			App->tex->UnLoad(texture_list[i].texture);
	}

	texture_list.clear();
}

// ------------------------------

uint Text::GetSpaces()
{
	int text_counter = 0;
	uint spaces_counter = 0;

	if (strlen(text) > 0)
	{
		while (*text != '\0')
		{
			if (*text == '\n')
				spaces_counter++;
			text++;
			text_counter++;
		}
		text -= text_counter;
	}
	return spaces_counter;
}


// ------------------------------

// Changes the font
void Text::SetFont(TTF_Font * _font)
{
	font = _font;
}

// ------------------------------

void Text::SetUpText()
{
	// Checks if we have a font
	if (font == NULL)
	{
		LOG("Text constructor: Could not load font because is NULL");
		return;
	}

	// Cleans all textures nad word positions form the lists
	CleanTextures();
	words_position.clear();

	// Set size of the char string
	int size = GetSpaces() + strlen(text) + 1;

	// Start
	char* tmp = text;
	char* to_add = new char[size];
	int char_lenght = 0;
	int previous = 0;

	// Add 0 position 
	words_position.add(0);

	// Do things
	while(*tmp != '\0')
	{
		*to_add = *tmp;
		
		// Add lenght of every word to the list
		if (*to_add != '\0')
		{
			to_add++; 
			*to_add = '\0';
			to_add--;
			SDL_Surface* surface = TTF_RenderText_Solid(font, to_add, color);

			if (*to_add == 'f' || *to_add == 'j') // Special cases that don't work well idk why :C
			{
				previous += surface->w - 2;
			}
			else
				previous += surface->w;

			words_position.add(previous - 2);

			//LOG("%d %d", previous, surface->w);
			SDL_FreeSurface(surface);
		}

		// Add a line
		if (*tmp == '\n')
		{
			*to_add = '\0'; to_add -= char_lenght;
			
			//LOG("%s", to_add);
			SDL_Surface* surface = TTF_RenderText_Solid(font, to_add, color);
			SDL_Texture* texture = App->tex->LoadSurface(surface);
			
			texture_list.add(TexAndLen(texture, surface->w));

			SetLonguestTexture(surface);

			SDL_FreeSurface(surface);
			to_add -= char_lenght;
			char_lenght = 0;
		}
		else
		{
			to_add++;
			char_lenght++;
		}

		tmp++;
	}

	// Add final line
	*to_add = *tmp; to_add -= char_lenght;

	//LOG("%s", to_add);
	SDL_Surface* surface = TTF_RenderText_Solid(font, to_add, color);
	if (surface != nullptr)
	{
		SDL_Texture* texture = App->tex->LoadSurface(surface);

		SetLonguestTexture(surface);

		texture_list.add(TexAndLen(texture, surface->w));
	}

	// Clean
	SDL_FreeSurface(surface);
	memset(&to_add[0], 0, sizeof(to_add));
}

// ------------------------------

// Prints text
void Text::PrintText()
{
	if (print == true)
	{
		switch(alignment)
		{
		case left:
			for (int i = 0; i < texture_list.count(); i++)
			{
				if(!is_ui)
					App->render->Blit(texture_list[i].texture, pos.x, pos.y + (i * spacing), NULL);
				else
					App->render->Blit(texture_list[i].texture, pos.x - App->render->camera.x, pos.y + (i * spacing) - App->render->camera.y, NULL);
			}
			break;

		case middle:
			for (int i = 0; i < texture_list.count(); i++)
			{
				int difference = longuest_texture - (texture_list[i].lenght);
				App->render->Blit(texture_list[i].texture, pos.x + (difference * 0.5), pos.y + (i * spacing), NULL);
			}
			break;

		case right:
			for (int i = 0; i < texture_list.count(); i++)
			{
				int difference = longuest_texture - (texture_list[i].lenght);
				App->render->Blit(texture_list[i].texture, pos.x + difference, pos.y + (i * spacing), NULL);
			}
			break;
		}
	}
}

// ------------------------------

void Text::SetLonguestTexture(SDL_Surface* surface)
{
	if (surface != nullptr)
	{
		if (surface->w > longuest_texture)
			longuest_texture = surface->w;
	}
}
