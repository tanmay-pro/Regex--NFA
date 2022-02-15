#include <bits/stdc++.h>
using namespace std;
#define ll long long
#define PI 3.14159265
#define br cout << endl
#define fo(i, n) for (int i = 0; i < n; i++)
#define Fo(i, k, n) for (int i = k; k < n ? i < n : i > n; k < n ? i += 1 : i -= 1)
#define amazing ios_base::sync_with_stdio(false), cin.tie(NULL), cout.tie(NULL);
#define f first
#define s second
#define pb push_back
#define file_read                     \
    freopen("input.txt", "r", stdin); \
    freopen("output.txt", "w", stdout);
typedef pair<int, int> pii;
typedef pair<ll, ll> pll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<string> vs;
typedef vector<pii> vpii;
typedef vector<pll> vpl;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef map<int, int> mii;
typedef map<ll, ll> mll;
int MOD = 1e9 + 7;
struct transition
{
    ll start;
    char symbol;
    ll final;
};
typedef struct transition transition;

string insert_concat(string regex);
string infixToPostfix(string regex);
void add_transition(ll start, char symbol, ll final);
int decider(char c);
void output();
pair<pll, pll> get_tuples();

vector<transition> edges;
deque<pll> temp_graphs;
ll node_maintainer = 0;

int main(int argc, char *argv[])
{
    amazing;

    if (argc > 3)
    {
        cout << "Error in input\n";
        return 0;
    }
    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);

    string regex;
    // cin >> regex;
    getline(cin, regex);
    // cout << regex;
    // br;
    string net_string = infixToPostfix(regex);

    fo(i, net_string.size())
    {
        char character = net_string[i];
        int ret_val = decider(character);

        if (ret_val == 1)
        {
            pair<pll, pll> tuples = get_tuples();
            add_transition(tuples.s.s, 'E', (tuples.f).f);
            temp_graphs.push_front(make_pair((tuples.s).f, (tuples.f).s));
        }

        else if (ret_val == 2)
        {
            pair<pll, pll> tuples = get_tuples();
            add_transition(node_maintainer + 1, 'E', (tuples.f).f);
            add_transition(node_maintainer + 1, 'E', (tuples.s).f);
            add_transition((tuples.f).s, 'E', node_maintainer + 2);
            add_transition((tuples.s).s, 'E', node_maintainer + 2);
            temp_graphs.push_front(make_pair(node_maintainer + 1, node_maintainer + 2));
            node_maintainer += 2;
        }

        else if (ret_val == 3)
        {
            add_transition((temp_graphs.front()).f, 'E', (temp_graphs.front()).s);
            add_transition((temp_graphs.front()).s, 'E', (temp_graphs.front()).f);
        }

        else if (ret_val == 4)
        {
            add_transition(node_maintainer + 1, character, node_maintainer + 2);
            temp_graphs.push_front(make_pair(node_maintainer + 1, node_maintainer + 2));
            node_maintainer += 2;
        }
    }
    output();
    return 0;
}

string infixToPostfix(string regex)
{
    regex = insert_concat(regex);
    stack<char> st;
    string result = "";
    for (int i = 0; i < regex.length(); i++)
    {
        char c = regex[i];
        if (c >= 'a' && c <= 'z')
            result += c;
        else if (c == '(')
            st.push('(');
        else if (c == '*')
            result += c;
        else if (c == ')')
        {
            while (st.top() != '(')
            {
                result += st.top();
                st.pop();
            }
            st.pop();
        }
        else if (c == '+')
        {
            while (!st.empty() && (st.top() == '.' || st.top() == '+'))
            {
                result += st.top();
                st.pop();
            }
            st.push(c);
        }
        else if (c == '.')
        {
            while (!st.empty() && st.top() == '.')
            {
                result += st.top();
                st.pop();
            }
            st.push(c);
        }
    }
    while (!st.empty())
    {
        result += st.top();
        st.pop();
    }
    return result;
}

string insert_concat(string regex)
{
    Fo(i, 1, regex.size())
    {
        if (((regex[i - 1] >= 'a' && regex[i - 1] <= 'z') || regex[i - 1] == '*' || regex[i - 1] == ')') && ((regex[i] >= 'a' && regex[i] <= 'z') || regex[i] == '('))
        {
            regex = regex.substr(0, i) + "." + regex.substr(i);
        }
    }
    return regex;
}

void add_transition(ll start, char symbol, ll final)
{
    transition temp;
    temp.start = start;
    temp.symbol = symbol;
    temp.final = final;
    edges.pb(temp);
}

int decider(char c)
{
    if (c == '.')
        return 1;
    else if (c == '+')
        return 2;
    else if (c == '*')
        return 3;
    else if (c <= 'z' && c >= 'a')
        return 4;
    else
        return -1;
}

pair<pll, pll> get_tuples()
{
    pll first_tuple = temp_graphs.front();
    temp_graphs.pop_front();
    pll second_tuple = temp_graphs.front();
    temp_graphs.pop_front();
    return make_pair(first_tuple, second_tuple);
}

void output()
{
    add_transition(0, 'E', temp_graphs.front().first);
    cout << node_maintainer + 1 << ' ' << edges.size() << " 1";
    br;
    cout << temp_graphs.front().s;
    br;
    fo(i, edges.size())
    {
        transition tuple = edges[i];
        cout << edges[i].start << ' ' << edges[i].symbol << ' ' << edges[i].final;
        br;
    }
    return;
}