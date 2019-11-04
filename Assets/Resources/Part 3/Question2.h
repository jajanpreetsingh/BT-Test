struct gameState
{
	int grid[5][5];
	int selectOrdr[5][5];
	std::vector<int> sequence;
	int NextRNGIndex;
	int score;



	gameState(){ ; }

	gameState(const gameState & other)
	{
		for(int i = 0 ; i < 5 ; i++)
			for(int j = 0 ; j < 5 ; j++)
			{
				grid[i][j] = other.grid[i][j];
				selectOrdr[i][j] = other.selectOrdr[i][j];
			}

		for (int i = 0; i < other.sequence.size(); i++) sequence.push_back(other.sequence[i]);
		NextRNGIndex = other.NextRNGIndex;
		score = other.score;
	}

};


class Match2TestWnd : public UserInterfaceWindow
{

public:

	Match2TestWnd(bool hidden, const std::string & title) : UserInterfaceWindow(hidden, title)
	{
		resetlayout();
	};

	//////////////////important functions///////////////////////
	
	void clearSelection(gameState * state);
	int validateSequence(std::vector<int>* cards);
	bool isValidTile(int i, int j, gameState * state);
	bool enterSequence(gameState * state);

	//////////////////////////////////////////////////////////////
	bool foundSol = false;
	bool WinsPossible(gameState * state, std::string * moves);
	void TestWin();
	////////////////////////////////////////////////////////////

	int hitKey(int x, int y, unsigned char key);
	virtual int hitWindow(int x, int y) override;
	virtual int passiveHover(int x, int y, ToolTipWindow & tooltipWindow) override;
	
	void reroll();
	void rerollRNG();
	void updateRNGLbl();

	virtual void resetlayout();
	virtual void tick(float DeltaTime);

private:

	int mouseX = 0;
	int mouseY = 0; 

	Label * RNGLbl = nullptr;
	Label * SolLbl = nullptr;
	Label * gridLbl[5][5];
	Label * gridLblWild[5][5];

	std::vector<int> RNG;
	int scoreToWin = 5000;

	gameState ms;  //ms: main state

};