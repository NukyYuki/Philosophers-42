NAME		= philo

# Compiler and flags
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -pthread -g
DEBUG_FLAGS	= -fsanitize=thread
INCLUDE		= -I includes/

# Directories
SRCDIR		= srcs/
INCDIR		= includes/

# Source files
SRC_FILES	= main.c \
			  parse.c \
			  init.c \
			  utils.c \
			  sim.c \
			  cleanup.c

# Object files (in src/)
SRCS		= $(addprefix $(SRCDIR), $(SRC_FILES))
OBJS		= $(SRCS:.c=.o)

# Header files
HEADERS		= $(INCDIR)philosophers.h $(INCDIR)structs.h

# Colors for pretty output
GREEN		= \033[0;32m
RED			= \033[0;31m
BLUE		= \033[0;34m
YELLOW		= \033[0;33m
NC			= \033[0m # No Color

# Rules
all: $(NAME)

$(NAME): $(OBJS)
	@echo "$(BLUE)Linking $(NAME)...$(NC)"
	@$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(NC)"

$(SRCDIR)%.o: $(SRCDIR)%.c $(HEADERS)
	@echo "$(BLUE)Compiling $<...$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

# Debug version with thread sanitizer
debug: CFLAGS += $(DEBUG_FLAGS)
debug: re
	@echo "$(YELLOW)Debug version compiled with thread sanitizer$(NC)"

# Clean object files
clean:
	rm -f $(OBJS)

# Clean everything
fclean: clean
	@if [ -f $(NAME) ]; then \
		rm -f $(NAME); \
		echo "$(RED)✓ $(NAME) removed$(NC)"; \
	fi

# Rebuild everything
re: fclean all

# Norminette check (if you're using it)
norm:
	@echo "$(BLUE)Checking norminette...$(NC)"
	@norminette $(SRCS) $(HEADERS)

# Test with different parameters
test: $(NAME)
	@echo "$(YELLOW)Running basic tests...$(NC)"
	@echo "$(BLUE)Test 1: 5 philosophers, should run indefinitely$(NC)"
	@timeout 5s ./$(NAME) 5 800 200 200 || true
	@echo "$(BLUE)Test 2: 4 philosophers with tight timing$(NC)"
	@timeout 5s ./$(NAME) 4 310 200 100 || true
	@echo "$(BLUE)Test 3: 1 philosopher (should die)$(NC)"
	@timeout 5s ./$(NAME) 1 800 200 200 || true

# Memory leak check with valgrind
valgrind: $(NAME)
	@echo "$(YELLOW)Running valgrind...$(NC)"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) 5 800 200 200 5

# Thread race condition check
helgrind: $(NAME)
	@echo "$(YELLOW)Running helgrind (thread race detector)...$(NC)"
	valgrind --tool=helgrind ./$(NAME) 5 800 200 200 5

# Show help
help:
	@echo "$(GREEN)Available targets:$(NC)"
	@echo "  $(BLUE)all$(NC)      - Build the project"
	@echo "  $(BLUE)debug$(NC)    - Build with debug flags and thread sanitizer"
	@echo "  $(BLUE)clean$(NC)    - Remove object files"
	@echo "  $(BLUE)fclean$(NC)   - Remove object files and executable"
	@echo "  $(BLUE)re$(NC)       - Rebuild everything"
	@echo "  $(BLUE)norm$(NC)     - Check norminette"
	@echo "  $(BLUE)test$(NC)     - Run basic tests"
	@echo "  $(BLUE)valgrind$(NC) - Check for memory leaks"
	@echo "  $(BLUE)helgrind$(NC) - Check for thread race conditions"
	@echo "  $(BLUE)help$(NC)     - Show this help message"

# Phony targets
.PHONY: all clean fclean re debug norm test valgrind helgrind help