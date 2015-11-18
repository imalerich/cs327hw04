#include <iostream>
#include <Channel.h>

using namespace std;

int main() {
	Channel c = Channel((size_t)8);
	
	for (long i = 0; i < 100; i++)
		c.push_sample(i);

	cout << c << endl;
}
