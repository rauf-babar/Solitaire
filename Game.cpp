// Project Properties -> C/C++ -> Command Line -> /utf-8                    (for making printing of unicode possible)

// Include necessary libraries
#include <iostream>
#include <string>
#include <windows.h>
using namespace std;

void setColor(int Color_No)   //function to set the console text color
{
    HANDLE myConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(myConsole, Color_No);
}

// ---------- CLASS LINKED LIST ---------------
template <class T>
class DoublyLinkedList {
private:
    // Nested class to represent a node in the list
    class Node {
    public:
        T data;
        Node* next;
        Node* prev;

        // Default constructor
        Node() : next(nullptr), prev(nullptr) {}

        // Overloaded constructor
        Node(T& val, Node* n = nullptr, Node* p = nullptr) : data(val), next(n), prev(p) {}
    };

    // Nested class to represent an iterator for the list
    class ListIterator {
    public:
        Node* iptr;

        // Constructor with a pointer to a node
        ListIterator(Node* ptr = nullptr) : iptr(ptr) {}

        // Pre-increment operator
        ListIterator& operator++() {
            if (iptr) iptr = iptr->next;
            return *this;
        }

        // Pre-decrement operator
        ListIterator& operator--() {
            if (iptr) iptr = iptr->prev;
            return *this;
        }

        // Dereference operator
        T& operator*() {
            return iptr->data;
        }

        // Equality operator
        bool operator==(const ListIterator& rhs) const {
            return iptr == rhs.iptr;
        }

        // Inequality operator
        bool operator!=(const ListIterator& rhs) const {
            return !(*this == rhs);
        }
    };

    // Private member variables
    int size;
    Node* head;
    Node* tail;

    // Function to get a pointer to a node at a given index
    Node* GetPointer(int index) {
        if (index >= 0 && index < size) { // bounds checking
            Node* temp = head->next;
            Iterator ite = begin();
            ite++;
            for (int i = 0; i < index; i++) {
                ite++;
            }
            return ite.iptr;;
        }
        return nullptr; // Return nullptr if index is out of bounds
    }

public:

    typedef ListIterator Iterator;       // Typedef to use publically

    // Constructor
    DoublyLinkedList() {
        head = new Node(); // Dummy head node
        tail = new Node(); // Dummy tail node
        head->next = tail;
        tail->prev = head;
        size = 0;
    }

    // Function to insert a new node at the end of the list
    void insertAtEnd(T& val) {
        Node* newNode = new Node(val, tail, tail->prev);
        tail->prev->next = newNode;
        tail->prev = newNode;
        size++;
    }

    // Function to attach Node at the end of List
    void insertNode(Iterator& ite) {
        ite.iptr->next = tail;
        ite.iptr->prev = tail->prev;
        tail->prev->next = ite.iptr;
        tail->prev = ite.iptr;
        size++;
    }

    // Function to delete a node at the tail of the list
    void deleteAtTail() {
        if (head->next != tail) {
            Node* temp = tail->prev;
            tail->prev = temp->prev;
            temp->prev->next = tail;
            delete temp;
            size--;
        }
    }

    // Function to Detach Node at the end of List
    void deleteTailNode() {
        if (head->next != tail) {
            Node* temp = tail->prev;
            tail->prev = temp->prev;
            temp->prev->next = tail;
            size--;
        }
    }

    // Function to get the size of the list
    int& GetSize() {
        return size;
    }

    // Function to get an iterator to the beginning of the list
    Iterator begin() const {
        return Iterator(head);
    }

    // Function to get an iterator to the end of the list
    Iterator end() const {
        return Iterator(tail);
    }

    // Function to return a Node using Iterator 
    Iterator returnFromIndex(int index)
    {
        Iterator ite;
        ite.iptr = head->next;
        for (int i = 0; i < index; i++)        // Iterating till index
            ite.iptr = ite.iptr->next;

        ite.iptr->prev->next = ite.iptr->next;      // Connecting Links
        ite.iptr->next->prev = ite.iptr->prev;

        size--;
        return ite;
    }

    // Function to Get Value at any Index By reference
    T& atIndex(int index)
    {
        Iterator ite = begin();
        ++ite;
        for (int i = 0; i < index; i++)
            ++ite;
        return *ite;
    }

    // Function to move nodes from one Linked List to another
    bool shiftNodes(DoublyLinkedList<T>& dest, int k, bool& cmd_undo, bool undo = false)
    {
        if (k > size)                       // if k is greater than size return false
            return false;
        Node* source_ite = tail;
        bool free = false;
        for (int i = 0; i < k; i++)
        {
            source_ite = source_ite->prev;
            if (!(source_ite->data).visibility())            // if card is unflipped return false
                return false;
        }
        if (!undo && dest.GetSize() == 0)               // if linked list is empty
        {
            if (!source_ite->data.isRank(13))           // and card is Not King return false
                return false;
            else free = true;
        }

        Node* dest_ite = dest.tail->prev;
        // Connecting Link between Nodes and checking all conditions
        if (free || undo || ((source_ite->data).rankComparision(dest_ite->data, -1) && (source_ite->data).isOppositeColor(dest_ite->data)))
        {
            if (!undo && !(source_ite->prev->data.visibility()))
            {
                source_ite->prev->data.FlipVisible();       // Flip Card if last Card was Unflipped
                cmd_undo = true;                            // Keeping Last Flip Card Info in Command to Use in Undo
            }
            else if (undo && cmd_undo)
                dest_ite->data.FlipVisible();           // if Last Card was Flipped, flip it

            source_ite->prev->next = tail;                  // Connecting Links between Nodes
            Node* tempTail = tail->prev;
            tail->prev = source_ite->prev;
            dest_ite->next = source_ite;
            source_ite->prev = dest_ite;
            tempTail->next = dest.tail;
            dest.tail->prev = tempTail;
            size -= k;                                      // Subtracting K to keep size updated
            dest.GetSize() += k;                            // Added K to keep size updated
            return true;
        }
        return false;
    }

    ~DoublyLinkedList() {                                   // Destructor
        while (size > 0) {
            deleteAtTail();                                 // Deleting Nodes
        }
        delete head;                                        // Clean up Dummy nodes
        delete tail;
    }
};

// ---------- Class STACK ---------------
template <class T>
class Stack {
    DoublyLinkedList<T> stackList;

    // Friend function to overload the << operator for output
    friend ostream& operator<<(ostream& out, Stack<T>& obj)
    {
        if (obj.isEmpty())          // if stack empty
            cout << "[    ]";
        else
            cout << (*(obj.top()));
        return out;
    }

public:

    // Push By attaching a Node At end of Stack
    void push(typename DoublyLinkedList<T>::Iterator ite) {
        stackList.insertNode(ite); // Change to insertAtEnd for consistency
    }

    // Pop By Detaching a Node At end of Stack
    void pop() {
        stackList.deleteTailNode();
    }

    // Pushing and creating a node at end with value
    void pushInsert(T& val)
    {
        stackList.insertAtEnd(val);
    }

    // Pop last element
    void delete_pop()
    {
        stackList.deleteAtTail();
    }

    // Function to Get Top object of stack
    typename DoublyLinkedList<T>::Iterator top() {
        typename DoublyLinkedList<T>::Iterator ite = stackList.end();
        return (--ite);
    }

    // Function to get Size Of stack
    int getSize()
    {
        return stackList.GetSize();
    }

    // FUnction to Know if Stack is Empty
    bool isEmpty()
    {
        return (stackList.GetSize() == 0);
    }
};

// ---------- CLASS SUIT ---------------
class Suits {
    string name;
    string color;

    // Friend function to overload the << operator for output
    friend ostream& operator<<(ostream& out, Suits& obj) {
        if (obj.name == "spade") {
            out << " ♠";  // spade (♠)
        }
        else if (obj.name == "heart") {
            setColor(4);
            out << " ♥";  // heart (♥)
            setColor(7);
        }
        else if (obj.name == "diamond") {
            setColor(4);
            out << " ♦";  // diamond (♦)
            setColor(7);
        }
        else if (obj.name == "club") {
            out << " ♣";  // club (♣)
        }
        return out;
    }

public:

    Suits() { }   // Default constructor

    Suits(string n) : name(n)   // Overloaded Constructor 
    {
        // Set the color based on the suit name
        if (name == "diamond"|| name == "heart") 
            color = "R";
        else 
            color = "B";
    }

    // Function to check if two suits have the same color
    bool checkColor(Suits& rhs)
    {
        return (color == rhs.color);
    }

    // Overload the == operator to compare two suits
    bool operator==(const Suits& other) const {
        return name == other.name;
    }
};

// ---------- CLASS CARD ---------------
class Card {
    Suits Suit;
    int rank;
    bool visible;

    // Friend function to overload the << operator for output
    friend ostream& operator<<(ostream& out, Card& obj) {
            if (obj.visible) {
                if (obj.rank == 1 || obj.rank >= 11 )
                    setColor(14);
                else
                    setColor(15);
                if (obj.rank == 1)              
                    out << "A";                     // Print Ace
                else if (obj.rank == 11)
                    out << "J";                     // Print Joker
                else if (obj.rank == 12)
                    out << "Q";                     // Print Queen
                else if (obj.rank == 13)
                    out << "K";                     // Print King
                else    
                    out << obj.rank;                // Print Other Ranks
                setColor(15);
                out << obj.Suit;
                setColor(7);
            }
            else {
                setColor(14);
                out << "🃴";                         // Card is hidden
            }
        return out;
    }

public:  
    Card() {}                             // Default Constructor

    // Overloaded Constructor
    Card(Suits s, int r, bool v) : Suit(s), rank(r), visible(v) {}

    bool isOppositeColor(Card& rhs)         // Function to check cards are of opposite color
    {
        return (!Suit.checkColor(rhs.Suit));
    }

    bool rankComparision(Card& rhs, int k)  // Function to check difference of rank in Cards
    {
        return rank == rhs.rank + k;
    }

    bool isRank(int rhs_Rank)            // Function to check Rank
    {
        return (rank == rhs_Rank);
    }

    bool isSameSuit(Card& rhs)           // Function to check cards have same Suit
    {
        return Suit == rhs.Suit;
    }

    bool visibility()                   // Function to check card is visible 
    {
        return visible;
    }

    void FlipVisible()                  // Function to Toggle Visibility of Cards
    {
        visible = !visible;
    }

    ~Card(){
}                           
};

// ---------- CLASS DECK ---------------
template <class T>
class Deck {
    DoublyLinkedList<T> deck;

public:
    
    // Function to Load Deck With 52 Cards
    void LoadDeck() {
        string suit_Names[4] = { "spade", "heart", "club", "diamond" };

        for (int i = 0; i < 4; ++i)                 // 4 suits                          
        {
            for (int j = 0; j < 13; ++j)            // 13 Ranks
            {
                Card *card = new Card(Suits(suit_Names[i]),j+1, false);         
                deck.insertAtEnd(*card);           // Form Card and insert in deck
            }
        }
    }

    // Function to get size
    int getSize()
    {
        return deck.GetSize();
    }

    // Function to return Random Card from Deck for Shuffling
    typename DoublyLinkedList<T>::Iterator ReturnRandomCard()
    {
        int index = rand() % deck.GetSize();
        return deck.returnFromIndex(index);
    }

};

// ---------- CLASS COMMAND ---------------
class Command
{
    string* command;                // Dynamic array of strings
    int tokenCount;                 // To store the number of tokens
    bool flippedLastCard;           // Storing Flip of Card for undo 

    // Friend function to overload the << operator for output
    friend ostream& operator<<(ostream& out, const Command& cmd) {
        if (cmd.tokenCount >= 1) {
            setColor(8); out << "Cmd: ";                  // Print Commnd
            setColor(15); out << cmd.command[0];
        }
        if (cmd.tokenCount >= 2) {
            setColor(8); out << "   Src : ";              // Print Source
            setColor(15); out << cmd.command[1];
        }
        if (cmd.tokenCount >= 3) {
            setColor(8); out << "   Dest : ";             // Print Destination
            setColor(15); out << cmd.command[2];
            setColor(8); out << "   Amount : ";           // Print Amount
            setColor(15);
            if (cmd.tokenCount >= 4)  out << cmd.command[3];
            else out << "1";
        }
        return out;
    }

    // Helper function to convert a string to lowercase
    void toLowerInPlace(string& str) {
        for (int i = 0; i < str.length(); ++i) {
            str[i] = tolower(str[i]);
        }
    }

    // Helper function to check if command is valid
    bool validArgument(const string& arr, bool source = true)
    {
        if (source)          // Checking Source
        {
            if (arr == "w" || arr == "c1" || arr == "c2" || arr == "c3" ||
                arr == "c4" || arr == "c5" || arr == "c6" || arr == "c7")
                return true;
        }
        else                 // Checking Destination
        {
            if (arr == "f1" || arr == "f2" || arr == "f3" || arr == "f4" || arr == "c1" || arr == "c2" ||
                arr == "c3" ||arr == "c4" || arr == "c5" || arr == "c6" || arr == "c7")
                return true;
        }
        return false;
    }

    // Checking Digits
    bool IsDigit(const string& arr) {
        for (int i = 0; i < arr.length(); ++i) {
            if (!isdigit(arr[i])) {
                return false;
            }
        }
        return true;
    }

public:

    // Default Constructor
    Command() : command(nullptr), tokenCount(0), flippedLastCard(false) {}

    // Function to Parse Commands
    void takeInput() {
        char buffer[100];
        setColor(8);
        cout << "\t\t Cmd Src Dest Num\n";
        setColor(15);
        cout << "\tCommand: ";
        cin.getline(buffer, 100);           // Input

        string tokens[5];                   // Temporary array to store tokens
        tokenCount = 0;
        string token = "";

        for (int i = 0; buffer[i] != '\0'; ++i) {
            // If we encounter a space or a comma, we treat it as a delimiter
            if (buffer[i] == ' ' || buffer[i] == ',') {
                if (!token.empty()) {  
                    toLowerInPlace(token);          // Storing Command in Lower Case
                    tokens[tokenCount++] = token;   // Store the token
                    token = "";  
                    if (tokenCount == 4) 
                        break;                      // Max of 4 tokens
                }
            }
            else {
                token += buffer[i];                 // Append character to token
            }
        }

        // Store the last token if present in lowercase
        if (!token.empty() && tokenCount < 4) {
            toLowerInPlace(token); 
            tokens[tokenCount++] = token;
        }

        // Allocate memory for the command array to store tokens
        command = new string[tokenCount];
        for (int i = 0; i < tokenCount; ++i) {
            command[i] = tokens[i];  // Copy tokens to command array
        }
    }

    // Checking Validity of COmmand
    int Valid()
    {
        if (command[0] == "s" && tokenCount == 1)      // Draw Stock Instruction
            return 1;
        else if (command[0] == "z" && tokenCount == 1)     // Undo Instruction
            return 2;
        else if (command[0] == "m")    // Move Instructions
        {
            if (validArgument(command[1]) && validArgument(command[2], false) && (tokenCount == 3 || IsDigit(command[3])))
            {
                return 3;
            }
            return -1;
        }
        else if (command[0] == "q")        // Quit Instruction
            return 4;
        return -1;    // Invalid Instruction
    }

    // Getter for Source in command
    string getSource()
    {
        if (tokenCount >= 2)
            return command[1];
    }

    // Getter for Destination in command
    string getDestination()
    {
        if (tokenCount >= 3)
            return command[2];
    }

    // Getter for Flipped Card by reference
    bool& cmd_undo()
    {
        return flippedLastCard;
    }

    // Getter for Count in command
    string getCount()
    {
        if (tokenCount >= 4)
            return command[3];
        return "1";
    }

    // Function to check if command is cmd (req command) or not
    bool isNotEqualTo(string cmd)
    {
        if (command[0] != cmd)
            return true;
        return false;
    }
};

// ---------- CLASS GAME ---------------
template <class T>
class Game
{ class MovementActions {
    public:
        // Static method for drawing from the stock and pushing back to stock
        static bool DrawStock(Stack<T>& Stock, Stack<T>& Waste, bool undo = false) {
            if (Stock.isEmpty() && Waste.isEmpty())     // return false if both are empty
                return false;
            // Card is present in stock
            if(!undo && !Stock.isEmpty())
            {
                typename DoublyLinkedList<T>::Iterator ite = Stock.top();
                (*ite).FlipVisible();     // Flip Card before attaching and popping
                Stock.pop();              // Detaching Link 
                Waste.push(ite);          // Attaching Link
            }
            else if(undo && !Waste.isEmpty())   // if undo and waste is not empty
            {
                typename DoublyLinkedList<T>::Iterator ite = Waste.top();
                (*ite).FlipVisible();
                Waste.pop();                // Detaching Link 
                Stock.push(ite);            // Attaching Link
            }
            else if (Stock.isEmpty() && !Waste.isEmpty())
            {
                while (!Waste.isEmpty())   // Moving All cards to stack from waste
                {
                    typename DoublyLinkedList<T>::Iterator ite = Waste.top();
                    (*ite).FlipVisible();
                    Waste.pop();                // Detaching Link 
                    Stock.push(ite);            // Attaching Link
                }
            }
            else if (undo && Waste.isEmpty())
            {
                while (!Stock.isEmpty())        // moving all cards to waste from stock in undo
                {
                    typename DoublyLinkedList<T>::Iterator ite = Stock.top();
                    (*ite).FlipVisible();
                    Stock.pop();              // Detaching Link 
                    Waste.push(ite);          // Attaching Link
                }
            }
            else  return false;
            return true;
        }

        // Static method for moving cards between tableau columns
        static bool TableuAndTableuMove(DoublyLinkedList<T>& source, DoublyLinkedList<T>& dest, int amount, bool undo, bool& cmd_undo) {
            if (undo)
                return dest.shiftNodes(source, amount,cmd_undo,undo);      // Call function with reversed paramters in undo
            return (source.shiftNodes(dest, amount,cmd_undo));  // Shift Cards
        }

        // Static method for moving cards between tableau and foundation
        static bool TableuAndFoundationMove(DoublyLinkedList<T>& tableau, Stack<T>& foundation,bool undo,bool& cmd_undo) {
            int tableau_Size = tableau.GetSize();
            if (!undo &&  tableau_Size > 0)               // if tableau has cards to move
            {
                typename DoublyLinkedList<T>::Iterator ite = tableau.end();
                --ite;
                // Move if either card is 1 and foundation is empty or card is of same suit with one greater rank
                if ((foundation.isEmpty() && (*ite).isRank(1)) || (*ite).isSameSuit(*(foundation.top())) && (*ite).rankComparision(*(foundation.top()), 1))
                {
                    tableau.deleteTailNode();
                    foundation.push(ite);
                    if(!tableau.atIndex(tableau_Size - 2).visibility())        // flip last card if it was unflipped
                    {
                        tableau.atIndex(tableau_Size - 2).FlipVisible();       // Size is stored before deleting Last Node
                        cmd_undo = true;                                       // store flip for undo
                    }
                    return true;
                }
            }
            else if (undo && foundation.getSize() > 0)                         // for undo operation
            {
                typename DoublyLinkedList<T>::Iterator ite = foundation.top();
                foundation.pop();
                if(cmd_undo)                                                   // flip if card was flipped in that command
                     tableau.atIndex(tableau_Size - 1).FlipVisible();
                tableau.insertNode(ite);                                       // attach node          
                return true;
            }
            return false;
        }

        // Static method for moving cards between waste and tableau
        static bool WasteAndTableuMove(Stack<T>& waste, DoublyLinkedList<T>& tableau, Stack<T>& Stock, bool undo) {
            int tableau_Size = tableau.GetSize();
            if (!undo && waste.getSize() > 0)                               // if waste is not empty
            {
                typename DoublyLinkedList<T>::Iterator ite = waste.top();
                if (tableau_Size == 0 && (*ite).isRank(13))                 // if tableau is empty insert only king
                {
                    waste.pop();
                    tableau.insertNode(ite);
                }                                                           // if rank is card has opposite color and one smaller rank
                else if ((*ite).rankComparision(tableau.atIndex(tableau_Size - 1), -1) && (*ite).isOppositeColor(tableau.atIndex(tableau_Size - 1)))
                {
                    waste.pop();
                    tableau.insertNode(ite);
                }
                else return false;
            }
            else if (undo && tableau_Size > 0)                              // for undo      
            {
                typename DoublyLinkedList<T>::Iterator ite = tableau.end();
                --ite;
                tableau.deleteTailNode();
                waste.push(ite);
            }
            else  return false;                                             // if no condition met
            return true;
        }

        // Static method for moving cards between waste and Foundations
        static bool WasteAndFoundationMove(Stack<T>& waste, Stack<T>& foundation, bool undo)
        {
            if (!undo && waste.getSize() > 0)                        // if waste is not empty
            {
                typename DoublyLinkedList<T>::Iterator ite = waste.top();
                if ((foundation.getSize() == 0 && (*ite).isRank(1)) || (foundation.getSize() && (*ite).rankComparision(*(foundation.top()), 1)) && (*ite).isSameSuit(*(foundation.top())))
                {                                                 // if foundation is empty move ace or else same suit card with one greater rank
                    waste.pop();
                    foundation.push(ite);
                }
                else return false;
            }
            else if (undo && foundation.getSize())                 // for undo 
            {
                typename DoublyLinkedList<T>::Iterator ite = foundation.top();
                foundation.pop();
                waste.push(ite);
            }
            else return false;                                      // no condition met
            return true;
        }

        // Static method to handle move command based on source and destination
        static bool Move(Game<T>& game, const string& source, const string& dest, int amount, Command& cmd, bool undo = false) {
            if (source[0] == 'c' && dest[0] == 'c')
                return TableuAndTableuMove(game.Tableau_Columns[stoi(source.substr(1)) - 1], game.Tableau_Columns[stoi(dest.substr(1)) - 1], amount, undo,cmd.cmd_undo());
            else if (source[0] == 'c' && dest[0] == 'f' || source[0] == 'f')
                return TableuAndFoundationMove(game.Tableau_Columns[stoi(source.substr(1)) - 1], game.Foundations[stoi(dest.substr(1)) - 1], undo,cmd.cmd_undo());
            else if (source[0] == 'w' && dest[0] == 'c')
                return WasteAndTableuMove(game.Waste, game.Tableau_Columns[stoi(dest.substr(1)) - 1],game.Stock, undo);
            else if (source[0] == 'w' && dest[0] == 'f')
                return WasteAndFoundationMove(game.Waste, game.Foundations[stoi(dest.substr(1)) - 1], undo);
        
            return false;  // Invalid move
        }
    };

    // Private data members of Class
    Deck<T> myDeck;
    Stack<T>  Stock;
    Stack<T>  Waste;
    Stack<T>  Foundations[4];
    Stack<Command> Commands;
    DoublyLinkedList<T> Tableau_Columns[7];
    int xp;

    // Function to Display Starting Window of Solitare
    bool displayWelcomeScreen() {
        setColor(4);  cout << "\n\n\t\t[♠]\t";
        setColor(1);  cout << "███████╗ ██████╗ ██╗     ██╗████████╗ █████╗ ██████╗ ███████╗\t";
        setColor(4);  cout << "[♠]\n\t\t[♠]\t";
        setColor(1); cout << "██╔════╝██╔═══██╗██║     ██║╚══██╔══╝██╔══██╗██╔══██╗██╔════╝\t";
        setColor(4); cout << "[♠] \n\t\t[♠]\t";
        setColor(1); cout << "███████╗██║   ██║██║     ██║   ██║   ███████║██████╔╝█████╗  \t";
        setColor(4); cout << "[♠]\n\t\t[♠]\t";
        setColor(1); cout << "╚════██║██║   ██║██║     ██║   ██║   ██╔══██║██╔══██╗██╔══╝  \t";
        setColor(4); cout << "[♠]\n\t\t[♠]\t";
        setColor(1); cout << "███████║╚██████╔╝███████╗██║   ██║   ██║  ██║██║  ██║███████╗\t";
        setColor(4); cout << "[♠]\n\t\t[♠]\t";
        setColor(1); cout << "╚══════╝ ╚═════╝ ╚══════╝╚═╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝\t";
        setColor(4); cout << "[♠]\n";
        setColor(3); cout << "\n\n\t\t  ─────────────────────────────────● ● ● ●────────────────────────────────\n\n";
        setColor(6);  cout << "\t\t\t\t\t⮚ Press Any Key To START GAME ⮘\n\n";
        setColor(6);  cout << "\t\t\t\t\t   ⮚ Press \"Q\" To QUIT GAME ⮘\n\n";
        setColor(3); cout << "\n\t\t  ─────────────────────────────────● ● ● ●────────────────────────────────\n\n";
        setColor(15); cout << "\t\t  Command: ";
        char ans[100]; cin >> ans;
        cin.ignore();
        if (ans[0] == 'q' || ans[0] == 'Q') {
            setColor(12); cout << "\t\t  QUITTING.....\n";
            Sleep(1000);
            return false;
        }
        setColor(2); cout << "\t\t  LOADING GAME.....\n";
        Sleep(1000);
        return true;
    }

    // Function to Remove Cards from deck and fill Stock and tableau Columns
    void DistributeDeckWithShuffling()
    {
        // Shifting Cards to Tableau according to their size
        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j <= i; j++)
            {
                typename DoublyLinkedList<T>::Iterator ite = myDeck.ReturnRandomCard();
                if (j == i)
                    (*ite).FlipVisible();                // Flip Last Card before Inserting
                Tableau_Columns[i].insertNode(ite);
            }
        }

        // Shifting Remaining Cards to Stock
        while (myDeck.getSize())
        {
            typename DoublyLinkedList<T>::Iterator ite = myDeck.ReturnRandomCard();
            Stock.push(ite);
        }
    }

    // Function To Display Interface of Game
    void DisplayInterface()
    {
        system("cls");
        setColor(8); cout << "    ------------------------------------------------------------------------------------------------------------------\n\n\t";
        setColor(11);  cout << "Score : ";
        setColor(15); cout << xp;
        setColor(11); cout << "\t\t\tLast Command: ";
        setColor(15);
        if (Commands.getSize())
            cout << (*Commands.top());
        else
            cout << "No History";
        setColor(8); cout << "\n\n    ------------------------------------------------------------------------------------------------------------------\n\n";
        setColor(11);
        cout << "\tStock\t" << "Waste\t\t\t" << "Foundation 1 \tFoundation 2 \tFoundation 3  \tFoundation 4\n\t";
        setColor(15);
        if (Stock.isEmpty())
            cout << " ⭕";
        else
            cout << "[  ]";
        cout << "\t" << Waste << "\t\t\t" << Foundations[0] << "\t\t" << Foundations[1] << "\t\t" << Foundations[2] << "\t\t" << Foundations[3] << endl;
        setColor(15);
        cout << "     (" <<Stock.getSize() << " cards)\t("<< Waste.getSize() << " cards)\t\t(" << Foundations[0].getSize() << " cards)\t(" << 
            Foundations[1].getSize() << " cards)\t(" << Foundations[2].getSize() << " cards)\t(" << Foundations[3].getSize() <<" cards)"<< endl << endl;
        setColor(8);
        cout << "    ------------------------------------------------------------------------------------------------------------------\n\n\t";
        int maxSize = 0;
        setColor(11);
        for (int i = 0; i < 7; i++)
            cout << "Column " << i + 1 << "\t";
        setColor(15);

        cout << "\n\t";
        for (int i = 0; i < 7; i++)
        {
            int size = Tableau_Columns[i].GetSize();
            cout << "(" << size << " Cards)\t";             
            maxSize = max(maxSize, size);               // getting max size of tableau Columns
        }
        cout << "\n\n\t  ";
        for (int i = 0; i < maxSize; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                if (i < Tableau_Columns[j].GetSize())     // Printing Columns in vertical display
                    cout << Tableau_Columns[j].atIndex(i);
                cout << "\t\t  ";
            }

            cout << "\n\t  ";
        }
        setColor(8);
        cout << "\n    ------------------------------------------------------------------------------------------------------------------\n\n";
        setColor(7);
    }

    int runCommand(Command*& command)                           // Function to run game
    {
        int valid = command->Valid();                           // checking if command is valid
        if (valid == 1)
        {
            if (MovementActions::DrawStock(Stock, Waste))       // Draw Stock 
                xp += 5;
        }
        else if (valid == 2)                                    // For undo         
        {
            if (Commands.getSize())                             // if there is one command already executed
            {
                bool undo_done = false;
                if (!(*Commands.top()).isNotEqualTo("s"))       // if last command was draw stock
                {
                    undo_done = MovementActions::DrawStock(Stock, Waste, true);
                    if (undo_done)  xp -= 5;
                }
                else                                           // else Movement commands
                {
                    string source = (*Commands.top()).getSource();                                          // take source
                    string destination = (*Commands.top()).getDestination();                                // take destination 
                    int amount = std::stoi((*Commands.top()).getCount());                                   // take amount
                    undo_done = MovementActions::Move(*this, source, destination, amount, *Commands.top(), true);
                    if (undo_done)                                                                          
                        if (destination[0] == 'f')    xp -= 20;
                        else xp -= 10 * amount;
                }

                if (undo_done)                                                                              // if undo executed pop last command
                    Commands.delete_pop();
                return  undo_done;
            }
            return false;                                                                                  // invalid command
        }
        else if (valid == 3)                                                                               // general movement instructions
        {
            string source = command->getSource();
            string destination = command->getDestination();
            int amount = std::stoi(command->getCount());
            if ((destination[0] == 'f' || source == "w") && amount != 1)                // if more than 1 cards to be moved in waste or foudndation movements
                return false;
            if (MovementActions::Move(*this, source, destination, amount,*command))     // call move function
                if (destination[0] == 'f')  xp += 20;
                else xp += 10 * amount;
            else return false;
        }
        else if (valid == 4)                                                          // Quit Instructions
            return -1;
        else return false;                                                            // invalid command
        return true;
    }

    // Function to Check if user won
    bool winCheck()
    {
        for (int i = 0; i < 4; i++)
            if (Foundations[i].getSize() != 13)       // win if all foundations are filled with 13 cards
                return false;
        xp += 250;
        system("cls"); setColor(2);
        cout << "\n\n\t\t\t██╗   ██╗ ██████╗ ██╗   ██╗    ██╗    ██╗██╗███╗   ██╗\n";
        cout << "\t\t\t╚██╗ ██╔╝██╔═══██╗██║   ██║    ██║    ██║██║████╗  ██║\n";
        cout << "\t\t\t ╚████╔╝ ██║   ██║██║   ██║    ██║ █╗ ██║██║██╔██╗ ██║\n";
        cout << "\t\t\t  ╚██╔╝  ██║   ██║██║   ██║    ██║███╗██║██║██║╚██╗██║\n";
        cout << "\t\t\t   ██║   ╚██████╔╝╚██████╔╝    ╚███╔███╔╝██║██║ ╚████║\n";
        cout << "\t\t\t   ╚═╝    ╚═════╝  ╚═════╝      ╚══╝╚══╝ ╚═╝╚═╝  ╚═══╝\n";
        setColor(15);  cout << "\n\t\t\t\t    ₒₒ∙∙·▫ᵒᴼ ( " << xp << " Points ) ᴼᵒ▫·∙∙ₒₒ\n";
        return true;
    }

public:

    // Default Constructor to load deck and shuffle 
    Game(): xp(0)
    {
        myDeck.LoadDeck();
        DistributeDeckWithShuffling();
    }
    
    // Function to run Game
    void run_Game()
    {
        if(displayWelcomeScreen())       // display home screen
        {
            while (true)
            {
                DisplayInterface();                     // display interface
                Command* command_Input = new Command;
                command_Input->takeInput();             // take input in command and allocate space
                int valid = runCommand(command_Input);        // run command
                
                if (valid == 1)                        // if valid
                {
                    if (command_Input->isNotEqualTo("z"))         // add command except undo command
                    {
                        Commands.pushInsert(*command_Input);
                    }
                    else delete command_Input;          // delete undo command
                    setColor(2);
                    cout << "\tCommand Executed Succesfully\n";
                }
                else if (valid == -1)                  // if command is quit
                {
                    setColor(12);
                    cout << "\tQuitting Game\n";
                    break;                           // break if quit
                }
                else                                 // invalid command
                {
                    setColor(12);
                    cout << "\tCommand Execution Unsuccesfull\n";
                    delete command_Input;             // delete invalid command
                }
                setColor(7);
                Sleep(1500);                     // Pause for 1.5 seconds
                if (winCheck())
                    break;                      // break if won
            }
        }
        setColor(7);
    }
};

int main() {

    SetConsoleOutputCP(CP_UTF8);                     // set console to print unicodes
    srand(time(0));                                  // for random numbers

    Game<Card> g1;                                   // game object
    g1.run_Game();

    return 0;
}