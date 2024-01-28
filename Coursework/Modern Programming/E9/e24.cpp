// gambling
// author:  Holger Arndt
// version: 07.12.2021
// framework for sheet 9, exercise 24
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <random>
#include <string>
using namespace std;

class Bet
{
	private:
		unsigned int money;
	    char result;	// one of o, e, 1, 2, 3, 4, 5, 6
	public:
		Bet(unsigned int m = 0, char r = '0'): money
		{
			m
		}, result
		{
			r
		} {}

	unsigned int getMoney() const
	{
		return money;
	}

	char getResult() const
	{
		return result;
	}
};

//////////////////////////////////////////////////////////////////
// Add the classes Player, HumanPlayer, and ComputerPlayer here	//
//////////////////////////////////////////////////////////////////

class Player{
	public:

		Player(string name){
			money = 1000;
			name = name;
		}

		int money;
        string name;

		void setMoney(int m);
	    int getMoney();
	    string getName();
		// pure virtual method
        virtual Bet bet() = 0;

		

        void win(int m);  

	// private:
		
};



void Player::setMoney(int m){
	money = m;
}

int Player::getMoney(){
	return money;
}

string Player::getName(){
	return name;
}

void Player::win(int m){
	money += m;
}

class HumanPlayer: public Player{
    public:

		HumanPlayer(string name) : Player(name) {
			(*this).name = name;
		}

        Bet bet();
};

Bet HumanPlayer::bet(){
    int m;
    char r;

	while (1){
		cout << "You have: " << money << endl;

		cout << "How much do you want to bet: ";
		cin >> m; 

		cout << "Which number you are betting on: ";
		cin >> r;	

		if (m > money){
			cout << "You do not have that money. Please bet low!" << endl;
		}else if (m <= money){
			break;
		}

	}   

	money -= m; 

    return Bet(m, r);
}

class ComputerPlayer: public Player{
    public:

		ComputerPlayer(string name) : Player(name) {
			(*this).name = name;
		}

        Bet bet();

};

Bet ComputerPlayer::bet(){
	cout << name << " has: " << money << endl;
	money /= 2; 
	cout << name << "  bet: " << money << " on odd numbers." << endl;
    return Bet(money, 'o');
}

int main()
{
	list<Player*> players;
	unsigned int n;
	string name;
	string dummy;
	char type;
	// setup players
	cout << "number of players: ";
	cin >> n;
	for (unsigned int i
		{ 1 }; i <= n; ++i)
	{
		cout << "Player " << i << ": human or computer (enter h/c): ";
		cin >> type;
		getline(cin, dummy);	// read rest of line after 'h' or 'c', at least '\n'
		cout << "player's name: ";
		getline(cin, name);
		if (type == 'h')
			players.push_back(new HumanPlayer
			{
				name });
		else
			players.push_back(new ComputerPlayer
			{
				name });
	}

	// start game
	auto plit
	{
		players.begin()
	};
	// intialize random number generator
	// mt19937_64 rng
	// {
	// 	random_device {}()
	// };
	// uniform_int_distribution dice
	// { 1, 6 };
	while (players.size() > 1)
	{
		Player *p
		{*plit
		};	// p is a pointer to the current player
		// Bet b
		// {
		// 	p->bet()
		// };	// ask player for the bet
		Bet b;
		b = p->bet();

		int r
		{
			// dice(rng)
			rand()%6+1
		};	//roll dice
		cout << "Rolled: " << r << endl;
		if ((b.getResult() == 'o' && (r == 1 || r == 3 || r == 5)) ||
			(b.getResult() == 'e' && (r == 2 || r == 4 || r == 6)))
		{
			p->win(2 *b.getMoney());	// add money
			cout << p->getName() << " wins " << 2 *b.getMoney() <<
				", now has " << p->getMoney() << endl;
		}
		else if (b.getResult() - '0' == r)
		{
			p->win(6 *b.getMoney());
			cout << p->getName() << " wins " << 6 *b.getMoney() <<
				", now has " << p->getMoney() << endl;
		}
		else
			cout << p->getName() << " looses, now has " <<
			p->getMoney() << endl;
		if (p->getMoney() == 0)
		{
			cout << p->getName() << " leaves the game" << endl;
			auto tmpit
			{
				plit
			};
			--plit;
			players.erase(tmpit);
		}

		++plit;
		if (plit == players.end())
			plit = players.begin();
	}

	// just one player left, announce the winner
	cout << "Winner: " << (*players.begin())->getName() << ", money: " <<
		(*players.begin())->getMoney() << endl;
}