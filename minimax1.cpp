#include <bits/stdc++.h>
using namespace std;

struct Move{
    int row, col;
};

char player='x', opponent='o';

bool isMovesLeft(char board[3][3]){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            if(board[i][j]=='_')
                return true;
        }
    }
    return false;
}

int evaluate(char board[3][3]){
    for(int r=0; r<3; r++){
        if(board[r][0]==board[r][1] && board[r][1]==board[r][2]){
            if(board[r][0]==player)
                return 10;
            else if(board[r][0]==opponent)
                return -10;
        }
    }
    for(int c=0; c<3; c++){
        if(board[0][c]==board[1][c] && board[1][c]==board[2][c]){
            if(board[0][c]==player)
                return 10;
            else if(board[0][c]==opponent)
                return -10;
        }
    }
    if(board[0][0]==board[1][1] && board[1][1]==board[2][2]){
        if(board[0][0]==player)
            return 10;
        else if(board[0][0]==opponent)
            return -10;
    }
    if(board[0][2]==board[1][1] && board[1][1]==board[2][0]){
        if(board[0][2]==player)
            return 10;
        else if(board[0][2]==opponent)
            return -10;
    }
    return 0;
}

int minimax(char board[3][3], int depth, bool isMax, int alpha, int beta){
    //isMax true = player, isMax false = opponent
    int score = evaluate(board);//evaluate checks current state,
    //if player won, opponent won or ongoing/draw
    if(score==10 || score==-10)
        return score;
    if(isMovesLeft(board)==false)
        return 0;
    if(isMax){
        int best=-1000;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j]=='_'){//if cell empty
                    board[i][j]=player;
                    best = max(best, minimax(board, depth+1, !isMax, alpha, beta));
                    board[i][j]='_';//undo
                    alpha = max(alpha, best);
                    if(beta<=alpha)
                        return best;
                }
            }
        }
        return best;
    }
    else{
        int best=1000;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j]=='_'){
                    board[i][j]=opponent;
                    best = min(best, minimax(board, depth+1, !isMax, alpha, beta));
                    board[i][j]='_';//undo
                    beta = min(beta, best);
                    if(beta<=alpha)
                        return best;
                }
            }
        }
        return best;
    }
}

Move findBestMove(char board[3][3], bool p){//player move
    Move bestMove;
    bestMove.row=-1;
    bestMove.col=-1;
    if(p){
        int bestVal = -1000;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j]=='_'){//if a cell is empty
                    board[i][j] = player;
                    int moveVal = minimax(board, 0, false, -1000, 1000);//compute for this move
                    board[i][j]='_';//reset to empty for further eval

                    if( moveVal > bestVal){
                        bestMove.row = i;
                        bestMove.col = j;
                        bestVal = moveVal;
                    }
                }
            }
        }
    }
    else{
        int bestVal = 1000;
        for(int i=0; i<3; i++){
            for(int j=0; j<3; j++){
                if(board[i][j]=='_'){
                    board[i][j] = opponent;
                    int moveVal = minimax(board, 0, true, -1000, 1000);
                    board[i][j]='_';

                    if(moveVal < bestVal){
                        bestMove.row = i;
                        bestMove.col = j;
                        bestVal = moveVal;
                    }
                }
            }
        }
    }
    //cout<<"\nValue for best move is: "<<bestVal;
    return bestMove;//row and col of bestMov
}

void printBoard(char board[3][3]){
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            cout<<board[i][j]<<" ";
        }
        cout<<"\n";
    }
}

//Driver function
int main(){
    char board[3][3]={
    {'_','_','_'},
    {'_','_','_'},
    {'_','_','_'}
    };
    char user;
    cout<<"Enter character to play: ";
    cin>>user;
    printBoard(board);
    char ai = (user=='x'?'o':'x');
    bool p = true;//start with x
    char playing='x';
    while(isMovesLeft(board)==true && evaluate(board)!=10 && evaluate(board)!=-10){
        cout<<"Player: "<<(p==true?'x':'o')<<"\n";
        if(user==playing){//user
            cout<<"Enter row and column: ";
            int row,col;
            cin>>row>>col;
            if(board[row-1][col-1]!='_'){
                continue;
            }
            board[row-1][col-1] = user;
            playing=ai;
        }
        else{//ai
            Move bestMove= findBestMove(board,p);
            board[bestMove.row][bestMove.col] = ai;
            playing=user;
        }
        printBoard(board);
        p = !p;
    }
    int state = evaluate(board);
    if(state==0)
        cout<<"\nDRAW";
    else if(state==10)
        cout<<"\nX win!";
    else
        cout<<"\O win!";
    return 0;
}
