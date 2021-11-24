#define _CRT_SECURE_NO_WARNINGS
#include"graph2.h"
#include<iostream>
#include<cmath>
#include<string>
#include<algorithm>
#include<vector>
#include<set>
#include<cstdio>
#include<iomanip>
#include<fstream>
#include<cstdlib>
#include<stack>
#include<map>
#include<list>
#include<queue>


typedef long long ll;

using namespace std;

Graph graph;
vector<bool> used;
bool flag = false;
const int INF = 1e9;

bool sml(pair<int, pair<int, int>> a, pair<int, pair<int, int>> b)
{
	return a.first < b.first;
}


void dfs(int v, int pred, map<int, set<Edge>> g, bool isDirected, int & cycle)
{
	used[v] = true;
	for (auto x : g[v])
	{
		if (!used[x.ver])
			dfs(x.ver, v, g, isDirected, cycle);
		else if (isDirected || (!isDirected && x.ver != pred))
			cycle++;
	}
}

bool bfs(int s, map<int, set<Edge>> g, int& cnt)
{
	int n = g.size() + 1;
	queue<int> q;
	q.push(s);
	vector<char> used(n, false);
	used[s] = true;
	cnt = 1;
	while (!q.empty())
	{
		int v = q.front();
		q.pop();
		for (auto to : g[v])
		{
			if (!used[to.ver])
			{
				used[to.ver] = true;
				q.push(to.ver);
				cnt++;
			}
			else
				return false;
		}
	}
	return true;
}

void deikstra(int s, map<int, set<Edge>> g, int k, int u)
{
	int n = g.size();
	vector<vector<int>> d(n, vector<int>(k, INF));
	vector<vector<pair<int, int>>> p(n, vector<pair<int, int>>(k, { -1, -1 }));
	d[s][0] = 0;
	set<pair<int, pair<int, int>>> q;
	q.insert({ d[s][0], { s, 0 } });
	while (!q.empty())
	{
		int v = q.begin()->second.first;
		int f = q.begin()->first;
		int pos = q.begin()->second.second;
		q.erase(q.begin());
		if (d[v][k - 1] < f)
			continue;
		for (auto x : g[v])
		{
			int to = x.ver;
			int len = x.weight;
			for(int i = 0; i < k; i++)
				if (f + len < d[to][i])
				{
					//q.erase({ d[to][i], to });
					for (int j = k - 2; j >= i; j--)
					{
						d[to][j+1] = d[to][j];
						for (auto h : g[to])
						{
							for (int r = 0; r < k; r++)
							{
								if (p[h.ver][r] == make_pair(to, j)) {
									p[h.ver][r].second = j + 1;
								}
							}
						}
						p[to][j+1] = p[to][j];
						q.erase({ d[to][j], {to, j} });
						q.insert({ d[to][j], {to, j+1} });
					}
					d[to][i] = f + len;
					p[to][i] = { v, pos };
					q.insert({ d[to][i], {to, i} });
					break;
				}
		}
	}
	for (int i = 0; i < k; i++)
	{
		cout << d[u][i] << endl;
		if (d[u][i] != INF)
		{
			int cur_v = u, cur_u = i;
			vector<int> path;
			while (cur_v != s || cur_u != 0)
			{
				path.push_back(cur_v);
				int tmp = cur_v;
				cur_v = p[cur_v][cur_u].first;
				cur_u = p[tmp][cur_u].second;
			}
			path.push_back(0);
			for (int j = path.size() - 1; j >= 0; j--)
				cout << path[j] << " ";
			cout << endl;
		}
	}
}

void floyd_warshall(map<int, set<Edge>> g, int p) {
	int n = g.size();
	vector<vector<int>> d(n, vector<int>(n, INF));
	for (int i = 0; i < n; i++)
		for (auto y : g[i])
			d[i][y.ver] = y.weight;
	for (int i = 0; i < n; i++) d[i][i] = 0;
	for (int k = 0; k < n; k++)
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (d[i][k] < INF && d[k][j] < INF && d[i][k] + d[k][j] < d[i][j])
					d[i][j] = d[i][k] + d[k][j];
	/*for (int i = 0; i < n; i++, cout << endl)
		for (int j = 0; j < n; j++)
			cout << d[i][j] << " ";*/
	vector<int> s(n, 0);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (d[i][j] < INF) s[i] += d[i][j];
	for (int i = 0; i < n; i++)
		if (s[i] <= p) cout << i << " " << s[i] << endl;
}

int ford_bellman(int u, map<int, set<Edge>> g) {
	int n = g.size();
	vector<int> d;
	d.assign(n, INF);
	d[u] = 0;
	int x;
	for (int iter = 0; iter < n; iter++) {
		x = -1;
		for (auto x : g)
			for(auto y : g[x.first])
				if (d[x.first] < INF && d[x.first] + y.weight < d[y.ver])
					d[y.ver] = max(-INF, d[x.first] + y.weight);
	}
	int ans = -INF;
	for (int i = 0; i < n; i++)
		if (d[i] < INF && i != u) ans = max(ans, d[i]);
	return ans;
}

template <typename T>
void print(vector<T> out_arr)
{
	for (auto x : out_arr)
		cout << x << " ";
	cout << endl;
}

int dfs_ford(int u, int mx, int t, map<int, set<Edge>>& g) {
	if (u == t) return mx;
	if (mx == 0 || used[u]) return 0;
	used[u] = true;
	for (set<Edge>::iterator e = g[u].begin(); e != g[u].end(); e++) {
		int r = (*e).weight - (*e).f;
		int f = dfs_ford((*e).ver, min(r, mx), t, g);
		if (f > 0) {
			auto tmp = (*e);
			g[u].erase(tmp);
			g[u].insert(Edge(tmp.ver, tmp.weight, (tmp.f + f)));
			for (set<Edge>::iterator x = g[tmp.ver].begin(); x != g[tmp.ver].end(); x++)
				if ((*x).ver == u)
				{
					auto tmp = (*x);
					g[u].erase((*x));
					g[u].insert(Edge(tmp.ver, tmp.weight, (tmp.f + f)));
				}
			//cout << e.ver << " " << e.f << endl;
			return f;
		}
	}
	return 0;
}


int ford_fulkerson(int s, int t, map<int, set<Edge>> g) {
	int n = g.size();
	int resF = 0;
	while (true) {
		used.assign(n, false);
		int f = dfs_ford(s, INF, t, g);
		if (f == 0) break;
		resF += f;
		/*for (auto x : g)
		{
			cout << x.first << " ";
			for (auto y : g[x.first])
				cout << y.f << " ";
			cout << endl;
		}*/
	}
	return resF;
}



void solve(string command, vector<string> args,  Graph& g)
{
	if (command == "addVertex")
		g.addVertex(stoi(args[0]));
	if (command == "addEdge")
	{
		if (args.size() == 2) args.push_back("0");
		g.addEdge(stoi(args[0]), stoi(args[1]), stoi(args[2]));
	}
	if (command == "deleteVertex")
		g.deleteVertex(stoi(args[0]));
	if (command == "deleteEdge")
	{
		if (args.size() == 2) args.push_back("0");
		g.deleteEdge(stoi(args[0]), stoi(args[1]), stoi(args[2]));
	}
	if (command == "getList")
		g.writeInTerminal();
	if (command == "getInFile")
	{
		ofstream file;
		file.open(args[0]);
		g.writeInFile(file);
	}
	if (command == "readFromFile")
	{
		ifstream file;
		file.open(args[0]);
		graph = Graph(file);
		cout << "�� ����� ������ ����:" << endl;
		graph.writeInTerminal();
	}
	if (command == "help")
	{
		cout << "�������� �������:\nreadFromFile <�������� ����� � �����������> -- ���������� �� �����\naddVertex <����� �������> -- �������� �������\naddEdge <����� ��������� �������> <����� �������� �������> [<���>] -- �������� �����\n";
		cout << "deleteVertex <����� �������> -- ������� �������\ndeleteEdge <����� ��������� �������> <����� �������� �������> [<���>] -- ������� �����\n";
		cout << "getList -- ��������� �������� � �����\ngetInFile <�������� ����� � �����������> -- ������ �������� � ����� � ����\n";
		cout << "help -- �������� ��������� �������\n\n";
	}
	if (command == "getLoops")
	{
		cout << "������� � �������: ";
		set<int> vertices;
		auto list = graph.getList();
		for (auto x : list)
		{
			vertices.insert(x.first);
		}
		bool f = true;
		for (auto v : vertices)
		{
			for (auto e : list[v])
			{
				if (e.ver == v)
				{
					f = false;
					cout << v << " ";
				}
			}
		}
		if (f) cout << "������� �� �������";
		cout << endl;
	}
	if (command == "getGeneral")
	{
		cout << "����� �������: ";
		set<int> vertices;
		auto list = graph.getList();
		for (auto x : list)
		{
			vertices.insert(x.first);
		}
		int x = stoi(args[0]);
		int y = stoi(args[1]);
		set<int> edgesX;
		for (auto e : list[x])
			edgesX.insert(e.ver);
		bool f = true;
		for (auto v : vertices)
		{
			for (auto e : list[v])
			{
				if (e.ver == x && e.ver != v)
					edgesX.insert(v);
			}
		}
		set<int> edgesY;
		for (auto e : list[y])
			edgesY.insert(e.ver);
		for (auto v : vertices)
		{
			for (auto e : list[v])
			{
				if (e.ver == y && e.ver != v)
					edgesY.insert(v);
			}
		}
		for (auto c : edgesY)
		{
			if (edgesX.count(c))
			{
				f = false;
				cout << c << " ";
			}
		}
		if (f)
			cout << "�� �������";
		cout << endl;
	}
	if (command == "NewOrgraph")
	{
		Graph newGraph;
		newGraph.NewOrgraph(graph);
		cout << "����� ������:" << endl;
		newGraph.writeInTerminal();
	}
	if (command == "cyclicality")
	{
		int cycle = 0;
		used.resize(graph.getList().size(), false);
		dfs(1, 1, graph.getList(), false, cycle);
		if (cycle == 0)
			cout << "���� ���������" << endl;
		else
			cout << "� ����� ���� ����(�)" << endl;
	}
	if (command == "istwv")
	{
		auto list = graph.getList();
		int n = graph.getNumberOfVertices();
		bool f = false;
		for (auto x : list)
		{
			Graph copygraph = Graph(graph);
			copygraph.deleteVertex(x.first);
			vector<int> st(n, 0);
			auto lst = copygraph.getList();
			set<int> vertices;
			for (auto y : lst)
			{
				vertices.insert(y.first);
			}
			for (auto v : vertices)
			{
				for (auto y : lst[v])
					st[y.ver]++;
			}
			int i = 0;
			while (i < n && (i == x.first || st[i] != 0))
				i++;
			int cnt = 1;
			if (i < n && bfs(i, lst, cnt) && cnt == n-1)
				f = true;
		}
		if (f)
			cout << "�� ����� ����� �������� ������ ��������� ����� �������" << endl;
		else
			cout << "�� ����� ������ �������� ������ ��������� ����� �������" << endl;
	}
	if (command == "karkas")
	{
		int n = graph.getNumberOfVertices();
		int m = graph.getNumberOfEdges();
		auto list = graph.getList();
		vector<pair<int, pair<int, int>>> g;
		set<int> vertices;
		for (auto y : list)
		{
			vertices.insert(y.first);
		}
		for (auto v : vertices)
		{
			for (auto e : list[v])
				g.push_back({ e.weight, { min(v, e.ver), max(v, e.ver) } });
		}
		int cost = 0;
		map<int, set<Edge>> res;

		sort(g.begin(), g.end());
		vector<int> tr_id(n);
		for (int i = 0; i < n; ++i)
			tr_id[i] = i;
		for (auto x : g)
		{
			int a = x.second.first, b = x.second.second, l = x.first;
			if (tr_id[a] != tr_id[b])
			{
				cost += l;
				res[a].insert(Edge(b, l));
				res[b].insert(Edge(a, l));
				int old_id = tr_id[b], new_id = tr_id[a];
				for (int j = 0; j < n; ++j)
					if (tr_id[j] == old_id)
						tr_id[j] = new_id;
			}
		}
		auto lst = res;
		vertices.clear();
		for (auto x : lst)
		{
			vertices.insert(x.first);
		}
		for (auto v : vertices)
		{
			cout << v << ": [ ";
			for (auto e : lst[v])
			{
				cout << e.ver;
				cout << "(" << e.weight << ") ";
			}
			cout << "]\n";
		}
	}
	if (command == "ktovu")
	{
		int v = stoi(args[0]);
		int u = stoi(args[1]);
		int k = stoi(args[2]);
		deikstra(v, graph.getList(), k, u);
	}
	if (command == "center")
	{
		auto lst = graph.getList();
		vector<int> rad(lst.size());
		int ans = INF;
		for (auto x : lst)
		{
			rad[x.first] = ford_bellman(x.first, lst);
			if (rad[x.first] > -INF)
				ans = min(ans, rad[x.first]);
		}
		for (auto x : lst)
			cout << rad[x.first] << " ";
		cout << endl;
		for (auto x : lst)
			if (rad[x.first] == ans)
				cout << x.first << " ";
		cout << endl;
	}
	if (command == "ltp")
	{
		floyd_warshall(graph.getList(), stoi(args[0]));
	}
	if (command == "flow")
	{
		cout << ford_fulkerson(stoi(args[0]), stoi(args[1]), graph.getList()) << endl;
	}
}

int main()
{
//	ios_base::sync_with_stdio(0);
//	cin.tie(0);
//	cout.tie(0);
	setlocale(LC_ALL, "Russian");
	
	cout << "�������� �������:\nreadFromFile <�������� ����� � �����������> -- ���������� �� �����\naddVertex <����� �������> -- �������� �������\naddEdge <����� ��������� �������> <����� �������� �������> [<���>] -- �������� �����\n";
	cout << "deleteVertex <����� �������> -- ������� �������\ndeleteEdge <����� ��������� �������> <����� �������� �������> [<���>] -- ������� �����\n";
	cout << "getList -- ��������� �������� � �����\ngetInFile <�������� ����� � �����������> -- ������ �������� � ����� � ����\n";
	cout << "help -- �������� ��������� �������\n\n";
	string command;
	cin >> command;
	while (command != "end")
	{
		vector<string> args;
		if (command != "getList" && command != "help" && command != "getLoops" && command != "NewOrgraph" && command != "cyclicality" && command != "istwv" && command != "center")
		{
			string line;
			getline(cin, line);
			string arg;
			int j = 0;
			for (int i = 0; i < line.size(); i++)
			{
				if (line[i] == ' ')
				{
					arg = line.substr(j, i - j);
					if (arg != "")
						args.push_back(line.substr(j, i - j));
					j = i + 1;
				}
			}
			arg = line.substr(j, line.size() - j);
			if (arg != "")
				args.push_back(line.substr(j, line.size() - j));
		}
		solve(command, args, graph);
		cin >> command;
	}
	
}