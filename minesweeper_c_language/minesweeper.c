#include<stdio.h>
#define FILENAME "field.txt"
#define FILENAME2 "scores.txt"
#define MAXROWS 8
#define MAXCOLS 8
#define MAXCOLS_score 2

void display_current_board(char array[][MAXCOLS]);
void display_scores(FILE*, int, int*, int, char array[][MAXCOLS_score]);
void Get_cell(int*, int*);
void Get_move(char*);
void Mark_flag(int, int, char array[][MAXCOLS]);
void Get_mine_board(FILE*, int, int, int array[][MAXCOLS]);
void clear_board(char array[][MAXCOLS]);
int Uncover_one_cell(int, int, char array[][MAXCOLS], int array2[][MAXCOLS]);
int Play_again();
int Check_surrounding_cells(int, int, char array[][MAXCOLS], int array2[][MAXCOLS]);
int Check_player_win(int*, char array[][MAXCOLS]);

int main(){
int i, j, cell_x, cell_y, index=0, end=0, check=0, score=0, win_check=0,mine_board[MAXROWS][MAXCOLS];
char move,name[20],scores[20][MAXCOLS_score],
    current_board[MAXROWS][MAXCOLS]=
    {{' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},
    {' ',' ',' ',' ',' ',' ',' ',' '},};



FILE* fP, *sc;
    fP = fopen(FILENAME, "r");
    sc = fopen(FILENAME2, "r");

    if(fP != NULL && sc != NULL){
        Get_mine_board(fP, MAXROWS, MAXCOLS, mine_board);
        display_scores(sc, 0, &index, MAXCOLS_score, scores);
        fclose(fP);
        fclose(sc);
        }
   
    while(1){
        
        display_current_board(current_board);

        while(!end){
            Get_cell(&cell_x, &cell_y);
            if (cell_x < 9 && cell_y < 9){
                end = 1;
            }
        }

        end = 0;

        while(!end){
            Get_move(&move);
            switch (move)
            {
            case 'M':
                Mark_flag(cell_x, cell_y, current_board);
                end = 1;
                break;
            case 'U':
                check = Uncover_one_cell(cell_x, cell_y, current_board, mine_board);
                end = 1;
                break;
            case 'C':
                check = Check_surrounding_cells(cell_x, cell_y, current_board, mine_board);
                end = 1;
                break;
            default:
                break;
            }
        }
        
        end = 0;
        
        if(check){
            display_current_board(current_board);
            printf("You lose :(\n");

            if(Play_again()){
                printf("What is your name?");
                scanf("%s%*c", &name);
                sc = fopen(FILENAME2, "a");
                if(sc != NULL){
                    fprintf(sc, "%s %d\n", name, score);
                    fclose(sc);
                }
                sc = fopen(FILENAME2, "r");
                if(sc != NULL){
                    display_scores(sc, 1, &index, MAXCOLS_score, scores);
                    fclose(sc);
                }
                break;
            }
            else{
                clear_board(current_board);
            }    
        }
        
        check = 0;
        win_check = Check_player_win(&score, current_board);

        if(win_check){
            display_current_board(current_board);
            printf("You win!\n");
            if(Play_again()){
                printf("What is your name?");
                scanf("%s%*c", &name);
                sc = fopen(FILENAME2, "a");
                if(sc != NULL){
                    fprintf(sc, "%s %d\n", name, score);
                    fclose(sc);
                }
                sc = fopen(FILENAME2, "r");
                if(sc != NULL){
                    display_scores(sc, 1, &index, MAXCOLS_score, scores);
                    fclose(sc);
                }
                break;
            }
            else{
                clear_board(current_board);
            } 
        }
        
        
    }
    return 0;
} 

void display_current_board(char board[][MAXCOLS]){
    int i;
    printf("    1   2   3   4   5   6   7   8  \n");
    printf("  ---------------------------------\n");
    for (i=0; i < MAXROWS; i++){
        printf("%d | %c | %c | %c | %c | %c | %c | %c | %c |\n", i+1,
        board[i][0], board[i][1], board[i][2], board[i][3],
        board[i][4], board[i][5], board[i][6], board[i][7]);
        printf("  ---------------------------------\n");
    }
}

void Get_mine_board(FILE* fP, int maxRows, int maxCols, int data[][MAXCOLS]){ 
    int rowIndex = 0;
    while(fscanf(fP, "%d%d%d%d%d%d%d%d", &data[rowIndex][0],&data[rowIndex][1],&data[rowIndex][2],&data[rowIndex][3],
    &data[rowIndex][4],&data[rowIndex][5],&data[rowIndex][6],&data[rowIndex][7]) == maxCols && rowIndex < maxRows+1){
        
        rowIndex++;
    }

}

void display_scores(FILE* fP, int select, int *index,  int maxCols, char data[][MAXCOLS_score]){ 
    int i, score;
    char name[20];
    switch (select)
    {
    case 0:
        printf("BEAT THE HIGH SCORES!\n");
        while(fscanf(fP, "%s %d", &name,&score) == maxCols){
        printf("%s",name);
        printf(":%d\n",score);
        *index++;
        }
        break;
    case 1:
        printf("Winners!\n");
        while(fscanf(fP, "%s %d", &name,&score) == maxCols){
        printf("%s",name);
        printf(":%d\n",score);
        *index++;
        }
        break;
    }
    
    
} 

void Get_cell(int * cell_x, int * cell_y){
    printf("Which cell?");
    scanf("%d%*c %d%*c", cell_x, cell_y);
}

void Get_move(char *move){
    printf("(M)ark this cell with a bomb flag\n");
    printf("(U)ncover to reveal what's in this cell\n");
    printf("(C)heck the surrounding cells\n");
    printf("Which move?");
    scanf("%c%*c", move);
}


void Mark_flag(int cell_x, int cell_y, char board[][MAXCOLS]){
    board[cell_x - 1][cell_y - 1] = 'F';
}

int Uncover_one_cell(int cell_x, int cell_y, char board[][MAXCOLS], int board2[][MAXCOLS]){
    
    if (board2[cell_x - 1][cell_y - 1] == 0){
        board[cell_x - 1][cell_y - 1] = '_';
        return 0;
    }
    else if (board2[cell_x - 1][cell_y - 1] == 99){
        board[cell_x - 1][cell_y - 1] = 'X';
        return 1;
    }
    else{
        board[cell_x - 1][cell_y - 1] = board2[cell_x - 1][cell_y - 1]+48;
        return 0;
    }
}

int Check_surrounding_cells(int cell_x, int cell_y, char board[][MAXCOLS], int board2[][MAXCOLS]){
int i,j, bomb=0;
    for (i=0; i<3; i++){
        for (j=0; j<3; j++){
            if(cell_x-j > -1 && cell_x-j < 9 && cell_y-i > -1 && cell_y-i < 9){
                if (board2[cell_x-j][cell_y-i] == 0){
                    board[cell_x-j][cell_y-i] = '_';
                }
                else if (board2[cell_x-j][cell_y-i] == 99){
                    board[cell_x-j][cell_y-i] = 'X';
                    bomb = 1;
                }
                else{
                    board[cell_x-j][cell_y-i] = board2[cell_x-j][cell_y-i]+48;
                }
            }
        }
    }
    return bomb;
}

int Check_player_win(int *score, char board[][MAXCOLS]){
int i, j, win =1, br=0;
    for (i=0; i<8; i++){
        for (j=0; j<8; j++){
            if (board[i][j] == ' '){
                win = 0;
                br = 1;
                break;   
            }
        if (br){
            break;
        }
        }
    }
    if (win){
        *score = *score + 1;
        printf("%d", *score);
    }
    return win;
}

int Play_again(){
int end=0;
char select;
    while(!end){
    printf("Play again (Y/N):");
    scanf("%c%*c", &select);
    if (select =='y' | select =='n' | select =='Y' | select =='N'){
        end = 1;
    }
    }
    if (select =='y' | select =='Y'){
        return 0;
    }
    else if(select =='n' | select =='N'){
        return 1;
    }
}

void clear_board(char board[][MAXCOLS]){
int i, j;
    for (i=0; i<8; i++){
        for (j=0; j<8; j++){
            board[i][j] = ' ';
        }
    }
}


