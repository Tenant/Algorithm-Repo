#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <queue>
#include <vector>
#include <algorithm>

#define N (50+10)
#define M (4000+4*N)
#define INF 1e10

typedef long long LL;

using namespace std;

struct edge {
	int v, cap, next;
};
edge e[M];

int head[N], level[N], cur[N];
int num_of_edges;
int map[55][55];
int bian[4500][2];

/*
* When there are multiple test sets, you need to re-initialize before each
*/
void dinic_init(void) {
	num_of_edges = 0;
	memset(head, -1, sizeof(head));
	return;
}

int add_edge(int u, int v, int c1, int c2) {
	int& i = num_of_edges;

	assert(c1 >= 0 && c2 >= 0 && c1 + c2 >= 0); // check for possibility of overflow
	e[i].v = v;
	e[i].cap = c1;
	e[i].next = head[u];
	head[u] = i++;

	e[i].v = u;
	e[i].cap = c2;
	e[i].next = head[v];
	head[v] = i++;
	return i;
}

void print_graph(int n) {
	for (int u = 0; u<n; u++) {
		printf("%d: ", u);
		for (int i = head[u]; i >= 0; i = e[i].next) {
			printf("%d(%d)", e[i].v, e[i].cap);
		}
		printf("\n");
	}
	return;
}

/*
* Find all augmentation paths in the current level graph
* This is the recursive version
*/
int dfs(int u, int t, int bn) {
	if (u == t) return bn;
	int left = bn;
	for (int &i = cur[u]; i >= 0; i = e[i].next) {
		int v = e[i].v;
		int c = e[i].cap;
		if (c > 0 && level[u] + 1 == level[v]) {
			int flow = dfs(v, t, min(left, c));
			if (flow > 0) {
				e[i].cap -= flow;
				e[i ^ 1].cap += flow;
				cur[u] = i;
				left -= flow;
				if (!left) break;
			}
		}
	}
	if (left > 0) level[u] = 0;
	return bn - left;
}

bool bfs(int s, int t) {
	memset(level, 0, sizeof(level));
	level[s] = 1;
	queue<int> q;
	q.push(s);
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		if (u == t) return true;
		for (int i = head[u]; i >= 0; i = e[i].next) {
			int v = e[i].v;
			if (!level[v] && e[i].cap > 0) {
				level[v] = level[u] + 1;
				q.push(v);
			}
		}
	}
	return false;
}

LL dinic(int s, int t) {
	LL max_flow = 0;

	while (bfs(s, t)) {
		memcpy(cur, head, sizeof(head));
		max_flow += dfs(s, t, INT_MAX);
	}
	return max_flow;
}

int upstream(int s, int n) {
	int cnt = 0;
	vector<bool> visited(n);
	queue<int> q;
	visited[s] = true;
	q.push(s);
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		for (int i = head[u]; i >= 0; i = e[i].next) {
			int v = e[i].v;
			if (e[i].cap > 0 && !visited[v]) {
				visited[v] = true;
				q.push(v);
				cnt++;
			}
		}
	}
	return cnt; // excluding s
}

int min(int a, int b){ return (a < b) ? a : b; }

int main(){
	int n, m, K;
	while (scanf("%d%d%d", &n, &m, &K) != EOF && !(m==0 && n==0 && K==0)){
		memset(map, INF, sizeof(map));
		for (int i = 1; i <= n; i++) map[i][i] = 0;
		dinic_init();
		int s = 1;
		int t = 2 * n;
		for (int i = 1; i <= m; i++){
			int _a, _b;
			scanf("%d%d", &_a, &_b);
			bian[i][0] = _a;
			bian[i][1] = _b;
			map[_a][_b] = 1;
		}
		for (int i = 1; i <= n;i++)
		for (int j = 1; j <= n;j++)
		for (int k = 1; k <= n; k++){
			map[j][k] = min(map[j][k], map[j][i] + map[i][k]);
		}
		add_edge(1, 1 + n, INF, 0);
		add_edge(n, n + n, INF, 0);
		for (int i = 2; i < n; i++) add_edge(i, i + n, 1, 0);
		for (int i = 0; i < m; i++){
			int x = bian[i][0];
			int y = bian[i][1];
			if (map[1][x] + map[y][n] < K){
				add_edge(x + n, y, 1, 0);
			}
		}
		int flow = dinic(s, t);
		// print_graph(2 * n + 1);
		printf("%d\n", flow);
	}
	return 0;
}