#include "test.h"
#include "scanner.h"

using namespace std;

test::test()
{
	cout << "hello world" << endl;
}

int main(int argc, char *argv[]) 
{
	test t;
    scanner s;
    s.run();
	return 0;
}
