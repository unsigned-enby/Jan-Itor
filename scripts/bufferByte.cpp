#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
using namespace std;
char* fileToBuffer(string file, int &fileSize) {
   char* retBuf = nullptr;
   ifstream inFile(file, std::ios::binary);
   if(!inFile) {
      cout << "Error, problem opening file: " << file << '\n';
      return nullptr;
   }
   inFile.seekg(0, std::ios::end);
   fileSize = inFile.tellg();
   retBuf = new char[fileSize];
   inFile.seekg(0, std::ios::beg);
   inFile.read(retBuf, fileSize); 
   return retBuf;
}
int main(int argc, char* argv[]) { 
   int fileSize = 0;
   char* buffer = fileToBuffer(argv[1], fileSize);
   int x = 0;
   while(x != -1) {
      cout << "x: ";
      cin >> x;
      if(x >= fileSize) {
         cout << "The requested byte is past the end of the file!\n";
      } else {
         if(isprint(buffer[x])) 
            cout << "buffer[" << x << "] = " << buffer[x] << '\n';
         else
            cout << "buffer[" << x << "] = " << (int)buffer[x] << '\n';
      }
   }
   return 0;
}


