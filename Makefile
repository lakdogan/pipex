# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/06 18:45:41 by lakdogan          #+#    #+#              #
#    Updated: 2025/05/10 05:45:26 by lakdogan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC 					:= 	cc 
CFLAGS				:=	-Wall -Wextra -Werror

LIBFT_URL			:=	https://github.com/lakdogan/libft.git	
LIBFT_DIR			:=	libft
	
INCLUDES			:=	-I./mandatory/inc -I./$(LIBFT_DIR)
BONUS_INCLUDES		:=	-I./bonus/inc_bonus -I./$(LIBFT_DIR)
	
NAME				:=	pipex
BONUS_NAME			:=	pipex_bonus

LIBFT				:=	$(LIBFT_DIR)/libft_ftpf_gnl.a
	
OBJECTS_DIR			:=	./objs/
BONUS_OBJECTS_DIR   :=  ./bonus_objs/
MANDATORY_DIR		:=	./mandatory/
SRCS_DIR			:=	$(MANDATORY_DIR)srcs/
BONUS_DIR			:=	./bonus/
UTILS_DIR			:=	$(SRCS_DIR)utils/
CONTEXT_DIR			:=	$(UTILS_DIR)context/
EXECUTION_DIR		:=	$(UTILS_DIR)execution/
PARSING_DIR			:=	$(UTILS_DIR)parsing/
BONUS_SRCS_DIR		:=	$(BONUS_DIR)srcs_bonus/
BONUS_UTILS_DIR		:=	$(BONUS_SRCS_DIR)utils_bonus/

PIPEX_MANDATORY_FILES := \
				$(SRCS_DIR)pipex.c \
				$(SRCS_DIR)launch_command.c \
				$(SRCS_DIR)setup_child.c \
				$(SRCS_DIR)setup_parent.c \
				$(SRCS_DIR)shell_split.c \
				$(CONTEXT_DIR)init_context.c \
				$(CONTEXT_DIR)free_context.c \
				$(EXECUTION_DIR)path_lookup.c \
				$(EXECUTION_DIR)path_exec.c \
				$(PARSING_DIR)utils_shell_split.c \
				$(PARSING_DIR)process_token.c \
				$(UTILS_DIR)cleanup_and_exit.c \
				$(UTILS_DIR)free_tab.c \
				$(UTILS_DIR)ft_isspace.c \
				$(UTILS_DIR)ft_strndup.c \
				$(UTILS_DIR)free_shell_split.c \
				$(UTILS_DIR)direct_exec.c \

PIPEX_BONUS_FILES := \
				$(BONUS_SRCS_DIR)pipex_bonus.c \
				$(BONUS_SRCS_DIR)launch_command_bonus.c \
				$(BONUS_SRCS_DIR)handle_heredoc_bonus.c \
				$(BONUS_SRCS_DIR)handle_processes_bonus1.c \
				$(BONUS_SRCS_DIR)handle_processes_bonus2.c \
				$(BONUS_UTILS_DIR)cleanup_and_exit_bonus.c \
				$(BONUS_UTILS_DIR)init_context_bonus.c \
				$(BONUS_UTILS_DIR)free_context_bonus.c \

BONUS_UTILS_FILES := \
				$(SRCS_DIR)shell_split.c \
				$(EXECUTION_DIR)path_exec.c \
				$(EXECUTION_DIR)path_lookup.c \
				$(PARSING_DIR)process_token.c \
				$(PARSING_DIR)utils_shell_split.c \
				$(UTILS_DIR)free_tab.c \
				$(UTILS_DIR)ft_isspace.c \
				$(UTILS_DIR)ft_strndup.c \
				$(UTILS_DIR)free_shell_split.c \
				$(UTILS_DIR)direct_exec.c \

PIPEX_OBJS	:= $(patsubst $(SRCS_DIR)%.c,$(OBJECTS_DIR)srcs/%.o,$(PIPEX_MANDATORY_FILES))
PIPEX_BONUS_OBJS := $(patsubst $(BONUS_SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs_bonus/%.o,$(PIPEX_BONUS_FILES))
PIPEX_BONUS_UTILS_OBJS := $(patsubst $(SRCS_DIR)%.c,$(BONUS_OBJECTS_DIR)srcs/%.o,$(BONUS_UTILS_FILES))

all: $(LIBFT) $(NAME)

$(LIBFT_DIR):
	@if [ ! -d "$(LIBFT_DIR)" ]; \
	then git clone $(LIBFT_URL)	$(LIBFT_DIR); \
	fi
	# @cd $(LIBFT_DIR) && make

$(LIBFT): $(LIBFT_DIR)
	@$(MAKE) -C $(LIBFT_DIR)
	@cd $(LIBFT_DIR) && make
	@touch $(LIBFT)
		
$(NAME): $(LIBFT) $(PIPEX_OBJS)
	$(CC) $(CFLAGS) $(PIPEX_OBJS) $(LIBFT) -o $(NAME)
	
$(OBJECTS_DIR)%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

bonus: $(LIBFT) $(BONUS_NAME)

$(BONUS_NAME): $(LIBFT) $(PIPEX_BONUS_OBJS) $(PIPEX_BONUS_UTILS_OBJS)
	$(CC) $(CFLAGS) $(PIPEX_BONUS_OBJS) $(PIPEX_BONUS_UTILS_OBJS) $(LIBFT) -o $(BONUS_NAME)

$(OBJECTS_DIR)srcs/%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BONUS_OBJECTS_DIR)srcs_bonus/%.o: $(BONUS_SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BONUS_INCLUDES) -c $< -o $@

$(BONUS_OBJECTS_DIR)srcs/%.o: $(SRCS_DIR)%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(BONUS_INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJECTS_DIR)
	rm -rf $(BONUS_OBJECTS_DIR)
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) clean; fi

fclean: clean
	rm -f $(NAME)
	rm -f $(BONUS_NAME)
	@if [ -d "$(LIBFT_DIR)" ]; then $(MAKE) -C $(LIBFT_DIR) clean; fi
	rm -rf $(LIBFT_DIR)

re: fclean all
	

.PHONY: all bonus clean fclean re