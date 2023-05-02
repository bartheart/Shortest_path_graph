/***********************************************************
  Member  : Bemnet Merkebu
  Synopsis: read from city.name and flight.txt
            store the graph in a adjacency matrix
***********************************************************/

#include <iostream>
#include <vector>
#include <ctime> 
#include <cstdlib>
#include <string>
#include <fstream>
#include <map>
#include <iomanip>
#include <climits>
#include <queue>

#include "minheap_pair.h"

using namespace std;

class Graph{
     public:
          Graph(int d);
          ~Graph(){};
	  int get(){return n;}
	  int (* getG())[140]{return adj;}
	  
          void print();
          void addEdge(int node1, int node2){ adj[node1][node2] = 1; };
     private: 
          int n;
          int adj[140][140]; // You may want to use adjacent list implementation for sparse graph instead
};

Graph::Graph(int d){
    for (int i=0; i < d; i++){
        for (int j=0; j < d; j++){
            adj[i][j] = 0;
        }
    }
    n = d;
};

void Graph::print(){
    for (int i=0; i < n; i++){
        cout << i << " -> ";
        for (int j=0; j < n; j++){
            if (adj[i][j]){
                cout << j << " ";
            }
        }
        cout << endl;
    }

};

void routeSearch(Graph graph);
void routeSearch_1(Graph graph, int city_A, int city_B, int num_connection);
void routeSearch_2(Graph graph, int city_A, int city_B, int city_C, int city_D);
void routeSearch_3(Graph graph, int city_A, int city_B, int city_C);
void Dijkstra(Graph graph, int s, int d[], int p[]);


/*to map a city name with an unique integer */
//map<const string, int, strCmp> city;
map<const string, int> city;

int main(int argc, char *argv[]){
   int i,j, node1, node2,n;
   string line;


   //cout << "Please enter the number of cities in your graph: " << endl;
   //cout << "---------------------------------------------------" << endl;
   n = 140;
   //cin >> n;
   Graph graph(n);
 
   char lineChar[256];

   ifstream cityFile;
   cityFile.open("city.name", ios::in);

   /*map a city name with an unique integer */
   for (i = 0; i < n; i++){
      std::getline(cityFile, line);
      line.erase(std::prev(line.end()));
      city[line] = i;
   }

   cityFile.close();

   ifstream flightFile;
   flightFile.open("flight.txt", ios::in);
  
   while (flightFile.getline(lineChar, 256)){
      // if line constains From: 
      line = lineChar;
      if (line.find("From:", 0) == 0 ){
          line.erase(0,7);
	  //printf("%s\n", line.c_str());
          if(city.find(line) != city.end()){
	  	//cout << "locate!" << line <<  line.length() << endl;
	  }
	  else{
	  	//cout << "NO!" << line <<  line.length() << endl;
		continue;
	  }
          node1 = city[line];
      } else {
	  line.erase(0,7);

          node2 = city[line];
          graph.addEdge(node1,node2);
      }
   }
   flightFile.close();
   
   /* print the graph */
   cout << endl << "The graph generated can be represented by the following adjacent matrix : " << endl;
   cout << "-----------------------------------------------------------------------------------" << endl;
   graph.print();

   /*print the map representation of cities*/
   map<const string, int>::iterator it;  
   for(it = city.begin(); it != city.end(); it++){
	cout << it->first << ": " << it->second << endl; 
   }

   routeSearch(graph);
}

void routeSearch(Graph graph){
	int choice;
	cout << "Please choose the type of the questions: " << endl;
	cout << "1: From city 'A' to city 'B' with less than x connestions?" << endl;
	cout << "2: Route with the samllest number of connections from city 'A' to city 'D' through city 'B' and 'C'?" << endl;	
	cout << "3: Find intermediate city, I, makes the total number of connections (A to I, B to I and C to I) the smallest." << endl;	
	cin >> choice;

	int city_A, city_B, city_C, city_D;
	int num_connection;

	switch (choice){
		case 1:
			cout << "Please enter the city A: ";
			cin >> city_A;

			cout << "Please enter the city B: ";
			cin >> city_B;

			cout << "number of connections: ";
			cin >> num_connection;

			routeSearch_1(graph, city_A, city_B, num_connection);
			break;	
		case 2:
			cout << "Please enter the city A: ";
			cin >> city_A;

			cout << "Please enter the city D: ";
			cin >> city_D;

			cout << "Please enter the city B: ";
			cin >> city_B;

			cout << "Please enter the city C: ";
			cin >> city_C;

			routeSearch_2(graph, city_A, city_B, city_C, city_D);
			break;	
		
		case 3:
			cout << "Please enter the city A: ";
			cin >> city_A;

			cout << "Please enter the city B: ";
			cin >> city_B;

			cout << "Please enter the city C: ";
			cin >> city_C;

			routeSearch_3(graph, city_A, city_B, city_C);
			break;
		
		default:
			cout << "incorrect choice" << endl;	
	}


}
//Dijkstra algorithm for finding the SSSP
void Dijkstra(Graph graph, int s, int d[], int p[]) {
	int n = graph.get();
	int (*mat)[140] = graph.getG();
	for(int i = 0; i < graph.get(); i++){
		d[i] = INT_MAX;
		p[i] = -1;	
	}
	d[s] = 0;
	//create a minheap
	MinPriorityQueue pq;
	for(int i = 0; i < n; i++){
		pq.insert(d[i], i);
	}
	while(!pq.is_empty()){
		int u = pq.extract_min();
		//Graph is unconnected
		if (d[u] == INT_MAX)
			break;
		for(int i = 0; i < n; i++){
			if(mat[u][i]){
				if(d[i] > d[u] + 1){
					d[i] = d[u] + 1;
					pq.decrease_key(i, d[i]);
					p[i] = u;
				}
			}
		}
	}		
}
//Task 1
void routeSearch_1(Graph graph, int city_A, int city_B, int num_connection) {
	int n = graph.get();
	int d[n], p[n];
	vector<int> ans;
	Dijkstra(graph, city_A, d, p);
	if(d[city_B] <= num_connection){ //there are less than x connections
		int cur_city = city_B;
		int i;
		while(cur_city != -1) {
			ans.insert(ans.begin(), cur_city);
			cur_city = p[cur_city];
		}
		for(i = 0; i < ans.size(); i++){
			if(i != ans.size() - 1){
				cout << "city" << ans[i] << " to ";
			}
			else{
				cout << "city" << ans[i];
			}
		}
		cout << endl;
		cout << "Total connection: " << i-1 << endl;
	}
	else{
		//can not fly from A to B with less than x connections
		cout << "No such route." << endl; 
	}
}

//Task 2
void routeSearch_2(Graph graph, int city_A, int city_B, int city_C, int city_D) {
    int n = graph.get();
    // initialize distance and parent variables
    int d[n], p[n];
    // initialize result path vector
    vector<int> pathAB, pathBC, pathCD;

    // calculate shortest path from city A to city B
    Dijkstra(graph, city_A, d, p);
    if (d[city_B] == INT_MAX) { // no path from A to B
        cout << "No such route." << endl;
        return;
    }
    int cur_city = city_B;
    while (cur_city != -1) {
        pathAB.insert(pathAB.begin(), cur_city);
        cur_city = p[cur_city];
    }

    // calculate shortest path from city B to city C
    Dijkstra(graph, city_B, d, p);
    if (d[city_C] == INT_MAX) { // no path from B to C
        cout << "No such route." << endl;
        return;
    }
    cur_city = city_C;
    while (cur_city != -1) {
        pathBC.insert(pathBC.begin(), cur_city);
        cur_city = p[cur_city];
    }

    // calculate shortest path from city C to city D
    Dijkstra(graph, city_C, d, p);
    if (d[city_D] == INT_MAX) { // no path from C to D
        cout << "No such route." << endl;
        return;
    }
    cur_city = city_D;
    while (cur_city != -1) {
        pathCD.insert(pathCD.begin(), cur_city);
        cur_city = p[cur_city];
    }

    //check if there is a route from A to D via B and C
    if (pathAB.size() > 0 && pathBC.size() > 0 && pathCD.size() > 0) {
        //combine the paths AB, BC, and CD into one path
        vector<int> path;
        path.insert(path.end(), pathAB.begin(), pathAB.end());
        path.insert(path.end(), pathBC.begin() + 1, pathBC.end());
        path.insert(path.end(), pathCD.begin() + 1, pathCD.end());

        //remove any duplicate nodes
        for (int i = 1; i < path.size() - 1; i++) {
            if (path[i] == path[i - 1]) {
                path.erase(path.begin() + i);
                i--;
            }
        }

        //print the path
        for (int i = 0; i < path.size(); i++) {
            if (i != path.size() - 1) {
                cout << "city" << path[i] << " to ";
            } else {
                cout << "city" << path[i];
            }
        }
        cout << endl;
        cout << "Total connection: " << path.size() - 1 << endl;
    } else {
        cout << "No such route." << endl;
    }
}


//Task 3
void routeSearch_3(Graph graph, int city_A, int city_B, int city_C) {
    int n = graph.get();
    int d_A[n], d_B[n], d_C[n];
    int p_A[n], p_B[n], p_C[n];
    vector<int> meeting_cities;
    
    // Calculate shortest paths from A, B, and C to all other cities
    Dijkstra(graph, city_A, d_A, p_A);
    if (d_A[city_B] == INT_MAX || d_A[city_C] == INT_MAX) {
        cout << "No such a city."<< endl;
        return;
    }
    
    Dijkstra(graph, city_B, d_B, p_B);
    if (d_B[city_A] == INT_MAX || d_B[city_C] == INT_MAX) {
        cout << "No such a city." << endl;
        return;
    }
    
    Dijkstra(graph, city_C, d_C, p_C);
    if (d_C[city_A] == INT_MAX || d_C[city_B] == INT_MAX) {
        cout << "No such a city."<< endl;
        return;
    }
    
    // Find the meeting city that minimizes the total number of connections among the three friends
    int min_connections = INT_MAX;
    int meeting_city = -1;
    for (int i = 0; i < n; i++) {
        if (i == city_A || i == city_B || i == city_C) {
            continue; // skip cities A, B, and C
        }
        int connections = d_A[i] + d_B[i] + d_C[i];
        if (connections < min_connections) {
            min_connections = connections;
            meeting_city = i;
        }
    }
    
    // Print results
    if (meeting_city == -1 ) {
        cout << "No such city exists." << endl;
    } else {
        cout << "You three should meet at city: " << meeting_city << endl;
		
        // Print routes for each friend to the meeting city
        vector<int> path_A, path_B, path_C;
		
        int cur_city = meeting_city;
		
        // Get path for friend A
        while (cur_city != city_A && cur_city != -1) {
            path_A.insert(path_A.begin(), cur_city);
            cur_city = p_A[cur_city];
        }
        if (cur_city == -1) {
            cout << "No such city." << endl;
        }
        else {
            path_A.insert(path_A.begin(), city_A);
            cout << "Route for the first person: ";
            for (int i = 0; i < path_A.size(); i++) {
                if (i > 0) {
                    cout << " to ";
                }
                cout << "city" << path_A[i];
            }
            cout << endl;
            //print the total number of connections
            cout << "Total connections: " << path_A.size() -1<< endl;
        }

        cur_city = meeting_city;
		
        // Get path for friend B
        while (cur_city != city_B && cur_city != -1) {
            path_B.insert(path_B.begin(), cur_city);
            cur_city = p_B[cur_city];
        }
        if (cur_city == -1) {
            cout << "No such city exists." << endl;
        }
        else {
            path_B.insert(path_B.begin(), city_B);
            cout << "Route for the second person: ";
            for (int i = 0; i < path_B.size(); i++) {
                if (i > 0) {
                    cout << " to ";
                }
                cout << "city" << path_B[i];
            }
            cout << endl;
            //print the total number of connections
            cout << "Total connections: " << path_B.size() -1<< endl;
        }
        
        cur_city = meeting_city;
		
        // Get path for friend C
        while (cur_city != city_C && cur_city != -1) {
            path_C.insert(path_C.begin(), cur_city);
            cur_city = p_C[cur_city];
        }
        if (cur_city == -1) {
            cout << "No such city exists." << endl;
        }
        else {
            path_C.insert(path_C.begin(), city_C);
            cout << "Route for the third person: ";
            for (int i = 0; i < path_C.size(); i++) {
                if (i > 0) {
                    cout << " to ";
                }
                cout << "city" << path_C[i];
            }
            cout << endl;
            //print the total number of connections
            cout << "Total connections: " << path_C.size() -1<< endl;
        }
    }
}
