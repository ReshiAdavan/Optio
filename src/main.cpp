#include "cli/cli.hpp"
#include <iostream>

using namespace std;

int main() {
    CommandLineInterface cli;
    int choice;

    do {
        cout << "\nWelcome to Optio!" << endl;
        cout << "1. Create Option" << endl;
        cout << "2. Display Option Info" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cli.displayMenu();
                break;

            case 2: {
                string optionName;
                cout << "Enter the name of the option to display: ";
                cin >> optionName;
                cli.displayOptionInfo(optionName);
                break;
            }

            case 3:
                cout << "Exiting program. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3);

    return 0;
}
