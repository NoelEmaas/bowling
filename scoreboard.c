#include "scoreboard.h"

Scoreboard createScoreboard() {
    Scoreboard scoreboard;

    scoreboard.current_round = 0;
    scoreboard.game_over = false;
    scoreboard.winner = -1;

    scoreboard.p1_final_score = 0;
    scoreboard.p2_final_score = 0;

    scoreboard.p1_games_won = 0;
    scoreboard.p2_games_won = 0;
    
    for (int i = 0; i < ROUND_NUM; i++) 
        scoreboard.rounds[i] = createRound(i);

    return scoreboard;
}

void displayScoreboard(Scoreboard *scoreboard, int currentPlayer) {
    // Display P1 & P2
    DrawText("1", 288, 30, 45, (currentPlayer == 1) ? PURPLE : GRAY);
    DrawText("2", 340, 30, 45, (currentPlayer == 2) ? PURPLE : GRAY);

    // Display rounds
    for (int i = 0; i < ROUND_NUM; i++) 
        displayRound(scoreboard->rounds[i], 21, 80 + i * 60, scoreboard->current_round == i);

    char buffer[20];

    if (scoreboard->game_over) {
        // Display final score
        sprintf(buffer, "%d", scoreboard->p1_final_score);
        DrawText(&buffer, 280, 686, 50, PURPLE);

        sprintf(buffer, "%d", scoreboard->p2_final_score);
        DrawText(&buffer, 338, 687, 50, PURPLE);
        // Display winner
        sprintf(buffer, "PLAYER %d - BOWL", scoreboard->winner);
        DrawText(&buffer, 50, 755, 35, PURPLE);
    }

    // Display games won
    sprintf(buffer, "%d", scoreboard->p1_games_won);
    DrawText(&buffer, 280, 806, 50, PURPLE);

    sprintf(buffer, "%d", scoreboard->p2_games_won);
    DrawText(&buffer, 338, 806, 50, PURPLE);
}

void roll(Scoreboard *scoreboard, int player, int throw_no, int score) {
    if (player == 1) {
        scoreboard->rounds[scoreboard->current_round].p1_throw_1 = (throw_no == 1) ? score : 0;
        scoreboard->rounds[scoreboard->current_round].p1_throw_2 = (throw_no == 2) ? score : 0;
        scoreboard->rounds[scoreboard->current_round].p1_score += score;
    } else {
        scoreboard->rounds[scoreboard->current_round].p2_throw_1 = (throw_no == 1) ? score : 0;
        scoreboard->rounds[scoreboard->current_round].p2_throw_2 = (throw_no == 2) ? score : 0;
        scoreboard->rounds[scoreboard->current_round].p2_score += score;
    }
}

void nextRound(Scoreboard *scoreboard) {
    scoreboard->current_round++;
}

void setWinner(Scoreboard *scoreboard) {
    int p1 = 0, p2 = 0;

    for (int i = 0; i < ROUND_NUM; i++) {
        p1 += scoreboard->rounds[i].p1_score;
        p2 += scoreboard->rounds[i].p2_score;
    }
}

Round createRound(int round_no) {
    Round round;

    round.round_no = round_no;

    round.p1_throw_1 = -1;
    round.p1_throw_2 = -1;
    round.p1_score = 0;

    round.p2_throw_1 = -1;
    round.p2_throw_2 = -1;
    round.p2_score = 0;

    return round;
}

void displayRound(Round round, int x, int y, bool current) {
    char buffer[10];

    // Display frame_no
    sprintf(buffer, "%d", round.round_no);
    DrawText(&buffer, x + 200, y + 10, 45, (current) ? GREEN : GRAY);

    // Display throw_scores for P1
    if (round.p1_throw_1 != -1) {
        sprintf(buffer, "%d", round.p1_throw_1);
        DrawText(&buffer, x + 251, y + 3, 25, GRAY);
    }

    if (round.p1_throw_2 != -1) {
        sprintf(buffer, "%d", round.p1_throw_2);
        DrawText(&buffer, x + 280, y + 3, 25, GRAY);
    }

    // Display throw_scores for P2
    if (round.p2_throw_1 != -1) {
        sprintf(buffer, "%d", round.p2_throw_1);
        DrawText(&buffer, x + 308, y + 3, 25, GRAY);
    }

    if (round.p2_throw_2 != -1) {
        sprintf(buffer, "%d", round.p2_throw_2);
        DrawText(&buffer, x + 337, y + 3, 25, GRAY);
    }

    // Display total frame score for p1
    if (round.p1_throw_1 != -1) {
        sprintf(buffer, "%d", round.p1_score);
        DrawText(&buffer, x + 265, y + 33, 25, GRAY);
    }

    // Display total frame score for p2
    if (round.p2_throw_1 != -1) {
        sprintf(buffer, "%d", round.p2_score);
        DrawText(&buffer, x + 320, y + 33, 25, GRAY);
    }
}
