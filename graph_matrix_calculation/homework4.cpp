 #include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>

#define VERTICES 100;

using namespace std;

class Graph
{
public:
	//Graph() {}
	Graph(int nodes) {
		this->nodes = nodes;
		matrix.reserve(nodes);
		for (int i = 0; i < nodes; ++i)
		{
			vector<int> row(nodes, 0);
			this->matrix.push_back(row);
		}
	}
	void add_edge(int u, int v) {
		this->matrix[u][v] = 1;
		this->matrix[v][u] = 1;
	}
	virtual ~Graph() {}

	void get_degree(vector<int>& degree) {
		for (int i = 0; i<this->nodes; i++) {
			if (this->matrix[i][i] == 1)
				degree[i] += 1;
			for (int j = 0; j<this->matrix[i].size(); j++) {
				degree[i] += matrix[i][j];
			}
		}
	}

	int get_total_edges() {
		int edges = 0;
		for (int i = 0; i<this->nodes; i++) {
			for (int j = i; j<this->nodes; j++) {
				edges += this->matrix[i][j];
			}
		}
		return edges;
	}

	int get_total_nodes() {
		return this->nodes;
	}

	int get_degree(int node_i) {
		int degree = 0;
		for (int i = 0; i < this->nodes; i++) {
			if (this->matrix[node_i][i] == 1)
				degree++;
		}
		return degree;
	}

	vector<vector<int> > get_graph() {
		return this->matrix;
	}

	void printMatrix(vector<vector<int> >& g) {
		for (int i = 0; i<this->nodes; i++) {
			for (int j = 0; j<this->nodes; j++)
				cout << g[i][j] << " ";
			cout << "\n";
		}
	}

	vector<vector<int> > multiply(vector<vector<int> >& g) {
		//g = gg
		vector<vector<int> > result;
		result.reserve(this->nodes);
		for (int i = 0; i<this->nodes; i++) {
			vector<int> row(nodes, 0);
			result.push_back(row);
		}

		for (int i = 0; i<this->nodes; i++) {
			for (int j = 0; j<this->nodes; j++) {
				for (int k = 0; k<this->nodes; k++) {
					result[i][j] += g[i][k] * this->matrix[k][j];
				}
			}
		}
		return result;
	}

	vector<vector<int> > multiply(vector<vector<int> >& g, vector<vector<int> >& h) {
		// g = gh
		vector<vector<int> > result;
		result.reserve(this->nodes);
		for (int i = 0; i<this->nodes; i++) {
			vector<int> row(nodes, 0);
			result.push_back(row);
		}

		for (int i = 0; i<this->nodes; i++) {
			for (int j = 0; j<this->nodes; j++) {
				for (int k = 0; k<this->nodes; k++) {
					result[i][j] += g[i][k] * h[k][j];
				}
			}
		}
		return result;
	}

	void zero_diagonal() {
		for (int i = 0; i<this->nodes; i++) {
			this->matrix[i][i] = 0;
		}
	}
protected:
	int nodes;
	vector<vector<int> > matrix;
};

class RandomGraph: public Graph
{
public:
	void generateRandomGraph(double p) {
		srand(time(NULL));
		for (int i = 0; i < this->nodes; i++)
		{
			for (int j = i + 1; j < this->nodes; j++)
			{
				if (((double)rand() / (RAND_MAX)) < p) {
					this->matrix[i][j] = 1;
					this->matrix[j][i] = 1;
				}
			}
		}
	}
	~RandomGraph() {};
};

class SmallWorldGraph : public Graph
{
public:
	void generateSmallWorldGraph(int k, double beta) {
		//Watts¨CStrogatz model
		//k is the mean degree
		//0<=belta<=1, N>>K>>lnN>>1
		//the model constructs an undirected graph with N nodes and NK/2 edges
		//1.each connects to K neighbors, K/2 on each side
		int delta = k / 2;
		for (int i = 0; i < delta; i++) {
			for (int j = this->nodes - (delta - i); j < this->nodes; j++) {
				this->matrix[i][j] = 1;
				this->matrix[j][i] = 1;
			}
			for (int j = 0; j <= i + delta; j++) {
				this->matrix[i][j] = 1;
				this->matrix[j][i] = 1;
			}
		}
		for (int i = delta; i < this->nodes-delta; i++)
		{
			for (int j = i - delta; j <= i + delta; j++) {
				this->matrix[i][j] = 1;
				this->matrix[j][i] = 1;
			}
		}
		for (int i = this->nodes-delta; i < this->nodes; i++) {
			for (int j = i - delta; j <this->nodes; j++) {
				this->matrix[i][j] = 1;
				this->matrix[j][i] = 1;
			}
			for (int j = 0; j <= (i+delta)-nodes; j++) {
				this->matrix[i][j] = 1;
				this->matrix[j][i] = 1;
			}
		}
		zero_diagonal();
		/*2.
		*/
	}
	~SmallWorldGraph() {};
};

class BAGraph : public Graph
{
public:
	void generateBAGraph() {
		matrix[0][1] = 1;
		matrix[1][0] = 1;
		matrix[1][2] = 1;
		matrix[2][1] = 1;
		int total_degree = 2*get_total_edges();
		for (int i = 3; i < this->nodes; i++) {
			//total_degree = 2 * get_total_edges();
			srand(time(NULL));
			for (int j = 0; j < i; j++) {
				if (((double)rand() / (RAND_MAX)) < ((double)get_degree(j) / total_degree)) {
					this->matrix[i][j] = 1;
					this->matrix[j][i] = 1;
					total_degree += 2;
				}
			}
		}
	}
};

int main() {
	/*
	read parameter from input.txt
	*/
	ifstream input;
	input.open("input.txt");
	if (!input) {
		cout << "unable to open input.txt" << endl;
	}
	double p,beta;
	int k;
	input >> p >> k >> beta;
	input.close();
	cout << p << k << beta;
	/*
	RandomGraph rg(VERTICES);
	SmallWorldGraph swg(VERTICES);
	BAGraph bag(VERTICES);
	rg.generateRandomGraph(0.3);
	swg.generateSmallWorldGraph();
	bag.generateBAGraph();
	*/
	return 0;
}