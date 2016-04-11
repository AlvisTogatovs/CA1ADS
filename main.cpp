#include <iostream>
#include <string>
#include "HuffmanCode.h"
#include <fstream>
#include <iomanip>



using namespace std;

int main() {
	int total[26] = { 0 };

	ifstream infile("TextCount.txt");
	if (!infile)
	{
		cout << "Error opening input file" << endl;
		return 0;
	}
	string str((istreambuf_iterator<char>(infile)),istreambuf_iterator<char>());//conversion of filename

/***************************************************************************************
*    Title: parsing ifstream to string
*    Author: SiCrane
*    Date: 2005
*    Code version: n/a
*    Availability:http://www.gamedev.net/topic/353162-reading-a-whole-file-into-a-string-with-ifstream/
*
***************************************************************************************/

	
		HuffmanCode* test = new HuffmanCode(str);
		string b;
	
		test->displayTable();
		test->displayHuffmanTable();
	
		string code = test->getEncodedString();
		cout << "Encoded string: " << code << endl;
		cout << "Decoded string: " << test->decodeString(code) << endl;

		



		b = test->decodeString(code);
		
		std::ofstream out("output.txt");
		out << b;
		out.close();
		delete test;
		cin.get();
		return 0;
}
/***************************************************************************************
*    Title: An In-Depth Look At Huffman Encoding
*    Author:posted by KYA
*    Date: jan.2011
*    Code version: n/a
*    Availability: http://www.dreamincode.net/forums/blog/324/entry-3150-an-in-depth-look-at-huffman-encoding/
*
***************************************************************************************/
