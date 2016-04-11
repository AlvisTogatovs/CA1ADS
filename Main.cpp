#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <cstdlib>
#include <string>



#include <vector>
#include <map>
#include <array>




#include <algorithm>
#include <cctype>

using namespace std;

/***************************************************************************************
*    Title: <ROT-13>
*    Author: <RosettaCode.org>
*    Date: <06/04/2016>
*    Code version: <C++>
*    Availability: <http://rosettacode.org/wiki/Rot-13#C.2B.2B>
*
***************************************************************************************/
// the rot13 function
string rot13(string s)
{
	// Initialised strings for the lower and
	// upper letters in the alphabet
	static::string const
		lcalph = "abcdefghijklmnopqrstuvwxyz",
		ucalph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	string result;
	string::size_type pos;

	result.reserve(s.length());
	// iterates through the string file from the start till the end
	for (string::iterator it = s.begin(); it != s.end(); ++it)
	{
		//gets the upper case letters into the result
		if ((pos = lcalph.find(*it)) != string::npos)
			result.push_back(lcalph[(pos + 13) % 26]);
		// gets the lower case letters into the result
		else if ((pos = ucalph.find(*it)) != string::npos)
			result.push_back(ucalph[(pos + 13) % 26]);
		// gets any symbols or spaces
		else
			result.push_back(*it);
	}

	return result;
}

// function to output the rot13 of a file on std::cout
// returns false if an error occurred processing the file, true otherwise
// on entry, the argument is must be open for reading
int rot13_stream(istream& is)
{
	string line;
	while (getline(is, line))
	{
		if (!(cout << rot13(line) << "\n"))
			return false;
	}
	return is.eof();
}




typedef array<pair<char, double>, 26> FreqArray;

/***************************************************************************************
*    Title: <Vignere Cipher>
*    Author: <RosettaCode.org>
*    Date: <12/01/2016>
*    Code version: <C++>
*    Availability: <http://rosettacode.org/wiki/Vigen%C3%A8re_cipher/Cryptanalysis>
*
***************************************************************************************/
class VigenereAnalyser
{
private:
	array<double, 26> targets;
	array<double, 26> sortedTargets;
	FreqArray freq;

	// Update the freqs array
	FreqArray& frequency(const string& input)
	{
		for (char c = 'A'; c <= 'Z'; ++c)
			freq[c - 'A'] = make_pair(c, 0);

		for (size_t i = 0; i < input.size(); ++i)
			freq[input[i] - 'A'].second++;

		return freq;
	}

	double correlation(const string& input)
	{
		double result = 0.0;
		frequency(input);

		sort(freq.begin(), freq.end(), [](pair<char, double> u, pair<char, double> v)->bool
		{ return u.second < v.second; });

		for (size_t i = 0; i < 26; ++i)
			result += freq[i].second * sortedTargets[i];

		return result;
	}

public:
	VigenereAnalyser(const array<double, 26>& targetFreqs)
	{
		targets = targetFreqs;
		sortedTargets = targets;
		sort(sortedTargets.begin(), sortedTargets.end());
	}

	pair<string, string> analyze(string input)
	{
		string cleaned;

		for (size_t i = 0; i < input.size(); ++i)
		{
			if (input[i] >= 'A' && input[i] <= 'Z')
				cleaned += input[i];
			else if (input[i] >= 'a' && input[i] <= 'z')
				cleaned += input[i] + 'A' - 'a';

		}

		size_t bestLength = 0;
		double bestCorr = -100.0;

		// Assume that if there are less than 20 characters
		// per column, the key's too long to guess
		for (size_t i = 2; i < cleaned.size() / 20; ++i)
		{
			vector<string> pieces(i);
			for (size_t j = 0; j < cleaned.size(); ++j)
				pieces[j % i] += cleaned[j];

			// The correlation increases artificially for smaller
			// pieces/longer keys, so weigh against them a little
			double corr = -0.5*i;
			for (size_t j = 0; j < i; ++j)
				corr += correlation(pieces[j]);

			if (corr > bestCorr)
			{
				bestLength = i;
				bestCorr = corr;
			}
		}

		if (bestLength == 0)
			return make_pair("Text is too short to analyze", "");

		vector<string> pieces(bestLength);
		for (size_t i = 0; i < cleaned.size(); ++i)
			pieces[i % bestLength] += cleaned[i];

		vector<FreqArray> freqs;
		for (size_t i = 0; i < bestLength; ++i)
			freqs.push_back(frequency(pieces[i]));

		string key = "";
		for (size_t i = 0; i < bestLength; ++i)
		{
			sort(freqs[i].begin(), freqs[i].end(), [](pair<char, double> u, pair<char, double> v)->bool
			{ return u.second > v.second; });

			size_t m = 0;
			double mCorr = 0.0;
			for (size_t j = 0; j < 26; ++j)
			{
				double corr = 0.0;
				char c = 'A' + j;
				for (size_t k = 0; k < 26; ++k)
				{
					int d = (freqs[i][k].first - c + 26) % 26;
					corr += freqs[i][k].second * targets[d];
				}

				if (corr > mCorr)
				{
					m = j;
					mCorr = corr;
				}
			}

			key += m + 'A';
		}

		string result = "";
		for (size_t i = 0; i < cleaned.size(); ++i)
			result += (cleaned[i] - key[i % key.length()] + 26) % 26 + 'A';

		return make_pair(result, key);
	}
};


// the main program
int main(int argc, char* argv[])
{
	cout << "Welcome To The Encryption Machine!" << endl;

	string isRight;

	char fileName2[50];
	char fileName[50];



	ifstream myFile(fileName);
	ifstream myFile2(fileName2);

	string output2;

	cout << "\t\nPlease enter the file you wish to open" << endl;
	cin.getline(fileName, 50);

	myFile.open(fileName);


	if (!myFile)
	{
		
		cout << " Im sorry that file does not exist" << endl;
	
	}



	/***************************************************************************************
	*    Title: <reading a whole file into a string with ifstream>
	*    Author: <SiCrane>
	*    Date: <21/10/2005>
	*    Code version: <C++>
	*    Availability: <http://www.gamedev.net/topic/353162-reading-a-whole-file-into-a-string-with-ifstream>
	*
	***************************************************************************************/


	cout << "\t\nPlease enter the encrypted file you wish to open" << endl;
	cin.getline(fileName2, 100);

	myFile2.open(fileName2);
	if (!myFile2)
	{
		
		cout << " Im sorry that file does not exist" << endl;
		system("pause");
		return 0;

	}

	string output((istreambuf_iterator<char>(myFile2)), istreambuf_iterator<char>());
	cout << "\n-------------------------------------------\n";
	cout << "\n_The File_" << endl;
	cout << "" << endl;
	cout << output;
	cout << "\n-------------------------------------------\n";


	array<double, 26> english = {
		0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228,
		0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025,
		0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987,
		0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
		0.01974, 0.00074 };

	VigenereAnalyser va(english);
	pair<string, string> result = va.analyze(output);

	cout << "Your Key is : " << result.second << endl << endl;
	cout << "_The Decrypted File_ " << endl;
	cout << result.first << endl;


	system("pause");
	return 0;

}



