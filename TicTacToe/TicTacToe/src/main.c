 #include <stdio.h>
 #include <math.h>
 #include <stdlib.h>
 
 // 412FinalProject.cpp : Defines the entry point for the console application.
 //
 long gamestate = 0;

 typedef enum {
	 O0 = 1,
	 X0 = 2,
	 O1 = 4,
	 X1 = 8,
	 O2 = 16,
	 X2 = 32,
	 O3 = 64,
	 X3 = 128,
	 O4 = 256,
	 X4 = 512,
	 O5 = 1024,
	 X5 = 2048,
	 O6 = 4096,
	 X6 = 8192,
	 O7 = 16394,
	 X7 = 32768,
	 O8 = 65536,
	 X8 = 13107
 } movelist;

 movelist allmoves[] = { X0, X1, X2, X3, X4, X5, X6, X7, X8, O0, O1, O2, O3, O4, O5, O6, O7, O8 };
 movelist allmovesrev[] = { O0, O1, O2, O3, O4, O5, O6, O7, O8, X0, X1, X2, X3, X4, X5, X6, X7, X8 };
 movelist Xmoves[] = { X0, X1, X2, X3, X4, X5, X6, X7, X8 };
 movelist Omoves[] = { O0, O1, O2, O3, O4, O5, O6, O7, O8 };
 const int AllMovesLength = 18;
 const int XMovesLength = 9;
 const int OMovesLength = 9;

 //movelist remainingOmoves[] = { O0, O1, O2, O3, O4, O5, O6, O7, O8 };
 //movelist remainingXmoves[] = { X0, X1, X2, X3, X4, X5, X6, X7, X8 };

 typedef enum {
	 TopHorizO = O0 + O1 + O2,
	 MidHorizO = O3 + O4 + O5,
	 BotHorizO = O6 + O7 + O8,
	 TopHorizX = X0 + X1 + X2,
	 MidHorizX = X3 + X4 + X5,
	 BotHorizX = X6 + X7 + X8,
	 TopVertO = O0 + O3 + O6,
	 MidVertO = O1 + O4 + O7,
	 BotVertO = O2 + O4 + O8,
	 TopVertX = X0 + X3 + X6,
	 MidVertX = X1 + X4 + X7,
	 BotVertX = X2 + X5 + X8,

	 DownRightDiagO = O0 + O4 + O8,
	 UpRightDiagO = O6 + O4 + O2,
	 DownRightDiagX = X0 + X4 + X8,
	 UpRightDiagX = X6 + X4 + X2,

 } winlist;

 winlist winlistOarray[] = { TopHorizO, MidHorizO, BotHorizO, TopVertO, MidVertO, BotVertO, DownRightDiagO, UpRightDiagO};
 winlist winlistXarray[] = { TopHorizX, MidHorizX, BotHorizX, TopVertX, MidVertX, BotVertX, DownRightDiagX, UpRightDiagX};
 const long WinListLength = 8;
 int OWins = 0;
 int XWins = 0;
 
  void Init(void);
  void LedTestOne(void);
  void LedTestTwo(void);
  void add_move(movelist);
  void delete_move(movelist);
  int check_win(winlist);
  int check_move(movelist);
  void check_full_board(void);
  void print_win(void);
  void bot_move_X_random(void);
  void bot_move_O_random(void);
  void bot_X_move(void);
  void bot_O_move(void);

 void add_move(movelist ml) {
	 gamestate = gamestate + ml;
	 return;
 }

 void delete_move(movelist ml) {
	 gamestate = gamestate - ml;
	 return;
 }

 int check_win(winlist wl) {
	 if ((wl) == (wl & gamestate)) { return 1; }
	 return 0;
 }

 int check_move(movelist wl) {
	 if ((wl) == (wl & gamestate)) { return 1; }
	 return 0;
 }

 void check_full_board() {
	 for (int iter = 0; iter < WinListLength; iter++) {
		 if (check_win(winlistOarray[iter])) {
			 OWins = 1;
		 }
		 if (check_win(winlistXarray[iter])) {
			 XWins = 1;
		 }
	 }
	 return;
 }

 void print_win() {
	 if (OWins) {
		 printf("O wins");
	 }
	 else if (XWins) {
		 printf("X wins");
	 }
	 return;
 }

 void bot_move_X_random() {

	 int randmovefound = 0;

	 while (!randmovefound) {

		 int currentrand = rand() % 9;

		 movelist OMove = Omoves[currentrand];
		 movelist XMove = Xmoves[currentrand];

		 if (!((check_move(OMove)) && (check_move(XMove)))) {
			 add_move(XMove);
			 randmovefound = 1;
		 }
	 }
	 return;
 }

 void bot_move_O_random() {

	 int randmovefound = 0;

	 while (!randmovefound) {

		 int currentrand = rand() % 9;

		 movelist OMove = Omoves[currentrand];
		 movelist XMove = Xmoves[currentrand];

		 if (!((check_move(OMove)) && (check_move(XMove)))) {
			 add_move(OMove);
			 randmovefound = 1;
		 }
	 }
	 return;
 }

 //The bot will be X
 void bot_X_move() {

	 int hasmoved = 0;

	 for (int iter1 = 0; iter1 < AllMovesLength; iter1++) {

		 movelist potentialmove = allmoves[iter1];

		 check_full_board();

		 if (XWins) {
			 add_move(potentialmove);
			 hasmoved = 1;
			 break;
		 }
		 else if (OWins) {
			 movelist Xoffset = allmoves[iter1 - 9];
			 add_move(Xoffset);
			 hasmoved = 1;
			 break;
		 }
	 }

	 if (!hasmoved) {
		 bot_move_X_random();
	 }

	 return;
 }

 void bot_O_move() {

	 int hasmoved = 0;

	 for (int iter1 = 0; iter1 < AllMovesLength; iter1++) {

		 movelist potentialmove = allmovesrev[iter1];

		 check_full_board();

		 if (OWins) {
			 add_move(potentialmove);
			 hasmoved = 1;
			 break;
		 }
		 else if (XWins) {
			 movelist Xoffset = allmoves[iter1 - 9];
			 add_move(Xoffset);
			 hasmoved = 1;
			 break;
		 }
	 }

	 if (!hasmoved) {
		 bot_move_O_random();
	 }

	 return;
 }

 int main(void)
 {
	 //winlist temp = TopHorizO;
	 //gamestate = gamestate + temp;

	 add_move(O0);
	 add_move(O1);
	 add_move(O2);

	 check_full_board();
	 print_win();

	 getchar();
	 return 0;
 }


 /* -- Depricated, maybe still useful
 void print_menu() {
 printf("1 to add a move\n2 to delete a move");
 return;
 }

 void make_move() {
 print_menu();
 int move = 0;
 scanf("%d", &move);
 switch (move)
 {
 case 1:

 break;

 case 2:

 break;

 default:
 break;
 }
 }
 */
 
 /*int main(void)
 {
	 Init();
	 while(1)
	 {
		LedTestOne(); 
	 }
 }*/