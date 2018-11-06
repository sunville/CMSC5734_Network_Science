#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class graph
{
public:
	graph(){}
	graph(int nodes){
		this->nodes = nodes;
		matrix.reserve(nodes);
		for (int i = 0; i < nodes; ++i)
		{
			vector<int> row(nodes,0);
			this->matrix.push_back(row);
		}
	}
	void add_edge(int u, int v){
		this->matrix[u][v] = 1;
		this->matrix[v][u] = 1;
	}
	~graph(){}

	void get_degree(vector<int>& degree){
		for(int i=0; i<this->nodes; i++){
			if(this->matrix[i][i] == 1)
				degree[i] += 1;
			for(int j=0; j<this->matrix[i].size(); j++){
				degree[i] += matrix[i][j];
			}
		}
	}

	int get_edges(){
		int edges = 0;
		for(int i=0; i<this->nodes; i++){
			for(int j=i; j<this->nodes; j++){
				edges += this->matrix[i][j];
			}
		}
		return edges;
	}

	int get_nodes(){
		return this->nodes;
	}

	vector<vector<int> > get_graph(){
		return this->matrix;
	}

	void printMatrix(vector<vector<int> >& g){
		for(int i=0; i<this->nodes; i++){
			for(int j=0; j<this->nodes;j++)
				cout<<g[i][j]<<" ";
			cout<<"\n";
		}
	}

	void multiply(vector<vector<int> >& g){
		vector<vector<int> > result;
		result.reserve(this->nodes);
		for(int i=0; i<this->nodes; i++){
			vector<int> row(nodes,0);
			result.push_back(row);
		}
		
		for(int i=0; i<this->nodes; i++){
			for(int j=0; j<this->nodes; j++){
				for(int k=0; k<this->nodes; k++){
					result[i][j] += g[i][k] * this->matrix[k][j];
				}
			}
		}

		for(int i=0; i<this->nodes; i++){
			for(int j=0; j<this->nodes;j++)
				g[i][j] = result[i][j];
		}
	}

	void multiply(vector<vector<int> >& g, vector<vector<int> >& h){
		vector<vector<int> > result;
		result.reserve(this->nodes);
		for(int i=0; i<this->nodes; i++){
			vector<int> row(nodes,0);
			result.push_back(row);
		}
		
		for(int i=0; i<this->nodes; i++){
			for(int j=0; j<this->nodes; j++){
				for(int k=0; k<this->nodes; k++){
					result[i][j] += g[i][k] * h[k][j];
				}
			}
		}

		for(int i=0; i<this->nodes; i++){
			for(int j=0; j<this->nodes;j++)
				g[i][j] = result[i][j];
		}
	}

	void minus_d(vector<vector<int> >& g){
		for(int i=0; i<g.size(); i++){
			g[i][i] = 0;
		}
	}

	int nodes;
	vector<vector<int> > matrix;
};


int main(){
	int n;//matrix size
	ifstream file;
	file.open("graph.txt");
	file>>n;
	graph G(n);
	int u,v;
	while(file>>u>>v){
		G.add_edge(u,v);
	}
	file.close();
	//degree vector
	vector<int> degree(n,0);
	G.get_degree(degree);
	cout<<"Degree vector: [ ";
	for (int i = 0; i < degree.size(); ++i)
	{
		cout<<degree[i]<<" ";
	}
	cout<<"]"<<"\n";
	//count total number of edges in graph
	int edges = G.get_edges();
	cout<<"Total edges: "<<edges<<"\n";
	//compute average degree of graph
	cout<<"Average degree: "<<2.0*edges/G.get_nodes()<<"\n";
	//compute density
	cout<<"Density: "<<2.0*edges/(G.get_nodes()*(G.get_nodes()-1))<<"\n";
	//compute number of paths of length 1 to 5 between node i and j
	//output 5 square matrix
	//g1
	vector<vector<int> > g1 = G.get_graph();
	vector<vector<int> > l1 = G.get_graph();
	vector<vector<int> > tmp = G.get_graph();
	G.minus_d(l1);
	G.minus_d(tmp);
	G.multiply(l1,tmp);
	cout<<"Number of paths length=1:\n";
	G.printMatrix(g1);
	//g2
	G.minus_d(l1);//l2
	G.multiply(l1,tmp);
	G.multiply(g1);
	cout<<"Number of paths length=2:\n";
	G.printMatrix(g1);
	//g3
	G.minus_d(l1);//l3
	G.multiply(l1,tmp);
	G.multiply(g1);
	cout<<"Number of paths length=3:\n";
	G.printMatrix(g1);
	//g4
	G.multiply(g1);
	G.minus_d(l1);//l4
	G.multiply(l1,tmp);
	cout<<"Number of paths length=4:\n";
	G.printMatrix(g1);
	//g5
	G.multiply(g1);
	G.minus_d(l1);//l5
	G.multiply(l1,tmp);
	cout<<"Number of paths length=5:\n";
	G.printMatrix(g1);
	//Compute the number of self-loop path with length 6
	//l6 g6
	G.minus_d(l1);//l6
	G.multiply(g1);
	long long total_loop = 0;
	for(int i=0; i<g1.size(); i++){
		for(int j=i; j<g1.size(); j++){
			total_loop -= l1[i][j];
			total_loop += g1[i][j];
		}
	}
	cout<<"Number of self-loop path with length 6: "<<total_loop<<"\n";
}