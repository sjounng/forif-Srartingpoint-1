# 타자 연습 프로그램 Makefile

# 컴파일러 설정
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
DEBUG_CFLAGS = -Wall -Wextra -std=c99 -g -DDEBUG

# 디렉토리 설정
SRCDIR = src
OBJDIR = obj
BINDIR = bin
INCDIR = include

# 소스 파일들
SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS = $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# 실행 파일 이름
TARGET = $(BINDIR)/typing_practice

# 기본 타겟
all: directories $(TARGET)

# 디렉토리 생성
directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(BINDIR)

# 실행 파일 생성
$(TARGET): $(OBJECTS)
	@echo "🔗 링킹 중..."
	$(CC) $(OBJECTS) -o $(TARGET)
	@echo "✅ 빌드 완료: $(TARGET)"

# 오브젝트 파일 생성
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo "🔨 컴파일 중: $<"
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

# 디버그 빌드
debug: CFLAGS = $(DEBUG_CFLAGS)
debug: directories $(TARGET)

# 실행
run: $(TARGET)
	@echo "🚀 프로그램 실행 중..."
	./$(TARGET)

# 정리
clean:
	@echo "🧹 정리 중..."
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "✅ 정리 완료"

# 설치 (시스템 경로에 복사)
install: $(TARGET)
	@echo "📦 설치 중..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "✅ 설치 완료"

# 제거
uninstall:
	@echo "🗑️ 제거 중..."
	sudo rm -f /usr/local/bin/typing_practice
	@echo "✅ 제거 완료"

# 도움말
help:
	@echo "=== 타자 연습 프로그램 Makefile ==="
	@echo "사용 가능한 명령어:"
	@echo "  make        - 프로그램 빌드"
	@echo "  make debug  - 디버그 모드로 빌드"
	@echo "  make run    - 프로그램 실행"
	@echo "  make clean  - 빌드 파일 정리"
	@echo "  make install   - 시스템에 설치"
	@echo "  make uninstall - 시스템에서 제거"
	@echo "  make help   - 이 도움말 표시"

# 의존성 확인
depend:
	@echo "📋 의존성 확인 중..."
	$(CC) -MM -I$(INCDIR) $(SOURCES) > .depend

# .PHONY 타겟들
.PHONY: all clean debug run install uninstall help directories depend

# 의존성 파일 포함
-include .depend 