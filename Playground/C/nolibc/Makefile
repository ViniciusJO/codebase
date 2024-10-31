# Nome do executável
TARGET = app

# Diretórios
SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = bin

# Compilador e flags
CXX = gcc
CXXFLAGS = -std=c23 -Wall -Wextra -g

# Diretórios de cabeçalhos adicionais
INCLUDE_DIRS = -Iinclude
# Diretórios de bibliotecas adicionais
LIB_DIRS = -Llib

# Bibliotecas a serem vinculadas
LIBS =

# Cores ANSI
COLOR_RESET = \033[0m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_BLUE = \033[34m

# Listar todos os arquivos .c como fontes
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Substituir .c por .o para criar a lista de objetos
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

# Número total de fontes e objetos
TOTAL_SRCS = $(words $(SRCS))
TOTAL_OBJS = $(words $(OBJS))

# Contador de progresso
PROGRESS = 0
INCREMENT = $(shell echo "scale=2; 100/$(TOTAL_SRCS)" | bc)

# Função para incrementar e mostrar progresso
define progress_bar
	$(eval PROGRESS=$(shell echo "$(PROGRESS) + $(INCREMENT)" | bc))
	@echo -ne "$(COLOR_BLUE)[Progress]$(COLOR_RESET) $(PROGRESS)%\r"
endef

# Regra padrão
all: $(BIN_DIR)/$(TARGET)

# TODO: fix nolibc
# Regra para compilar o executável
$(BIN_DIR)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo -e "$(COLOR_GREEN)[Linking]$(COLOR_RESET) ./$@ <== $^"
	echo "$(NOLIBC)"
	$(CXX) $(OBJS) $([[ -n "$(NOLIBC)" ]] && printf "-nostdlib"`) -o $@ $(LIB_DIRS) $(LIBS)
	$(call progress_bar)

# Regra para compilar os arquivos objeto
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo -e "$(COLOR_YELLOW)[Compiling]$(COLOR_RESET) $<"
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@
	$(call progress_bar)

watch:
	@while true; do \
		echo -e "\n$(COLOR_BLUE)------------------------------------------------$(COLOR_RESET)";\
		echo -e "$(COLOR_BLUE)--------------------RELOAD----------------------$(COLOR_RESET)";\
		echo -e "$(COLOR_BLUE)------------------------------------------------$(COLOR_RESET)";\
		make --quiet;\
		echo -e "$(COLOR_YELLOW)[Running]$(COLOR_RESET) ./$(BIN_DIR)/$(TARGET)\n";\
		./$(BIN_DIR)/$(TARGET) && echo "._.";\
		inotifywait -qr $(SRC_DIR) &> /dev/null;\
		pkill $(TARGET) &>/dev/null;\
	done;\
	pkill $(TARGET)

# Limpar os arquivos compilados
clean:
	@echo -e "$(COLOR_BLUE)[Cleaning]$(COLOR_RESET)"
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Limpar arquivos de configuração do clangd
clean-clangd:
	@echo -e "$(COLOR_BLUE)[Cleaning Clangd Configurations]$(COLOR_RESET)"
	rm -f compile_commands.json

# Gerar compile_commands.json usando Bear
compile_commands: clean-clangd
	@echo -e "$(COLOR_BLUE)[Generating]$(COLOR_RESET) compile_commands.json with Bear"
	bear -- make

symbols: $(OBJS)
	nm $^

# Regra para depuração
debug: CXXFLAGS += -DDEBUG
debug: all

# Phony targets
.PHONY: all clean clean-clangd compile_commands debug

