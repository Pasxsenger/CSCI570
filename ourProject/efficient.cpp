#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <sys/resource.h>
#include <sys/time.h>
#include <errno.h>
#include <limits.h>

using namespace std;

int alpha[4][4] = { {0, 110, 48, 94},
                    {110, 0, 118, 48},
                    {48, 118, 0, 110},
                    {94, 48, 110, 0} };

int gapPenalty = 30;

int getNumber(char ch) {
    if(ch == 'A')         return 0;
    else if(ch == 'C')    return 1;
    else if(ch == 'G')    return 2;
    else                  return 3;
}

vector<string> alignment(string X, string Y) {
    int m = X.length();
    int n = Y.length();
    vector<vector<int> > A(m+1,vector<int>(n+1,0));

    for (int i = 0; i <= m; i++)
        A[i][0] = i * gapPenalty;
    for (int j = 0; j <= n; j++)
        A[0][j] = j * gapPenalty;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1])
                A[i][j] = A[i - 1][j - 1];
            else
                A[i][j] = min(A[i - 1][j - 1] + alpha[getNumber(X[i-1])][getNumber(Y[j-1])], min(A[i - 1][j] + gapPenalty, A[i][j - 1] + gapPenalty));
        }
    }

    int i = m, j = n;
    string alignX = "", alignY = "";
    while (i > 0 && j > 0) {
        if(A[i][j] == A[i-1][j-1] + alpha[getNumber(X[i-1])][getNumber(Y[j-1])]){
            alignX = X[i-1]+alignX;
            alignY = Y[j-1]+alignY;
            i--;
            j--;
        }
        else if(A[i][j] == A[i-1][j] + gapPenalty) {
            alignX = X[i-1]+alignX;
            alignY =  "_"+ alignY;
            i--;
        }
        else if(A[i][j] == A[i][j-1] + gapPenalty) {
            alignX =  "_"+alignX;
            alignY = Y[j-1] +alignY;
            j--;
        }
    }

    if(i > 0)
        alignX = alignX + X.substr(0, i);
    if(j > 0)
        alignY = alignY + Y.substr(0, j);

    int diff = alignX.length() - alignY.length();
    if(diff > 0)
        for(int i=0;i<diff;i++)
            alignY = "_"+alignY ;
    if(diff < 0){
        diff = -diff;
        for(int i = 0; i < diff; i++)
            alignX = "_"+alignX ;
    }

    vector<string> dpOutput;
    dpOutput.push_back(alignX);
    dpOutput.push_back(alignY);
    dpOutput.push_back(to_string(A[m][n]));
    return dpOutput;
}

vector<int> space_efficient_alignment(string x,string y, string alignment) {
    int m = x.length();
    int n = y.length();

    vector<vector<int> > B(m+1, vector<int>(2));
    vector<int> res;

    for (int i = 0; i <= m; i++)
        B[i][0] = i * gapPenalty;

    for (int j = 1; j <= n; j++) {
        B[0][1] = j * gapPenalty;
        for (int i = 1; i <= m; i++) {
            int p, q, r;
            if(alignment == "forward")
                p = alpha[getNumber(x[i-1])][getNumber(y[j-1])] + B[i-1][0];
            else
                p = alpha[getNumber(x[m-i])][getNumber(y[n-j])] + B[i-1][0];
            q = gapPenalty + B[i-1][1]; 
            r = gapPenalty + B[i][0];  
            B[i][1] = min(min(p,q), r); 
        }

        for (int i = 0; i <= m; i++)
            B[i][0] = B[i][1];
    }

    for(int i=0;i<=m;i++)
        res.push_back(B[i][1]);

    return res;
}

vector<string> Divide_and_Conquer_Alignment(string X, string Y) {
    int m = X.length();
    int n = Y.length();

    if(m <= 2 || n <= 2)
        return alignment(X, Y);

    int midY = n / 2;

    vector<int> forwardAlignment = space_efficient_alignment(X, Y.substr(0, midY), "forward");
    vector<int> backwardAlignment = space_efficient_alignment(X, Y.substr(midY), "backward");

    vector<int> alignmentSum;
    int q = 0, min = INT_MAX;

    for(int j = 0; j <= m; j++) {
        int val = forwardAlignment[j] + backwardAlignment[m-j];
        alignmentSum.push_back(val);
    }

    for(int j = 0; j < alignmentSum.size(); j++) {
        if(alignmentSum[j] < min){
            min = alignmentSum[j];
            q = j;
        }
    }

    forwardAlignment.clear();
    backwardAlignment.clear();
    alignmentSum.clear();

    vector<string> forward = Divide_and_Conquer_Alignment(X.substr(0, q), Y.substr(0, midY));
    vector<string> backward = Divide_and_Conquer_Alignment(X.substr(q), Y.substr(midY));

    vector<string> dcOutput;
    for(int k = 0; k < 3; k++) {
        if(k == 2) {
            stringstream convert(forward[k]);
            int x = 0;
            convert >> x;

            stringstream convert2(backward[k]);
            int y = 0;
            convert2 >> y;

            string s = to_string(x + y);
            dcOutput.push_back(s);
        }
        else
            dcOutput.push_back(forward[k] + backward[k]);
    }

    return dcOutput;
}

vector<string> String_Generation(string filename)
{
    string dnaX = "", dnaY = "";    
    int flag = 0, times = 0;       
    vector<string> results;

    ifstream fs;
    fs.open(filename);
    while(fs.good()){
        string line;
        getline(fs, line);
        if(isalpha(line[0]) && flag == 0){
            dnaX = line;
            flag = 1;   
        }
        else if(isalpha(line[0]) == 0 && flag == 1){
            int index = stoi(line);
            dnaX.insert(index+1, dnaX);
            times++;
            auto itor1 = remove_if(dnaX.begin(), dnaX.end(), ::isspace);
            dnaX.erase(itor1, dnaX.end());
        }
        else if(isalpha(line[0]) && flag != 0){
            dnaY = line;
            flag = 2;   
        }
        else if(isalpha(line[0]) == 0 && flag == 2 && times > 0){
            int index = stoi(line
            );
            dnaY.insert(index+1, dnaY);
            times--;

        auto itor2 = remove_if(dnaY.begin(), dnaY.end(), ::isspace);
        dnaY.erase(itor2, dnaY.end());
        }
    }
    fs.close();
    

    results.push_back(dnaX);
    results.push_back(dnaY);
    return results;
}
extern int errno;
long getTotalMemory() {
    struct rusage usage;
    int returnCode = getrusage(RUSAGE_SELF, &usage);
    if(returnCode == 0) {
        return usage.ru_maxrss;
    } else {
       printf("error %d", errno);
    return -1; }
}

int main(int argc, const char * argv[]) {
    //string path = "./datapoints/";  
    //string inputfile = path + argv[1];
    string inputfile = argv[1];
    vector<string> dna_str, results;
    dna_str = String_Generation(inputfile);
    string X = dna_str[0], Y = dna_str[1];
    struct timeval begin, end;
    gettimeofday(&begin, 0);

    results = Divide_and_Conquer_Alignment(X, Y);

    double totalmemory = getTotalMemory() / 10;
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double totaltime = seconds*1000 + microseconds*1e-3;

    //string outputfile = path + argv[2];
    string outputfile = argv[2];

    ofstream of(outputfile);
    of << results[2] << endl;
    of << results[0] << endl;
    of << results[1] << endl;
    of << totaltime << endl;
    of << totalmemory << endl;

    return 0;
}
