#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "raylib.h"

#define ROUND_NUM 10

typedef struct {
    int round_no;

    int p1_throw_1;
    int p1_throw_2;
    int p1_score;

    int p2_throw_1;
    int p2_throw_2;
    int p2_score;
} Round;

typedef struct {
    Round rounds[ROUND_NUM];
    int current_round;

    int p1_final_score;
    int p2_final_score;

    int p1_games_won;
    int p2_games_won;

    bool game_over;
    int winner;
} Scoreboard;

Scoreboard createScoreboard();
void displayScoreboard(Scoreboard *scoreboard, int currentPlayer);
void roll(Scoreboard *scoreboard, int player, int throw_no, int score);
void nextRound(Scoreboard *scoreboard);
void setWinner(Scoreboard *scoreboard);
Round createRound(int round_no);
void displayRound(Round round, int x, int y, bool current);

#endif
