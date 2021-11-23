#include <iostream>
#include <cstdlib>
#include <random>

using namespace std;

string decode(const char &i)
{

    switch (i)
    {
    case 'r':
        return "rock";

    case 'p':
        return "paper";

    case 's':
        return "scissors";
    default:
       throw invalid_argument( "received invalid argument value" );
    }

    return "";
}

int main()
{
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<int> dis(1, 3);

    char computer = 0;
    char user = 0;

    switch (dis(gen))
    {
    case 1:
        computer = 'r';
        break;
    case 2:
        computer = 'p';
        break;
    case 3:
        computer = 's';
        break;
    default:
        throw invalid_argument( "random_device broken" );
    }

    cout << "Please choose: rock (r) - paper (p) - scissors (s)" << endl;
    cout << "Choose: ";
    cin >> user;
    cout << "You choose " << decode(user) << ", I choose " << decode(computer) << endl;

    if (user == computer)
    {
        cout << "Draw";
    }
    else if (user == 'r' && computer == 's')
    {
        cout << "You Win: " << decode(user) << " beats " << decode(computer);
    }
    else if (user == 's' && computer == 'p')
    {
        cout << "You Win: " << decode(user) << " beats " << decode(computer);
    }
    else if (user == 'p' && computer == 'r')
    {
        cout << "You Win: " << decode(user) << " beats " << decode(computer);
    }
    else
    {
        cout << "I Win: " << decode(computer) << " beats " << decode(user);
    }
    cout << endl;
}
