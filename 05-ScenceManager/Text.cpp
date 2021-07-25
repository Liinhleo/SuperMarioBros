#include "Text.h"

void Text::RenderCharacter(char c, D3DXVECTOR2 position)
{
	character = CSprites::GetInstance()->Get(spriteID.at(c));
	character->Draw(position.x, position.y);
}

void Text::Render(char* s, D3DXVECTOR2 position)
{
	for (int i = 0; i < strlen(s) / 2; i++) {
		char temp = s[i];
		s[i] = s[strlen(s) - i - 1];
		s[strlen(s) - i - 1] = temp;
	}

	for (int i = strlen(s) - 1; i >= 0; i--) {
		RenderCharacter(s[i], { position.x - i * CHAR_SIZE, position.y });
	}
}
