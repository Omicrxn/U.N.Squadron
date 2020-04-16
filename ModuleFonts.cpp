#include "Application.h"

#include "ModuleTextureManager.h"
#include "ModuleRenderer.h"
#include "ModuleFonts.h"

#include<string.h>

ModuleFonts::ModuleFonts(bool isEnabled) : Module(isEnabled)
{

}

ModuleFonts::~ModuleFonts()
{

}

// Load new texture from file path
int ModuleFonts::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = App->textures->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
		if (fonts[id].texture == nullptr)
			break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	Font& font = fonts[id];

	font.texture = tex;
	font.rows = rows;

	// totalLength ---	length of the lookup table
	// table ---------  All characters displayed in the same order as the texture
	// columns -------  Amount of chars per row of the texture
	// char_w --------	Width of each character
	// char_h --------	Height of each character

	strcpy_s(font.table, characters);
	font.totalLength = strlen(font.table);

	font.columns = (font.totalLength / font.rows);

	uint texture_w, texture_h;
	App->textures->GetSize(font.texture, texture_w, texture_h);
	font.char_w = (texture_w / font.columns);
	font.char_h = (texture_h / font.rows);

	LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void ModuleFonts::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].texture != nullptr)
	{
		App->textures->Unload(fonts[font_id].texture);
		fonts[font_id].texture = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

void ModuleFonts::BlitText(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].texture == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect spriteRect;
	uint len = strlen(text);

	spriteRect.w = font->char_w;
	spriteRect.h = font->char_h;

	for (uint i = 0; i < len; ++i)
	{
		// 1 - Find the location of the current character in the lookup table
		uint locationInTable = 0;
		for (uint j = 0; j < strlen(font->table); ++j) {
			if (font->table[j] == text[i]) {
				locationInTable = j;
				break;
			}
		}

		// 2 - Retrieve the position of the current character in the sprite
		spriteRect.x = (locationInTable % font->columns) * font->char_w;
		spriteRect.y = (locationInTable / font->columns) * font->char_h;

		// 3 - Blit the character at its proper position
		App->render->Blit(font->texture, x, y, &spriteRect, false);

		// 4 - Advance the position where we blit the next character
		x += font->char_w;
	}
}
