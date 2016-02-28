//-------------------------------------------------------------------
// File		:hw3.C
// Author	:Michael Benson
// Course	:CS1129
// Date		:Febuary 20, 2006
//
// Homework 3 is a text based music library manager that will read
// a text file for library data and then allow manipulation of that 
// data and finally save it back to the file upon request.
//
// Additional Implementations:
//
// * Graphical Logo - Basic *
//  	Uses a basic graphical logo on startup
//
// * Space Acceptance in title, album, and artist fields *
//  	Will accept spaces (blank characters) in the possable multi-
//  	word fields for title, album, and artist entries
// ------------------------------------------------------------------

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cctype>
#include <cstring>

using namespace std;

// Global Variables and Enumerations
const int MAX_TITLE = 27;
const int MAX_ALBUM = 24;
const int MAX_ARTIST = 24;
const int MAXLS = 100;
const int TITLEL = MAX_TITLE +1;
const int ALBUML = MAX_ALBUM +1;
const int ARTISTL = MAX_ARTIST +1;
const char libFile[] = "musicLibrary.txt";

// Startup Functions
void initLib(char titleData[][TITLEL], char albumData[][ALBUML],
	char artistData[][ARTISTL], char genre[], int rating[]);

// I/O file open functions
bool openFile(ifstream& inStream);
void openFile(ofstream& outStream);

// User interface functions
void splashScreen();
void menuControl(char titleData[][TITLEL], char albumData[][ALBUML], 
	char artistData[][ARTISTL], char genre[], int rating[],
	ofstream& outStream);
void mainMenu(int& select);

// Library Initialization
void loadLibrary(ifstream& inStream, char titleData[][TITLEL],
	char albumData[][ALBUML], char artistData[][ARTISTL],
	char genre[], int rating[]);
void libSizeReturn(int& libSize, ifstream& inStream);
void moveToNext(ifstream inStream);

// Library Display Functions
void displayLib(const char titleData[][TITLEL], 
	const char albumData[][ALBUML], 
	const char artistData[][ARTISTL], const char genre[], 
	const int rating[]);
bool genreTranslate(const char genre);
void ratingTranslate(const int rating);

// Song addition functions
void addSong(char titleData[][TITLEL], char albumData[][ALBUML], 
	char artistData[][ARTISTL], char genre[], int rating[]);
int findOpen(const char titleData[][TITLEL]);
void addTitle(char titleData[][TITLEL], int addLocation);
void addAlbum(char albumData[][ALBUML], int addLocation);
void addArtist(char artistData[][ARTISTL], int addLocation);
void addGenre(char genre[], int addLocation);
void addRating(int rating[], int addLocation);

// Song removal functions
void removeSong(char titleData[][TITLEL],char albumData[][ALBUML],
	char artistData[][ARTISTL],char genre[], int rating[]);
void displaySongData(const char titleData[][TITLEL],
	const char albumData[][ALBUML],
	const char artistData[][ARTISTL],const char genre[], 
	const int rating[],int display);

// Library Status functions
void libStats(const char titleData[][TITLEL]);

// Write out functions
void writeLib(ofstream& outStream, char titleData[][TITLEL], 
	char albumData[][ALBUML], char artistData[][ARTISTL],
	char genre[], int rating[]);

// Quit program save function
bool leaving();

// Utility Functions
void skipSpaces(ifstream& inStream);
void getNoError(char input[],int length);
void getNoError(char& input);
bool confirm(const int input);
bool confirm(const char input[]);
bool confirm();
void printGenres();

int main()
{
	// Declair Library Arrays
	char titleData[MAXLS][TITLEL];
	char albumData[MAXLS][ALBUML];
	char artistData[MAXLS][ARTISTL];
	char genre[MAXLS];
	int rating[MAXLS];

	// Set up stream objects
	ifstream inStream;
	ofstream outStream;
	
	// Initalize the Arrays with Null's
	initLib(titleData, albumData, artistData, genre, rating);
	
	// Read In the Media Library
	if(openFile(inStream) != false)
		loadLibrary(inStream, titleData, albumData, 
		artistData, genre, rating);

	// Start UI
	splashScreen();
	menuControl(titleData, albumData, artistData, genre, rating, 
			outStream); 

	inStream.close(); // alpha code to ensure close
	outStream.close(); // alpha code to ensure close
	
	return 0;
}

//-------------------------------------------------------------------
//	initLib(char[][], char[][], char[][], char[], int[])
//
// Preconditions	:Takes title, album, artist, genre, and
//			 rating strings as arguements
//
// Postconditions	:Adds a NULL character to the beginning of
//			 each array passed to the function
//-------------------------------------------------------------------

void initLib(char titleData[][TITLEL], char albumData[][ALBUML], 
	char artistData[][ARTISTL], char genre[], int rating[])
{
	for (int i = 0; i < 100; i++)
	{
		titleData[i][0] = '\0';
		albumData[i][0] = '\0';
		artistData[i][0] = '\0';
		genre[i] = '\0';
		rating[i] = '\0';
	}
}

//-------------------------------------------------------------------
//	openFile(ifstream&)
//
// Preconditions	:Takes a reference to a ifstream object
//
// Postconditions	:Loads mediaLibrary.txt into the stream for 
// 			 reading. If the opening fails, will report 
// 			 the error to the user
//-------------------------------------------------------------------

bool openFile(ifstream& inStream)
{
	inStream.open(libFile);
	if (inStream.fail())
	{
		cout << "musicLibrary.txt could not be read\n";
		cout << "Please make sure another program is not ";
		cout << "accessing the file,\n";
		cout << "and that it is in the local directory.\n";
		cout << "Opening with empty Media Library" << endl;
		return false;
	}
	return true;
}

//-------------------------------------------------------------------
//	openFile(ofstream&)
//
// Preconditions	:Takes a reference to a ofstream object
//
// Postconditions	:Loads mediaLibrary.txt into the stream for 
// 			 writing. If the opening fails, will report 
// 			 the error to the user
//-------------------------------------------------------------------

void openFile(ofstream& outStream)
{
	outStream.open(libFile);
	if (outStream.fail())
	{
		cout << "musicLibrary.txt could not be read\n";
		cout << "Please make sure another program is not ";
		cout << "accessing the file,\n";
		cout << "and that it is in the local directory.\n";
	}
}

//-------------------------------------------------------------------
//	splashScreen()
//
// Preconditions	:No preconditions
//
// Postconditions	:Prints out the ASCII splash screen
//-------------------------------------------------------------------

void splashScreen()
{
	cout << "           #        #######          \n";
	cout << "           #           #             \n";
	cout << "           #           #             \n";
	cout << "# ### ###  #           #   #  #   #  \n";
	cout << "##   #   # #           #    ##   ### \n";
	cout << "#    #   # #      ##   #    ##    #  \n";
	cout << "#    #   # ###### ##   #   #  #   #  \n";
}

//-------------------------------------------------------------------
//	menuControl(char [][], char[][], char[][], char[], int[], 
//			ofstream&)
//
// Preconditions	:Takes in title, album, artist, genre, and 
//			 rating data, and an ofstream reference
//
// Postconditions	:Calls mainMenu(int) and redirects the
// 			 user to the desired functions untill quit
// 			 is selected.
//-------------------------------------------------------------------

void menuControl(char titleData[][TITLEL], char albumData[][ALBUML], 
	char artistData[][ARTISTL], char genre[], int rating[], 
	ofstream& outStream)
{
	int select = 0;
	bool save;
	
	while (select != 6)
	{
		mainMenu(select);
		
		switch(select)
		{
			case 1:
				addSong(titleData, albumData, 
					artistData, genre, rating);
				break;
			case 2:
				displayLib(titleData, albumData, 
					artistData, genre, rating);
				cout << endl;
				removeSong(titleData, albumData,
					artistData, genre, rating);
				break;
			case 3:
				displayLib(titleData, albumData, 
					artistData, genre, rating);
				break;
			case 4:
				libStats(titleData);
				break;
			case 5:
				writeLib(outStream, titleData,
				albumData, artistData, genre, rating);
				break;
			case 6:
				save = leaving();
				if (save == true)
				{
					writeLib(outStream, titleData,
					albumData, artistData, genre, 
					rating);
				}
				break;
			default:
				cout << "Invalid Selection";
				cout << endl;
				select = 0;
				break;
		}
	}
}

//-------------------------------------------------------------------
//	mainMenu(int&)
//
// Preconditions	:Takes an int arguement to be modified
//
// Postconditions	:Prints out the main menu and prompts for 
// 			 input. Modifies int select based on input
//-------------------------------------------------------------------

void mainMenu(int& select)
{
	char temp;
	cout << endl;
	cout << "mL.Txt Music Library Main Menu\n";
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
	cout << endl;
	cout << "Please make your selection: \n";
	cout << endl;
	cout << "\t1 - Add a new song to the library\n";
	cout << "\t2 - Delete a song\n";
	cout << "\t3 - View library\n";
	cout << "\t4 - Library Stats\n";
	cout << "\t5 - Save\n";
	cout << "\t6 - Quit\n";
	cout << endl;
	
	cout << "Selection : ";
	
	cin >> temp;

	// Check to make sure user did not input anything they were
	// not suposed to
	while ( isalnum(cin.peek()) )
	{
		cout << "Invalid Selection";
		
		while (cin.get() != '\n' )
		{}
		
		return mainMenu(select);
	}
	
	select = temp - 48;

}

//-------------------------------------------------------------------
//	loadLibrary(ifstream&, char[][], char[][], cahr[][], char[], 
//			int[])
//
// Preconditions	:Takes a reference to an ifstream and title, 
//			 album, artist, genre, and rating data
//
// Postconditions	:Loads the data in musicLibrary.txt into 
//			 the represitive arrays
//-------------------------------------------------------------------

void loadLibrary(ifstream& inStream, char titleData[][TITLEL], 
	char albumData[][ALBUML], char artistData[][ARTISTL],
	char genre[], int rating[])
{
	int libSize = 0;
	char temp;
	bool moveOn = false;

	libSizeReturn(libSize, inStream);
	inStream.close();
	openFile(inStream);

	for (int i = 0; ! isspace(temp); i++)
	{
		inStream.get(temp);
	}

	for (int i = 0; i < libSize; i++)
	{
		skipSpaces(inStream);
		inStream.get(titleData[i], MAX_TITLE+1);
		titleData[i][TITLEL -1] = '\0';

		skipSpaces(inStream);
		inStream.get(albumData[i], MAX_ALBUM+1);
		albumData[i][ALBUML -1] = '\0';

		skipSpaces(inStream);
		inStream.get(artistData[i], MAX_ARTIST+1);
		artistData[i][ARTISTL -1] = '\0';
		
		skipSpaces(inStream);
		inStream.get(genre[i]);
		
		skipSpaces(inStream);
		inStream.get(temp);
		rating[i] = temp-48;

	}

	inStream.close();
}

//-------------------------------------------------------------------
//	libSizeReturn(int&, ifstream&)
//
// Preconditions	:Takes a int and ifstream reference
//
// Postconditions	:Modifies the passed int, libSize, to the 
//			 number of songs in musicLibrary.txt
//-------------------------------------------------------------------

void libSizeReturn(int& libSize, ifstream& inStream)
{

	char asciiForm[5];
	int i = 0;

	for (i = 0; ( ! isspace(asciiForm[i]) ) && (i < 5); i++)
	{
		inStream.get(asciiForm[i]);
	}
	
	for (i = 0; asciiForm[i] != '\n' && i < 5; i++)
	{}
	
	switch(i)
	{
		case 3:
			libSize += (asciiForm[i-3]-48)*100;
		case 2:
			libSize += (asciiForm[i-2]-48)*10;
		case 1:
			libSize += (asciiForm[i-1]-48);
			break;
		default:
			libSize = -1;
			cout << "Malformed lib. size" << endl;
	}	
}

//-------------------------------------------------------------------
//	displayLib(char[][], char[][], char[][], char[], int[])
//
// Preconditions	:Takes title, album, artist, genre and rating
//			 data
//
// Postconditions	:Prints out in format the contents of the 
//			 media library from memory
//-------------------------------------------------------------------

void displayLib(const char titleData[][TITLEL], 
	const char albumData[][ALBUML], 
	const char artistData[][ARTISTL], const char genre[], 
	const int rating[])
{

	cout.setf(ios::left);
	cout.width(4);
	cout << "ID";
	cout.width(30);
	cout << "Title";
	cout.width(26);
	cout << "Album";
	cout.width(26);
	cout << "Artist";
	cout.width(15);
	cout << "Genre";
	cout.width(6);
	cout << "Rating";
	cout << endl;
	
	for (int i = 0; i < 120; i++)
	{
		cout << "~";
	}

	cout << endl;

	for (int i = 0; i < MAXLS; i++)
	{
		if (titleData[i][0] != '\0')
		{
			cout.width(4);
			cout << i;
			cout.width(30);
			cout << titleData[i];
			cout.width(26);
			cout << albumData[i];
			cout.width(26);
			cout << artistData[i];
			cout.width(15);
			genreTranslate(genre[i]);
			ratingTranslate(rating[i]);
			cout << endl;
		}
	}

	cout.unsetf(ios::left);

}

//-------------------------------------------------------------------
//	genreTranslate(char)
//
// Preconditions	:Takes a char
//
// Postconditions	:Prints out the genre represented by the char
//			 arguement, if Invalid returns a fall boolean
//-------------------------------------------------------------------

bool genreTranslate(const char genre)
{
	bool goodGenre = true;
	switch (genre)
	{
		case 'a':
			cout << "Alternative";
			break;
		case 'b':
			cout << "Blues";
			break;
		case 'c':
			cout << "Classical";
			break;
		case 'd':
			cout << "Country";
			break;
		case 'e':
			cout << "Hip-Hop";
			break;
		case 'f':
			cout << "Jazz";
			break;
		case 'g':
			cout << "Pop";
			break;
		case 'h':
			cout << "Rap";
			break;
		case 'i':
			cout << "Reggae";
			break;
		case 'j':
			cout << "Religious";
			break;
		case 'k':
			cout << "Rock";
			break;
		case 'l':
			cout << "R&B";
			break;
		case 'm':
			cout << "Techno";
			break;
		case 'n':
			cout << "World";
			break;
		case 'o':
			cout << "Other";
			break;
		default: // Return a false value on Invalid Genre's
			cout << "InvalidGere";
			goodGenre = false;
	}
	return goodGenre;
}

//-------------------------------------------------------------------
//	ratingTranslate(int)
//
// Preconditions	:Takes an int
//
// Postconditions	:Prints out as *'s the rating represented by
//			 the int arguuement
//-------------------------------------------------------------------

void ratingTranslate(const int rating)
{
	switch (rating)
	{
		case 5:
			cout << "*";
		case 4:
			cout << "*";
		case 3:
			cout << "*";
		case 2:
			cout << "*";
		case 1:
			cout << "*";
			break;
	}
}

//-------------------------------------------------------------------
//	addSong(char[][], char[][], char[][], char[], int[])
//
// Preconditions	:Takes title, album, artist, genre and rating
//			 data arrays
//
// Postconditions	:Adds a user inputed song at the closest
//			 location to 0 in memory
//-------------------------------------------------------------------

void addSong(char titleData[][TITLEL], char albumData[][ALBUML], 
	char artistData[][ARTISTL], char genre[], int rating[])
{
	// Find the next Open location in Memory
	int addLocation = findOpen(titleData);
	
	// Check for a full Library
	if (addLocation == -1)
		cout << "Error: Library full, unable to add song\n";
	else
	{
		addTitle(titleData, addLocation);
		addAlbum(albumData, addLocation);
		addArtist(artistData, addLocation);
		addGenre(genre, addLocation);
		addRating(rating, addLocation);

		// Display added data
		cout << endl << "Song was successfully added" << endl;
		displaySongData(titleData, albumData, artistData,
			genre, rating, addLocation);
		cout << endl;
	}	
}

//-------------------------------------------------------------------
//	findOpen(char[][])
//
// Preconditions	:Takes in the titleData char array
//
// Postconditions	:Returns a value of the lowest memory
//			 location available to add a song.  Returns
//			 -1 if no location is available
//-------------------------------------------------------------------

int findOpen(const char titleData[][TITLEL])
{
	int openLocation = -1;
	
	for (int i = 0; openLocation == -1 && i < MAXLS; i++)
	{
		if ( titleData[i][0] == '\0' )
			openLocation = i;
		
	}

	return openLocation;
}

//-------------------------------------------------------------------
//	addTitle(char[][], int)
//
// Preconditions	:Takes titleData array and an int location
//
// Postconditions	:Prompts for and adds a title for a song to 
//			 the library
//-------------------------------------------------------------------

void addTitle(char titleData[][TITLEL], int addLocation)
{
	cout << "Please enter the songs title (max 27) : ";
	getNoError(titleData[addLocation], MAX_TITLE+1);
	
	for ( int i = 0; i < MAX_TITLE; i++ )
	{
		if (titleData[addLocation][i] == '\0')
		{
			titleData[addLocation][i] = ' ';
			titleData[addLocation][i+1] = '\0';
		}
	}

	if (confirm(titleData[addLocation]) == false)
		return addTitle(titleData, addLocation);
	
}

//-------------------------------------------------------------------
//	addAlbum(char[][], int)
//
// Preconditions	:Takes albumData array and an int location
//
// Postconditions	:Prompts for and adds an album for a song to 
//			 the library
//-------------------------------------------------------------------

void addAlbum(char albumData[][ALBUML], int addLocation)
{
	cout << "Please enter the song's album (max 24) : ";
	getNoError(albumData[addLocation], MAX_ALBUM+1);

	for ( int i = 0; i < MAX_ALBUM; i++ )
	{
		if (albumData[addLocation][i] == '\0')
		{
			albumData[addLocation][i] = ' ';
			albumData[addLocation][i+1] = '\0';
		}
	}
	if (confirm(albumData[addLocation]) == false)
		return addAlbum(albumData, addLocation);
}

//-------------------------------------------------------------------
//	addArtist(char[][], int)
//
// Preconditions	:Takes artistData array and an int location
//
// Postconditions	:Prompts for and adds an artist for a song to 
//			 the library
//-------------------------------------------------------------------

void addArtist(char artistData[][ARTISTL], int addLocation)
{
	cout << "Please enter the song's artist (max 24) : ";
	getNoError(artistData[addLocation], MAX_ARTIST+1);
	
	for ( int i = 0; i < MAX_ARTIST; i++ )
	{
		if (artistData[addLocation][i] == '\0')
		{
			artistData[addLocation][i] = ' ';
			artistData[addLocation][i+1] = '\0';
		}
	}
	if (confirm(artistData[addLocation]) == false)
		return addArtist(artistData, addLocation);
}

//-------------------------------------------------------------------
//	addGenre(char[], int)
//
// Preconditions	:Takes genre array and an int location
//
// Postconditions	:Prompts for and adds an genre for a song to 
//			 the library
//-------------------------------------------------------------------

void addGenre(char genre[], int addLocation)
{
	printGenres();
	cout << "Please enter the genre of the song : ";
	getNoError(genre[addLocation]);
	
	genre[addLocation] = tolower(genre[addLocation]);
	
	// Display Genre and check for invalid input
	if ( genreTranslate(genre[addLocation]) == false )
	{
		cout << endl;
		return addGenre(genre, addLocation);
	}
	
	cout << endl << "Is this correct? [y/N] : ";
	if (confirm() == false)
		return addGenre(genre, addLocation);
}

//-------------------------------------------------------------------
//	addRating(int[], int)
//
// Preconditions	:Takes the rating array and an int location
//
// Postconditions	:Prompts for and adds a rating for a song to 
//			 the library
//-------------------------------------------------------------------

void addRating(int rating[], int addLocation)
{
	char temp;
	int newRating = 0;
	
	cout << "Please enter a raing for the song between";
	cout << " 1 and 5 (i.e. 3 = ***) : ";

	// Skip carrage return in the input stream
	cin.get(temp);

	// Check for input other than *'s
	while (temp != '*')
	{
		cout << endl;
		cout << "Error: Malformed rating\n";
		cout << "Please enter a raing for the song between";
		cout << " 1 and 5 (i.e. 3 = ***) : ";
		cin.get(temp);
		cout << endl;
	}
	
	// Extract and store numerical rating
	for ( int i = 0; temp == '*'; i++ )
	{
		if ( temp == '*' )
			newRating++;
		cin.get(temp);
	}

	// Check to make sure rating is less than or equal to 5
	if (newRating > 5)
	{
		cout << endl;
		cout << "Error: Rating out of range" << endl;
		return addRating(rating, addLocation);
	}
	
	rating[addLocation] = newRating;
	
	cout << endl << "You entered : ";
	ratingTranslate(rating[addLocation]);
	cout << endl << "Is this correct? [y/N] : ";
	if (confirm() == false)
		return addRating(rating, addLocation);
}

//-------------------------------------------------------------------
//	removeSong(char[][])
//
// Preconditions	:Takes the char array titleData
//
// Postconditions	:Places a null character at titleData[][0]
//			 in the location of a song specified by input
//-------------------------------------------------------------------

void removeSong(char titleData[][TITLEL],char albumData[][ALBUML],
	char artistData[][ARTISTL],char genre[], int rating[])
{
	char toRemove[TITLEL];
	int removeLocation = -1;

	cout << "Enter the title of the song you would like";
	cout << " to remove : ";

	getNoError(toRemove, MAX_TITLE);
	
	if ( confirm(toRemove) == false )
		return removeSong(titleData, albumData, artistData,
			genre, rating);
	
	// Fill the input array to match stored data	
	for ( int i = 0; i < MAX_TITLE; i++ )
	{
		if (toRemove[i] == '\0')
		{
			toRemove[i] = ' ';
			toRemove[i+1] = '\0';
		}
	}
	
	// Search for array match
	for ( int i = 0; i < MAXLS && removeLocation == -1; i++ )
	{
		if ( strcmp(toRemove, titleData[i]) == 0 )
			removeLocation = i;
	}

	// Return error if song not found
	if (removeLocation == -1)
		cout << "Error: Song not found in Library" << endl;

	// Display song data and prompt for confirmation
	if (removeLocation != -1)
	{
		displaySongData(titleData, albumData, artistData, 
		genre, rating, removeLocation);
		cout << "Please confirm deleation [y/N] : ";
		if (confirm() == false)
			removeLocation = -1;
	}
	
	// If confirmed, Delete song
	if (removeLocation != -1)
	{
		titleData[removeLocation][0] = '\0';
		cout << "Song " << removeLocation << " Removed\n";
	}
	
	else
		cout << "Song Not Removed" << endl;
}

//-------------------------------------------------------------------
//	displaySongData(char[][], char[][], char[][], char[], char[],
//		int[], int)
//
// Preconditions	:Needs the media library arrays and a int
// 			 for the song to be displayed
// 
// Postconditions	:Displays the data of the song indicated by 
// 			 the int "display"
//-------------------------------------------------------------------

void displaySongData(const char titleData[][TITLEL],
	const char albumData[][ALBUML], 
	const char artistData[][ARTISTL],
	const char genre[], 
	const int rating[], const int display)
{
	cout.setf(ios::left);

	cout.width(4);
	cout << "ID";
	cout.width(30);
	cout << "Title";
	cout.width(26);
	cout << "Album";
	cout.width(26);
	cout << "Artist";
	cout.width(15);
	cout << "Genre";
	cout << "Rating";
	cout << endl;
	
	for (int i = 0; i < 120; i++)
	{
		cout << "~";
	}

	cout << endl;
	cout.width(4);
	cout << display;
	cout.width(30);
	cout << titleData[display];
	cout.width(26);
	cout << albumData[display];
	cout.width(26);
	cout << artistData[display];
	cout.width(15);
	genreTranslate(genre[display]);
	ratingTranslate(rating[display]);
	cout << endl;
}

//-------------------------------------------------------------------
//	libStats(char[][])
//
// Preconditions	:Takes a char array titleData
//
// Postconditions	:Prints out formated data about the library
//-------------------------------------------------------------------

void libStats(const char titleData[][TITLEL])
{
	int libSize = 0;

	for ( int i = 0; i < MAXLS; i++)
	{
		if (titleData[i][0] != '\0')
			libSize++;
	}

	cout << endl;
	cout << "LIBRARY STATS\n\n";
	cout << "Songs = " << libSize << endl;
	cout << "Capacity = " << MAXLS << endl;
	cout << "Available = " << (MAXLS - libSize) << endl;
	cout << "Version = 1.0" << endl;
}

//-------------------------------------------------------------------
//	writeLib(ofstream&, char[][], char[][], char[][], char[],
//			int[])
//
// Preconditions	:Takes an ofstream reference, and title,
//			 album, artist, genre, and rating arrays
//
// Postconditions	:Writes to the file "musicLibrary.txt" the 
//			 current contents of memory
//-------------------------------------------------------------------

void writeLib(ofstream& outStream, char titleData[][TITLEL], 
	char albumData[][ALBUML], char artistData[][ARTISTL],
	char genre[], int rating[])
{
	openFile(outStream);
	outStream.setf(ios::left);

	int libSize = 0;

	for ( int i = 0; i < MAXLS; i++)
	{
		if (titleData[i][0] != '\0')
			libSize++;
	}

	outStream << libSize << endl;
	
	for ( int i = 0; i < MAXLS; i++)
	{
		if (titleData[i][0] != '\0')
		{
			outStream.width(30);
			outStream << titleData[i];
			outStream.width(26);
			outStream << albumData[i];
			outStream.width(26);
			outStream << artistData[i];
			outStream.width(5);
			outStream << genre[i];
			outStream << rating[i];
			outStream << "\n";
		}
	}
	outStream.unsetf(ios::left);
	outStream.close();
	cout << "Library saved!" << endl;
}

//-------------------------------------------------------------------
//	leaving()
//
// Preconditions	:None
//
// Postconditions	:Prompts user to save the current media
//			 library before exiting.  Rreturns true/false
//-------------------------------------------------------------------
bool leaving()
{
	char test;
	
	cout << "Would you like to save the library before exiting";
	cout << " [y/N] : ";
	cin >> test;

	if (test == 'y' || test == 'Y')
		return true;
}

//-------------------------------------------------------------------
//	skipSpaces(ifstream&)
// 
// Preconditions	:Takes an ifstream object that is open
//
// Postconditions	:Moves the ifstream pointer to the next 
// 			 non-white space character
//-------------------------------------------------------------------

void skipSpaces(ifstream& inStream)
{
	bool moveOn = false;
	char temp = 'a';
	while ( moveOn != true )
	{
		temp = inStream.peek();
	
		if ( temp == ' ' || temp == '\t' || temp == '\n') 
			inStream.get(temp);
		else
			moveOn = true;
	}
}
	
//-------------------------------------------------------------------
//	getNoError(char[], int)
//
// Preconditions	:Takes a character array and an integer
// 			 representing how many characters to read
// 			 into the array
// 
// Postconditions	:Fills the char[] with the nuber of 
// 			 characters specified by int from cin
//
// NOTES		:This method will safely skip over extra, 
// 			 unwanted \n characters in the beginning and
// 			 end of the cin stream to prevent looping on
// 			 input
//-------------------------------------------------------------------

void getNoError(char input[],int length)
{
    while (cin.peek() == '\n')
    {
	cin.ignore(1);
    }
    
    cin.get(input, length, ' ');

    while (cin.get() != '\n')
    {	}
}

//-------------------------------------------------------------------
//	getNoError(char&)
//
// Preconditions	:Takes a character to read input into
// 
// Postconditions	:Fills the char with data from cin
//
// NOTES		:This method will safely skip over extra, 
// 			 unwanted \n characters in the beginning and
// 			 end of the cin stream to prevent looping on
// 			 input
//-------------------------------------------------------------------

void getNoError(char& input)
{
    while (cin.peek() == '\n')
    {
	cin.ignore(1);
    }
    
    cin.get(input);

    while (cin.get() != '\n')
    {}
}

//-------------------------------------------------------------------
//	confirm(int)
//
// Preconditions	:Takes an int to display for confirmation
//
// Postconditions	:Returns a boolean based on user input 
//-------------------------------------------------------------------

bool confirm(const int input)
{
	char check;
	cout << "You entered : " << input << endl;
	cout << "Is this correct? [y/N] : ";
	getNoError(check);
	cout << endl;
	
	if (check == 'y' || check == 'Y')
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------
//	confirm(char[])
//
// Preconditions	:Takes a char[] to display for confirmation
//
// Postconditions	:Returns a boolean based on user input 
//-------------------------------------------------------------------

bool confirm(const char input[])
{
	char check;
	cout << "You entered : " << input << endl;
	cout << "Is this correct? [y/N] : ";
	getNoError(check);
	cout << endl;
	
	if (check == 'y' || check == 'Y')
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------
//	confirm()
//
// Preconditions	:None
//
// Postconditions	:Returns a boolean based on user input 
//-------------------------------------------------------------------

bool confirm()
{
	char check;
	getNoError(check);
	cout << endl;
	if (check == 'y' || check == 'Y')
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------
//	printGenres()
//
// Preconditions	:None
//
// Postconditions	:Prints out the available genres for the user
//-------------------------------------------------------------------

void printGenres()
{
	cout << "a = Alternative" << endl;
	cout << "b = Blues" << endl;
	cout << "c = Classical" << endl;
	cout << "d = Country" << endl;
	cout << "e = Hip-Hop" << endl;
	cout << "f = Jazz" << endl;
	cout << "g = Pop" << endl;
	cout << "h = Rap" << endl;
	cout << "i = Reggae" << endl;
	cout << "j = Religious" << endl;
	cout << "k = Rock" << endl;
	cout << "l = R&B" << endl;
	cout << "m = Techno" << endl;
	cout << "o = World" << endl;
	cout << "p = Other" << endl;
}
