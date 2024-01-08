#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
const int ERROR_CHANCE=10;
void randomTable(int rows = 500, int maxItems = 10, bool randomError = false) {
	vector<string> myVec {"This", "is", "a", "test", "blue", "green", "red", "one", "two", "three"};
	vector<char> delim {' ', '\n', ',', '\t', ';'};
	char quote = '"';
	vector<string> PIDs;
	vector<string> Responses;
	for(int i=0; i<rows; i++) {
		string resp;
		char deli = delim[rand() % delim.size()];
		bool found = true;
		while(found) {
			string str = to_string(rand() % 10000);
			found = false;
			for(string pid : PIDs) {
				if(str == pid) {
					found = true;
					break;
				}
			}
			if(!found) {
				PIDs.push_back(str);
			}
		}
		int numItems = rand() % (maxItems + 1);
		for(int i_2=0; i_2<numItems; i_2++) {
			string item = myVec[rand() % myVec.size()];	
			if(randomError && rand() % 100 < ERROR_CHANCE) {
				char ch = item[rand() % item.size()];
				item[rand() % item.size()] = ch;
			}
			resp += item + deli;
		}
		if(!resp.empty())
			resp.pop_back();
		Responses.push_back(resp);
	}
	ofstream outFile("RandomWords.csv");
	outFile << "PIDs" << ';' << "Responses" << '\n';
	for(int i=0; i<rows; i++) {
		outFile << PIDs[i] << ';' << quote << Responses[i] << quote << '\n';
	}
	outFile.close();
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
	randomTable();
	return 0;
}

