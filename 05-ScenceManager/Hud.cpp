#include "Hud.h"

#define MARGINLEFT			10
#define board_margin_y		192

#define time_margin_x		150

#define money_margin_y		199

#define speedbar_margin_x	61

#define left_margin_x		48

//score
#define score_margin_x		110
#define score_margin_y		207

// card
#define card_1_margin_x		174
#define card_2_margin_x		199
#define card_3_margin_x		222
#define card_margin_y		198


Hud::Hud() {
	this->board = CAnimationSets::GetInstance()->Get(1101);
	this->background = CSprites::GetInstance()->Get(2102);
	this->card = CAnimationSets::GetInstance()->Get(1130);

	speedBar = new SpeedBar();
}

char* IntToChar(int value, int len = 10)
{
	char* c = new char[len + 1];
	c[len] = '\0';
	for (int i = len - 1; i >= 0; i--)
	{
		c[i] = value % 10 + 48;
		value = value / 10;
	}
	return c;
}

void Hud::Render(D3DXVECTOR2 position, CMario* mario, int reamainingTime, int stage) {
	background->Draw(position.x, position.y + board_margin_y);
	board->at(0)->Render(position.x + MARGINLEFT, position.y + board_margin_y);

	// scores
	text.Render(IntToChar(mario->GetScore(), 7), { position.x + score_margin_x, position.y + score_margin_y });

	// money
	int coin = mario->GetCoin();
	string strCoin = to_string(coin);
	int length = strCoin.length();

	text.Render(IntToChar(mario->GetCoin(), length), { position.x + time_margin_x, position.y + money_margin_y });

	// mario's level
	text.Render(IntToChar(mario->GetLevel(), 1), { position.x + left_margin_x, position.y + score_margin_y });

	// time
	text.Render(IntToChar(reamainingTime, 3), { position.x + time_margin_x, position.y + score_margin_y });

	// item
	for (size_t i = 0; i < mario->cards.size(); i++)
	{
		switch (i)
		{
		case 0:
			card->at(mario->cards.at(i))->Render(position.x + card_1_margin_x, position.y + card_margin_y);
			break;
		case 1:
			card->at(mario->cards.at(i))->Render(position.x + card_2_margin_x, position.y + card_margin_y);
			break;
		case 2:
			card->at(mario->cards.at(i))->Render(position.x + card_3_margin_x, position.y + card_margin_y);
			break;
		}
	}
	//world map
	text.Render(IntToChar(1, 1), { position.x + left_margin_x, position.y + money_margin_y });

	//speedbar
	speedBar->Render(position.x + speedbar_margin_x, position.y + money_margin_y, mario->vx);

}

