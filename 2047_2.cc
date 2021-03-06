#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Tag {
    int a, b, w;

    Tag() {}
    Tag(int a, int b, int w) : a(a), b(b), w(w) {}

    bool operator < (const Tag &other) const {
        return a < other.a;
    }
};

int best_cost;
vector<vector<int> > best_hist;

int dfs(const vector<vector<pair<int, int> > > &edges, int pos, int cost, vector<int> &used, vector<int> &hist) {

    if(pos == edges.size()-1 && cost >= best_cost) {
        if(cost > best_cost) {
            best_hist.clear();
            best_cost = cost;
        }
        best_hist.push_back(hist);

        return cost;
    }

    int maxcost = 0;
    used[pos] = true;
    hist.push_back(pos);
    for(int i = 0; i < edges[pos].size(); ++i) {
        const pair<int,int> &p = edges[pos][i];
        if(used[p.first]) continue;

        int tmp = dfs(edges, p.first, cost + p.second, used, hist);
        if(tmp > maxcost) {
            maxcost = tmp;
        }
    }
    hist.pop_back();
    used[pos] = false;
    return maxcost;
}

int main() {
    while(true) {
        int N;
        cin >> N;
        if(!N) break;

        vector<Tag> ts(N);
        for(int i = 0; i < N; ++i) 
            cin >> ts[i].a >> ts[i].b >> ts[i].w;
        ts.push_back(Tag(-100, 0, 0));
        ts.push_back(Tag(1000, 2000, 0));
        sort(ts.begin(), ts.end());

        vector<vector<pair<int, int> > > edges(N+2);
        for(int i = 0; i < N+2; ++i) {
            const Tag &ti = ts[i];
            for(int j = 0; j < N+2; ++j) {
                const Tag &tj = ts[j];
                if(ti.b >= tj.a) continue;
                edges[i].push_back(make_pair(j, tj.w));
            }
        }

        int ans = 0;
        vector<int> used(N+2, 0);
        for(int cnt = 0; cnt < 2; ++cnt) {
            vector<int> cost(N+2);
            vector<int> prev(N+2, -1);
            for(int i = 0; i < N+2; ++i) {
                for(int j = 0; j < edges[i].size(); ++j) {
                    const pair<int,int> &e = edges[i][j];
                    if(used[e.first]) continue;
                    int tmp = cost[i] + e.second;
                    if(tmp > cost[e.first]) {
                        cost[e.first] = tmp;
                        prev[e.first] = i;
                    }
                }
            }
            ans += cost[N+1];
            int pos = prev[N+1];
            while(pos != -1) {
                used[pos] = 1;
                pos = prev[pos];
            }
        }

        cout << ans << endl;
    }
    return 0;
}
