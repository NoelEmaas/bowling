#include "scoreboard.h"

#include <stdio.h>

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

int getConfigIdx(int score) {
    if (score > 99) return 2;
    else if (score > 9) return 1;
    return 0;
}

void displayScoreboard(Scoreboard *scoreboard, int currentPlayer) {
    // Display P1 & P2
    DrawText("1", 288, 30, 45, (currentPlayer == 1) ? PURPLE : GRAY);
    DrawText("2", 340, 30, 45, (currentPlayer == 2) ? PURPLE : GRAY);

    // Display rounds
    for (int i = 0; i < ROUND_NUM; i++) 
        displayRound(scoreboard->rounds[i], 21, 80 + i * 60, scoreboard->current_round == i);

    if (scoreboard->game_over) {
        // Display final score
        int Y_CONF[3] = {686, 692, 696};
        int P1_X_CONF[3] = {280, 270, 270};
        int P2_X_CONF[3] = {338, 329, 329};
        int FONT_CONF[3] = {50, 40, 29};
        
        int p1_conf_idx = getConfigIdx(scoreboard->p1_final_score);
        int p2_conf_idx = getConfigIdx(scoreboard->p2_final_score);

        // p1 config
        int p1_final_score_y = Y_CONF[p1_conf_idx];
        int p1_final_score_x = P1_X_CONF[p1_conf_idx];
        int p1_font_size = FONT_CONF[p1_conf_idx];

        // p2 config
        int p2_final_score_y = Y_CONF[p2_conf_idx];
        int p2_final_score_x = P2_X_CONF[p2_conf_idx];
        int p2_font_size = FONT_CONF[p2_conf_idx];

        const char *final_score = TextFormat("%d", scoreboard->p1_final_score);
        DrawText(final_score, p1_final_score_x, p1_final_score_y, p1_font_size, PURPLE);

        const char *p2_final_score = TextFormat("%d", scoreboard->p2_final_score);
        DrawText(p2_final_score, p2_final_score_x, p2_final_score_y, p2_font_size, PURPLE);

        // Display winner
        const char *winner = TextFormat("PLAYER %d - BOWL", scoreboard->winner);
        DrawText(winner, 50, 755, 35, PURPLE);
    }

    // Display games won
    const char *p1_games_won = TextFormat("%d", scoreboard->p1_games_won);
    DrawText(p1_games_won, 280, 806, 50, PURPLE);

    const char *p2_games_won = TextFormat("%d", scoreboard->p2_games_won);
    DrawText(p2_games_won, 338, 806, 50, PURPLE);
}

void roll(Scoreboard *scoreboard, int player, int throw_no, int score) {
    printf("Player %d, Throw %d, Score %d\n", player, throw_no, score);
    if (player == 1) {
        if (throw_no == 1) {
            scoreboard->rounds[scoreboard->current_round].p1_throw_1 = score;
        } else {
            scoreboard->rounds[scoreboard->current_round].p1_throw_2 = score;
        }

        scoreboard->rounds[scoreboard->current_round].p1_score += score;
    } else {
        if (throw_no == 1) {
            scoreboard->rounds[scoreboard->current_round].p2_throw_1 = score;
        } else {
            scoreboard->rounds[scoreboard->current_round].p2_throw_2 = score;
        }

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

    if (p1 > p2) {
        scoreboard->winner = 1;
        scoreboard->p1_games_won++;
    } else {
        scoreboard->winner = 2;
        scoreboard->p2_games_won++;
    }

    scoreboard->p1_final_score = p1;
    scoreboard->p2_final_score = p2;
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
    // char buffer[10];

    // Display round_no
    const char *round_no = TextFormat("%d", round.round_no);
    DrawText(round_no, x + 200, y + 10, 45, (current) ? GREEN : GRAY);

    // Display throw_scores for P1
    if (round.p1_throw_1 != -1) {
        const char *p1_throw_1 = TextFormat("%d", round.p1_throw_1);
        DrawText(p1_throw_1, x + 251, y + 3, 25, GRAY);
    }

    if (round.p1_throw_2 != -1) {
        const char *p1_throw_2 = TextFormat("%d", round.p1_throw_2);
        DrawText(p1_throw_2, x + 280, y + 3, 25, GRAY);
    }

    // Display throw_scores for P2
    if (round.p2_throw_1 != -1) {
        const char *p2_throw_1 = TextFormat("%d", round.p2_throw_1);
        DrawText(p2_throw_1, x + 308, y + 3, 25, GRAY);
    }

    if (round.p2_throw_2 != -1) {
        const char *p2_throw_2 = TextFormat("%d", round.p2_throw_2);
        DrawText(p2_throw_2, x + 337, y + 3, 25, GRAY);
    }

    // Display total frame score for p1
    if (round.p1_throw_1 != -1) {
        const char *p1_score = TextFormat("%d", round.p1_score);
        DrawText(p1_score, x + 265, y + 33, 25, GRAY);
    }

    // Display total frame score for p2
    if (round.p2_throw_1 != -1) {
        printf("p2_score: %d\n", round.p2_score);
        const char *p2_score = TextFormat("%d", round.p2_score);
        DrawText(p2_score, x + 320, y + 33, 25, GRAY);
    }
}
