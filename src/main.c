#include "typing_practice.h"

int main() {
    int choice;
    
    // 터미널 인코딩을 UTF-8로 설정
    system("export LANG=ko_KR.UTF-8");
    system("export LC_ALL=ko_KR.UTF-8");
    
    // 프로그램 시작
    clear_screen();
    draw_title();
    
    printf("            🎯 스테이지 타자 연습 게임에 오신 것을 환영합니다!\n");
    printf("            5개 스테이지를 모두 클리어해보세요!\n");
    printf("            Enter를 눌러 시작하세요...");
    getchar();
    
    while (1) {
        show_menu();
        scanf("%d", &choice);
        getchar(); // 버퍼 비우기
        
        switch (choice) {
            case 1:
                typing_practice();
                break;
            case 2:
                clear_screen();
                draw_title();
                printf("            🚪 프로그램을 종료합니다. 수고하셨습니다! 👋\n");
                printf("            Enter를 눌러 종료하세요...");
                getchar();
                return 0;
            default:
                printf("\n            ❌ 잘못된 선택입니다. 다시 선택해주세요.\n");
                printf("            Enter를 눌러 계속하세요...");
                getchar();
        }
    }
    
    return 0;
} 