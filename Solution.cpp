#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long int ll; // using this to avoid any possible overflow

ll totalBlockWeight = 4000000; // Given in the problem

//ll totalBlockWeight = 1000;

vector<string> ordering; // to store the Correct ordering

// Structure to store information regarding the Transaction

struct TransactionInfo
{

    string tx_id;
    ll fee;
    ll weight;
    vector<string> parents;
};

// This is used to fetch all the information from the "Each Ordering Element" with the help of HashMap
struct mydata
{

    ll myfee;                 // same as fee
    ll myweight;              // same as weight above
    vector<string> myparents; // same as parents above
};

vector<TransactionInfo> transactions; // To store all the transactions
map<string, ll> indexOfId;            // Map each Transaction Id to a integer/ll
map<string, mydata> helper;           // Helper map used to fetch data via Ordering

// Using c++ Streams to read and parse data inside the input file
void readCSV()
{

    cout << "Reading data" << endl;
    ifstream mempoolCsv("mempool.csv"); // Actual File
    //ifstream mempoolCsv("sample.csv"); // Sample file used
    string blockData;

    getline(mempoolCsv, blockData);

    while (getline(mempoolCsv, blockData))
    {
        TransactionInfo tempTransaction;
        string substr;
        stringstream ss(blockData);

        for (ll i = 0; i < 4; i++)
        {
            getline(ss, substr, ',');
            switch (i)
            {
            case 0:
                tempTransaction.tx_id = substr; // Id of Each Transaction
                break;

            case 1:
                tempTransaction.fee = atoi(substr.c_str()); // Fees of Each Transaction
                break;

            case 2:
                tempTransaction.weight = atoi(substr.c_str()); // Weight of Each Transaction
                break;

            case 3:
                stringstream ss2(substr);
                while (getline(ss2, substr, ';')) // As there can be multiple parents of a Node/Transaction
                {
                    tempTransaction.parents.push_back(substr);
                }
                break;
            }
        }
        indexOfId[tempTransaction.tx_id] = transactions.size(); // Mapping Each transactionId to an integer
        transactions.push_back(tempTransaction);                // storing the transactions
        helper[tempTransaction.tx_id] = {tempTransaction.fee,   // Maintaing the helper HashMap
                                         tempTransaction.weight,
                                         tempTransaction.parents};
    }
    mempoolCsv.close();
    cout << "Data successfully read from the file " << endl;
}

// Function to write the final result on the output file
void getOutput(vector<string> &output, string filename)
{
    ofstream myfile(filename);
    for (auto s : output)
    {
        myfile << s << endl;
    }
    myfile.close();

    cout << "Output file Written Successfuly " << endl;
}

void topologicalsort()
{

    // Here we perform the toplogical Sort , Here I have assumed that the transactions don't have any
    // conflict among themselves , that is the graph of the transactions is Acyclic .

    unordered_map<string, ll> indegree;

    map<string, list<string>> graph;

    // making an adjaceny list , to perform toplogical sort by using Kahn's Algorithm of Toplogical Sorting
    for (auto it : transactions)
    {
        if (it.parents.size() > 0)
        {
            for (ll j = 0; j < it.parents.size(); j++)
            {
                graph[it.parents[j]].push_back(it.tx_id);
                indegree[it.tx_id]++;
            }
        }

        else
        {
            indegree[it.tx_id] = 0;
        }
    }

    queue<string> q;

    for (auto it : indegree)
    {

        if (it.second == 0)
        {
            q.push(it.first);
        }
    }

    vector<string> osf;
    while (q.size() > 0)
    {

        string top = q.front();
        q.pop();
        ordering.push_back(top);
        for (auto it : graph[top])
        {
            indegree[it]--;
            if (indegree[it] == 0)
            {
                q.push(it);
            }
        }
    }

    // At this point we have all the Transactions ordered in right manner in the Ordering Vector .
}

ll ans = 0;

vector<string> res; // String to store the result

map<string, bool> vis; // used inside the Greedy Method in Main

bool isValid(ll start, ll currwt)

// Checks wether each Transaction is valid or not by following the problem statement

//  It ensures two things  :
//  1 . Wether taking the current transaction exceeds the weight beyond the limit or not
//  2. Wether all the parents of the current have been visited or not

{
    if (currwt + helper[ordering[start]].myweight > totalBlockWeight)
        return false; // including this will cause overweight

    ll cnt = 0;

    for (ll i = 0; i < helper[ordering[start]].myparents.size(); i++)
    {
        if (vis[helper[ordering[start]].myparents[i]] == 0)
        {
            // if unvisited parent return false ;
            return false;
        }
    }

    // No unvisited parent and including this will keep the overall weight  under the limit

    return true;
}

ll ans_weight = 0; // To store the sum of weights of all the transactions that give the result .

// Naive recursive function , that gives the exact answer but runs infinitely .
void utility(ll start, ll currwt, ll amount, vector<string> &osf)
{

    if (start >= ordering.size())
    {

        if (amount > ans)
        {
            ans = amount;

            res = osf;

            ans_weight = currwt;
        }

        return;
    }

    if (isValid(start, currwt)) // Checks if it is valid to add the current item as Transaction
    {
        vis[ordering[start]] = 1;

        osf.push_back(ordering[start]);
        utility(start + 1, currwt + helper[ordering[start]].myweight, amount + helper[ordering[start]].myfee, osf);
        // 2 options if we can take the current weight ;
        vis[ordering[start]] = 0;
        osf.pop_back();

        utility(start + 1, currwt, amount, osf);
    }

    else
    {

        // If we cannot take the Transaction

        utility(start + 1, currwt, amount, osf);
    }
}

ll max_cost = 0; // variable to store the maximum fees

// Function used to find the maximum fees

void good(double m)
{
    vector<string> osf;
    ll curr_wt = 0;
    ll curr_cost = 0;
    for (int i = 0; i < ordering.size(); i++)
    {
        double curr_ratio = double(helper[ordering[i]].myfee) / double(helper[ordering[i]].myweight);

        if (curr_ratio >= m && isValid(i, curr_wt))
        {
            vis[ordering[i]] = 1;
            curr_cost += helper[ordering[i]].myfee;
            curr_wt += helper[ordering[i]].myweight;
            osf.push_back(ordering[i]);
        }
    }

    if (curr_cost > max_cost)
    {
        max_cost = curr_cost;
        res = osf;
        ans_weight = curr_wt;
    }

    return;
}

// function to find the average ratio of the dataset (as per  "fee/weight" ) .
double avg_ratio()
{
    double average_ratio = 0;
    for (int i = 0; i < transactions.size(); i++)
    {
        average_ratio += double(transactions[i].fee) / double(transactions[i].weight);
    }
    return average_ratio = average_ratio / double(transactions.size());
}

// Main Function
int main()
{

    readCSV(); // read the CSV file

    topologicalsort(); // perform toplogical sorting to order the all the transactions in a way
    // such that all parents come before their children in the ordering array we created above .

    // dry run on sample output
    // for (auto it : ordering)
    // {
    //     cout << it << endl;
    // }

    // utility(0, 0, 0, osf);
    // this is a naive recursive approach but it runs in O(2 ^ N) time complexity

    // Now since the naive approach won't be able to run out code efficiently , so now we will try to
    // find the best possible ratio by going from 0.1 to ratio_avg and find the best ratio greedily .

    double ratio_avg = avg_ratio(); // Avg of all the ratios of (fee/weight) of the dataset .

    // Greedy Method

    // Here we go over each possible ratio from 0 to ratio_avg and determine which ratio gives the best Fees
    // and also keeps the overall Weight under the limit .

    for (double i = 0; i < ratio_avg; i += 0.1)
    {
        for (auto it : ordering)
        {
            vis[it] = 0;
        }

        good(i); // void function which helps to determine the best transactions fees
    }

    cout << endl;

    cout << "Maxmimum Fees : " << max_cost << endl;

    cout << endl;

    cout << "Total weight in current block : " << ans_weight << endl;

    cout << endl;

    getOutput(res, "block.txt");

    return 0;
}