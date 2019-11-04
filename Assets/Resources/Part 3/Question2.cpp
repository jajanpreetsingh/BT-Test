#include "../UserInterface.h"

Match2TestWnd match2TestWnd(false, "Match2TestWnd");
#define NOT_SELECTED -1
#define ENTERED 0



void Match2TestWnd::clearSelection(gameState * state)
{
	state->sequence.clear();

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			state->selectOrdr[i][j] = NOT_SELECTED;
		}
}

// return 0 is bad sequence, otherwise returns sequence value
const int WILD = 1;
int Match2TestWnd::validateSequence(std::vector<int>* tiles)
{
	if (tiles->size() < 2) return 0;									//need at least 2 to make a valid sequence

	int base = 0;

	if ((*tiles)[0] != WILD) base = (*tiles)[0];
	else base = (*tiles)[1];                                    		// first card is wild

	if (base == WILD)                                           		// at least first two are wild
	{
		int w;
		for (w = 2; base == WILD && w < tiles->size(); w++) 			//traverse through sequence and find first non-wild
		{
			base = (*tiles)[w];
		}

		if (base == WILD) return (int)pow(2, tiles->size());			//entire sequence is wild //(good sequence)

		base >>= (w - 2);
		if (base < 2) return 0;                                 		// sequence must start at at least a value of 2
	}

	//validate sequence and return value
	if (((*tiles)[0] == base || (*tiles)[0] == WILD) && ((*tiles)[1] == base || (*tiles)[1] == WILD))
	{
		for (int i = 2; i < tiles->size(); i++)
		{
			base <<= 1;
			if ((*tiles)[i] != base && (*tiles)[i] != WILD) return 0;
		}
		return base << 1;
	}

	return 0;                                                   		//bad sequence
}


bool Match2TestWnd::isValidTile(int i, int j, gameState * state)
{
	state->sequence.push_back(state->grid[i][j]);

	if (state->sequence.size() == 1
		|| (i > 0) && (state->sequence.size() - 2 == state->selectOrdr[i - 1][j] && state->selectOrdr[i - 1][j] != NOT_SELECTED)
		|| (i < 4) && (state->sequence.size() - 2 == state->selectOrdr[i + 1][j] && state->selectOrdr[i + 1][j] != NOT_SELECTED)
		|| (j > 0) && (state->sequence.size() - 2 == state->selectOrdr[i][j - 1] && state->selectOrdr[i][j - 1] != NOT_SELECTED)
		|| (j < 4) && (state->sequence.size() - 2 == state->selectOrdr[i][j + 1] && state->selectOrdr[i][j + 1] != NOT_SELECTED)
		)
	{
		state->sequence.pop_back();
		return true;
	}
	else if ((state->selectOrdr[i][j] != NOT_SELECTED) 
		|| (validateSequence(&state->sequence) == 0)
		)
	{
		state->sequence.pop_back();
		return false;
	}

}


bool Match2TestWnd::enterSequence(gameState * state)
{
	if (state->sequence.size() < 2) return false;

	state->score += validateSequence(&state->sequence);

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			if (state->selectOrdr[i][j] != NOT_SELECTED) state->grid[i][j] = ENTERED;
		}

	for (int i = 0; i < 5; i++)
		for (int j = 4; j >= 0; j--)
		{
			if (state->grid[i][j] == ENTERED)
			{
				for (int k = j; k >= 0; k--)
				{
					if (state->grid[i][k] != ENTERED)
					{
						state->grid[i][j] = state->grid[i][k];
						state->grid[i][k] = ENTERED;
						break;
					}
				}
			}
		}

	for (int i = 0; i < 5; i++)
		for (int j = 4; j >= 0; j--)
		{
			if (state->grid[i][j] == ENTERED) state->grid[i][j] = RNG[state->NextRNGIndex++];

		}

	
	clearSelection(state);

	return true;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool Match2TestWnd::WinsPossible(gameState * state, std::string * moves)
{
	//your code here
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void Match2TestWnd::TestWin()
{
	foundSol = false;
	*SolLbl->getStrPntr() = "";

	std::string moves = "";

	print(WinsPossible(&ms, &moves));
}


int Match2TestWnd::hitKey(int x, int y, unsigned char key)
{
	if (key == '\r')
	{
		enterSequence(&ms);
		updateRNGLbl();

		if (SolLbl->getStrPntr()->length() > 0) TestWin();

	}

	return UserInterfaceWindow::hitKey(x, y, key);
}

int Match2TestWnd::hitWindow(int x, int y)
{
	if (x > this->x + 50 && x < this->x + 300 && y > this->y + 100 && y < this->y + 350)
	{
		int i = (x - (this->x + 50)) / 50;
		int j = (y - (this->y + 100)) / 50;

		if (!isValidTile(i, j, &ms)) clearSelection(&ms);
		else
		{
			ms.sequence.push_back(ms.grid[i][j]);
			ms.selectOrdr[i][j] = ms.sequence.size() - 1;
		}
	}
	else clearSelection(&ms);

	return UserInterfaceWindow::hitWindow(x, y);
}

int Match2TestWnd::passiveHover(int x, int y, ToolTipWindow & tooltipWindow)
{
	mouseX = x;
	mouseY = y;
	
	return UserInterfaceWindow::passiveHover(x, y, tooltipWindow);
}


void Match2TestWnd::reroll()
{
	ms.score = 0;

	clearSelection(&ms);

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			ms.grid[i][j] = pow(2, rand_int(0, 10));
			ms.selectOrdr[i][j] = NOT_SELECTED;
		}
}

void Match2TestWnd::rerollRNG()
{
	RNG.clear();
	for(int i = 0 ; i < 100000 ; i++) RNG.push_back((int)pow(2, rand_int(0, 10)));

	ms.NextRNGIndex = 0;
	updateRNGLbl();
}

void Match2TestWnd::updateRNGLbl()
{
	std::string * pntr = RNGLbl->getStrPntr();
	pntr->assign("");
	for (int i = ms.NextRNGIndex; i < ms.NextRNGIndex + 200 && i < RNG.size(); i++)
	{
		pntr->append((RNG[i] == 1 ? "WILD" : iTOa(RNG[i]))).append(",");
	}
}

void Match2TestWnd::resetlayout()
{
	clearWindowElements();
	UserInterfaceWindow::initWindowlayout();

	for(int i = 0 ; i < 5 ; i++)
	{
		attachBorder(50 + (i*50), 100, 250, 50, 2, "");
		attachBorder(50, 100 + (i * 50), 50, 250, 2, "");
	}

	for (int i = 0; i < 5; i++)
	{
		std::string s = "";
		s += (char)(65 + i);
		attachLabel(70 + (i * 50), 80, s);
	}

	for (int j = 0; j < 5; j++)
	{
		std::string s = "";
		s += (char)(48 + j);
		attachLabel(30, 130 + (j*50), s);
	}

	attachLabel(55,425,"SCORE: %i / %i","ii",&ms.score,&scoreToWin);

	attachButton(55, 370, "ReRoll")->setObjectFunction(&Match2TestWnd::reroll, this);
	attachButton(145, 370, "RollRNG")->setObjectFunction(&Match2TestWnd::rerollRNG, this);
	attachButton(235, 370, "TestWin")->setObjectFunction(&Match2TestWnd::TestWin, this);


	srand(time(NULL));

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			gridLbl[i][j] = attachLabel(60 + i * 50, 130 + j * 50, "%i","i",&ms.grid[i][j]);
			gridLblWild[i][j] = attachLabel(60 + i * 50, 130 + j * 50, "WILD");
			ms.selectOrdr[i][j] = NOT_SELECTED;
		}

	reroll();

	RNGLbl = attachLabel(50, 50, "");
	rerollRNG();

	SolLbl = attachLabel(305, 385, "");

	hideResetWindowButton(true);

	setXY(10, 8);
	setHeight(970);
	setWidth(1120);

}


void Match2TestWnd::tick(float DeltaTime)
{

	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			if (ms.selectOrdr[i][j] != NOT_SELECTED)	{ gridLbl[i][j]->setColor(clrRGBA::BLUE); gridLblWild[i][j]->setColor(clrRGBA::BLUE); }
			else										{ gridLbl[i][j]->setColor(clrRGBA::WHITE); gridLblWild[i][j]->setColor(clrRGBA::WHITE); }
		}

	if (mouseX > this->x + 50 && mouseX < this->x + 300 && mouseY > this->y + 100 && mouseY < this->y + 350)
	{
		int i = (mouseX - (this->x + 50)) / 50;
		int j = (mouseY - (this->y + 100)) / 50;

		if (gridLbl[i][j]->getColor() != clrRGBA::BLUE) { gridLbl[i][j]->setColor(clrRGBA::YELLOW); gridLblWild[i][j]->setColor(clrRGBA::YELLOW); }
	}

	for (int k = 0; k < SolLbl->getStrPntr()->length(); k++)
	{
		char c = (*SolLbl->getStrPntr())[k];

		if (c == ',') continue;
		if (c == '*') break;

		int i = c - 65; k++;
		c = (*SolLbl->getStrPntr())[k];
		int j = c - 48;

		if (gridLbl[i][j]->getColor() != clrRGBA::BLUE) { gridLbl[i][j]->setColor(clrRGBA::ORANGE); gridLblWild[i][j]->setColor(clrRGBA::ORANGE); }
	}

	// to simply the overall logic, let this swap in and out the word "WILD" as needed
	for(int i = 0 ; i < 5 ; i++)
		for(int j = 0 ; j < 5 ; j++)
		{
			gridLbl[i][j]->setHidden(ms.grid[i][j] == 1);
			gridLblWild[i][j]->setHidden(ms.grid[i][j] != 1);
		}

}