NAME			=	libasm.a

SRCS			=	ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s ft_read.s ft_strdup.s
OBJS			=	$(SRCS:.s=.o)

ASM				=	nasm
ASM_FLAGS	=	-f elf64

CC				=	cc
C_FLAGS		=	-Wall -Wextra -Werror
SANITIZE	=	-fsanitize=address -g

.PHONY: all clean fclean re test

all: $(NAME)

$(NAME): $(OBJS)
	 ar rcs $(NAME) $(OBJS)

%.o: %.s
	$(ASM) $(ASM_FLAGS) $< -o $@

test :
	$(CC) $(C_FLAGS) $(SANITIZE) main.c $(NAME) -L. -lasm

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME) ./a.out test_write.txt test_read.txt

re: fclean all
