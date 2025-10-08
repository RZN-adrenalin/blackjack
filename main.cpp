#include <iostream>
#include <vector>
#include <random>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

class Card {
public:
    string suit;
    string rank;
    int value;

    Card(string s, string r, int v) : suit(s), rank(r), value(v) {}

    string toString() const {
        return rank + suit;
    }

    
    vector<string> getCardArt() const {
        vector<string> cardLines(7);
        
        cardLines[0] = "┌─────────┐";
        cardLines[1] = "│" + rank + string(8 - rank.length(), ' ') + " │";
        cardLines[2] = "│         │";
        cardLines[3] = "│    " + suit + "    │";
        cardLines[4] = "│         │";
        cardLines[5] = "│ " + string(8 - rank.length(), ' ') + rank + "│";
        cardLines[6] = "└─────────┘";
        
        return cardLines;
    }

    
    static vector<string> getHiddenCardArt() {
        vector<string> cardLines(7);
        
        cardLines[0] = "┌─────────┐";
        cardLines[1] = "│░░░░░░░░░│";
        cardLines[2] = "│░░░░░░░░░│";
        cardLines[3] = "│░░░░░░░░░│";
        cardLines[4] = "│░░░░░░░░░│";
        cardLines[5] = "│░░░░░░░░░│";
        cardLines[6] = "└─────────┘";
        
        return cardLines;
    }
};

class Desk {
private:
    vector<Card> cards;
    mt19937 rng;

public:
    Desk() : rng(random_device{}()) {
        vector<string> suits = {"♠", "♥", "♦", "♣"};
        vector<string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
        vector<int> values = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};

        for (const string& suit : suits) {
            for (int i = 0; i < ranks.size(); i++) {
                cards.push_back(Card(suit, ranks[i], values[i]));
            }
        }
        shuffle();
    }

    void shuffle() {
        std::shuffle(cards.begin(), cards.end(), rng);
    }

    Card drawCard() {
        if (cards.empty()) {
            throw runtime_error("The desk is empty!");
        }
        Card card = cards.back();
        cards.pop_back();
        return card;
    }
};

class Hand {
private:
    vector<Card> cards;

public:
    void addCard(const Card& card) {
        cards.push_back(card);
    }

    int getValue() const {
        int total = 0;
        int aces = 0;

        for (const Card& card : cards) {
            total += card.value;
            if (card.rank == "A") {
                aces++;
            }
        }

        while (total > 21 && aces > 0) {
            total -= 10;
            aces--;
        }

        return total;
    }

    void display(bool hideFirst = false) const {
        cout << "\nCards: ";
        for (int i = 0; i < cards.size(); i++) {
            if (i == 0 && hideFirst) {
                cout << "[Hidden] ";
            } else {
                cout << cards[i].toString() << " ";
            }
        }

        if (!hideFirst) {
            cout << "(Sum: " << getValue() << ")";
        }
        cout << endl;
        
        
        displayCardsVisual(hideFirst);
    }

    void displayCardsVisual(bool hideFirst = false) const {
        if (cards.empty()) return;
        
        vector<vector<string>> allCards;
        
        for (int i = 0; i < cards.size(); i++) {
            if (i == 0 && hideFirst) {
                allCards.push_back(Card::getHiddenCardArt());
            } else {
                allCards.push_back(cards[i].getCardArt());
            }
        }
        

        for (int line = 0; line < 7; line++) {
            for (int cardIndex = 0; cardIndex < allCards.size(); cardIndex++) {
                cout << allCards[cardIndex][line];
                if (cardIndex < allCards.size() - 1) {
                    cout << " ";  
                }
            }
            cout << endl;
        }
    }

    void clear() {
        cards.clear();
    }

    size_t size() const {
        return cards.size();
    }
};

class BlackjackGame {
private:
    Desk desk;
    Hand playerHand;
    Hand dealerHand;
    int bank = 1000;
    int bet = 0;
    int gamesPlayed = 0;
    int gamesWon = 0;
    int gamesLost = 0;
    int gamesDraw = 0;

    void inputBet() {
        cout << "\nYour current bank: $" << bank << endl;
        do {
            cout << "Enter your bet: $";
            cin >> bet;
            if (cin.fail() || bet <= 0 || bet > bank) {
                cout << "Invalid bet. Enter a number between 1 and " << bank << "." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
            }
        } while (bet <= 0 || bet > bank);
    }

    void playerTurn() {
        char choice;
        while (playerHand.getValue() < 21) {
            cout << "\nYour turn. Take a card (h) or pass (s)? ";
            cin >> choice;

            if (choice == 'h' || choice == 'H') {
                playerHand.addCard(desk.drawCard());
                cout << "\nYou take card: " << endl;
                playerHand.display();

                if (playerHand.getValue() > 21) {
                    cout << "\nOVERKILL! You lost." << endl;
                    return;
                }
            } else if (choice == 's' || choice == 'S') {
                cout << "\nYou stopped." << endl;
                break;
            } else {
                cout << "Incorrect input. Use 'h' to take card or 's' to stop." << endl;
            }
        }
    }

    void dealerTurn() {
        cout << "\nDealer's move: " << endl;
        dealerHand.display();

        while (dealerHand.getValue() < 17) {
            cout << "\nDealer takes card..." << endl;
            dealerHand.addCard(desk.drawCard());
            dealerHand.display();
        }

        if (dealerHand.getValue() > 21) {
            cout << "\nDealer overkill!" << endl;
        } else {
            cout << "\nDealer stopped." << endl;
        }
    }

    void showResults() {

cout << "      ██████╗ ███████╗███████╗██╗   ██╗██╗  ████████╗     \n";
cout << "      ██╔══██╗██╔════╝██╔════╝██║   ██║██║  ╚══██╔══╝     \n";
cout << "      ██████╔╝█████╗  ███████╗██║   ██║██║     ██║        \n";
cout << "      ██╔══██╗██╔══╝  ╚════██║██║   ██║██║     ██║        \n";
cout << "      ██║  ██║███████╗███████║╚██████╔╝███████╗██║        \n";
cout << "      ╚═╝  ╚═╝╚══════╝╚══════╝ ╚═════╝ ╚══════╝╚═╝        \n";

        cout << "Your cards: ";
        playerHand.display();
        cout << "\nDealer's cards: ";
        dealerHand.display();

        int playerValue = playerHand.getValue();
        int dealerValue = dealerHand.getValue();

        gamesPlayed++;

        if (playerValue > 21) {
            cout << "\nYou lost (overkill)!" << endl;
            bank -= bet;
            gamesLost++;
        } else if (dealerValue > 21) {
            cout << "You win (dealer overkill)!" << endl;
            bank += bet;
            gamesWon++;
        } else if (playerValue == 21) {
            cout << "\nBLACKJACK! You win 2x your bet!" << endl;
            bank += static_cast<int>(2 * bet);
            gamesWon++;
        } else if (playerValue > dealerValue) {
            cout << "\nYou win!" << endl;
            bank += bet;
            gamesWon++;
        } else if (playerValue < dealerValue) {
            cout << "\nYou lost!" << endl;
            bank -= bet;
            gamesLost++;
        } else {
            cout << "\nDraw! Bet returned." << endl;
            gamesDraw++;
        }

        cout << "Current bank: $" << bank << endl;
        cout << "Statistics: Wins: " << gamesWon << " | Losses: " << gamesLost << " | Draws: " << gamesDraw << endl;
    }

public:
    void playRound() {

        cout << " ██████╗  ██╗      █████╗  ██████╗██╗  ██╗      ██╗ █████╗  ██████ ██╗  ██╗ \n";
        cout << " ██╔═══██╗██║     ██╔══██╗██╔════╝██║ ██╔╝      ██║██╔══██╗██╔════╝██║ ██╔╝ \n";
        cout << " ██║ ██║  ██║     ███████║██║     █████╔╝       ██║███████║██║     █████╔╝  \n";
        cout << " ██║   ██║██║     ██╔══██║██║     ██╔═██╗  ██   ██║██╔══██║██║     ██╔═██╗  \n";
        cout << " ╚██████╔╝███████╗██║  ██║╚██████╗██║  ██╗ ╚█████╔╝██║  ██║╚██████╗██║  ██╗ \n";
        cout << "  ╚═════╝ ╚══════╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝  ╚════╝ ╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝ \n";

        if (bank <= 0) {
            cout << "You are bankrupt! Cannot continue.\n";
            return;
        }

        inputBet();

        playerHand.clear();
        dealerHand.clear();

        playerHand.addCard(desk.drawCard());
        dealerHand.addCard(desk.drawCard());
        playerHand.addCard(desk.drawCard());
        dealerHand.addCard(desk.drawCard());

        cout << "\nYour cards: " << endl;
        playerHand.display();

        cout << "\nDealer cards: " << endl;
        dealerHand.display(true);

        if (playerHand.getValue() == 21 ) {
            cout << "\nBLACKJACK! CONGRATULATIONS!" << endl;
            showResults();
            return;
        }

        playerTurn();
        if (playerHand.getValue() <= 21) {
            dealerTurn();
        }

        showResults();
    }

    void startGame() {
        cout << "Welcome to BLACKJACK!" << endl;
        cout << "Goal: get the sum of the cards as close to 21 as possible, but not more." << endl;
        cout << "Ace = 11 or 1, Face cards = 10, others = face value." << endl;
        cout << "Starting bank: $" << bank << endl;

        char playAgain;

        do {
            if (bank <= 0) {
                cout << "\n╔════════════════════════════════════════╗" << endl;
                cout << "║     YOU ARE BANKRUPT! GAME OVER!      ║" << endl;
                cout << "╚════════════════════════════════════════╝" << endl;
                cout << "\nWould you like to restart with a new bank? (Y/N): ";
                cin >> playAgain;
                
                if (playAgain == 'Y' || playAgain == 'y') {
                    bank = 1000;
                    gamesPlayed = 0;
                    gamesWon = 0;
                    gamesLost = 0;
                    gamesDraw = 0;
                    cout << "\nNew game started! Bank reset to $1000" << endl;
                    continue;
                } else {
                    break;
                }
            }

            playRound();

            if (bank > 0) {
                cout << "\nWant to play again? (Y/N): ";
                cin >> playAgain;
                
                if (playAgain != 'Y' && playAgain != 'y') {
                    break;
                }
            }

        } while (true);

cout << "████████ ██   ██  █████  ███    ██ ██   ██ ███████          ███████  ██████  ██████         ████████ ██   ██ ███████            ██████    █████  ███    ███ ███████  ██\n";
cout << "   ██    ██   ██ ██   ██ ████   ██ ██  ██  ██               ██      ██    ██ ██   ██           ██    ██   ██ ██                 ██       ██   ██ ████  ████ ██       ██\n";
cout << "   ██    ███████ ███████ ██ ██  ██ █████   ███████          █████   ██    ██ ██████            ██    ███████ █████              ██   ███ ███████ ██ ████ ██ █████    ██\n";
cout << "   ██    ██   ██ ██   ██ ██  ██ ██ ██  ██       ██          ██      ██    ██ ██   ██           ██    ██   ██ ██                 ██    ██ ██   ██ ██  ██  ██ ██         \n";
cout << "   ██    ██   ██ ██   ██ ██   ████ ██   ██ ███████          ██       ██████  ██   ██           ██    ██   ██ ███████             ██████  ██   ██ ██      ██ ███████  ██\n";

        cout << "\n╔════════════════════════════════════════╗" << endl;
        cout << "║          FINAL STATISTICS             ║" << endl;
        cout << "╠════════════════════════════════════════╣" << endl;
        cout << "║ Final bank: $" << setw(24) << bank << " ║" << endl;
        cout << "║ Games played: " << setw(23) << gamesPlayed << " ║" << endl;
        cout << "║ Games won: " << setw(26) << gamesWon << " ║" << endl;
        cout << "║ Games lost: " << setw(25) << gamesLost << " ║" << endl;
        cout << "║ Draws: " << setw(30) << gamesDraw << " ║" << endl;
        
        if (gamesPlayed > 0) {
            double winRate = (double)gamesWon / gamesPlayed * 100;
            cout << "║ Win rate: " << setw(23) << fixed << setprecision(1) << winRate << "% ║" << endl;
        }
        
        cout << "╚════════════════════════════════════════╝" << endl;
    }
};

int main() {
    BlackjackGame game;
    game.startGame();
    return 0;
}