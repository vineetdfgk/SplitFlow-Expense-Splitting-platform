#include <bits/stdc++.h>
using namespace std;

// Function to convert the flow graph to an adjacency matrix
vector<vector<int>> convertToMatrix(int n, map<int, vector<pair<int, int>>> &flowGraph) {

    vector<vector<int>> arr(n, vector<int>(n, 0));

    for(auto it : flowGraph)
        {
        int src_node = it.first;
        for(auto jt : it.second)
        {
            int next_node = jt.first;
            int node_wt = jt.second;
            arr[src_node][next_node] = node_wt;
        }
    }
    return arr;
}

// Breadth-first search to find augmenting paths
bool bfs(vector<vector<int>> &rGraph, int s, int t, vector<int> &parent) {

    vector<int> visited(t + 1, 0);
    queue<int> q;
    visited[s] = 1;
    q.push(s);
    parent[s] = -1;

    while(!q.empty())
    {
        int u = q.front();
        q.pop();
        for(int v = 0; v <= t; v++)
        {
            if(visited[v] != 1 && rGraph[u][v] > 0)
            {
                if(v == t)
                {
                    parent[v] = u;
                    return true;
                }
                parent[v] = u;
                q.push(v);
                visited[v] = 1;W
            }
        }
    }
    return false;
}

// Ford-Fulkerson algorithm to find maximum flow
int maxFlowAlgo(vector<vector<int>> graph, int s, int t, vector<vector<int>> &paths, vector<int> &amt_pending) {

    vector<int> temp;
    cout << "maxFlowAlgo" << endl;
    int max_flow = 0;
    vector<int> parent(t + 1, -1);
    vector<vector<int>> rGraph = graph;

    while(bfs(rGraph, s, t, parent))
        {
            int path_flow = INT_MAX;
            for(int v = t; v != s; v = parent[v])
            {
                int u = parent[v];
                path_flow = min(path_flow, rGraph[u][v]);
            }
        for(int v = t; v != s; v = parent[v])
            {
            int u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
            }
        temp = {};
        for(int v = t; v != s; v = parent[v])
        {
            temp.push_back(v);
        }
        paths.push_back(temp);
        temp = {};
        amt_pending.push_back(path_flow);
        max_flow += path_flow;
        cout << "Added path_flow: " << path_flow << endl;
    }
    return max_flow;
}

// Function to check if a name is present in the list of people
bool checkName(string str1, vector<string> &names_people) {

    int n = names_people.size();
    bool check = false;
    for(int i = 0; i < n; i++)
        {
        if(str1 == names_people[i])
        {
            check = true;
            break;
        }
    }
    return check;
}

int main()
{

    int num_people;
    cout << "Enter the Number of People in The Group:" << endl;
    cin >> num_people;
    vector<string> names_people;
    map<string,int> people_debt;
    cout << endl << "!! Make sure all names typed are in lower case only !!!" << endl << endl;
    for(int i = 0; i < num_people; i++)
        {
        string name;
        cout << "Enter the Name of the " << i + 1 << " individual" << endl;
        cin >> name;
        names_people.push_back(name);
        people_debt[name] = 0;
        }
    cout << "Enter the total Number of transactions made inside the group:" << endl;
    int num_transactions;
    cin >> num_transactions;
    int i = 0;
    while(i < num_transactions)
    {
        fflush(stdin);
        string str1, str2;
        int amt;
        cout << "Enter the Person that gives out the money:" << endl;
        cin >> str1;
        if(!checkName(str1, names_people))
        {
            cout << endl << "!! Incorrect Name !!" << endl << endl;
            continue;
        }
        cout << "Enter the Person that receives the money:" << endl;
        cin >> str2;
        if(str2 == str1 || !checkName(str2, names_people))
        {
            cout << endl << "!! Incorrect Name !!" << endl << endl;
            continue;
        }
        cout << "Enter the amount:" << endl;
        cin >> amt;
        people_debt[str1] -= amt;
        people_debt[str2] += amt;
        i++;
        fflush(stdin);
    }
    vector<pair<string,int>> givers;
    vector<pair<string,int>> takers;
    for(auto it : people_debt)
    {
        if(it.second > 0)
            takers.push_back({it.first, abs(it.second)});
        else if(it.second < 0)
            givers.push_back({it.first, abs(it.second)});
    }
    cout<<endl<<"Givers:"<<endl;
    for(auto it:givers)
    {
        cout<<it.first<<"->"<<it.second<<endl;
    }
    cout<<endl<<"Receivers:"<<endl;
    for(auto it:takers)
    {
        cout<<it.first<<"->"<<it.second<<endl;
    }
    int num_connections = givers.size() + (takers.size() * givers.size()) + takers.size();
    int num_nodes = 2 + givers.size() + takers.size();
    map<int,string> ind_name;
    ind_name[0] = "source";
    for(int i = 0; i < givers.size(); i++)
    {
        ind_name[i + 1] = givers[i].first;
    }
    for(int i = 0; i < takers.size(); i++)
    {
        ind_name[givers.size() + i + 1] = takers[i].first;
    }
    ind_name[num_nodes - 1] = "sink";
    map<int,vector<pair<int,int>>> flowGraph;
    for(int i = 0; i < givers.size(); i++)
    {
        flowGraph[0].push_back({i + 1, givers[i].second}); // Insert edges from source to the giver nodes
    }
    for(int i = 0; i < takers.size(); i++)
    {
        for(int j = 0; j < givers.size(); j++)
        {
            flowGraph[j + 1].push_back({givers.size() + i + 1, givers[j].second}); // Insert edges between the givers and the takers
        }
    }
    for(int i = 0; i < takers.size(); i++)
    {
        flowGraph[givers.size() + i + 1].push_back({num_nodes - 1, takers[i].second}); // Insert edges between the takers and the sink node
    }
    int matrix_size = flowGraph.size();
    vector<vector<int>> flowMatrix(matrix_size + 1, vector<int>(matrix_size + 1, 0));
    flowMatrix = convertToMatrix(matrix_size + 1, flowGraph);
    for(int i = 0; i <= matrix_size; i++)
    {
        for(int j = 0; j <= matrix_size; j++)
        {
            cout << flowMatrix[i][j] << ' ';
        }
        cout << endl;
    }

    vector<vector<int>> paths;
    vector<int> amt_pending;
    int ans = maxFlowAlgo(flowMatrix, 0, matrix_size, paths, amt_pending);
    vector<pair<string,string>> names_paths;
    for(auto it : paths)
    {
        names_paths.push_back({ind_name[it[2]], ind_name[it[1]]});
    }
    for(int i = 0; i < amt_pending.size(); i++)
    {
        int amt = amt_pending[i];
        string person1 = names_paths[i].first;
        string person2 = names_paths[i].second;
        cout << person1 << " pays " << amt << " to " << person2 << endl;
    }

    return 0;
}
