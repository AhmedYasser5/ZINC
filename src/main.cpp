#include <iostream>

using namespace std;

int main(int argc, char **argv) {
  if (argc < 2) {
    cout << "Wrong number of arguments\n";
    cout << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }
  return 0;
}
