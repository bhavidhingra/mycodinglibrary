// the program simulates inventory maintenance for a
// video store. the program stores the title of a film
// and the number of copies the store owns in a video
// object. the video class has functions that access
// and update the number of copies, compare objects
// by title, and output a title and the number of copies.
// the function setupInventory() inputs film titles
// from the file "films.dat" and creates the BSTree
// object inventory of video data. after listing the
// films in the inventory, in an interactive loop
// the clerk inputs whether the customer wishes
// to rent a film, return a film, or whether business
// is over for the day. when a customer rents a film,
// the program updates the inventory by reducing the
// number of copies of the film by 1, and adds the film
// to the BSTree object rentals that maintains a database
// of rented films. when a customer returns a film, the
// program removes 1 copy of the film from the rentals
// object and increases the number of copies in inventory
// by 1. at the end of the business day, the program outputs
// the list of rented films and the films remaining in
// the inventory

#include <iostream>
#include <fstream>
#include <string>
#include <utility>      // for pair class

#include "BSTree.h"         // BSTree class
#include "WriteContainer.h" // for WriteBSTree()

using namespace std;

class video
{
    public:
        // constructor. initialize film title and numCopies
        video(const string& film = "", int copies = 1):
            filmTitle(film), numCopies(copies)
        {}

        // add n to the number of copies. note that if n < 0
        // the function decreases the number of copies
        void updateCopies(int n)
        {
            numCopies += n;
        }
        
        // return the number of copies of the film title
        int getCopies()
        {
            return numCopies;
        }
        
        // two video objects are "equal" if they have the same title
        friend bool operator== (const video& lhs, const video& rhs)
        {
            return lhs.filmTitle == rhs.filmTitle;
        }

        // two video objects are "not equal" if they don't have the same title
        friend bool operator!= (const video& lhs, const video& rhs)
        {
            return lhs.filmTitle != rhs.filmTitle;
        }

        // compare video objects by comparing film titles
        friend bool operator< (const video& lhs, const video& rhs)
        {
            return lhs.filmTitle < rhs.filmTitle;
        }

        // output a video object
        friend ostream& operator<< (ostream& ostr, const video& obj)
        { 
            ostr << obj.filmTitle << " (" << obj.numCopies << ")" ;
            return ostr;
        }
    private:
        // title of the film
        string filmTitle;
        // number of copies (>= 0)
        int numCopies;
};

// initialize inventoryList from file "films.dat"
void setupInventory(BSTree<video>& inventory);

// process the return of a film
void returnTransaction(BSTree<video>& inventory, BSTree<video>& rentals,
                       const string& filmName);

// process the rental of a film
void rentalTransaction(BSTree<video>& inventory, BSTree<video>& rentals,
                       const string& filmName);

int main()
{
    // the inventory and rental lists
    BSTree<video> inventory, rentals;
    // assign return value from find() to filmIter
    BSTree<video>::iterator filmIter;
    // input from store operator. transactionType = "Rent", "Return",
    // or "Done"
    string transactionType;

    // film requested by a customer
    string filmName;

    // read and output inventory file
    setupInventory(inventory);
    cout << "Initial inventory list:" << endl;
    WriteBSTree(inventory, "\n");
    cout << endl;

    // process customers by entering "Rental" or "Return"
    // followed by the film name or "Done" to end the program.
    // for "Rent", decrease number of copies in inventory by 1
    // and add the copy to the rental database. for "Return",
    // remove copy from rental database and increase number
    // of copies in inventory by 1
    cout << "Transactions: Enter type (Rent, Return, Done)" << endl;
    cout << "followed by film name or space if done" << endl << endl;
    while (true)
    {
        // input the transaction type. the input must terminate with
        // a blank
        cout << "Transaction: ";
        getline(cin, transactionType, ' ');
        // if "Done", terminate the loop
        if (transactionType == "Done")
            break;

        // get film name
        getline(cin, filmName,'\n');

        if (transactionType == "Return")
            returnTransaction(inventory, rentals, filmName);
        else
            rentalTransaction(inventory, rentals, filmName);
    }
    cout << endl;

    // output the final rental and inventory lists.
    cout << "Rented Films: " << endl << endl;
    WriteBSTree(rentals, "\n");
    cout << endl;
    cout << "Films Remaining in Inventory:" << endl << endl;
    WriteBSTree(inventory, "\n");

    return 0;
}

void setupInventory(BSTree<video>& inventory)
{
    ifstream filmFile("films.dat");  // input stream
    string filmName;                 // individual file names
    // use with BSTree insert()
    pair<BSTree<video>::iterator, bool> p;

    // open the file "films.dat"
    //filmFile.open("films.dat");
    if (!filmFile)
    {
        cerr << "File 'films.dat' not found!" << endl;
        exit(1);
    }

    // read lines until EOF; insert names in inventory list
    while(true)
    {
        getline(filmFile, filmName, '\n');
        if (!filmFile)
            break;

        // try an insertion with default of 1 copy
        p = inventory.insert(video(filmName));
        // see if video already in the inventory
        if (p.second == false)
            // it is in the inventory. increment number of copies
            (*(p.first)).updateCopies(1);
    }
}

void returnTransaction(BSTree<video>& inventory, BSTree<video>& rentals,
                       const string& filmName)
{
    BSTree<video>::iterator filmIter;

    // locate the film in the return database
    filmIter = rentals.find(video(filmName));
    // if there is only 1 copy left, erase the entry;
    // otherwise, decrease the number of rented copies
    // by 1
    if ((*filmIter).getCopies() == 1)
        rentals.erase(filmIter);
    else
        (*filmIter).updateCopies(-1);

    // locate the film in the inventory and increase the
    // number of copies available by 1
    filmIter = inventory.find(video(filmName));
    (*filmIter).updateCopies(1);

}

void rentalTransaction(BSTree<video>& inventory, BSTree<video>& rentals,
                       const string& filmName)
{
    BSTree<video>::iterator filmIter;
    // use pObj with BSTree insert()
    pair<BSTree<video>::iterator,bool> pObj;

    // is film available?
    filmIter = inventory.find(video(filmName));
    if (filmIter == inventory.end())
        // film is not in the store's inventory
        cout << "Film " << filmName << " is not in inventory" << endl;
    else if ((*filmIter).getCopies() == 0)
        // all copies are checked out
        cout << "All copies of " << filmName << " are checked out" << endl;
    else
    {
        // decrease the number of copies in the inventory
        // by 1
        (*filmIter).updateCopies(-1);

        // attempt to insert the film into rentalList. if it is
        // inserted, the number of copies will be 1
        pObj = rentals.insert(video(filmName));
        // if film not inserted, increase number of rented copies
        // by 1
        if (pObj.second == false)
            (*(pObj.first)).updateCopies(1);
    }
}

/*
Run:

Initial inventory list:
Frequency (1)
Gladiator (2)
Lord of the Rings (4)
U-571 (2)

Transactions: Enter type (Rent, Return, Done)
followed by film name or space if done

Transaction: Rent Gladiator
Transaction: Rent Frequency
Transaction: Rent Shaft
Film Shaft is not in inventory
Transaction: Rent Frequency
All copies of Frequency are checked out
Transaction: Done

Rented Films:

Frequency (1)
Gladiator (1)

Films Remaining in Inventory:

Frequency (0)
Gladiator (1)
Lord of the Rings (4)
U-571 (2)
*/
