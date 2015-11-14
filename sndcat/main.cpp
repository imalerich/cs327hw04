#include <iostream>
#include <Channel.h>

using namespace std;

int main() {
	Channel c = Channel(8);
	
	for (auto i = 0; i < 100; i++)
		c.push_sample(i);

	cout << c << endl;
}
