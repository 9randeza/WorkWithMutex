#include <mutex>
#include <queue>
#include <thread>
#include <iostream>

class threadSafeQ{
	std::queue<int> rawQ;
	std::mutex m;
	public:
		void print(){
			m.lock();
			std::cout << "queue elments: \n";
			while(!rawQ.empty()){
				std::cout << rawQ.front() << " ";
				rawQ.pop();
			}
			std::cout << "\n";
			m.unlock();
		}
		
		threadSafeQ(std::queue<int> rawQ){
			this -> rawQ = rawQ;
		}
		
		int& retrieveAndDelete(){
			int front_value = 0;
			m.lock();
			if(!rawQ.empty()){
				front_value = rawQ.front();
				rawQ.pop();
			}
			m.unlock();
			return front_value;
		}
		void push(int val){
			m.lock();
			rawQ.push(val);
			m.unlock();
		}
		
		
};


int main(){
	std::queue<int> q1;
	for(int i = 0; i < 5; i++){
		q1.push(i);
	}

	threadSafeQ Q1(q1);
	
	std::thread pusht(&threadSafeQ::retrieveAndDelete, &Q1);
	std::thread popt(&threadSafeQ::push, &Q1, 3);
	
	if(pusht.joinable()){
		pusht.join();
	}
	if(popt.joinable()){
		popt.join();
	}
	Q1.print();
}
