
#include<bits/stdc++.h>
#include<time.h>
using namespace std;

struct Board{//Data Structure for board state
   vector<vector<int>> board;
   char dir;
};

//All function prototypes

//To implement up and down move, we first rotate the board so that we can use the functionality of left right moves.
void rotate_board_c(vector<vector<int>>& board);//to rotate the board in clockwise direction
void rotate_board_a(vector<vector<int>>& board);//to rotate the board in anticlockwise direction

void print_board(vector<vector<int>> board);//to print the current state of the board.

bool isFull(vector<vector<int>> board);//Checks if the board is full and no moves are possible

void spawnRandom(vector<vector<int>> &board);//To randomly generate a 2 or 4 numbered tile in an empty position on the board.

int mergeRow(vector<int> &row, char d);//Takes a row from the board and returns the merged state of the board, depending on the direction of merge.

int MakeMove(vector<vector<int>> &board, char d);//Handles the left, right, up, down move on the board.

int findSum(vector<vector<int>> board);//Finds total sum of non-zero tiles of the board

void print_all_states(vector<Board> all_states);//Prints the path of board states from initial state to final state, including backtracked ones.

void initialize(vector<vector<int>> &board);//Calls spawnRandom twice to initialize the state of the board with two non-empty 2/4 tiles.

Board moveBoard(vector<vector<int>> board, char d);//Makes the left, right and up, down move by using rotation features.

//main starts

int main(){

    srand(time(0));//random seed according system time to get different output each time

    vector<vector<int>> board(4, vector<int>(4,0));//Board data structure
  	vector<Board> all_states;//Data structure to store the path starting from the initial state to target state
  	stack<Board> dfs_stack;//A stack for dfs

  	int stoppingSum = 2048;//stopping criteria

    initialize(board);//spawned two random tiles at the beginning

    vector<char> keys  = {'w','a','s','d'};//direction to explore from each state
    /*
    w: Move Up
    a: Move left
    s: Move Down
    d: Move Right
    */

    //Make an initial board structure and store in the stack.
  	Board initial;
  	initial.board = board;//initial board
  	initial.dir = 'n';//null direction

    dfs_stack.push(initial);//initial state in stack

    while(1){//Progress of the game engine

        //newboard stores the board at the top of the stack currently being explored
      	Board newboard;
      	newboard.board = dfs_stack.top().board;
      	newboard.dir = dfs_stack.top().dir;

      	all_states.push_back(newboard);//push the state of the board in all_states path data structure
      	dfs_stack.pop();//pop from the stack

      	if(findSum(newboard.board) == stoppingSum){//If sum of all non-zero tiles is exactly equal to stopping criteria (8 in our case), we stop our game engine
          	break;
        }

      	if(findSum(newboard.board) < stoppingSum){//If current board is eligible, we push all its children moves in the stack space.
         		for(auto key: keys){//Looping over four possible moves.
         		 		Board temp = moveBoard(newboard.board, key);
         		 		if(temp.board != newboard.board)//checks if the board is not stuck(i.e our state of board is changed). If not, only then push to the stack.
                            dfs_stack.push(temp);
        		}
        }

    }
    cout<<"Game Progress.";

    print_all_states(all_states);//Print the path followed by the game engine at the end.

    return 0;
}

//main ends

//Function definitions

void rotate_board_c(vector<vector<int>>& board){
    vector<vector<int>> temp_board(4, vector<int>(4));
    int R=3, C=3;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            temp_board[j][C-i]=board[i][j];
        }
    }
    board = temp_board;
}

void rotate_board_a(vector<vector<int>>& board){
    vector<vector<int>> temp_board(4, vector<int>(4));
    int R=3, C=3;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            temp_board[i][j]=board[j][C-i];
        }
    }
    board = temp_board;
}

void print_board(vector<vector<int>> board){
    cout<<"\n";
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            cout<<board[i][j]<<" ";
        }
        cout<<"\n";
    }
}

bool isFull(vector<vector<int>> board){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            if(board[i][j]==0){
                return false;
            }
        }
    }
    return true;
}

void spawnRandom(vector<vector<int>> &board){
    int randomx = rand()%4;//0..3
    int randomy = rand()%4;
    if(board[randomx][randomy]!=0){
        spawnRandom(board);
    }
    else{
      	int rand_arr[2] = {2,4};
        board[randomx][randomy] = rand_arr[rand()%2];
    }
}

int mergeRow(vector<int> &row, char d){
    int sc = 0;
    for(int i=0; i<row.size()-1; i++){
        if(row[i]==row[i+1]){//if adjacent equal
            sc+=row[i];
            row[i] += row[i+1];
            for(int j=i+1; j<row.size()-1; j++){//hops
                row[j] = row[j+1];
            }
            row.pop_back();
            if(row[i]==row[i+1]) i--;
        }
    }
    if(d=='d') reverse(row.begin(), row.end());
    return sc;
}

int MakeMove(vector<vector<int>> &board, char d){
    int sc=0;
    bool stateChanged = false;
    for(int i=0; i<4; i++){//each row
        vector<int> temp_row;//left shifted temp row
        for(int j=0; j<4; j++){//each column
            if(board[i][j]!=0){
                temp_row.push_back(board[i][j]);
            }
        }
        if(temp_row.size()==0){
            continue;
        }
        if(d=='d') reverse(temp_row.begin(), temp_row.end());
        vector<int> temp_row_before = board[i];
        int new_sc = mergeRow(temp_row, d);

        sc += new_sc;
        while(temp_row.size()<4){
            if(d=='a')
                temp_row.push_back(0);//fill to size
            if(d=='d')
                temp_row.insert(temp_row.begin(),0);//fill to size
        }
        if(temp_row_before != temp_row){
            stateChanged = true;
        }
        board[i] = temp_row;//replace old row with new
    }
    if(!isFull(board) && stateChanged){
        spawnRandom(board);
    }
    return sc;
}

int findSum(vector<vector<int>> board){
    int sum=0;
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            sum += board[i][j];
        }
    }
    return sum;
}

void print_all_states(vector<Board> all_states){
    map<char,string> mp;
    mp['a']="left"; mp['d']="right"; mp['w']="up"; mp['s']="down";
    for(int i=0; i<all_states.size(); i++){
        cout<<"\n"<<mp[all_states[i].dir]<<"\n";
        print_board(all_states[i].board);
        int sum = findSum(all_states[i].board);
        cout<<"\nSum: "<<sum<<"\n";
        if(sum>2048) cout<<"As sum > 2048, Now we will Backtrack.\n";

    }
}

void initialize(vector<vector<int>> &board){
    spawnRandom(board);
    spawnRandom(board);
}

Board moveBoard(vector<vector<int>> board, char d){
        if(d=='w'){
            rotate_board_a(board);
            d='a';
            MakeMove(board, d);
            rotate_board_c(board);
            d='w';
        }
        else if(d=='s'){
            rotate_board_c(board);
            d='a';
            MakeMove(board, d);
            rotate_board_a(board);
            d='s';
        }
        else if(d=='a' || d=='d')
            MakeMove(board, d);

        Board newboard;
        newboard.board = board;
        newboard.dir = d;

        return newboard;
}
