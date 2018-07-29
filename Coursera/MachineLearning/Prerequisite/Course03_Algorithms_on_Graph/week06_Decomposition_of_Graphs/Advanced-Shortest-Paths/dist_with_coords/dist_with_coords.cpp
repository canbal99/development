#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <cmath>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

using namespace std;
using std::cout;

const int F = 0;
const int R = 1;

// See the explanations of these typedefs and constants in the starter for friend_suggestion
typedef vector<vector<vector<int>>> Adj;
typedef long long Len;
typedef vector<priority_queue<pair<Len, int>,vector<pair<Len,int>>,greater<pair<Len,int>>>> Queue;

const Len INFINITY_ = numeric_limits<Len>::max() / 4;

class AStar {
    // See the descriptions of these fields in the starter for friend_suggestion
    int n_;
    const Adj &adj_;
    const Adj &cost_;
    vector<vector<Len>> distance_;
    vector<int> workset_;
    //vector<bool> visited_;
    vector<unordered_set<int>> processedset_;
    // Coordinates of the nodes
    std::vector<std::pair<Len,Len>> xy_;
    bool usePotential_;
    int source,target;

public:
    AStar(int n, Adj& adj, Adj& cost, std::vector<std::pair<Len,Len>> xy, bool usePotential=true)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<Len>(n_, INFINITY_)), processedset_(2,unordered_set<int>(n)), xy_(xy),
                usePotential_(usePotential)
        { workset_.reserve(n); processedset_[F].reserve(n); processedset_[R].reserve(n); }

    // See the description of this method in the starter for friend_suggestion
    void clear() {
        for (int v : workset_) {
            distance_[0][v] = distance_[1][v] = INFINITY_;
        }
        workset_.clear();
        processedset_[F].clear();
        processedset_[R].clear();
    }

    // See the description of this method in the starter for friend_suggestion

    void visit(Queue& q, int side, int v, Len dist) {
        // Implement this method yourself
        workset_.push_back(v);
        distance_[side][v] = dist;
        q[side].push(make_pair(dist, v));
        //cout << "(visited:" << distance_[side][v] << ")";
    }
    
    inline Len getDistance(int p1, int p2) {
        Len xDiff = xy_[p1].first-xy_[p2].first;
        Len yDiff = xy_[p1].second-xy_[p2].second;
        return sqrt(xDiff*xDiff+yDiff*yDiff);
    }
    
    Len getPotential(int side, int v) {
        if (!usePotential_)
            return 0;
        //if (potential_.count(v)==0) {
            int potential = (getDistance(v,target) - getDistance(v,source)) / 2;
        //    potential_[v] = potential;
            return (side ? -1*potential : potential);
        //}
        //return (side?-1:1)*potential_[v];
    }
    
    void process(Queue& q, int side, int v) {
        // Implement this method yourself
        int adj;
        Len newDist;
        processedset_[side].insert(v);
        Len dist = distance_[side][v];
        //cout << "\nProcessing(" << (side?"R":"F") << "):" << v+1 << "(dist:" << dist << ") => Checking:";
        for (int i=adj_[side][v].size()-1; i>=0; i--) {
            adj = adj_[side][v][i];
            newDist = dist + cost_[side][v][i] - getPotential(side, v) + getPotential(side, adj);
            //cout << " " << adj+1 << "(dist:" << (distance_[side][adj]==INFINITY_?"INF":to_string(distance_[side][adj])) << "-cost:" << cost << ")";
            if (distance_[side][adj] > newDist) {
                visit(q, side, adj, newDist);
            }
        }
        //cout<<"\n";
    }
    
    Len returnShortest() {
        Len minimum = INFINITY_;
        Len candidate;
        for (int v : processedset_[F]) {
            candidate = distance_[F][v] + distance_[R][v];
            if (minimum > candidate) minimum = candidate;
        }
        for (int v : processedset_[R]) {
            candidate = distance_[F][v] + distance_[R][v];
            if (minimum > candidate) minimum = candidate;
        }
        return minimum + 2 * getPotential(F,source);
    }

    // Returns the distance from s to t in the graph
    Len query(int s, int t) {
        source = s;
        target = t;
        if (s==t) return 0;
        clear();
        Queue q(2);
        visit(q, 0, s, 0);
        visit(q, 1, t, 0);
        // Implement the rest of the algorithm yourself

        int f,r;
        while (!q[F].empty() || !q[R].empty()) {
            
            if (!q[F].empty()) {
                f = q[F].top().second;q[F].pop();
                if (processedset_[F].count(f)==0)
                {
                    process(q, F, f);
                    if (processedset_[R].count(f)==1) {
                        return returnShortest();
                    }
                }
            }
            
            if (!q[R].empty()) {
                r = q[R].top().second;q[R].pop();
                if (processedset_[R].count(r)==0)
                {
                    process(q, R, r);
                    if (processedset_[F].count(r)==1) {
                        return returnShortest();
                    }
                }
            }
        }

        return -1;
    }
    int getProcessedCount() {
        return processedset_[F].size() + processedset_[R].size();
    }
    int getWorksetCount() {
        return workset_.size();
    }
};

int main() {
    
    int n, m;
    scanf("%d%d", &n, &m);
    std::vector<std::pair<Len,Len>> xy(n);
    for (int i=0;i<n;++i){
        int a, b;
        scanf("%d%d", &a, &b);
        xy[i] = make_pair(a,b);
    }
    Adj adj(2, vector<vector<int>>(n));
    Adj cost(2, vector<vector<int>>(n));
    for (int i=0; i<m; ++i) {
        int u, v, c;
        scanf("%d%d%d", &u, &v, &c);
        adj[0][u-1].push_back(v-1);
        cost[0][u-1].push_back(c);
        adj[1][v-1].push_back(u-1);
        cost[1][v-1].push_back(c);
    }

    AStar astar(n, adj, cost, xy);

    int t;
    scanf("%d", &t);
    for (int i=0; i<t; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%lld\n", astar.query(u-1, v-1));
    }
}



/*

class TimeElapsed {
    
    clock_t begin;
    clock_t end;

public:    
    TimeElapsed() {start();};
    void start() {begin = clock();}
    void stop(string title="")  {
        end = clock();
        int msec = double(end - begin) / 1000;
        cout << "TimeElapsed for <" << title << "> is " << msec << " ms\n";
    }
};

#define DISP                "%lld"
#define SIDE                500
#define COUNT               (SIDE*SIDE)
#define POS_(x,y)           ((y)*SIDE+(x))
#define POS                 POS_(x,y)
#define COST                10//(20 + (rand()%1000))

void addConn(Adj& adj, Adj& cost, int pos, int adjPos) {
    if (adjPos<0 || adjPos>=COUNT) return;
    if (pos+1==adjPos && adjPos%SIDE==0) return;
    if (pos==adjPos+1 && pos%SIDE==0) return;
    
    int c = COST;
    adj[F][pos].push_back(adjPos);
    cost[F][pos].push_back(c);
    adj[R][adjPos].push_back(pos);
    cost[R][adjPos].push_back(c);
}

void test_() {
    
    return;
    
    TimeElapsed elapsed;
    int test_count = 0;
    while (true) {
        std::vector<std::pair<Len,Len>> xy(COUNT);
        Adj adj(2, vector<vector<int>>(COUNT));
        Adj cost(2, vector<vector<int>>(COUNT));

        for (int y=0; y<SIDE; y++) {
            for (int x=0; x<SIDE; x++) {
                xy[POS] = make_pair(x*10,y*10);
                addConn(adj, cost, POS, POS_(x,y-1));
                addConn(adj, cost, POS, POS_(x,y+1));
                addConn(adj, cost, POS, POS_(x-1,y));
                addConn(adj, cost, POS, POS_(x+1,y));
            }
        }
    
        vector<int> path;
        elapsed.start();
        AStar astar(COUNT, adj, cost, xy);
        elapsed.stop("AStar init");
        elapsed.start();
        AStar dijks(COUNT, adj, cost, xy, false);
        elapsed.stop("Dijkstra init");
        
        //int source = 0;
        //int dest = SIDE*(SIDE-1);
        int source = rand()%COUNT;
        int dest = rand()%COUNT;
        
        cout << test_count++ << " source:" << source << " destination:" << dest << "\n";
        
        elapsed.start();
        Len resAStar = astar.query(source,dest);
        elapsed.stop("AStar query");
        elapsed.start();
        Len resDijks = dijks.query(source,dest);
        elapsed.stop("Dijkstra query");
        //cout << "sample cost:" << COST << "\n";
        cout << "astar:" << resAStar << " processed:" << astar.getProcessedCount() << " workset:" << astar.getWorksetCount() << "\n";
        cout << "dijks:" << resDijks << " processed:" << dijks.getProcessedCount() << " workset:" << dijks.getWorksetCount() << "\n";
        
        if (resAStar != resDijks) {
            printf(DISP "==" DISP "\n", resAStar, resDijks);
        
            //path = astar.getPath();
            //std::for_each(path.begin(), path.end(), [](int& elem) {cout << elem << "->";});cout << "\n";

            //path = dijks.getPath();
            //std::for_each(path.begin(), path.end(), [](int& elem) {cout << elem << "->";});cout << "\n";
            
            for (int i = 0; i < COUNT; i++) {
                cout << "\nPoint:" << i << "(" << xy[i].first << "," << xy[i].second << ")" << " adj:";
                for (int j = 0; j < adj[F][i].size(); j++) {
                    cout << " " << adj[F][i][j] << "(" << cost[F][i][j] << ")";
                }
            }
            cout << "\n";
            for (int i = 0; i < COUNT; i++) {
                cout << "\nPoint:" << i << "(" << xy[i].first << "," << xy[i].second << ")" << " adj:";
                for (int j = 0; j < adj[R][i].size(); j++) {
                    cout << " " << adj[R][i][j] << "(" << cost[R][i][j] << ")";
                }
            }
            cout << "\n";

            break;
        }
        
        if (astar.getProcessedCount() > dijks.getProcessedCount()) {
            getchar();
        }
        //cout << "\n\n\n\n\n\n";
        //getchar();
    }
}
*/