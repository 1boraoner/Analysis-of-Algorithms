//#include <iostream>
//#include <fstream>
//#include <algorithm>
//#include <vector>
//#include <cstdlib>
//#include <cmath>
//#include<string>
//#include <ctime>
//
//using namespace std;
//
//struct Taxi {
//	int taxi_number;
//	double distance;
//}typedef Taxi;
//
//void print(vector<Taxi> vec) {
//
//	for (int i = 0; i < vec.size(); i++) {
//		cout << "Taxi# "<< vec[i].taxi_number << ", Distance= " << vec[i].distance << "  " << endl;
//	}
//}
//
//void minify(vector<Taxi>&arr,int index) { // x ==> heap index number, yani addlerken-> arr.size(), decrease key --> taxi_index + 1, extract_min-> 
//
//	int parent_index = index / 2;
//
//	if (parent_index - 1  >= 0) {
//
//		if (arr[index - 1].distance < arr[parent_index - 1].distance) {
//
//			swap(arr[index - 1], arr[parent_index - 1]);
//			minify(arr, parent_index);
//		}
//	}
//}
//
//void fix_minheap(vector<Taxi>&arr,int parent) { //heap number x -> parent itself comapre with child 2*i 2*i+1
//
//	int smallest = parent;
//	int left_child = parent * 2;
//	int right_child = parent * 2 + 1;
//
//	if (right_child <= arr.size() || left_child <= arr.size()) {
//
//		if (right_child <= arr.size() && (arr[left_child - 1].distance > arr[right_child - 1].distance)) { 
//			smallest = right_child;
//		}
//		else {
//			smallest = left_child;
//		}
//
//		if (arr[parent - 1].distance > arr[smallest - 1].distance) {
//			swap(arr[parent - 1], arr[smallest - 1]);
//			fix_minheap(arr, smallest);
//		}
//	}
//}
//
//
//Taxi extract_min(vector<Taxi>& arr) {
//
//	Taxi min_dist_taxi = arr[0];
//	arr[0] = arr[arr.size() - 1];
//	arr.erase(arr.begin() + arr.size() - 1);
//	fix_minheap(arr, 1);
//	return min_dist_taxi;
//}
//
//void key_update(vector<Taxi>& arr, int taxi_pos) {
//	arr[taxi_pos].distance -= 0.01;
//	if (arr[taxi_pos].distance <= 0) {
//		arr[taxi_pos].distance = 0;
//	}
//	minify(arr, taxi_pos+1);
//}
//
//
//int main(/*int argc, char* argv[]*/)
//{
//	/*
//	if (argc != 3) {
//		cout<<"Not Enough Arguments"<<endl;
//		return(0);
//	}
//	
//
//	double p = atof(argv[1]);
//	int m = atoi(argv[2]);
//	*/
//	double p = 0;
//	int m = 100;
//
//	ifstream fp;
//	fp.open("locations.txt");
//	
//	vector<Taxi> priority_queue;
//	double lon = 0;
//	double lat = 0;
//
//	srand(time(NULL));
//
//	string first_line;
//	getline(fp, first_line);
//	
//	int counter = 1;
//	
//	int nupdate = 0;
//	int naddition = 0;
//	int nextract = 0;
//	
//	clock_t start_time = clock();  //start time assigned
//	for (int i = 1; i <= m; i++) {
//
//		if(i % 5 == 0){
//			
//			nextract++;
//		
//			Taxi nearest_taxi = extract_min(priority_queue);
//			cout <<"The Nearest Taxi = Taxi#" << nearest_taxi.taxi_number << " is called and with the smallest distance of " << nearest_taxi.distance  <<endl;
//		}
//		else {
//
//			double rand_op = (double) rand() / RAND_MAX;
//			if (rand_op > p || i==1) {
//
//				naddition++;
//
//				fp >> lon;
//				fp >> lat;
//				double dist = sqrt(pow((33.40819 - lon), 2) + pow((39.19001 - lat), 2));
//
//				Taxi ntaxi = { counter,dist };
//				priority_queue.push_back(ntaxi);
//				minify(priority_queue, priority_queue.size());
//
//				counter++;
//			}
//			else {
//				try {
//					nupdate++;
//					int rand_index = rand() % priority_queue.size(); //random index between (0-vec.size()-1)
//					key_update(priority_queue, rand_index);
//				}catch (string Ex) {
//					cout << "Zero Divison" << endl;
//				}
//
//			}
//		}
//	}
//
//	clock_t end_time = clock()- start_time;
//	double elapsed_time = (double) (end_time / CLOCKS_PER_SEC) *1000; //time difference(in terms of clocks)
//	cout << "Total time elapsed is = " <<elapsed_time<< endl; //elapsed time is divided by CLOCKS_PER_SEC becuase clock() returns time in terms of clocks
//	cout << "Total update operation = " << nupdate << "  Total addition operation  = " << naddition<<endl;
//	cout << "Total extract operation = " << nextract << endl;
//
//	//print(priority_queue);
//	return 0;
//}
//
