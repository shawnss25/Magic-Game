/*******************************************************************************
 *  Program Name: Program5
 *  Created Date: 4/26/19
 *  Created By: Shawn Sewell-Snyder
 *  Purpose: To see if contestants are able to say all of the magic words in their sentence.
 *  Acknowledgements: None
 *******************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "ITreeType.hpp"

using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;

void InsertMagicWords(ifstream& ,ofstream&, TreeType&);
void AddTextPass(string, string, ofstream&, TreeType&);
void ResetMagicTree(ifstream&, TreeType&);

/*******************************************************************************
 *  Function Name: main()
 *  Parameters: None
 *  Return Value: int
 *  Purpose: To run the program.
 *******************************************************************************/
int main() {
    ifstream magicFile; // Input text file with magic words
    ifstream textPassFile; // Input text file with contestants and their sentences
    ofstream oFile; // Output text file that is echo printed
    string line; // Holds the data from each line read in from textPassFile
    string userInfo; // Holds user's name
    string textPass; // Holds user's sentence
    string temp; // Holds the number which determines whether the line contains the contestant's name or sentence
    TreeType MagicTree; // Holds magic words
    int contestantNumber = 1; // Keeps track of the number of contestants
    
    try {
        
        // Open files
        textPassFile.open("TextPass.txt");
        oFile.open("Output.txt");
        
        InsertMagicWords(magicFile, oFile, MagicTree); // Insert magic words into magic tree
        
        // Loop until end of file
        while(!textPassFile.eof()) {
            
            getline(textPassFile, line); // Get content on each line
            
            // Loop through each line of textPassFile
            temp = line.substr(0,1);
            
            // If it's a line with the contestant's name
            if(temp == "1") {
                    
                // Log and write contestant number
                cout << "\n\nContestant Number: " << contestantNumber;
                oFile << "\n\nContestant Number: " << contestantNumber;
                    
                userInfo = line.substr(2, line.length()); // Ignore the number from text file
                    
                // Log and write name of contestant
                cout << "\nName: " << userInfo << endl;
                oFile << "\nName: " << userInfo << endl;
                    
            } else if(temp == "2") { // If it's a line with the sentence
                textPass = line.substr(2, line.length()); // Ignore the number from text file
                    
                AddTextPass(textPass, userInfo, oFile, MagicTree); // Check in each word and see if it's a match with the magic words
                
                // Make tree completely empty and re-insert magic words back into tree for next contestant
                ResetMagicTree(magicFile, MagicTree);
                
                contestantNumber++; // Increase contestant number counter
            }
        }
        // Close files
        textPassFile.close();
        oFile.close();
        
        return 0;
    } catch(std::exception& ex){ std::cerr << "\nSomething went wrong\n"; }
}

/*******************************************************************************
 *  Function Name: InsertMagicWords()
 *  Parameters: ifstream&, ofstream&, TreeType&
 *  Return Value: void
 *  Purpose: To scan in the magic words from the Magic.txt file and put them into the MagicTree
 *******************************************************************************/
void InsertMagicWords(ifstream &magicFile, ofstream &oFile, TreeType &MagicTree) {
    string item; // Holds the word from the text file that is being scanned in
    magicFile.open("Magic.txt"); // Open file
    
    cout << "\nList of magic words:\n---\n";
    oFile << "\nList of magic words:\n---\n";
    
    // Scan in magic words and put them into the magic tree
    while(magicFile >> item) {
        MagicTree.PutItem(item); // Put item into MagicTree
        oFile << item << endl;
        cout << item << endl;
    }
    cout << "---\n";
    oFile << "---\n";
    magicFile.close(); // Close file
}

/*******************************************************************************
 *  Function Name: AddTextPass()
 *  Parameters: string, string, ofstream&, TreeType&
 *  Return Value: void
 *  Purpose: Compare the words in the contestant's sentence to the magic words
 *******************************************************************************/
void AddTextPass(string textPass, string userInfo, ofstream &oFile, TreeType &MagicTree) {
    bool found; // If magic word is found
    bool finished; // If GetNextItem is finished
    string word; // Holds each word of contestant's sentence
    OrderType order = IN_ORDER; // The order in which the MagicTree will return the data
    
    // Display what contestant said
    cout << "\n---\n" << userInfo << " says:\n" << textPass << endl;
    oFile << "\n---\n" << userInfo << " says:\n" << textPass << endl;
    
    // Remove punctuation
    textPass.erase(std::remove(textPass.begin(), textPass.end(), '.'), textPass.end());
    textPass.erase(std::remove(textPass.begin(), textPass.end(), '!'), textPass.end());
    textPass.erase(std::remove(textPass.begin(), textPass.end(), ','), textPass.end());
    textPass.erase(std::remove(textPass.begin(), textPass.end(), '?'), textPass.end());
    textPass.erase(std::remove(textPass.begin(), textPass.end(), '\''), textPass.end());
    textPass.erase(std::remove(textPass.begin(), textPass.end(), ':'), textPass.end());
    textPass.erase(std::remove(textPass.begin(), textPass.end(), ';'), textPass.end());
    
    // Make textPass a stream to read in each word for comparison
    std::stringstream ss(textPass);

    // Loop through each word in the user's sentence
    while(ss >> word) {
     // Check to see if the scanned in word is a magic word
        word = MagicTree.GetItem(word, found);
        
        // If the word is found, delete it from the magic tree
        if(found)
            MagicTree.DeleteItem(word);
        
        // If Magic tree is empty, user wins -> print winning message
        if(MagicTree.IsEmpty()) {
            cout << "\n---\nCongratulations, " << userInfo << ", you have won the prize!\n---\n\n";
            oFile << "\n---\nCongratulations, " << userInfo << ", you have won the prize!\n---\n\n";
            break;
        }
    }
    
    // If the magic tree is not empty, the user loses
    if(!MagicTree.IsEmpty()) {
        cout << "\n\nSorry, " << userInfo << ", you failed to say the following magic words:\n---\n";
        oFile << "\n\nSorry, " << userInfo << ", you failed to say the following magic words:\n---\n";
        
        // Reset Magic Tree and print remaining magic words
        MagicTree.ResetTree(order);
        for(int count = 0; count < MagicTree.GetLength(); count++) {
            word = MagicTree.GetNextItem(order, finished); // Word holds the next item in MagicTree
            cout << word << endl;
            oFile << word << endl;
        }
        cout << "---\n\n";
        oFile << "---\n\n";
    }
}

/*******************************************************************************
 *  Function Name: ResetMagicTree()
 *  Parameters: ifstream&, TreeType&
 *  Return Value: void
 *  Purpose: To reset the MagicTree for the next contestant
 *******************************************************************************/
void ResetMagicTree(ifstream &magicFile, TreeType &MagicTree) {
    string item; // Temporarily holds scanned in string
    
    magicFile.open("Magic.txt"); // Open file
    MagicTree.MakeEmpty(); // Empties tree
    
    // Scan in magic words from file and put them into MagicTree
    while(magicFile >> item)
        MagicTree.PutItem(item);
    
    magicFile.close(); // Close file
}
