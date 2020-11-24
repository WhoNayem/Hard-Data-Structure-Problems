#include <bits/stdc++.h>
using namespace std;
#define __Mammamia__                ios_base :: sync_with_stdio(false);
#define ll                          long long
#define MAXN                        100007

int n, q, type;
vector <int> height(MAXN), idx(MAXN), vec(MAXN), go_forward(MAXN), go_back(MAXN), next_greater(MAXN), tree_max_range(MAXN * 4), tree_forward(MAXN * 4), tree_back(MAXN * 4);

void build(vector <int> &tree, vector <int> &vec, int node, int b, int e, function <int(int, int)> combine)
{
    if (b == e)
    {
        tree[node] = vec[b];
        return;
    }
    int mid = (b + e) >> 1;
    build(tree, vec, node * 2 + 1, b, mid, combine);
    build(tree, vec, node * 2 + 2, mid + 1, e, combine);
    tree[node] = combine(tree[node * 2 + 1], tree[node * 2 + 2]);
}

int query(vector <int> &tree, int node, int b, int e, int left, int right, function <int(int, int)> combine, int id)
{
    if (b > right || e < left) return id;
    if (b >= left && e <= right) return tree[node];
    int mid = (b + e) >> 1;
    return combine(query(tree, node * 2 + 1, b, mid, left, right, combine, id),  query(tree, node * 2 + 2, mid + 1, e, left, right, combine, id));
}

int get_max_in_range(int left, int right)
{
    return query(tree_max_range, 0, 1, n, left, right, [&](int a, int b)
    {
        if (a < 1 || a > n) return b;
        if (b < 1 || b > n) return a;
        if (height[a] < height[b]) return b;
        return a;
    }, 0);
}

int get_max_forward(int left, int right)
{
    return query(tree_forward, 0, 1, n, left, right, [](int a, int b){return max(a, b);}, 0);
}

int get_max_back(int left, int right)
{
    return query(tree_back, 0, 1, n, left, right, [](int a, int b){return max(a, b);}, 0);
}

int main ()
{
    __Mammamia__

    cin >> n >> q >> type;
    for (int i = 1; i <= n; ++i)
    {
        cin >> height[i];
        go_forward[i] = n + 1;
        go_back[i] = 1;
        idx[i] = i;
    }

    stack <int> Stack;
    for (int i = 1; i <= n; ++i)
    {
        while(!Stack.empty() && height[Stack.top()] < height[i])
        {
            next_greater[Stack.top()] = i;
            go_back[i] = max(go_back[i], go_back[Stack.top()] + 1);
            Stack.pop();
        }
        Stack.push(i);
    }

    for (int i = n; i > 0; --i)
        go_forward[i] = go_forward[next_greater[i]] + 1;

    build(tree_max_range, idx, 0, 1, n, [&](int a, int b)
    {
        if (a < 1 || a > n) return a;
        if (b < 1 || b > n) return b;
        if (height[a] < height[b]) return b;
        return a;
    });

    build(tree_forward, go_forward, 0, 1, n, [](int a, int b){return max(a, b);});
    build(tree_back, go_back, 0, 1, n, [](int a, int b){return max(a, b);});

    int res = 0, left, right;
    while(q--)
    {
        int x, y; cin >> x >> y;

        left = (x + type * 1 * res - 1) % n + 1;
        right = (y + type * 1 * res - 1) % n + 1;
        if (left > right)
            swap(left, right);

        res = 1;

        int max_in_range = get_max_in_range(left, right);

        res = max(res, get_max_forward(left, max_in_range) - go_forward[max_in_range] + 1);
        if (max_in_range < right)
            res = max(res, get_max_back(max_in_range + 1, right));

        cout << res << endl;
    }



    return 0;
}















