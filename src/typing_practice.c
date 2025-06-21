#include "typing_practice.h"
#include <sys/select.h>
#include <termios.h>

// 각 스테이지별 문제(화면에 보여줄 문장)
char* questions[MAX_STAGES][MAX_SENTENCES] = {
    // 1단계
    {
        "이제는 더 이상 물러날 곳이 없다",
        "챔피언 소리 지르는 니가",
        "늦었다고 생각할 때가 진짜 늦었다",
        "정신차려 이 각박한 세상 속에서",
        "꿈은 없고요 그냥 놀고 싶습니다",
        "쓰레기 같은 고민 했구나",
        "돈이 세상에서 제일 중요한 거거든",
        "세상이 호락호락하지가 않아",
        "저는 모든 걸 내려놓을 거예요",
        "고생 끝에 골병 난다",
        "시험이 내일이라니 이건 거의 재난이다",
        "내일 시험인데 오늘 처음 책을 폈다",
        "이번엔 진짜 망할 각이다",
        "현실을 부정하며 하루를 시작했다",
        "긴 여정의 끝이 눈앞에 다가왔다"
    },
    // 2단계: 점이 포함된 문장
    {
        "솔.직.히.잘.모.르.겠.다.그.래.도.멈.출.수.는.없.잖.아.",
        "기.대.는.안.해.그.래.도.해.보.는.거.지.뭐.",
        "여.기.까.지.온.것.도.나.름.선.방.이.다.",
        "희.망.은.없.지.만.버.티.는.방.법.은.안.다.",
        "무.의.미.해.보.여.도.의.미.있.을.수.있.다.",
        "그.래.도.이.왕.이.면.잘.됐.으.면.좋.겠.다.",
        "계.획.과.방.향.이.없.어.도.일.단.굴.러.간.다.",
        "오.늘.도.무.사.히.망.하.지.않.았.다.그.걸.로.됐.다.",
        "사.는.게.버.거.운.데.죽.을.만.큼.은.또.아.니.다.",
        "그.냥.끝.까.지.한.번.가.보.는.거.야.화.이.팅.",
        "책.을.펼.치.자.마.자.졸.음.이.몰.려.왔.다.",
        "머.릿.속.이.백.지.라.서.책.도.하.얘.보.였.다.",
        "암.기.과.목.인.데.내.가.암.기.한.건.오.직.좌.절.",
        "공.부.를.시.작.했.지.만.손.이.자.꾸.냉.장.고.로.갔.다.",
        "펜.을.잡.고.멍.하.니.창.밖.만.바.라.봤.다."
    },
    // 3단계: 자음/모음 생략된 문장
    {
        "산이 노라고 두ㅕ워ㅎ지 말아ㄹ",
        "ㄱ장 ㅇ두ㅇㄴ 밤ㄷ 결구 ㅐ벽이 온다",
        "시작ㅎ지 안으면 ㅏ무 일ㅗ 안일ㅇ난ㅏ",
        "한ㄱ는 마음이 ㅁㄴ든 화상일 ㅜㄴ이다",
        "진시은 ㅏㄴ드ㅣ 꼭 길을 찾ㄴ다",
        "내ㅣㄹ은 ㅗ늘보ㄷ 분ㅁㅇ 더 ㅏ을ㄱ야",
        "지그의 불아ㅡㄴ ㅓㅇ장ㅢ 증ㄱㅓ다",
        "ㅐ가 나를 믿ㄴ 순간, 길ㅡㄴ 열ㅣㄴ다",
        "ㅎ내는 ㅅㅏ람ㅡㄴ 결ㅜㄱ 해ㄴ다",
        "하발 ㅏㄴ발ㅣ 모두 의ㅁ가 있다",
        "커피ㅡㄹ 다섯 잔 마셔ㅣ만 정신ㅡㄴ ㅕ전했ㅏ",
        "벼ㅏㄱ치기 ㅏ려다 ㅕ락 마은 기부이ㅓㅆ다",
        "ㅢ자에 ㅏㄴㅈ은 ㅐ로 정신을 놓ㅏㅆ다",
        "책을 외ㅜㄴ다기ㅗ단 책이 나ㅡㄹ 보는 느끼이었다",
        "ㅣㅂ중하려 했지만 고양이가 내 노트를 차지했다"
    },
    // 4단계: 특수문자 포함 문장
    {
        "도전은,+용기를,필요로,&한다!!**.",
        "도전없는,삶은!결코성장하지#않는다@.",
        "실패@는성공%!의어머니~*$이다./",
        "위대^한)일은#시간%이걸린+[다.",
        "한계는/#마음이@만든환상~일뿐이다^.",
        "포기<하지*말아라!/도전해라~34.",
        "산:이 높다!고두려&워하지말(아라.",
        "시작하#지않_으면아무 일!도안일어난다/.",
        "가장&어_두운/ 밤#도결국@새벽;:이온다.",
        "도전*한%순간이미변@화는시작^되었(다.",
        "그래도,인간의!끈기로^다시#책을@폈다.*",
        "기적을!바라는@마음으로#하이라이트만^읽었다.*",
        "그&와중에+손글씨를!정리하고#있는$나%자신.~",
        "오늘^밤은*길&것!같았다@.아주#길고$고통스럽게.%",
        "외우다#말고^가사처럼!읊조리고&있었다.*"
    },
    // 5단계: 원문 (화면에 보여줄 문장)
    {
        "죽기를 각오하면 살고 살고자 하면 반드시 죽는다",
        "작은 용기가 쌓여 언젠가 큰 꿈을 이루게 해줄 거야",
        "괜찮아 오늘 한 걸음 내디뎠다면 그것만으로 충분해",
        "인생은 끊임없는 반복 반복에 지치지 않는 자가 성취한다",
        "괜찮아 린닝닌닝닝 링닌닝링닝",
        "한 번도 보지 못한 어딘가 새롭고 낯선 곳을 찾기 위해",
        "문이 열리면 주저하지 말고 계속 나아가 무작정 달려보면 돼",
        "좋은 기억들만 담아서 난 다음 미래로 가",
        "꿈을 꾸던 그 시간에 나도 꿈을 꿨지 두 눈 똑바로 뜬 채로",
        "어두운 하늘을 날아다니는 저 빛나는 별을 타고서",
        "시험장 공기는 왜 이렇게 무거운 건가",
        "감독관의 발소리조차 압박으로 다가왔다",
        "첫 문제부터 모르는 단어가 반겨줬다",
        "내가 분명 공부했는데 얘는 어디서 온 문제지",
        "OMR 마킹이 오늘의 유일한 확신이었다"
    }
};

// answers 배열 (2중 배열 구조로 가독성 향상)
char* answers[MAX_STAGES][MAX_SENTENCES] = {
    // 1단계 정답 (원문)
    {
        "이제는 더 이상 물러날 곳이 없다",
        "챔피언 소리 지르는 니가",
        "늦었다고 생각할 때가 진짜 늦었다",
        "정신차려 이 각박한 세상 속에서",
        "꿈은 없고요 그냥 놀고 싶습니다",
        "쓰레기 같은 고민 했구나",
        "돈이 세상에서 제일 중요한 거거든",
        "세상이 호락호락하지가 않아",
        "저는 모든 걸 내려놓을 거예요",
        "고생 끝에 골병 난다",
        "시험이 내일이라니 이건 거의 재난이다",
        "내일 시험인데 오늘 처음 책을 폈다",
        "이번엔 진짜 망할 각이다",
        "현실을 부정하며 하루를 시작했다",
        "긴 여정의 끝이 눈앞에 다가왔다"
    },
    // 2단계 정답 (점 제거, 띄어쓰기 포함)
    {
        "솔직히 잘 모르겠다 그래도 멈출 수는 없잖아",
        "기대는 안 해 그래도 해보는 거지 뭐",
        "여기까지 온 것도 나름 선방이다",
        "희망은 없지만 버티는 방법은 안다",
        "무의미해보여도 의미있을 수 있다",
        "그래도 이왕이면 잘 됐으면 좋겠다",
        "계획과 방향이 없어도 일단 굴러간다",
        "오늘도 무사히 망하지 않았다 그걸로 됐다",
        "사는 게 버거운데 죽을 만큼은 또 아니다",
        "그냥 끝까지 한 번 가보는 거야 화이팅",
        "책을 펼치자마자 졸음이 몰려왔다",
        "머릿속이 백지라서 책도 하얘 보였다",
        "암기과목인데 내가 암기한 건 오직 좌절",
        "공부를 시작했지만 손이 자꾸 냉장고로 갔다",
        "펜을 잡고 멍하니 창밖만 바라봤다"
    },
    // 3단계 정답 (띄어쓰기 포함 원문)
    {
        "산이 높다고 두려워하지 말아라",
        "가장 어두운 밤도 결국 새벽이 온다",
        "시작하지 않으면 아무 일도 안 일어난다",
        "한계는 마음이 만든 환상일 뿐이다",
        "진심은 반드시 꼭 길을 찾는다",
        "내일은 오늘보다 분명 더 나을거야",
        "지금의 불안은 성장의 증거다",
        "내가 나를 믿는 순간, 길은 열린다",
        "해내는 사람은 결국 해낸다",
        "한발 한발 모두 의미가 있다",
        "커피를 다섯 잔 마셨지만 정신은 여전했다",
        "벼락치기 하려다 벼락 맞은 기분이었다",
        "의자에 앉은 채로 정신을 놓았다",
        "책을 외운다기보단 책이 나를 보는 느낌이었다",
        "집중하려 했지만 고양이가 내 노트를 차지했다"
    },
    // 4단계 정답 (띄어쓰기 포함, 특수문자 제거)
    {
        "도전은 용기를 필요로 한다",
        "도전없는 삶은 결코 성장하지 않는다",
        "실패는 성공의 어머니이다",
        "위대한 일은 시간이 걸린다",
        "한계는 마음이 만든 환상일 뿐이다",
        "포기하지 말아라 도전해라",
        "산이 높다고 두려워하지 말아라",
        "시작하지 않으면 아무 일도 안 일어난다",
        "가장 어두운 밤도 결국 새벽이 온다",
        "도전한 순간 이미 변화는 시작되었다",
        "그래도 인간의 끈기로 다시 책을 폈다",
        "기적을 바라는 마음으로 하이라이트만 읽었다",
        "그 와중에 손글씨를 정리하고 있는 나 자신",
        "오늘 밤은 길 것 같았다 아주 길고 고통스럽게",
        "외우다 말고 가사처럼 읊조리고 있었다"
    },
    // 5단계 정답 (뒤에서부터 앞으로 단어 재배열)
    {
        "는다죽 시드반 면하 자고살 고살 면하오각 를기죽",
        "야거 줄해 게루이 을꿈 큰 가젠언 여쌓 가기용 은작",
        "해분충 로으만것그 다면뎠디내 음걸 한 늘오 아찮괜",
        "다한취성 가자 는않 지치지 에복반 복반 는없임끊 은생인",
        "닝링닝닌링 닝닝닌닝린 아찮괜",
        "해위 기찾 을곳 선낯 고롭새 가딘어 한못 지보 도번 한",
        "돼 면보려달 정작무 가아나 속계 고말 지하저주 면리열 이문",
        "가 로래미 음다 난 서아담 만들억기 은좋",
        "로채 뜬 로바똑 눈 두 지꿨 을꿈 도나 에간시 그 던꾸 을꿈",
        "서고타 을별 나는빛 저 는니다아날 을늘하 운두어",
        "가건 운거무 게렇이 왜 는기공 장험시",
        "다왔가다 로으박압 차조리소발 의관독감",
        "다줬겨반 가어단 는르모 터부제문 첫",
        "지제문 온 서디어 는얘 데는했부공 명분 가내",
        "다었이신확 한일유 의늘오 이킹마 RMO"
    }
};

// 스테이지별 시간 제한 (초 단위)
int stage_time_limits[MAX_STAGES] = {30, 30, 20, 15, 15};

// 스테이지별 필요한 문제 수
int stage_problems_required[MAX_STAGES] = {3, 3, 3, 2, 2};

// 스테이지별 설명과 난이도
typedef struct {
    char* description;
    char* difficulty;
    char* instruction;
} StageInfo;

// 스테이지 정보 배열
StageInfo stage_info[MAX_STAGES] = {
    {
        "주어지는 문장을 따라 입력하는 게임입니다.",
        "매우 쉬움",
        "제한시간 안에 정확히 입력하면 성공입니다."
    },
    {
        "주어진 문장의 점을 제거하고 띄어쓰기를 적용하여 입력하는 게임입니다.",
        "쉬움", 
        "제한시간 안에 점을 제거하고 띄어쓰기를 적용하여 입력하면 성공입니다."
    },
    {
        "일부 자음이나 모음이 지워진 문장을 입력하는 게임입니다.",
        "보통",
        "제한시간 안에 적절한 자음, 모음을 넣어서 문장을 완성하면 성공입니다."
    },
    {
        "특수문자가 포함된 문장을 정확히 입력하는 게임입니다.",
        "어려움",
        "제한시간 안에 정확히 입력하면 성공입니다."
    },
    {
        "주어진 문장의 각 단어를 개별적으로 뒤집어 입력하는 게임입니다.",
        "매우 어려움",
        "제한시간 안에 각 단어를 정확하게 뒤집어 입력하면 성공입니다."
    }
};

// 문자열 앞뒤 공백 제거 함수
void trim_whitespace(char* str) {
    int len = strlen(str);
    int start = 0, end = len - 1;
    
    // 앞쪽 공백 제거
    while (start < len && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r')) {
        start++;
    }
    
    // 뒤쪽 공백 제거
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n' || str[end] == '\r')) {
        end--;
    }
    
    // 문자열을 앞으로 이동
    if (start > 0) {
        for (int i = 0; i <= end - start; i++) {
            str[i] = str[start + i];
        }
        str[end - start + 1] = '\0';
    } else if (end < len - 1) {
        str[end + 1] = '\0';
    }
}

// 화면 클리어 함수
void clear_screen() {
    // 터미널 인코딩을 UTF-8로 설정
    system("export LANG=ko_KR.UTF-8");
    system("export LC_ALL=ko_KR.UTF-8");
    system("clear");
}

// 제목 그리기
void draw_title() {
    printf("\n");
    printf("            ███████╗ ██████╗ ██████╗ ███████╗███████╗\n");
    printf("            ██╔════╝██╔═══██╗██╔══██╗  ██╔══╝██╔════╝\n");
    printf("            ██████╗ ██║   ██║██████╔╝  ██║   ██████╗\n");
    printf("            ██╔═══╝ ██║   ██║██╔══██╗  ██║   ██╔═══╝\n");
    printf("            ██║     ╚██████╔╝██║  ██║███████╗██║\n");
    printf("            ╚═╝      ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝\n\n\n");
}

// 게임 상태 초기화
void init_game_state(GameState* game_state) {
    game_state->current_stage = 1;
    game_state->current_difficulty = 1;
    game_state->stage_cleared = 0;
    game_state->stage_time_limit = stage_time_limits[0]; // 초 단위
    game_state->problems_required = stage_problems_required[0];
    game_state->problems_solved = 0;
    
    // 사용된 문제 추적 배열 초기화
    for (int i = 0; i < MAX_SENTENCES; i++) {
        game_state->used_questions[i] = 0;
    }
}

// 중복되지 않는 랜덤 문제 선택 함수
int get_random_unused_question(GameState* game_state) {
    int available_count = 0;
    int available_indices[MAX_SENTENCES];
    
    // 사용되지 않은 문제들의 인덱스를 수집
    for (int i = 0; i < MAX_SENTENCES; i++) {
        if (game_state->used_questions[i] == 0) {
            available_indices[available_count] = i;
            available_count++;
        }
    }
    
    // 모든 문제를 다 사용했다면 다시 초기화
    if (available_count == 0) {
        for (int i = 0; i < MAX_SENTENCES; i++) {
            game_state->used_questions[i] = 0;
        }
        available_count = MAX_SENTENCES;
        for (int i = 0; i < MAX_SENTENCES; i++) {
            available_indices[i] = i;
        }
    }
    
    // 랜덤 시드 갱신 (더 다양한 선택을 위해)
    srand(time(NULL) + rand());
    
    // Fisher-Yates 셔플 알고리즘으로 사용 가능한 문제들을 섞기
    for (int i = available_count - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // 인덱스 교환
        int temp = available_indices[i];
        available_indices[i] = available_indices[j];
        available_indices[j] = temp;
    }
    
    // 첫 번째 문제 선택 (이미 섞여있으므로 랜덤)
    int selected_question = available_indices[0];
    
    // 선택된 문제를 사용됨으로 표시
    game_state->used_questions[selected_question] = 1;
    
    return selected_question;
}

// 스테이지 정보 표시
void draw_stage_info(GameState* game_state) {
    // 남은 시간 계산
    clock_t current_time = clock();
    double elapsed_time = ((double)(current_time - game_state->stage_start_time)) / CLOCKS_PER_SEC;
    int remaining_time = game_state->stage_time_limit - (int)elapsed_time;
    
    if (remaining_time < 0) remaining_time = 0;
    
    printf("            🎮 스테이지 %d / %d  |  ⏱️  남은시간: %d초  |  📝 문제: %d/%d\n", 
           game_state->current_stage, MAX_STAGES, 
           remaining_time,
           game_state->problems_solved, game_state->problems_required);
}

// 타이핑 영역 그리기 (스테이지 정보 포함)z
void draw_typing_area(char* target_sentence, GameState* game_state) {
    clear_screen();
    draw_title();
    
    // 스테이지 정보 표시
    draw_stage_info(game_state);
    printf("\n");
    
    // 박스 상단
    printf("            ");
    for (int i = 0; i < BOX_WIDTH; i++) {
        printf("#");
    }
    printf("\n");
    
    // 제목 영역 (중앙 정렬)
    printf("            #");
    printf("                               🎯 스테이지 %d 🎯                               ", game_state->current_stage);
    printf("#\n");
    
    // 구분선
    printf("            #");
    for (int i = 0; i < BOX_WIDTH - 2; i++) {
        printf("-");
    }
    printf("#\n");
    
    // 목표 문장 표시
    printf("            # 목표 문장:                                                                   #\n");
    print_wrapped_text(target_sentence, BOX_WIDTH);
    
    // 구분선
    printf("            #");
    for (int i = 0; i < BOX_WIDTH - 2; i++) {
        printf("-");
    }
    printf("#\n");
    
    // 박스 하단
    printf("            #");
    for (int i = 0; i < BOX_WIDTH - 2; i++) {
        printf("#");
    }
    printf("#\n");
}

// 문장을 박스 너비에 맞게 여러 줄로 나누어 표시하는 함수
void print_wrapped_text(const char* text, int box_width) {
    int max_chars_per_line = box_width - 2; // "# " 와 " #" 제외하고 여유 공간 확보 (2 + 2 + 8 = 12)
    int text_len = strlen(text);
    int current_pos = 0;
    
    while (current_pos < text_len) {
        printf("            # ");
        
        // 현재 줄에 들어갈 문자 수 계산
        int chars_this_line = (text_len - current_pos > max_chars_per_line) ? 
                             max_chars_per_line : (text_len - current_pos);
        
        // 현재 줄 출력
        for (int i = 0; i < chars_this_line; i++) {
            printf("%c", text[current_pos + i]);
        }
        
        // 나머지 공간을 공백으로 채움
        for (int i = chars_this_line; i < max_chars_per_line; i++) {
            printf(" ");
        }
        
        printf(" #\n");
        current_pos += chars_this_line;
    }
}

// 실시간 타이핑 함수 (시간 제한 없음 - 스테이지 전체 시간으로 관리)
int realtime_typing_check(char* target_sentence, GameState* game_state) {
    char user_input[MAX_LENGTH] = "";
    
    // 화면 표시
    draw_typing_area(target_sentence, game_state);
    printf("            입력:   ");
    fflush(stdout);
    
    // macOS에서 백스페이스 정상 작동을 위한 설정
    system("stty erase ^H");
    system("stty erase ^?");
    
    // fgets를 사용하여 입력 받기 (백스페이스 문제 해결)
    if (fgets(user_input, MAX_LENGTH, stdin) != NULL) {
        // 개행 문자 제거
        user_input[strcspn(user_input, "\n")] = 0;
        trim_whitespace(user_input);
    } else {
        return -1; // 입력 실패
    }
    
    // 정답 확인
    int stage = game_state->current_stage;
    int sentence_index = 0;
    
    // 현재 문장의 인덱스 찾기
    for (int i = 0; i < MAX_SENTENCES; i++) {
        if (strcmp(questions[stage - 1][i], target_sentence) == 0) {
            sentence_index = i;
            break;
        }
    }
    
    // 디버깅: 모든 스테이지에 대해 디버깅 정보 출력
    // printf("\n            [디버깅] 스테이지: %d\n", stage);
    // printf("            [디버깅] 화면 문장: '%s'\n", target_sentence);
    // printf("            [디버깅] 찾은 인덱스: %d\n", sentence_index);
    // printf("            [디버깅] 사용자 입력: '%s'\n", user_input);
    // printf("            [디버깅] 정답: '%s'\n", answers[stage-1][sentence_index]);
    // sleep(2);
    
    int is_correct = 0;
    if (stage == 4) {
        // 스테이지 4는 사용자가 입력한 그대로와 정답을 비교
        if (strcmp(user_input, answers[stage-1][sentence_index]) == 0) is_correct = 1;
    } else {
        if (strcmp(user_input, answers[stage-1][sentence_index]) == 0) is_correct = 1;
    }
    
    return is_correct ? 1 : -1; // 1: 정답, -1: 오답
}

// 타이핑 연습 함수 (스테이지 기반)
void typing_practice() {
    // 게임 상태 초기화
    GameState game_state;
    init_game_state(&game_state);
    
    // 랜덤 시드 설정
    srand(time(NULL));
    
    // 각 스테이지 진행
    while (game_state.current_stage <= MAX_STAGES) {
        // 스테이지별 설정 초기화
        game_state.stage_time_limit = stage_time_limits[game_state.current_stage - 1];
        game_state.problems_required = stage_problems_required[game_state.current_stage - 1];
        game_state.problems_solved = 0;
        
        // 스테이지 시작 시간 기록
        game_state.stage_start_time = clock();
        
        // 스테이지별 랜덤 시드 갱신 (더 다양한 문제 선택을 위해)
        srand(time(NULL) + game_state.current_stage * 1000);
        
        // 사용된 문제 추적 배열 초기화
        for (int i = 0; i < MAX_SENTENCES; i++) {
            game_state.used_questions[i] = 0;
        }
        
        // 스테이지 시작 안내
        clear_screen();
        draw_title();
        printf("            🎮 스테이지 %d 시작!\n\n", game_state.current_stage);
        printf("            📝 이번 스테이지에서는 %d문제를 풀어야 합니다.\n", game_state.problems_required);
        printf("            ⏱️  제한시간: %d초\n\n", game_state.stage_time_limit);
        
        // 스테이지별 규칙 표시
        printf("            🎯 스테이지 %d 규칙:\n", game_state.current_stage);
        printf("            • %s\n", stage_info[game_state.current_stage - 1].description);
        printf("            • 난이도: %s\n", stage_info[game_state.current_stage - 1].difficulty);
        printf("            • %s\n\n", stage_info[game_state.current_stage - 1].instruction);
        
        printf("            Enter를 눌러 시작하세요...");
        getchar();
        
        // 필요한 문제 수만큼 반복
        while (game_state.problems_solved < game_state.problems_required) {
            // 시간 체크
            clock_t current_time = clock();
            double elapsed_time = ((double)(current_time - game_state.stage_start_time)) / CLOCKS_PER_SEC;
            
            if (elapsed_time >= game_state.stage_time_limit) {
                // 스테이지 시간 초과
                draw_game_over(&game_state);
                return; // 메인 메뉴로 돌아가기
            }
            
            // 랜덤 문장 선택
            int sentence_index = get_random_unused_question(&game_state);
            char* target_sentence = questions[game_state.current_stage - 1][sentence_index];
            
            // 타이핑 체크 (제한시간 내에 정답할 때까지 반복)
            while (1) {
                // 시간 체크
                current_time = clock();
                elapsed_time = ((double)(current_time - game_state.stage_start_time)) / CLOCKS_PER_SEC;
                
                if (elapsed_time >= game_state.stage_time_limit) {
                    // 스테이지 시간 초과
                    draw_game_over(&game_state);
                    return; // 메인 메뉴로 돌아가기
                }
                
                int result = realtime_typing_check(target_sentence, &game_state);
                
                if (result == 0) {
                    // 시간 초과 (개별 문제 시간 제한은 제거했으므로 이 부분은 실행되지 않음)
                    draw_game_over(&game_state);
                    return; // 메인 메뉴로 돌아가기
                } else if (result == 1) {
                    // 정답
                    game_state.problems_solved++;
                    printf("\n            ✅ 정답입니다! (%d/%d)\n", game_state.problems_solved, game_state.problems_required);
                    
                    if (game_state.problems_solved >= game_state.problems_required) {
                        // 스테이지 완료
                        draw_stage_complete(&game_state);
                        game_state.current_stage++;
                    } else {
                        // 다음 문제로
                        printf("            Enter를 눌러 다음 문제로...");
                        getchar();
                    }
                    break;
                } else {
                    // 오답
                    printf("\n            ❌ 틀렸습니다! 다시 시도하세요.\n");
                    printf("            Enter를 눌러 계속...");
                    getchar();
                }
            }
        }
    }
    
    // 모든 스테이지 완료
    clear_screen();
    draw_title();
    printf("            🏆 축하합니다! 모든 스테이지를 완료했습니다! 🏆\n");
    printf("            Enter를 눌러 메인 메뉴로 돌아가세요...");
    getchar();
}

// 메인 메뉴 함수
void show_menu() {
    clear_screen();
    draw_title();
    
    printf("            🎮 메인 메뉴\n\n");
    printf("            ");
    for (int i = 0; i < BOX_WIDTH; i++) {
        printf("#");
    }
    printf("\n");
    
    // 메뉴 항목 1
    printf("            # 1. 🎯 게임 시작                                                              #\n");
    
    // 메뉴 항목 2
    printf("            # 2. 🚪 프로그램 종료                                                          #\n");
    
    printf("            #");
    for (int i = 0; i < BOX_WIDTH - 2; i++) {
        printf("#");
    }
    printf("#\n");
    
    printf("\n            선택하세요: ");
}

// 스테이지 완료 표시
void draw_stage_complete(GameState* game_state) {
    clear_screen();
    draw_title();
    
    printf("            🎉 스테이지 %d 완료! 🎉\n\n", game_state->current_stage);
    
    printf("            ");
    for (int i = 0; i < BOX_WIDTH; i++) {
        printf("#");
    }
    printf("\n");
    
    // 제목 중앙 정렬
    printf("            #                         🏆 스테이지 클리어! 🏆                               #\n");
    
    // 구분선
    printf("            #");
    for (int i = 0; i < BOX_WIDTH - 2; i++) {
        printf("-");
    }
    printf("#\n");
    
    // 메시지 1: 스테이지 완료
    printf("            # 🎯 스테이지 %d를 성공적으로 완료했습니다!                                     #\n", game_state->current_stage);
    
    // 메시지 2: 다음 스테이지 또는 완료
    if (game_state->current_stage < MAX_STAGES) {
        printf("            # 🚀 다음 스테이지로 진행합니다!                                               #\n");
    } else {
        printf("            # 🏆 모든 스테이지를 완료했습니다!                                             #\n");
    }
    
    // 박스 하단
    printf("            #");
    for (int i = 0; i < BOX_WIDTH - 2; i++) {
        printf("#");
    }
    printf("#\n");
    
    printf("\n            Enter를 눌러 계속하세요...");
    getchar();
}

// 게임 오버 표시
void draw_game_over(GameState* game_state) {
    // forif ASCII 아트 표시 (게임 오버 화면보다 먼저)
    clear_screen();
    printf("\n");
    printf("  __ _  __ _ _ __ ___   ___    _____   _____ _ __\n");
    printf(" / _` |/ _` | '_ ` _ \\ / _ \\  / _ \\ \\ / / _ \\ '__|\n");
    printf("| (_| | (_| | | | | | |  __/ | (_) \\ V /  __/ |\n");
    printf(" \\__, |\\__,_|_| |_| |_|\\___|  \\___/ \\_/ \\___|_|\n");
    printf(" |___/\n");
    printf("\n");
    sleep(2);
    
    clear_screen();
    draw_title();
    
    printf("            💀 게임 오버! 💀\n\n");
    
    printf("            ");
    for (int i = 0; i < BOX_WIDTH; i++) {
        printf("#");
    }
    printf("\n");
    
    // 제목 중앙 정렬
    printf("            #                                 ⏰ 시간 초과! ⏰                             #\n");
    
    // 구분선
    printf("            #");
    for (int i = 0; i < BOX_WIDTH - 2; i++) {
        printf("-");
    }
    printf("#\n");
    
    // 메시지 1: 시간 초과
    printf("            # 😔 스테이지 %d에서 시간이 초과되었습니다.                                      #\n", game_state->current_stage);
    
    // 메시지 2: 다시 도전
    printf("            # 💪 다시 도전해보세요!                                                         #\n");
    
    // 박스 하단
    printf("            #");
    for (int i = 0; i < BOX_WIDTH - 2; i++) {
        printf("#");
    }
    printf("#\n");
    
    printf("\n            메인 메뉴로 돌아갑니다...\n");
    
    // 3초 카운트다운
    for (int i = 3; i > 0; i--) {
        printf("            %d초 후 메인 메뉴로 이동합니다...\n", i);
        sleep(1);
    }
    
    printf("            메인 메뉴로 돌아갑니다.\n");
    sleep(1);
} 