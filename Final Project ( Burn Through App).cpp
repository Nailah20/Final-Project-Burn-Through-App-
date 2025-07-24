#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;

struct CaloriesEntry {
	string name; // users name 
	string date; // date of entry
	double caloriesWantToBurn; // calories user wants to burn
	double milesToWalk; // miles calculated to walk

	CaloriesEntry(string n, string d, double ctb, double mtw) // constructor to initialize the CaloriesEntry
		: name(n), date(d), caloriesWantToBurn(ctb), milesToWalk(mtw) {
	}
	CaloriesEntry() : name(""), date(""), caloriesWantToBurn(0.0), milesToWalk(0.0) { // default constructor
	}
};

double calculateMiles(double calories) {
	const double caloriesPerMile = 100.0; // average calories burned per mile
	return calories / caloriesPerMile; // calculate miles based on calories
}
void saveEntry(const CaloriesEntry& entry, const string& filename) {

	ofstream outFile(filename, ios::app); // open file in append mode if file doesn't exist, it will be created
	if (outFile.is_open()) {
		outFile << entry.name << "," << entry.date << "," << entry.caloriesWantToBurn << "," << entry.milesToWalk << "\n"; // write entry to file
		outFile.close(); // close the file after writing 
		cout << "Entry saved successfully to " << filename << endl;
	}
	else {
		cerr << "Error opening file for writing: " << filename << endl; // error message if file cannot be opened
	}
}
vector<CaloriesEntry> loadEntries(const string& filename) {
	vector<CaloriesEntry> entries; // vector to store entries
	ifstream inFile(filename); // open file for reading

	if (inFile.is_open()) {
		string line;
		while (getline(inFile, line)) {
			size_t pos1 = line.find(','); // finding the positions of commas to parse the data 
			size_t pos2 = line.find(',', pos1 + 1); 
			size_t pos3 = line.find(',', pos2 + 1); 

			// Extracting substring based on comma positions
			string name = line.substr(0, pos1); // extract name
			string date = line.substr(pos1 + 1, pos2 - (pos1 + 1)); // extract date

			// Coverting string to double for calories and miles 
			double caloriesWantToBurn = stod(line.substr(pos2 + 1, pos3 - (pos2 + 1))); // extract calories
			double milesToWalk = stod(line.substr(pos3 + 1)); // extract miles

			// Creating a CalorieEntry object and adding it to the vector
			entries.emplace_back(name, date, caloriesWantToBurn, milesToWalk);
		}
		inFile.close(); // close the file after reading
		cout << "Loaded" << entries.size() << "entries from " << filename << endl;
	}
	else {
		cout << "Information: File" << filename << "not found or could not be opened." << endl;
	}
	return entries; // return the vector of entries
}

void displayEntries(const vector<CaloriesEntry>& entries) {
	if (entries.empty()) {
		cout << "\nNo past entries found." << endl;
		return; // if no entries, return
	}
	cout << "\n Past Calories Entries:" << endl;
	for (const auto& entry : entries) { // loop through each entry
		cout << "Name: " << entry.name << ", Date: " << entry.date 
			<< ", Calories to Burn: " << entry.caloriesWantToBurn 
			<< ", Miles to Walk: " << entry.milesToWalk << endl; // display the entry
	}
	cout << "---------------------" << endl; 
}

// A helper function to get validated double input from the user.
// prompt the user and ensure a valid number is entered.

double getValidatedDoubleInput(const string& prompt) { 
	double value;
	while (true) { // loop until valid input is received 
		cout << prompt;
		cin >> value;
		if (cin.fail() || value < 0) { // checking for unput failure or negative value
			cout << "Invalid input. Please enter a positive number" << endl;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
		else {
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			break;
        }
		
	}
	return value;
}
int main() {
	
	const string DATA_FILENAME = " calorie_data.txt"; // File to store data 
	cout << " Calorie Burning Calculator" << endl;

	// Getting user input
	string userName;
	cout << "Enter your name: ";
	getline(cin, userName); // read the user's name

	string entryDate;
	cout << "Enter the date (YYYY-MM-DD): ";
	getline(cin, entryDate); // read the date of entry

	// Using the helper function for validated numeric input 
	double calories = getValidatedDoubleInput("Enter the number of calories you want to burn: "); // get calories to burn

	// Calcuate miles 
	double miles = calculateMiles(calories);
	cout << "\nTo burn " << calories << " calories, you need to walk approximately " 
		<< miles << " miles." << endl;

	// create and save the entry 
	CaloriesEntry newEntry(userName, entryDate, calories, miles);
	saveEntry(newEntry, DATA_FILENAME);

	// Load and Display all pasy entries 
	vector<CaloriesEntry> allEntries = loadEntries(DATA_FILENAME);
	displayEntries(allEntries);

	cout << "\nThanks for using Burn Through App!" << endl;
	cin.get(); // wait for user input before exiting

	return 0;

}

