#include <iostream>
#include <vector>

using namespace std;

class Solution {
public:
	vector<int> singleNumber(vector<int>& nums) {
		
		int xy = 0;
		for(int n: nums) xy ^= n;
		xy &= -xy;
		vector<int> result = {0, 0};
		for(int n: nums){
			if(xy & n) result[0] ^= n;
			else result[1] ^= n;
		}
		return result;
		
	}
};

int main(){
	vector<int> v1 = {1,2,1,3,2,5};
	Solution s;
	auto result = s.singleNumber(v1);
	
	cout << "Output..." << endl;
	
	for (int i: result){
		cout << i << endl;
	}
	
	return 0;
}