#include <iostream>
#include<cstring>
#include <list>
#include <string>
#include <math.h>
#include <unordered_map>

using namespace std;

int _edges = 0;

void setNumberOfEdges(int edges){
    _edges = edges;
}
int getNumEdges() {
    return _edges;
}


class Graph {
public:
    const static int MAXNUMVERTICES = 100;
    float theGraph[MAXNUMVERTICES][MAXNUMVERTICES];

    void fill() {
        for (int i = 0; i < MAXNUMVERTICES; i++) {
            for (int j = 0; j < MAXNUMVERTICES; j++) {
                theGraph[i][j] = 0;
            }
        }
    }

    void insertEdge(int from, int to) {
        theGraph[to][from] = 1.0;
    }

    void print(){
        //i print starting at 1 since hashes start at 1
        for(int i = 1; i<= getNumEdges(); i++){
            for(int j = 1; j<= getNumEdges(); j++){
                cout<<theGraph[i][j];
                cout<<" ";
            }
            cout<<"\n";
        }
    }
    float getvalue(int i, int j){
        return theGraph[i][j];
    }

    int calculateoutdegree(int column){
        int count = 0;
        for (int i = 0; i< MAXNUMVERTICES; i++){
            if  (theGraph[i][column] == 1){
                count++;
            }
        }
        return count;
    }

    void incorportateoutdegree(){
        int outdegree = 0;
        for (int j = 0; j< MAXNUMVERTICES; j++){ //column
            outdegree = calculateoutdegree(j);
            for (int i = 0; i<MAXNUMVERTICES;i++){
                if (outdegree != 0){
                    theGraph[i][j] = theGraph[i][j] / outdegree;
                }
            }
        }
    }
};

void userInput() {
    string line;
    int numberoflines = 0;
    int poweriterationstoperform = 0;
    list<string> vertices;
    Graph *adjMatrix;
    //The below array has all the unique URLS and will later have the final ranks concatenated and printed
    string *HashedUrls;
    int uniquevertices = 0;
    int linesread = 0;
    //The below boolean will be used when a power iteration of 1 is to be performed
    bool returnr0 = false;
    unordered_map<string, float> URLsandKeys;

    //The first line contains information for initialization, so we are handling this differently.
    getline(cin, line);
    int index = line.find(' ');
    //finds first integer
    while (index == 0){
        line = line.substr(1, line.length() - 1);
        index = line.find(' ');
    }

    numberoflines =  stoi(line.substr(0, index - 0 ));
    HashedUrls = new string[numberoflines * 2];
    adjMatrix = new Graph();
    adjMatrix->fill();

    //removes beginning characters of number of lines
    line = line.substr(index + 1, line.length() - index);
    index = line.find(' ');
    //finds next integer
    while (index == 0){
        line = line.substr(1, line.length() - 1);
        index = line.find(' ');
    }
    poweriterationstoperform = stoi(line.substr(0, index));
    //If power iterattions to perform exceeds 10, the output will be 0.
    if (poweriterationstoperform>=10){
        poweriterationstoperform = 10;
    }


    if (numberoflines <= 0 || poweriterationstoperform <= 0) {
        return;
    }

    //The below loop handles the rest of the lines
    while (linesread < numberoflines) {
        getline(cin, line);
        linesread++;

        string first = "";
        string second = "";

        //Deliminating by space
        //end is the index where the space is
        int end = line.find(' ');

        //handles case for if the first character in the string is a space and the string is not empty or is just a space
        while(end == 0 && line.length()!=0){
            line = line.substr(1, line.length() - 1);
            end = line.find(' ');
        }

        //handles if there is only one URL
        if(end == -1){
            first = line.substr(0, line.length());
        }
        else {

            first = line.substr(0, end);
        }

        //handles case for if there is more than one space in between URLS
        while(end == 0 && line.length()!=0){
            line = line.substr(1, line.length() - 1);
            end = line.find(' ');
        }


        if(end == -1){
            second = "";
        }
        else {
            second = line.substr(end + 1, line.length() - end);
        }

        bool hashed = false;
        int j;
        //checking if URL is hashed
        if (URLsandKeys.find(first) != URLsandKeys.end()){
            hashed= true;
        }

        //hash first URL if needed
        if (!hashed) {
            HashedUrls[uniquevertices+1] = first;
            URLsandKeys[first] = uniquevertices+1;
            uniquevertices++;
        }

        hashed = false;

        //checking if URL is hashed
        if (URLsandKeys.find(second) != URLsandKeys.end()){
            hashed= true;
        }

        //hash second URL if needed
        if (!hashed) {
            HashedUrls[uniquevertices+1] = second;
            URLsandKeys[second] = uniquevertices+1;
            uniquevertices++;
        }

        if(!second.empty()) {
            //adding edge
            //insert edge(to from) from is adjacent to to
            adjMatrix->insertEdge(URLsandKeys.at(first), URLsandKeys.at(second));

        }

    }

    //After all the lines are read

    if (linesread == numberoflines) {
        setNumberOfEdges(uniquevertices);
        adjMatrix->incorportateoutdegree();

        //Putting graph into another matrix to allow access by indexing
        float val = 0.0;
        float **adjacencyMatrix = new float *[uniquevertices];
        for (int i = 0; i < uniquevertices; i++) {
            adjacencyMatrix[i] = new float[uniquevertices];
        }
        for (int i = 1; i <= uniquevertices; i++) {
            for (int j = 1; j <= uniquevertices; j++) {
                val = adjMatrix->getvalue(i, j);
                if (isnan(val)){
                    val = 0.0;
                }
                adjacencyMatrix[i - 1][j - 1] = val;
                val = 0.00;
            }

        }

        //Power Iteration
        float sum = 0.0;
        string theURL = "";
        string thefloat = "";


        //Creating temp matrix and resultant matrix for matrix multiplication
        float **temp = new float *[uniquevertices];
        for (int i = 0; i < uniquevertices; i++) {
            temp[i] = new float[uniquevertices];
        }
        for (int i = 0; i < uniquevertices; i++) {
            for (int j = 0; j < uniquevertices; j++) {
                temp[i][j] = adjacencyMatrix[i][j];
            }
        }

        float **result = new float *[uniquevertices];
        for (int i = 0; i < uniquevertices; i++) {
            result[i] = new float[uniquevertices];
        }
        for (int i = 0; i < uniquevertices; i++) {
            for (int j = 0; j < uniquevertices; j++) {
                result[i][j] = 0;
            }
        }


        poweriterationstoperform--;

        int r1 = 0, c2 = 0 , k = 0;
        if (poweriterationstoperform == 0) {
            //Base Cases
            //Mr(0)
            returnr0 = true;
        } else if (poweriterationstoperform > 1){
            int powerperformed = 1;
            while (powerperformed < poweriterationstoperform) {

                //Matrix Multiplication
                for (r1 = 0; r1 < uniquevertices; r1++) {
                    for (c2 = 0; c2 < uniquevertices; c2++) {
                        //dot product
                        for (k = 0; k < uniquevertices; k++) {
                            result[r1][c2] += temp[r1][k] * adjacencyMatrix[k][c2];
                        }
                    }
                }
                //the power function is performed again with the resultant matrix and the original adjacency matrix
                for (int i = 0; i < uniquevertices; i++) {
                    for (int j = 0; j < uniquevertices; j++) {
                        temp[i][j] = result[i][j] ;
                    }
                }

                for (int i = 0; i < uniquevertices; i++) {
                    for (int j = 0; j < uniquevertices; j++) {
                        result[i][j] = 0 ;
                    }
                }
                powerperformed++;
            }
        }

        //Multuplying A ^ n times r(0)
        for (int i = 0; i < uniquevertices; i++) {
            sum = 0.0;
            for (int j = 0; j < uniquevertices; j++) {
                if(poweriterationstoperform == 0){
                    sum = float(1.00 / uniquevertices);
                    break;
                }
                else {
                    //dot product of resulting matrix with r(0)
                    sum += temp[i][j] * float(1.00 / uniquevertices);
                }
                if (isnan(sum)){
                    sum = 0.0;
                }
            }
            theURL = HashedUrls[i + 1];
            thefloat = to_string(round(sum*100)/100).substr(0,4);
            //if the power iterations to perform is 1, r(0) will be printed
            if(!returnr0){
                HashedUrls[i + 1] = theURL.append(" " + thefloat);
            }
            else if(returnr0){
                HashedUrls[i + 1] = theURL + " " + to_string(round((1.00/uniquevertices)*100)/100).substr(0,4);
            }
                //if a URL does not point to anything, print ranks of 0

        }

        //Alphabetizing the string of URLs with the concatenated ranks  using bubble sort
        bool swapped;
        do {
            swapped = false;
            for (int j = 1; j <= uniquevertices; j++) {
                if (HashedUrls[j - 1] > HashedUrls[j]) {
                    std::swap(HashedUrls[j - 1], HashedUrls[j]);
                    swapped = true;
                }
            }
        } while (swapped);

        for (int i = 0; i < uniquevertices; i++) {
            cout << HashedUrls[i + 1] << endl;
        }

        return;

    }
}

int main() {
    userInput();
    return 0;
}

