#include <iostream>
#include <cstring>
#include <string>
#include <random>

const int N_MAX = 10001;
int rowIndex[N_MAX]{};
int queensInRow[N_MAX]{};
int queensInMains [2*N_MAX-1]{};
int queensInSecondaries [2*N_MAX-1]{};
int N = -1;

int maxSideways = 25;

unsigned int seed = std::random_device{}();
std::default_random_engine generator (seed);

//O(N)
std::string print(){
    std::string result (N*(N+1),'_');
    for (int i=0;i<N;i++){
        result[N*(i+1)+i] = '\n';
        result[(N+1)*rowIndex[i]+i] = '*';
    }
    return result;
}

//O(1)
// conflicts for a queen = count of queens in the row + count of queens in one diagonal + count of queens in other diagonal - 3
inline int getConflicts(int column){
    return (queensInRow[rowIndex[column]] + queensInSecondaries[column+rowIndex[column]] + queensInMains[rowIndex[column]-column+N-1] - 3);
}

//O(1)
//reduce row, diag1, diag2 count of queens containing current queen,
//move queen to new location,
//increase row,diag1,diag2 of new position of queen
inline void setQueenInRow(int column,int row){

    queensInRow[rowIndex[column]]--;
    queensInSecondaries[column+rowIndex[column]]--;
    queensInMains[rowIndex[column]-column+N-1]--;

    rowIndex[column] = row;

    queensInRow[rowIndex[column]]++;
    queensInSecondaries[column+rowIndex[column]]++;
    queensInMains[rowIndex[column]-column+N-1]++;

}

int main(int argc, const char** argv)
{
    //if provided take N from command line argument
    if (argc == 2){
        N = atoi(*(argv+1));
    }//else get N from stdin
    else{
        std::cin>>N;
    }
    if (N<=3){
        std::cout<<"N has to be more than three...\n";
        return 0;
    }

    memset(rowIndex,0,N_MAX*sizeof(int));
    memset(queensInRow,0,N_MAX*sizeof(int));
    memset(queensInMains,0,(2*N_MAX-1)*sizeof(int));
    memset(queensInSecondaries,0,(2*N_MAX-1)*sizeof(int));

    std::uniform_int_distribution<int> distribution(0,N-1);

    //O(N)
    for (int column=0;column<N;column++){
        rowIndex[column] = distribution(generator);
        queensInRow[rowIndex[column]]++;
        queensInSecondaries[column+rowIndex[column]]++;
        queensInMains[rowIndex[column]-column+N-1]++;
    }

    int sideways = 0;
    std::uniform_int_distribution<int> toss(0,1);

    //O(N) for each iteration
    //while not in goal Board
    while (true){
        //if enough iterations have been made without reducing conflicts, make a random move to prevent local minimums
        if (sideways > maxSideways){
            setQueenInRow( distribution(generator) , distribution(generator) );
            sideways = 0;
        }

        int currentMax = 0;
        int maxConflictColumn = 0;

        for (int columnIndex=0;columnIndex<N;columnIndex++){
            int conflicts = getConflicts(columnIndex);
            //if better , change current max
            if (conflicts > currentMax){
                currentMax = conflicts;
                maxConflictColumn = columnIndex;
            }
            //if equal, choose one of them at random
            else if(conflicts == currentMax){
                if(toss(generator)){
                    maxConflictColumn = columnIndex;
                }
            }
        }

        //if highest conflict column has 0 conflicts, we 're done
        if (getConflicts(maxConflictColumn) == 0){
            break;
        }

        int currentMin = getConflicts(maxConflictColumn);
        int currentMinIndex = rowIndex[maxConflictColumn];
        bool reduced = false;
        //find minimal row of maxConflictColumn
        for (int index=0;index<N;index++){
            setQueenInRow(maxConflictColumn,index);
            //get number of conflicts for that row
            int conflicts = getConflicts(maxConflictColumn);
            //if better , change current min
            if (conflicts < currentMin){
                reduced = true;
                currentMin = conflicts;
                currentMinIndex = index;
            }
            //if equal, choose one of them at random
            else if(conflicts == currentMin){
                if(toss(generator)){
                    currentMinIndex = index;
                }
            }
        }
        //if conflicts have been reduced
        if (reduced){
            sideways = 0;
        }
        //if not
        else{
            sideways++;
        }
        //move the queen in the maxConflict column to one of the least conflict rows in it
        setQueenInRow(maxConflictColumn,currentMinIndex);
    }

    //print the result
    std::cout<<print()<<std::endl;  
      
    return 0;
}
