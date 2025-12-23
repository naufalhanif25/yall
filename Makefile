include config.mk

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
    CXX := g++
    RM  := rm -rf
    EXE :=
endif

ifeq ($(UNAME_S),Darwin)
    CXX := clang++
    RM  := rm -rf
    EXE :=
endif

ifeq ($(OS),Windows_NT)
    CXX := g++
    RM  := del /Q
    EXE := .exe
endif

APP_BIN := $(BUILD_DIR)/$(APP_NAME)$(EXE)

.PHONY: build run dev clean test

build:
	$(CXX) $(FLAGS) -o $(APP_BIN) $(SRC_DIR)/*.cpp $(MOD_DIR)/*.cpp $(LIB_DIR)/*.cpp $(INCLUDE_DIR)/*.cpp

run:
	./$(APP_BIN) --run src/main.yall

dev: build run

clean:
	$(RM) $(BUILD_DIR)/*

test:
	$(CXX) $(FLAGS) $(TEST_DIR)/*.cpp $(INCLUDE_DIR)/*.cpp $(LIB_DIR)/*.cpp -o $(BUILD_DIR)/test$(EXE) && ./$(BUILD_DIR)/test$(EXE)
	$(RM) $(BUILD_DIR)/test$(EXE)