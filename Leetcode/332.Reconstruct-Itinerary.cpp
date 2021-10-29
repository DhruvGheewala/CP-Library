// Link: https://leetcode.com/problems/reconstruct-itinerary/

class Solution {
private:
    vector<string> order;
    map<string, priority_queue<string, vector<string>, greater<string>>> adj;
    
    void visit(const string &u) {
        while(!adj[u].empty()) {
            const string v = adj[u].top();
            adj[u].pop();
            
            visit(v);
        }
        order.push_back(u);
    }
    
public:
    vector<string> findItinerary(vector<vector<string>>& tickets) {
        adj.clear();
        for(vector<string> &ticket: tickets)
            adj[ticket[0]].push(ticket[1]);
        
        order.clear();
        visit("JFK");
        reverse(order.begin(), order.end());
        return order;
    }
};