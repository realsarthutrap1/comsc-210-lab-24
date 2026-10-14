// COMSC-210 | Lab 24 | Sarthak Pani
// adapted from the instructor goat manager starter

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>
#include "Goat.h"
using namespace std;

const int SZ_NAMES = 200, SZ_COLORS = 25, MAX_AGE = 20;

int select_goat(const set<Goat>& trip);
void delete_goat(set<Goat> &trip);
void add_goat(set<Goat> &trip, string [], string []);
void display_trip(const set<Goat>& trip);
int main_menu();

int main() {
    srand(time(0));

    // read & populate arrays for names and colors
    ifstream fin("names.txt");
    string names[SZ_NAMES];
    for (int i = 0; i < SZ_NAMES; ++i) {
        if (!(fin >> names[i])) {
            cerr << "Could not read all names from names.txt" << endl;
            return 1;
        }
    }
    fin.close();
    ifstream fin1("colors.txt");
    string colors[SZ_COLORS];
    for (int i = 0; i < SZ_COLORS; ++i) {
        if (!(fin1 >> colors[i])) {
            cerr << "Could not read all colors from colors.txt" << endl;
            return 1;
        }
    }
    fin1.close();

    // keep the menu running until the user quits
    set<Goat> trip;
    int choice;
    do {
        choice = main_menu();
        switch (choice) {
        case 1:
            add_goat(trip, names, colors);
            break;
        case 2:
            delete_goat(trip);
            break;
        case 3:
            display_trip(trip);
            break;
        case 4:
            cout << "Goodbye!" << endl;
            break;
        }
    } while (choice != 4);

    return 0;
}

int main_menu() {
    cout << "\n*** GOAT MANAGER 3002 ***\n"
         << "[1] Add a goat\n"
         << "[2] Delete a goat\n"
         << "[3] List goats\n"
         << "[4] Quit\n";

    while (true) {
        cout << "Choice --> ";
        string input;
        if (!getline(cin, input))
            return 4;

        // accept one whole integer and reject extra input
        istringstream entry(input);
        int choice;
        char extra;
        if (entry >> choice && !(entry >> extra) && choice >= 1 && choice <= 4)
            return choice;

        cout << "Please enter a number from 1 to 4." << endl;
    }
}

void add_goat(set<Goat>& trip, string names[], string colors[]) {
    string name = names[rand() % SZ_NAMES];
    string color = colors[rand() % SZ_COLORS];
    int age = rand() % (MAX_AGE + 1);
    // names are unique because the set compares goats by name
    auto result = trip.insert(Goat(name, age, color));
    if (result.second)
        cout << "\nAdded: " << name << " (" << age << ", " << color << ")" << endl;
    else
        cout << "\n" << name << " is already in the trip. No goat added." << endl;
}

void display_trip(const set<Goat>& trip) {
    cout << "\nGoats in current trip: " << trip.size() << " (sorted by name)" << endl;
    if (trip.empty()) {
        cout << "No goats in the trip." << endl;
        return;
    }

    int number = 1;
    for (const Goat& goat : trip) {
        cout << "    [" << number++ << "] " << goat.get_name()
             << " (" << goat.get_age() << ", " << goat.get_color() << ")" << endl;
    }
}

int select_goat(const set<Goat>& trip) {
    display_trip(trip);
    if (trip.empty())
        return 0;

    while (true) {
        cout << "Goat number --> ";
        string input;
        if (!getline(cin, input)) {
            cout << "\nDeletion canceled: input ended." << endl;
            return 0;
        }

        istringstream entry(input);
        int number;
        char extra;
        if (entry >> number && !(entry >> extra) && number >= 1 &&
            static_cast<size_t>(number) <= trip.size())
            return number;

        cout << "Please enter a number from 1 to " << trip.size() << "." << endl;
    }
}

void delete_goat(set<Goat>& trip) {
    if (trip.empty()) {
        cout << "\nNo goats to delete. Add a goat first." << endl;
        return;
    }
    cout << "\nSelect a goat to delete." << endl;
    int number = select_goat(trip);
    if (number == 0)
        return;

    // walk to the selected goat rather than matching its name
    auto goat = trip.begin();
    for (int i = 1; i < number; ++i)
        ++goat;
    cout << "Deleted: " << goat->get_name() << " (" << goat->get_age()
         << ", " << goat->get_color() << ")" << endl;
    trip.erase(goat);
}
