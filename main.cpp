/*
Компилировать в с++11, например командой `g++ -std=c++11 main.cpp -o main`
или можно выполнить `make all`
алгоритм:
идем bfs по обратным ребрам одновременно ото всех вершин одной темы, 
чтобы посчитать растояние от других вершин до вершин с темами.
Делаем для каждрой темы, суммируем расстояния
(для тех, из которых обе темы достижимы)  и сортируем наш результат
Асимтотика O(V + E + V log V), где V - кол-во вершин и E - кол-во ребер
*/
#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <algorithm>


const int INTMAX = 2147483647;

//Хранение графа и обратных ребер с помощью списка смежности
std::vector<std::vector<int>> edges;
std::vector<std::vector<int>> rev_edges;
void add_edge(int v, int u) {
	edges[v].push_back(u);
	rev_edges[u].push_back(v);
}

//Темы статей
enum Theme {
	UNKNOWN,
	FIRST,
	SECOND,
	BOTH
};
std::vector<Theme> themes;
void setTheme(int v, Theme type) {
	if (themes[v] != type) {
		themes[v] = BOTH;
	} else {
		themes[v] = type;
	}
}
//Списки статей определенной темы
std::vector<int> vertexsFirstThemes, vertexsSecondThemes;

std::vector<std::pair<int, int>> distance;
void updateMinDist(int node, int len, Theme start_theme) {
	auto &dist = start_theme == FIRST ? distance[node].first : distance[node].second;
	dist = std::min(dist, len);
}


//Основной алгоритм	
void bfs(std::vector<int> &nodesTheme, int n, Theme theme) {
	std::vector<bool> used(n, false);
	std::queue<std::pair<int, int>> bfs_queue;
	for (int node : nodesTheme) {
		bfs_queue.emplace(node, 0);
		used[node] = true;
	}
 	while (!bfs_queue.empty()) {
		auto p = bfs_queue.front();
		int v = p.first;
		int len = p.second;
		bfs_queue.pop();
		updateMinDist(v, len, theme);
		for (int u : rev_edges[v]) {
			if (!used[u]) {
				used[u] = true;
				bfs_queue.emplace(u, len + 1);
			}
		}
	}
}

int main() {
	int n, m;
	std::cin >> n >> m;

	edges.resize(n, std::vector<int>(0));
	rev_edges.resize(n, std::vector<int>(0));
	distance.resize(n, std::make_pair(INTMAX, INTMAX));

	for (int i = 0; i < m; ++i) {
		int v, u;
		std::cin >> v >> u;
		--v;
		--u;
		add_edge(v, u);
	}

	themes.resize(n, UNKNOWN);
	int nFirstTheme, nSecondTheme;
	std::cin >> nFirstTheme;
	for (int i = 0; i < nFirstTheme; ++i) {
		int v;
		std::cin >> v;
		--v;
		setTheme(v, FIRST);
		vertexsFirstThemes.push_back(v);
	}
	std::cin >> nSecondTheme;
	for (int i = 0; i < nFirstTheme; ++i) {
		int v;
		std::cin >> v;
		--v;
		setTheme(v, SECOND);
		vertexsSecondThemes.push_back(v);
	}

	bfs(vertexsFirstThemes, n, FIRST);
	bfs(vertexsSecondThemes, n, SECOND);

	std::vector<std::pair<int, int>> sum_dist(0);
	for (int i = 0; i < n; ++i) {
		auto &pr = distance[i];
		if (pr.first < INTMAX && pr.second < INTMAX) {
			sum_dist.emplace_back(distance[i].first + distance[i].second, i);
		}
	}
	std::sort(sum_dist.begin(), sum_dist.end());

	for (auto &pr : sum_dist) {
		std::cout << pr.second + 1 << '\n';
	}
}