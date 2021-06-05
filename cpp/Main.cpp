#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <iostream>
using namespace std;
//--------------------------------------//
void ItoA(int x, char* y)
{
	char a[10];
	int b;
	int i = 0;
	int j = 0;

	while (x > 0)
	{
		b = x % 10;
		x = x / 10;
		a[i] = b + 48;
		i++;
	}
	i--;
	for (; i >= 0; j++, i--)
		y[j] = a[i];
	y[j] = 0;
}
//--------------------------------------//
struct UserInputs {
	int row;
	int col;
	int max_row;
	int max_col;
};
struct NodeValues{
	sf::RectangleShape rec;
	sf::Color color;
	sf::Vector2f size;
	int row;
	int col;
	int x;
	int y;
};
struct NodePositionValues {
	sf::Vector2f startPos;
	sf::Vector2f gap;
};
struct TextValues{
	sf::Text text;
	sf::Font font;
	sf::Text::Style style;
	sf::Vector2f textPos;
	sf::Vector2f gap;
	sf::Color color;
	int size;
};
struct DefaultValues {
	sf::Sprite background;
	sf::Texture texture;
	sf::Color nodeColor;
	sf::Color startColor;
	sf::Color endColor;
	sf::Color obstacleColor;
	sf::Color backgroundColor;
	sf::Color pathColor;
	sf::Vector2u windowSize;
	sf::Uint32 windowStyle;
	string windowText;
};
struct SearchValues {
	int pathArrayIterator;
	int endNode;
	int pathArraySize;
	int searchArraySize;
	int* pathArray = NULL;
	int* searchArray = NULL;
};
struct ExtraValues {
	bool display;
	int mouseClicks;
};
UserInputs UI1;
NodeValues* NV1 = NULL;
NodePositionValues NPV1;
TextValues TV1;
DefaultValues DF1;
SearchValues SV1;
ExtraValues EV1;
//--------------------------------------//
void DeallocateMemory()
{
	if (NV1 != NULL)
	{
		delete[] NV1;
		NV1 = NULL;
	}
	if (SV1.pathArray != NULL)
	{
		delete[] SV1.pathArray;
		SV1.pathArray = NULL;
	}
	if (SV1.searchArray != NULL)
	{
		delete[] SV1.searchArray;
		SV1.searchArray = NULL;
	}
}
//--------------------------------------//
void LoadFromMemory()
{
	if (!TV1.font.loadFromFile("Fonts/Normal.otf"))
		cout << "Error Loading Font\n";
	if (!DF1.texture.loadFromFile("Images/Background.png"))
		cout << "Error Loading Texture\n";
}
void PreInitialize()
{
	UI1.max_row = 15;
	UI1.max_col = 15;
	UI1.row = UI1.max_row + 1;
	UI1.col = UI1.max_col + 1;

	NPV1.startPos.x = 20;
	NPV1.startPos.y = 20;
	NPV1.gap.x = 10;
	NPV1.gap.y = 10;

	DF1.background.setTexture(DF1.texture);
	DF1.nodeColor = sf::Color::Blue;
	DF1.startColor = sf::Color(70, 140, 0);
	DF1.endColor = sf::Color::Magenta;
	DF1.obstacleColor = sf::Color::Black;
	DF1.backgroundColor = sf::Color::Black;
	DF1.pathColor = sf::Color(247, 0, 2);
	DF1.windowSize.x = 750;
	DF1.windowSize.y = 550;
	DF1.windowStyle = sf::Style::Default;
	DF1.windowText = "A* Search Algorithm";

	EV1.display = true;
	EV1.mouseClicks = 0;

	TV1.text.setString("");
	TV1.style = sf::Text::Style::Bold;
	TV1.textPos.x = 500.0f;
	TV1.textPos.y = 10.0f;
	TV1.gap.x = 0;
	TV1.gap.y = 30;
	TV1.color = sf::Color::Black;
	TV1.size = 18;

	TV1.text.setFont(TV1.font);
	TV1.text.setFillColor(TV1.color);
	TV1.text.setCharacterSize(TV1.size);
	TV1.text.setStyle(TV1.style);
}
void PostInitialize()
{
	if (NV1 != NULL)
		cout << "Error! Node Values Array not NULL\n";
	if (SV1.pathArray != NULL)
		cout << "Error! Path Array not NULL\n";
	if (SV1.searchArray != NULL)
		cout << "Error! Search Array not NULL\n";

	int sizetemp = UI1.row * UI1.col;
	NV1 = new NodeValues[sizetemp];
	for (int i = 0; i < sizetemp; i++)
	{
		NV1[i].color = sf::Color::Blue;
		NV1[i].size.x = 18;
		NV1[i].size.y = 18;
		NV1[i].row = int(i / UI1.col);
		NV1[i].col = int(i % UI1.col);

		NV1[i].x = int(NPV1.startPos.x + NV1[i].col * (NV1[i].size.x + NPV1.gap.x));
		NV1[i].y = int(NPV1.startPos.y + NV1[i].row * (NV1[i].size.y + NPV1.gap.y));
		NV1[i].rec.setSize(NV1[i].size);
		NV1[i].rec.setPosition((float)NV1[i].x, (float)NV1[i].y);
		NV1[i].rec.setOrigin((float)NV1[i].size.x / 2, (float)NV1[i].size.y / 2);
	}

	SV1.endNode = -1;
	SV1.pathArraySize = 2 * UI1.row * UI1.col;
	SV1.searchArraySize = 2 * UI1.row * UI1.col;
	SV1.pathArrayIterator = 0;
	SV1.pathArray = new int[SV1.pathArraySize];
	SV1.searchArray = new int[SV1.searchArraySize];

	for (int i = 0; i < SV1.pathArraySize; i++)
		SV1.pathArray[i] = -1;
	for (int i = 0; i < SV1.searchArraySize; i++)
		SV1.searchArray[i] = -1;
}
//--------------------------------------//
void TakeInputsFromUser()
{
	while (UI1.row > UI1.max_row || UI1.row <= 0)
	{
		cout << "Enter Rows\n";
		cin >> UI1.row;
		if (UI1.row > UI1.max_row)
			cout << UI1.max_row << " is maximum\n";
		else if (UI1.row <= 0)
			cout << "Enter positive non Zero value\n";
	}
	while (UI1.col > UI1.max_col || UI1.col <= 0)
	{
		cout << "Enter Colums\n";
		cin >> UI1.col;
		if (UI1.col > UI1.max_col)
			cout << UI1.max_col << " is maximum\n";
		else if (UI1.col <= 0)
			cout << "Enter positive non Zero value\n";
	}
}
//--------------------------------------//
void DrawNodes(sf::RenderWindow& window)
{
	for (int i = 0; i < UI1.row * UI1.col; i++)
	{
		NV1[i].rec.setFillColor(NV1[i].color);
		window.draw(NV1[i].rec);
	}
}
void DrawText(sf::RenderWindow& window)
{
	char a[20];
	char b[10];
	TV1.text.setPosition(TV1.textPos);
	TV1.text.setString("1) Esc to go back");
	window.draw(TV1.text);

	TV1.text.move(TV1.gap.x, TV1.gap.y);
	TV1.text.setString("2) R to Restart");
	window.draw(TV1.text);

	strcpy_s(a, "Rows = ");
	ItoA(UI1.row, b);
	strcat_s(a, b);
	TV1.text.move(TV1.gap.x, 2*TV1.gap.y);
	TV1.text.setString(a);
	window.draw(TV1.text);

	strcpy_s(a, "Cols = ");
	ItoA(UI1.col, b);
	strcat_s(a, b);
	TV1.text.move(TV1.gap.x, TV1.gap.y);
	TV1.text.setString(a);
	window.draw(TV1.text);

	TV1.text.move(TV1.gap.x, 2*TV1.gap.y);
	if (EV1.mouseClicks == 0)
		TV1.text.setString("5) Choose Start...");
	else if (EV1.mouseClicks == 1)
		TV1.text.setString("5) Choose End...");
	else if (EV1.mouseClicks == 2)
		TV1.text.setString("5) Choose Obstacles...");
	else if (EV1.mouseClicks > 2)
		TV1.text.setString("5) Press Space...");
	window.draw(TV1.text);
}
void Display(sf::RenderWindow& window)
{
	window.clear(DF1.backgroundColor);
	window.draw(DF1.background);
	DrawNodes(window);
	DrawText(window);
	window.display();
}
//--------------------------------------//
void ColorPathNodes()
{
	for (int i = 0; i < SV1.pathArraySize && SV1.pathArray[i] != -1; i++)
	{
		if (NV1[SV1.pathArray[i]].color == DF1.nodeColor)
			NV1[SV1.pathArray[i]].color = DF1.pathColor;
	}
	EV1.display = true;
}
int CalculateDistance(const int row, const int col, const int row1, const int col1)
{
	int x = row1 - row;
	int y = col1 - col;
	if (x < 0)
		x = -x;
	if (y < 0)
		y = -y;
	return (x + y);
}
void InsertInSearchArray(const int row, const int col, const int dist)
{
	int nodetemp = UI1.col * row + col;
	for (int j = 0; j < SV1.searchArraySize; j = j + 2)
		if (nodetemp == SV1.searchArray[j])
			return;

	for (int j = 0; j < SV1.pathArraySize && SV1.pathArray[j] != -1; j++)
		if (nodetemp == SV1.pathArray[j])
			return;

	if (NV1[nodetemp].color == DF1.obstacleColor)
		return;

	int i = 0;
	for (i; i < SV1.searchArraySize; i = i + 2)
	{
		if (SV1.searchArray[i] == -1)
		{
			SV1.searchArray[i] = nodetemp;
			SV1.searchArray[i + 1] = dist;
			break;
		}
	}
	if (i == SV1.searchArraySize)
		cout << "Error! Search Array Full\n";
}
void AddToSearchArray(const int row, const int col, const int row1, const int col1)
{
	int rowtemp = -1;
	int coltemp = -1;
	int disttemp = -1;

	rowtemp = row + 1;
	coltemp = col;
	if (rowtemp < UI1.row)
	{
		disttemp = CalculateDistance(rowtemp, coltemp, row1, col1);
		InsertInSearchArray(rowtemp, coltemp, disttemp);
	}

	rowtemp = row - 1;
	coltemp = col;
	if (rowtemp >= 0)
	{
		disttemp = CalculateDistance(rowtemp, coltemp, row1, col1);
		InsertInSearchArray(rowtemp, coltemp, disttemp);
	}

	rowtemp = row;
	coltemp = col + 1;
	if (coltemp < UI1.col)
	{
		disttemp = CalculateDistance(rowtemp, coltemp, row1, col1);
		InsertInSearchArray(rowtemp, coltemp, disttemp);
	}

	rowtemp = row;
	coltemp = col - 1;
	if (coltemp >= 0)
	{
		disttemp = CalculateDistance(rowtemp, coltemp, row1, col1);
		InsertInSearchArray(rowtemp, coltemp, disttemp);
	}

	rowtemp = row + 1;
	coltemp = col + 1;
	if (rowtemp < UI1.row && coltemp < UI1.col)
	{
		disttemp = CalculateDistance(rowtemp, coltemp, row1, col1);
		InsertInSearchArray(rowtemp, coltemp, disttemp);
	}
	rowtemp = row - 1;
	coltemp = col - 1;
	if (rowtemp >= 0 && coltemp >= 0)
	{
		disttemp = CalculateDistance(rowtemp, coltemp, row1, col1);
		InsertInSearchArray(rowtemp, coltemp, disttemp);
	}
	rowtemp = row + 1;
	coltemp = col - 1;
	if (rowtemp < UI1.row && coltemp >= 0)
	{
		disttemp = CalculateDistance(rowtemp, coltemp, row1, col1);
		InsertInSearchArray(rowtemp, coltemp, disttemp);
	}
	rowtemp = row - 1;
	coltemp = col + 1;
	if (rowtemp >= 0 && coltemp < UI1.col)
	{
		disttemp = CalculateDistance(rowtemp, coltemp, row1, col1);
		InsertInSearchArray(rowtemp, coltemp, disttemp);
	}
}
int CalculateMinimun()
{
	int n = -1, x = -1, y = -1;
	if (SV1.searchArray == NULL)
		cout << "Error! Search Array is Not Initialized in Calculate Minimum Function\n";

	for (int i = 1; i < SV1.searchArraySize; i = i + 2)
	{
		if (SV1.searchArray[i] != -1)
		{
			n = SV1.searchArray[i - 1];
			x = SV1.searchArray[i];
			y = i;
			break;
		}
	}
	if (y == -1)
		return -1;

	for (int i = y + 2; i < SV1.searchArraySize; i = i + 2)
	{
		if (SV1.searchArray[i] < x && SV1.searchArray[i] != -1)
		{
			n = SV1.searchArray[i - 1];
			x = SV1.searchArray[i];
			y = i;
		}
	}
	SV1.searchArray[y] = -1;
	SV1.searchArray[y - 1] = -1;

	return n;
}
void InsertInPathArray(int x)
{
	SV1.pathArrayIterator++;
	if (SV1.pathArrayIterator == SV1.pathArraySize)
		cout << "Error! Path Array Full\n";
	SV1.pathArray[SV1.pathArrayIterator] = x;
}
void SearchPath()
{
	int arraySize = UI1.row * UI1.col;
	int rowtemp = -1;
	int coltemp = -1;
	int rowtemp1 = -1;
	int coltemp1 = -1;
	int xtemp = -2;

	for (int i = 0; i < arraySize; i++)
	{
		if (NV1[i].color == DF1.startColor)
			SV1.pathArray[SV1.pathArrayIterator] = i;
		if (NV1[i].color == DF1.endColor)
			SV1.endNode = i;
	}
	rowtemp1 = NV1[SV1.endNode].row;
	coltemp1 = NV1[SV1.endNode].col;

	while (SV1.pathArray[SV1.pathArrayIterator] != SV1.endNode && xtemp != -1)
	{
		rowtemp = NV1[SV1.pathArray[SV1.pathArrayIterator]].row;
		coltemp = NV1[SV1.pathArray[SV1.pathArrayIterator]].col;

		AddToSearchArray(rowtemp, coltemp, rowtemp1, coltemp1);
		xtemp = CalculateMinimun();
		if (xtemp != -1)
			InsertInPathArray(xtemp);
	}
}
//--------------------------------------//
int GetClickedNode(sf::RenderWindow& window)
{
	sf::Vector2i mousePosTemp = sf::Mouse::getPosition(window);
	sf::Vector2f translatedPosTemp = window.mapPixelToCoords(mousePosTemp);

	int i = 0;
	for (i; i < UI1.row * UI1.col; i++)
		if (NV1[i].rec.getGlobalBounds().contains(translatedPosTemp))
			break;
	if (i == UI1.row * UI1.col)
		return -1;
	return i;
}
void HandleMouseEvents(sf::RenderWindow& window, sf::Event e)
{
	int x;
	if (e.key.code == sf::Mouse::Left)
	{
		x = GetClickedNode(window);
		if (x == -1)
			return;
		if (EV1.mouseClicks == 0)	//start state
		{
			NV1[x].color = DF1.startColor;
			EV1.mouseClicks++;
			EV1.display = true;
			return;
		}
		else if (EV1.mouseClicks == 1 && NV1[x].color == DF1.nodeColor) //end state
		{
			NV1[x].color = DF1.endColor;
			EV1.mouseClicks++;
			EV1.display = true;
			return;
		}
		else if (EV1.mouseClicks >= 2 && NV1[x].color == DF1.nodeColor && SV1.pathArray[0] == -1) //obstacle
		{
			NV1[x].color = DF1.obstacleColor;
			EV1.mouseClicks++;
			EV1.display = true;
			return;
		}
	}
}
void HandleKeyboardEvents(sf::RenderWindow& window, sf::Event e)
{
	if (e.key.code == sf::Keyboard::Escape)
	{
		window.close();
		return;
	}
	if (e.key.code == sf::Keyboard::R)
	{
		return;
	}
	if (e.key.code == sf::Keyboard::Space && EV1.mouseClicks >= 2)
	{
		if (SV1.pathArray[0] == -1)
		{
			SearchPath();
			ColorPathNodes();
		}
		return;
	}
}
//--------------------------------------//
void RunWindow()
{
	sf::RenderWindow window(sf::VideoMode(DF1.windowSize.x, DF1.windowSize.y), DF1.windowText, DF1.windowStyle);
	while (window.isOpen())
	{
		sf::Event e;
		while (window.pollEvent(e))
		{
			if (e.type == sf::Event::Closed)
			{
				window.close();
				return;
			}
			if (e.type == sf::Event::MouseButtonPressed)
			{
				HandleMouseEvents(window, e);
			}
			if (e.type == sf::Event::KeyPressed)
			{
				HandleKeyboardEvents(window, e);
			}
		}
		if (EV1.display)
		{
			Display(window);
			EV1.display = false;
		}
	}
}
//--------------------------------------//
int main()
{
	LoadFromMemory();
	while (true)
	{
		DeallocateMemory();
		PreInitialize();
		TakeInputsFromUser();
		PostInitialize();
		RunWindow();
	}
}
//--------------------------------------//