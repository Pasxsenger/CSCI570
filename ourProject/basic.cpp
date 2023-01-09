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
/**
 * @brief Get the Number object
 *  Aux function for turing A C G T to numbers 
 * @param ch 
 * @return int 
 */
int getNumber(char ch) {
    if(ch == 'A')         return 0;
    else if(ch == 'C')    return 1;
    else if(ch == 'G')    return 2;
    else                  return 3;
}
/**
 * @brief top down to find the actual alignment
 * 
 * @param A 
 * @param width of the array = X.size
 * @param height of the array = Y.size
 * @param X 
 * @param Y 
 * @return vector<string> 
 */
vector<string> find_solution(vector<vector<int> > A, int width, int height, string X, string Y){
        int i = width, j = height;
        string alignX = "", alignY = "";
         while (i > 0 && j > 0) {
            int match = A[i-1][j-1] + alpha[getNumber(X[i-1])][getNumber(Y[j-1])];
            int gapX = A[i-1][j] + gapPenalty;
            int gapY = A[i][j -1] + gapPenalty;
            if (match == A[i][j]){
                alignX = X[i-1]+alignX;
                alignY = Y[j-1]+alignY;
                i--;
                j--;
            }
             else if(A[i][j] == gapY) {
                alignX =  "_"+alignX;
                alignY = Y[j-1] +alignY;
                j--;
            }
            else if(A[i][j] == gapX) {
                alignX = X[i-1]+alignX;
                alignY =  "_"+ alignY;
                i--;
            }
        }
        //concatenate with the rest
        if(i > 0){
            for (int k = i; k >0; k --){
                alignX = X[k-1] + alignX;
                alignY =   "_" + alignY;
                i--;
            }
        }else{
            for (int l = j; l >0; l-- ){
                alignX =  "_" + alignX ;
                alignY =  Y[l-1] +alignY ;
            }
        }

        vector<string> dpOutput;
        dpOutput.push_back(to_string(A[width][height]));
        dpOutput.push_back(alignX);
        dpOutput.push_back(alignY);
        return dpOutput;
}
/**
 * @brief bottom up to fill the 2 d array
 * 
 * @param X 
 * @param Y 
 * @return vector<string> 
 */
vector<string> alignment(string X, string Y) {
    int m = X.length();
    int n = Y.length();

    vector<vector<int> > A(m+1,vector<int>(n+1,0));
    //intialization
    for (int i = 0; i <= m; i++)
        A[i][0] = i * gapPenalty;
    for (int j = 0; j <= n; j++)
        A[0][j] = j * gapPenalty;

    //filling out dp array, the solution is at a[m,n]
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (X[i - 1] == Y[j - 1])
                A[i][j] = A[i - 1][j - 1];
            else
                A[i][j] = min(A[i - 1][j - 1] + alpha[getNumber(X[i-1])][getNumber(Y[j-1])], min(A[i - 1][j] + gapPenalty, A[i][j - 1] + gapPenalty));
        }
    }
    //topdown to find the actual alignment
    vector<string> rtn = find_solution(A, m, n, X, Y);
    return rtn;
}


/**
 * @brief generate string
 * 
 * @param filename 
 * @return vector<string> 
 */
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
       //It should never occur. Check man getrusage for more info to debug.
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

    results = alignment(X, Y);
    
    double totalmemory = getTotalMemory() / 10;
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double totaltime = seconds*1000 + microseconds*1e-3;

    //string outputfile = path + argv[2];
    string outputfile = argv[2];

    ofstream of(outputfile);
    of << results[0] << endl;
    of << results[1] << endl;
    of << results[2] << endl;
    of << totaltime << endl;
    of << totalmemory << endl;

    return 0;
}
