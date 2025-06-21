#ifndef TYPING_PRACTICE_H
#define TYPING_PRACTICE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>

// 상수 정의
#define MAX_STAGES 5
#define MAX_SENTENCES 10
#define MAX_LENGTH 1000
#define BOX_WIDTH 80
#define BOX_HEIGHT 15
#define DEFAULT_TIME 1000 // 기본시간 설정 (1000ms = 1초)

// 게임 상태 구조체
// 현재 게임의 진행 상황과 설정을 관리하는 핵심 구조체
typedef struct {
    int current_stage;                    // 현재 진행 중인 스테이지 번호 (1~5)
    int current_difficulty;               // 현재 스테이지의 난이도 레벨
    int stage_cleared;                    // 현재 스테이지 클리어 여부 (0: 미클리어, 1: 클리어)
    int stage_time_limit;                 // 현재 스테이지의 시간 제한 (초 단위)
    clock_t stage_start_time;             // 스테이지 시작 시간 (타이머용)
    int problems_required;                // 스테이지 클리어에 필요한 문제 수
    int problems_solved;                  // 현재까지 해결한 문제 수
    int used_questions[MAX_SENTENCES];    // 이미 출제된 문제 추적 배열 (0: 미사용, 1: 사용됨)
} GameState;

// 함수 선언
void trim_whitespace(char* str);
void clear_screen();
void draw_title();
void init_game_state(GameState* game_state);
void draw_stage_info(GameState* game_state);
void print_wrapped_text(const char* text, int box_width);
void draw_typing_area(char* target_sentence, GameState* game_state);
void draw_stage_complete(GameState* game_state);
void draw_game_over(GameState* game_state);
int realtime_typing_check(char* target_sentence, GameState* game_state);
void typing_practice();
void show_menu();

#endif // TYPING_PRACTICE_H 