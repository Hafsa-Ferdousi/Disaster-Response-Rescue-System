#include <bits/stdc++.h>
using namespace std;

int getInt(const string &msg){
    cout <<msg;
    cout.flush();
    int x;
    while(!(cin>>x)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid number. Try again: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}


string getString(const string &msg) {
    cout << msg;
    cout.flush();
    string s;
    getline(cin, s);
    while (s.empty()) getline(cin, s);
    return s;
}




struct Victim{
    int id;
    string name;
    int injury; //1=minor ,2=modarate, 3=critical
    int zone;
    bool transported = false;


    int priority() const {
        return injury * 10;
         }

};

struct Team {
    int id;
    bool available = true;
    string name;
};

struct Hospital {
    int id;
    string name;
    int zone;
    int capacity;
    int used = 0;
    bool emergencyUnit;
    bool burnUnit;
    bool traumaUnit;

    bool hasBed() const {
        return used < capacity;
         }
};


struct Vehicle {
    int id;
    string type; // ambu & van
    double speed;
    bool available = true;
    string name;
};


struct VictimCompare{
    bool operator()(const Victim &a, const Victim &b) const {
        if (a.priority() == b.priority() )
            return a.id>b.id;
        return a.priority() < b.priority();
    }
};

const double INF = 1e15;
struct System {
    priority_queue<Victim, vector<Victim>, VictimCompare> pq;
    unordered_map<int, Victim> victimDB;

    vector<Team> teams;
    vector<Hospital> hospitals;
    vector<Vehicle> vehicles;


    int nextVictimId = 1;

    int zonesCount = 0;
    vector<string> zoneNames;

    vector<vector<pair<int,double>>> graph;
    vector<vector<double>> distFW;

    set<pair<int,int>> blockedEdges;

    System() { initData();

     }

      void initData() {
        zonesCount = 6;
        zoneNames = {"Dhanmondi",
        "Gulshan",
        "Uttara",
        "Mirpur",
        "Banani",
        "Motijheel"};

        graph.assign(zonesCount, {});
        distFW.assign(zonesCount, vector<double>(zonesCount, INF));

        addRoad(0,1,5);  addRoad(0,3,6);
        addRoad(1,4,3);  addRoad(1,2,10);
        addRoad(2,3,4);  addRoad(3,5,12);
        addRoad(4,5,6);  addRoad(0,4,8);

        for (int i=1; i<=4; i++)
            teams.push_back({i, true, "Team~" + to_string(i)});

        vehicles.push_back({1, "ambulance", 1.0, true, "Ambulance~1"}); //sp
        vehicles.push_back({2, "van",       0.8, true, "Van~1"});

         hospitals.push_back({1, "City Hospital",            0, 5, 0, true,  false, true});
        hospitals.push_back({2, "General Hospital",         2, 8, 0, false, true,  false});
        hospitals.push_back({3, "Emergency Medical Center", 5, 4, 0, true,  true,  true});
        hospitals.push_back({4, "Burn & Trauma Center",     4, 6, 0, false, true,  true});

        computeFloydWarshall();
    }


 void addRoad(int a, int b, double d) {
        graph[a].push_back({b,d});
        graph[b].push_back({a,d});
        distFW[a][b] = distFW[b][a] = d;
        distFW[a][a] = distFW[b][b] = 0;
    }

 /* FLOYD–WARSHALL  */
    void computeFloydWarshall() {

        for (int i=0; i<zonesCount; i++)
            for (int j=0; j<zonesCount; j++)
                if (i == j) distFW[i][j] = 0;

        for (int k=0; k<zonesCount; k++)
            for (int i=0; i<zonesCount; i++) {
                if (distFW[i][k] >= INF) continue;
                for (int j=0; j<zonesCount; j++) {
                    if (distFW[k][j] >= INF) continue;
                    distFW[i][j] = min(distFW[i][j], distFW[i][k] + distFW[k][j]);
                }
            }
    }


    /*diskstra*/
    vector<double> dijkstra(int src){
        vector<double> dist(zonesCount,INF);
        dist[src]=0;

         using P = pair<double,int>;
        priority_queue<P, vector<P>, greater<P>> pq;
        pq.push({0,src});

        while (!pq.empty()) {
            auto [d,u] = pq.top();
            pq.pop();
            if (d != dist[u]) continue;

            for (auto &edge : graph[u]) {
                int v = edge.first;
                double w = edge.second;

                if (blockedEdges.count({u,v}) || blockedEdges.count({v,u}))
                    continue;

                if (dist[v] > d + w) {
                    dist[v] = d + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }


   void showRoadStatus() {
        cout << "\n=== ROAD STATUS ===\n";

        cout << "\nBlocked Roads:\n";
        bool hasBlocked = false;
        for (auto &p : blockedEdges) {
            int a = p.first, b = p.second;
            if (a < b) {
                cout << " " << zoneNames[a] << " <-> " << zoneNames[b] << "\n";
                hasBlocked = true;
            }
        }
        if (!hasBlocked) cout << " None\n";

        cout << "\nOpen Roads:\n";
        bool hasOpen = false;
        for (int u = 0; u < zonesCount; u++) {
            for (auto &edge : graph[u]) {
                int v = edge.first;
                if (u < v) {
                    if (!blockedEdges.count({u,v}) && !blockedEdges.count({v,u})) {
                        cout << " " << zoneNames[u] << " <-> " << zoneNames[v]
                             << " (" << edge.second << " km)\n";
                        hasOpen = true;
                    }
                }
            }
        }
        if (!hasOpen) cout << " None\n";

        cout << "=====================\n";
    }

    /* ADD VICTIM  */
    void addVictim() {
        Victim v;
        v.id = nextVictimId++;
        v.name = getString("Enter victim name: ");
        v.injury = max(1, min(3, getInt("Injury (1-3): ")));

        cout << "\nZones:\n";
        for (int i=0; i<zonesCount; i++)
            cout << " " << i << " -> " << zoneNames[i] << "\n";

        v.zone = getInt("Enter victim zone index: ");
        if (v.zone < 0 || v.zone >= zonesCount) v.zone = 0;

        pq.push(v);
        victimDB[v.id] = v;

        cout << "Victim added: " << v.name << " (ID " << v.id << ")\n";
    }

    /* TEAM PICK*/
    int pickTeam() {
        for (auto &t : teams)
            if (t.available) return t.id;
        return -1;
    }

    /*  VEHICLE PICK   */
    int pickVehicleForVictim(const Victim &v, int hospitalZone) {
        auto dist = dijkstra(v.zone);
        if (dist[hospitalZone] >= INF) return -1;

        int ambulanceIndex = -1;
        int vanIndex = -1;

        for (int i = 0; i < vehicles.size(); i++) {
            if (vehicles[i].type == "ambulance" && vehicles[i].available)
                ambulanceIndex = i;
            if (vehicles[i].type == "van" && vehicles[i].available)
                vanIndex = i;
        }


    // Critical~Ambulance first
        if (v.injury == 3) {
            if (ambulanceIndex != -1) return ambulanceIndex;
            return vanIndex;
        }

        // Minor/Moderate~Van first
        if (vanIndex != -1) return vanIndex;
        return ambulanceIndex;
    }


    /*  PICK hospital*/
    int pickHospitalEnhanced(const Victim &v) {
        auto dist = dijkstra(v.zone);

        double bestScore = 1e18;
        int bestIdx = -1;

        for (int i=0; i<hospitals.size(); i++) {
            auto &h = hospitals[i];
            if (!h.hasBed()) continue;

            double score = 0;

            if (v.injury == 3 && !h.emergencyUnit) score += 10000;
            if (v.injury == 3 && !h.traumaUnit)    score += 2000;

            double d = dist[h.zone];
            if (d >= INF) continue;

            score += d;
            score -= 0.3 * (h.capacity - h.used);

            if (score < bestScore) {
                bestScore = score;
                bestIdx = i;
            }
        }
        return bestIdx;
    }

    /* DISPATCH */
    void dispatchVictim() {
        if (pq.empty()) {
            cout << "No victims.\n";
            return;
        }

        Victim v = pq.top();
        pq.pop();

        int teamId = pickTeam();
        if (teamId == -1) {
            cout << "No free teams.\n";
            pq.push(v);
            return;
        }
        teams[teamId-1].available = false;

        int hIdx = pickHospitalEnhanced(v);
        if (hIdx == -1) {
            cout << "No hospital available.\n";
            pq.push(v);
            teams[teamId-1].available = true;
            return;
        }

int vehIdx = pickVehicleForVictim(v, hospitals[hIdx].zone);
        if (vehIdx == -1) {
            cout << "No available vehicle.\n";
            pq.push(v);
            teams[teamId-1].available = true;
            return;
        }

        vehicles[vehIdx].available = false;

        auto dist = dijkstra(v.zone);
        double distance = dist[hospitals[hIdx].zone];
        double time = distance / vehicles[vehIdx].speed;

        hospitals[hIdx].used++;
        v.transported = true;
        victimDB[v.id] = v;

        cout << "Transporting " << v.name
             << " to " << hospitals[hIdx].name
             << " , Distance: " << distance
             << " , Time: " << time
             << " , Vehicle: " << vehicles[vehIdx].name << "\n";

        teams[teamId-1].available = true;
        vehicles[vehIdx].available = true;
    }
/*  BLOCK ROAD  */
    void blockRoadInteractive() {
        cout << "\nZones:\n";
        for (int i=0; i<zonesCount; i++)
            cout << " " << i << " -> " << zoneNames[i] << "\n";

        int a = getInt("Enter FROM-zone index: ");
        int b = getInt("Enter TO-zone index: ");

        if (a < 0 || a >= zonesCount || b < 0 || b >= zonesCount) {
            cout << "Invalid zone.\n";
            return;
        }

        blockedEdges.insert({a,b});
        blockedEdges.insert({b,a});

        cout << "Road BLOCKED: "
             << zoneNames[a] << " <-> " << zoneNames[b] << "\n";

        computeFloydWarshall();
        showRoadStatus();
    }

    // UNBLOCK ROAD
    void unblockRoadInteractive() {
        cout << "\nZones:\n";
        for (int i=0; i<zonesCount; i++)
            cout << " " << i << " -> " << zoneNames[i] << "\n";

        int a = getInt("Enter FROM-zone index: ");
        int b = getInt("Enter TO-zone index: ");

        if (a < 0 || a >= zonesCount || b < 0 || b >= zonesCount) {
            cout << "Invalid zone.\n";
            return;
        }

        blockedEdges.erase({a,b});
        blockedEdges.erase({b,a});

        cout << "Road UNBLOCKED: "
             << zoneNames[a] << " <-> " << zoneNames[b] << "\n";

        computeFloydWarshall();
        showRoadStatus();
    }

    // SHOW STATUS
    void showStatus() {
        cout << "\n=== SYSTEM STATUS ===\n";

        showRoadStatus();

        cout << "\nTeams:\n";
        for (auto &t : teams)
            cout << " " << t.name << ": "
                 << (t.available ? "Available" : "Busy") << "\n";

        cout << "\nVehicles:\n";
        for (auto &v : vehicles)
            cout << " " << v.name << " (" << v.type << ") - "
                 << (v.available ? "Free" : "Busy") << "\n";

        cout << "\nHospitals:\n";
        for (auto &h : hospitals)
            cout << " " << h.name
                 << " | Zone: " << zoneNames[h.zone]
                 << " | Beds: " << h.used << "/" << h.capacity << "\n";

        cout << "\nPending Victims:\n";
        auto tmp = pq;
        if (tmp.empty()) cout << " None\n";
        while (!tmp.empty()) {
            auto v = tmp.top(); tmp.pop();
            cout << " " << v.id << " - " << v.name
                 << " , Level " << v.injury
                 << " , Zone " << zoneNames[v.zone] << "\n";
        }

        cout << "\n====================\n";
    }

    /*  MENU */
    void run() {
        while (true) {
            cout << "\n--- DISASTER RESCUE SYSTEM ---\n";
            cout << "1. Add victim\n";
            cout << "2. Dispatch rescue\n";
            cout << "3. Show status\n";
            cout << "4. Block road\n";
            cout << "5. Unblock road\n";
            cout << "0. Exit\n";

            int op = getInt("Option: ");

            if (op == 1) addVictim();
            else if (op == 2) dispatchVictim();
            else if (op == 3) showStatus();
            else if (op == 4) blockRoadInteractive();
            else if (op == 5) unblockRoadInteractive();
            else if (op == 0) break;
        }
    }
};

/*  MAIN */

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    System sys;
    sys.run();
    return 0;
}
