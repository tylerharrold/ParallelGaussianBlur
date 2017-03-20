#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

void func(){
	long long number = 0;

	for(long long i = 0 ; i < 2000000 ; i++){
		number+=5;
	}
}
 
int main(){
	high_resolution_clock::time_point start = high_resolution_clock::now();
	func();
	high_resolution_clock::time_point end = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(end - start).count();

	cout << duration;
	return 0;
}
