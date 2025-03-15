#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <conio.h>
//more about VK at : 
//https://www.amibroker.com/guide/afl/getasynckeystate.html

typedef struct cell {
	int type;
	int discovered;
}cell;

/* ( letter, background )
0 = background black|1 = blue|2 = green|3 = light blue|4 = red|5 = purple|6 = dark yellow|7 = letter white|8 = grey|
9 = strong sky blue|10 = bright-ish green|11 = cyan|12=weak red|13 weird purple|14 = deseart yellow|15 = bright white*/
void SetColorAndBackground(int ForgC, int BackC);

void gotoxy(int x, int y);

void discover_blank( cell map[15][15], int i, int j, int map_size);

int generate_bombs(cell map[15][15], float total_bombs, int map_size);

void generate_nums( cell map[15][15], int map_size);

void printingTable( int color, cell map[15][15], int game_over, int map_size);

int main(){
	
	
	cell map[15][15];
	
	setlocale(LC_ALL, "portuguese");
	
	int i, j;
	time_t tempo;
	float total_bombs = 14, main_bombs; 
	char sign[3];
	int sign_value, map_size = 15, sel_x, sel_y, total_flags, bombs_found;
	int colors = 1, game_over;//int's as booleans
	//Opening Game Screen
	system("cls");
	gotoxy(60 - (strlen("Minesweeper") / 2), 14);
	printf("Minesweeper");
	gotoxy(60- (strlen("press any key to continue") / 2), 16);
	printf("press any key to continue");
	getch();
	while ( GetAsyncKeyState(VK_RETURN) <0){
		//wait
	}
	
	for ( ; ; ){
		system("cls");
		sign_value = 0;
		for ( ; ; ){ //Main Menu ( play, menu, quit)
			
			for ( i = 0; i < sizeof(sign[0] )* sizeof(sign) ; i++)
				if ( i != sign_value)
					sign[i] = ' ';	
				else
				sign[i] = 'X';
				
			
			gotoxy(60 - (strlen("minesweeper") / 2), 14);
			printf("Minesweeper");
			gotoxy(30- ( strlen("Play") / 2), 22);
			printf("Play %c", sign[0]);
			gotoxy(60- ( strlen("Menu") / 2), 22);
			printf("Menu %c", sign[1]);
			gotoxy(90- ( strlen("Quit") / 2), 22);
			printf("Quit %c", sign[2]);
			
			if ( GetAsyncKeyState( VK_RETURN) &0x25){
				while ( GetAsyncKeyState( VK_RETURN) <0){ 
					//wait
				}
				break;
			}
			
			if ( ( GetAsyncKeyState(65 /*VK_A*/ ) &0x25 || GetAsyncKeyState(VK_LEFT) &0x25 )&& sign_value != 0)
				sign_value--;
			if ( ( GetAsyncKeyState(68 /*VK_D*/ ) &0x25 || GetAsyncKeyState(VK_RIGHT) &0x25 )&& sign_value != 2)
				sign_value++;
			
			gotoxy(119, 29);
			
			usleep(25000);
			
		}
		system("cls");
		//Quit
		if ( sign_value == 2)
			break;
		
		//Menu 
		if ( sign_value == 1){
			sign_value = 0;
			
			for ( ; ; ){
				system("cls");
				gotoxy(60 - (strlen("Menu")/ 2), 5);
				printf("Menu");
				gotoxy(60 - ( strlen("Map Size:")), 9);
				printf("Map Size:   -   %d   + ", map_size);
				gotoxy(60 - ( strlen("Percentage of bombs:")), 13);
				printf("Percentage of bombs:   -   %.0f    + ", total_bombs);
				gotoxy(60 - ( strlen("Colors:")), 17);
				printf("Colors: off         on");
				gotoxy(60 + strlen(" off   "), 17);
				if ( colors == 0)
					printf("OFF");
				else
				printf("ON");	
				gotoxy(60 - ( strlen("Back")), 20);
				printf("Back");
				switch ( sign_value ){
					
					case 0:
					gotoxy(60 +  strlen("   -  ")-1, 9);
					printf("[");
					gotoxy(60 +  strlen("   -  ") +4, 9);
					printf("]");
					break;
				
					case 1 :
					gotoxy(60 +  strlen("   -  ")-1, 13);
					printf("[");
					gotoxy(60 +  strlen("   -  ") +4, 13);
					printf("]");
					break;
					
					case 2 : 
					gotoxy(60 + strlen(" off   ") -1, 17);
					printf("[");
					gotoxy(60 + strlen(" off   ") +4, 17);
					printf("]");
					break;
					
					case 3 :
					gotoxy(64, 20);
					printf("[ENTER/Q]");	
					break;
				}
				
				gotoxy(119, 29);
				
				for ( ; ; ){
				
					if (  (GetAsyncKeyState( 87/*VK_W*/) &0x25 || GetAsyncKeyState( VK_UP) &0x25 ) && sign_value != 0  ) {
						sign_value--;
						break;
					}
						
					if ( GetAsyncKeyState( 68 /*VK_D*/) &0x25 || GetAsyncKeyState( VK_RIGHT) &0x25 || GetAsyncKeyState(107 /*add sign NUMPADy*/) &0x25){
						
						if ( sign_value == 0 && map_size != 15)
							map_size++;
						else if ( sign_value == 1 && total_bombs != 100)
							total_bombs++;
						else if ( sign_value == 2)	
							colors = 1;	
						
						break;
					}
					
					if (  (GetAsyncKeyState( 83 /*VK_S*/) &0x25 || GetAsyncKeyState( VK_DOWN) &0x25 ) && sign_value != 3 ){
						sign_value++;
						break;
					}
					
					if ( GetAsyncKeyState( 65 /*VK_A*/) &0x25 || GetAsyncKeyState( VK_LEFT) &0x25  || GetAsyncKeyState(109 /*subtract sign NUMPAD*/) &0x25){
						
						if ( sign_value == 0 && map_size != 5)
							map_size--;
						else if ( sign_value == 1 && total_bombs != 1)
							total_bombs--;
						else if ( sign_value == 2)	
							colors = 0;	
						break;
					}
					
					if ( GetAsyncKeyState( 81 /*VK_Q*/) &0x25 || GetAsyncKeyState(VK_RETURN) <0 && sign_value == 3){
						sign_value= 10;
						break;
					}
				
				
				}
				if ( sign_value == 10)
					break;
			}
			sign_value = 1;
		}
		
		//Play
		if ( sign_value == 0){
			for ( i = 0; i < map_size; i++)
				for ( j = 0; j < map_size; j++){
					map[i][j].type = 0;
					map[i][j].discovered = 0;
				}
			
			sel_x = 0;
			sel_y = 0;
			game_over = 0;
			total_flags = 0;
			bombs_found = 0;
			srand((unsigned) time(&tempo));
			total_bombs = generate_bombs(map, total_bombs, map_size);
			generate_nums(map, map_size);
			
			for ( ; ; ){
				//creating map
				//printing table
				for ( i = 0; i < map_size +1; i++){
					gotoxy(44, 4+ i);
					printf("#");
					gotoxy( ( 44 + (  map_size * 2)  )+1, 5+ i);
					printf(" #");
					gotoxy(45 + ( i *2), 4);
					printf("##");
					gotoxy(44 + ( i *2), map_size +5);
					printf("##");
				}
				printf("#");
				
				gotoxy(100- ( strlen("Controls:") /2), 4);
				printf("Controls:");
				gotoxy(100 - ( strlen("break block : SPACE, 'B'") / 2), 5);
				printf("break block : SPACE, 'B'");
				gotoxy(100 - ( strlen("Place flag : 'F'") / 2), 6);
				printf("place flag : 'F'");
				gotoxy(100- (strlen("Give up : 'Q'") /2), 7);
				printf("give up : 'Q'");
				gotoxy(100 - ( strlen("total bombs : xx") / 2), 10);
				printf("total bombs : %.0f", total_bombs);
				gotoxy(100- (strlen("flags missing : %d") /2), 11);
				printf("flags missing : %.0f  ", total_bombs -total_flags );
				gotoxy(100- ( strlen("cell : x = %d / y = %d ") /2), 13);
				printf("cell : x = %d / y = %d ",sel_x , sel_y);
				
				printingTable(colors, map, game_over, map_size);
				
				
				gotoxy(45 + (sel_x *2), 5 +sel_y);
				printf("[");
				gotoxy( (45 + ( sel_x * 2) ) +2, 5 + sel_y);
				printf("]");
				gotoxy(119,29);
				
				
				for ( ; ; ){
					
					if ( (GetAsyncKeyState( 87/*VK_W*/) &0x25 || GetAsyncKeyState( VK_UP) &0x25 )&& sel_y != 0){
						sel_y--;
						break;
					}
						
					if ( (GetAsyncKeyState( 68 /*VK_D*/) &0x25 || GetAsyncKeyState( VK_RIGHT) &0x25)  && sel_x != map_size -1){
						sel_x++;
						break;
					}
					
					if ( (GetAsyncKeyState( 83 /*VK_S*/) &0x25 || GetAsyncKeyState( VK_DOWN) &0x25)  && sel_y != map_size -1){
						sel_y++;
						break;
					}
					
					if ( (GetAsyncKeyState( 65 /*VK_A*/) &0x25 || GetAsyncKeyState( VK_LEFT) &0x25)  && sel_x != 0){
						sel_x--;
						break;
					}
					
					if ( GetAsyncKeyState( 81 /*VK_Q*/) &0x25){
						game_over = 1;
						break;
					}
						
					if ( GetAsyncKeyState( 70 /*VK_F*/) &0x25){
						if ( map[sel_y][sel_x].discovered == 0 ){
							if ( map[sel_y][sel_x].type == -1){
								bombs_found++;
							}
							map[sel_y][sel_x].discovered = 2;
							total_flags++;
						}
						else if ( map[sel_y][sel_x].discovered == 2){
							if ( map[sel_y][sel_x].type == -1){
								bombs_found--;
							}
							map[sel_y][sel_x].discovered = 0;
							total_flags--;
						}
						
							
						break;
					}
					
					
					if ( ( GetAsyncKeyState(66) &0x25 || GetAsyncKeyState( VK_SPACE) &0x25) ){
						map[sel_y][sel_x].discovered = 1;
						if ( map[sel_y][sel_x].type == 0)
							discover_blank(map, sel_y, sel_x, map_size);
						else
						if ( map[sel_y][sel_x].type == -1)
							game_over = 1;	
						break;
					}
					
					
				}
				
				if ( total_bombs == bombs_found && total_bombs -total_flags == 0)
					game_over = 2;
				
				if ( game_over != 0)
						break;
				
				
			}
			//game_over
			system("cls");
			if ( game_over == 1){
				gotoxy(60 -( strlen("you lost") /2), 25);
				printf("you lost");
			}
			else{
				gotoxy(60 -(strlen("you won") /2), 25);
				printf("you won");
			}
			for ( i = 0; i < map_size +1; i++){
					gotoxy(44, 4+ i);
					printf("#");
					gotoxy(( 44 + (  map_size * 2)  )+1, 5+ i);
					printf(" #");
					gotoxy(45 + ( i *2), 4);
					printf("##");
					gotoxy(44 + ( i *2), 5 +map_size);
					printf("##");
				}
			printf("#");
			
			printingTable(colors, map, game_over, map_size);
			gotoxy(60- (strlen("press ENTER to continue") / 2), 28);
			printf("press ENTER to continue");
			gotoxy(119, 29);
			
			
			for ( ; ; ){
				if ( GetAsyncKeyState( VK_RETURN) &0x25){
					while ( GetAsyncKeyState( VK_RETURN) < 0 ){
						//wait
					}
					break;
				}
			}
			total_bombs =  total_bombs * 100 / (map_size * map_size ) ;
			
			for ( i = 0; i < map_size; i++)
				for ( j = 0; j < map_size; j++){
					map[i][j].type = 0;
					map[i][j].discovered = 0;
				}
					
		
			
			
		}
		
	}
	
	gotoxy(60 - (strlen("minesweeper") / 2), 14);
	printf("Minesweeper");
	gotoxy(60- (strlen("Thank you for playing") / 2), 16);
	printf("Thank you for playing");
	
	sleep(2);
	
	
	
	return ;
	
	
	
}

void printingTable( int color, cell map[15][15], int game_over, int map_size){
	int i, j;
	
	for (i = 0; i < map_size; i++){   
		gotoxy(45, 5 + i);
		for ( j = 0; j < map_size; j++)
			if ( map[i][j].discovered == 0 && game_over == 0)
				printf(" -");
			else{
				if ( map[i][j].discovered == 2){
					if ( color != 0)
						SetColorAndBackground( 6,0);
					printf(" F");
					SetColorAndBackground( 7, 0);
				}
				else if ( map[i][j].type == -1 ){
					if ( color != 0)
						SetColorAndBackground(10, 0);
					printf(" *");
					SetColorAndBackground( 7, 0);
				}
				else	
				if ( map[i][j].type == 0)
					printf("  ");
				else{
					if ( color == 1){
						
						switch( map[i][j].type){
							
							case 1 :
							SetColorAndBackground( 3,0);
							break;
							
							case 2 : 
							SetColorAndBackground( 11, 0);
							break;
							
							case 3 : 
							SetColorAndBackground( 9, 0);
							break;
							
							case 4 : 
							SetColorAndBackground( 1, 0);
							break;
								
							case 5 : 
							SetColorAndBackground( 13, 0);
							break;
							
							case 6 : 
							SetColorAndBackground( 5, 0);
							break;
							
							case 7 : 
							SetColorAndBackground( 12, 0);
							break;	
							
							case 8 :
							SetColorAndBackground( 4, 0);
							break;
							
							default : 
							SetColorAndBackground( 7, 0);
						}
							
							
					}
					printf(" %d", map[i][j].type);
					SetColorAndBackground( 7, 0);
					
				}	
			}
				
	}
	
	
}

void discover_blank( cell map[15][15],int i,int j, int map_size){
	
	if ( i != 0 ){
		// i -1 = the top cell
		if ( map[i-1][j].type == 0 && map[i-1][j].discovered == 0){
			map[i -1][j].discovered = 1;
			discover_blank( map, i-1, j, map_size);
		}
		if ( map[i-1][j].type > 0 && map[i-1][j].discovered == 0)
			map[i -1][j].discovered = 1;
		
		// i -1 && j -1 = the top left cell
		if ( j != 0 && map[i -1][j -1].type == 0 && map[i-1][j-1].discovered == 0){
			map[i-1][j -1].discovered = 1;
			discover_blank( map, i-1, j-1, map_size);
		}
		if ( j != 0 && map[i -1][j -1].type > 0 && map[i-1][j-1].discovered == 0)
			map[i-1][j -1].discovered = 1;
		
		// i -1 && j +1 = the top right cell
		if ( j != map_size -1 && map[i -1][j +1].type == 0 && map[i -1][j +1].discovered == 0){
			map[i-1][j+1].discovered = 1;
			discover_blank( map,i-1, j+1, map_size);
		}
		if ( j != map_size -1 && map[i -1][j +1].type > 0 && map[i -1][ j +1].discovered == 0)
			map[i-1][j+1].discovered = 1;					
							
	}
	
	// j -1 = the left cell
	if ( j != 0 && map[i][j-1].type == 0 && map[i][j-1].discovered == 0){
		map[i][j-1].discovered = 1;
		discover_blank( map,i ,j-1, map_size);
	}
	if ( j != 0 && map[i][j-1].type > 0 && map[i][j-1].discovered == 0)
		map[i][j-1].discovered = 1;
	
	// j +1 = the right cell
	if ( j != map_size -1 && map[i][j+1].type == 0 && map[i][j+1].discovered == 0){
		map[i][j+1].discovered = 1;
		discover_blank( map,i , j+1, map_size);
	}
	if ( j != map_size -1 && map[i][j+1].type > 0 && map[i][j+1].discovered == 0)
		map[i][j+1].discovered = 1;
		
	if ( i != map_size -1){
		// i +1 = the bottom cell
		if ( map[i+1][j].type == 0 && map[i+1][j].discovered == 0){
			map[i+1][j].discovered = 1;
			discover_blank( map, i+1, j, map_size);
		}
		if ( map[i+1][j].type > 0 && map[i+1][j].discovered == 0)
			map[i+1][j].discovered = 1;
		
		// i +1 && j -1 = the bottom left cell
		if ( j != 0 && map[i+1][j-1].type == 0 && map[i+1][j-1].discovered == 0){
			map[i+1][j-1].discovered = 1;
			discover_blank( map, i+1, j-1, map_size);
		}
		if ( j != 0 && map[i+1][j-1].type > 0 && map[i+1][j-1].discovered == 0)
			map[i+1][j-1].discovered = 1;
		
		// i +1 && j +1 = the bottom right cell
		if ( j != map_size -1 && map[i+1][j+1].type == 0 && map[i+1][j+1].discovered == 0){
			map[i+1][j+1].discovered = 1;
			discover_blank( map, i+1, j+1, map_size);
		}	
		if ( j != map_size -1 && map[i+1][j+1].type > 0 && map[i+1][j+1].discovered == 0)
			map[i+1][j+1].discovered = 1;
	}		
	
 	return;
}


int generate_bombs(cell map[15][15], float total_bombs, int map_size){
	time_t tempo;
	srand((unsigned) time(&tempo));
	int bombs = 0, i, j;
	
	for ( ; ; ){
		
		if ( bombs == ceil( total_bombs / 100 * (map_size * map_size ) ) )
			break;
		else{
			for ( i= 0; i <map_size; i++){
				for ( j = 0; j < map_size; j++){		
					if( rand()% 99 <= total_bombs -1 && map[i][j].type != -1 &&   bombs < ceil( (total_bombs / 100) * (map_size * map_size ) )  ){
						map[i][j].type = -1;
						bombs++;
					}
					else 
					if ( map[i][j].type != 0 && map[i][j].type != -1 )
						map[i][j].type = 0;
					map[i][j].discovered = 0;
				}
			}
		}	
	}
	
	return bombs ;
}

void generate_nums( cell map[15][15], int map_size){
	int i, j;
	for (i = 0 ;i < map_size; i++ ){
		for ( j = 0 ; j <map_size; j++){
			
			if ( map[i][j].type == 0){
				
				if ( i != 0 ){
					if ( map[i-1][j].type == -1)
						map[i][j].type++;
					if ( j != 0 && map[i -1][j -1].type == -1)
						map[i][j].type++;
					if ( j != map_size -1 && map[i -1][j +1].type == -1)
						map[i][j].type++;		
						
				}
				
				if ( j != 0 && map[i][j-1].type == -1)
					map[i][j].type++;
				
				if ( j != map_size -1 && map[i][j+1].type == -1)
					map[i][j].type++;
					
				if ( i != map_size -1){
					if ( map[i+1][j].type == -1)
						map[i][j].type++;
					if ( j != 0 && map[i+1][j-1].type == -1)
						map[i][j].type++;
					if ( j != map_size -1 && map[i+1][j+1].type == -1)
						map[i][j].type++;	
				}		
			}
		}
	}
	return;
}


void SetColorAndBackground(int ForgC, int BackC) // Background color |||  cor de letra  | cor de background
{
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}

void gotoxy(int x, int y)	// Gotoxy(x, y)
{
  static HANDLE h = NULL;  
  if(!h)
    h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD c = { x, y };  
  SetConsoleCursorPosition(h,c);
}
