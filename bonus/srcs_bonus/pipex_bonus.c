/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 04:25:23 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 07:56:00 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc_bonus/pipex_bonus.h"

/**
 * @brief Prints usage instructions to stderr
 *
 * @param exit_code The exit code to return
 * @return The exit code provided
 */
static int	print_usage(int exit_code)
{
	write(STDERR_FILENO, "Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2\n", 47);
	write(STDERR_FILENO, "   or: ./pipex here_doc LIMITER cmd1 cmd2 file\n",
		47);
	return (exit_code);
}

/**
 * @brief Initializes the context structure for heredoc mode
 *
 * @param context Double pointer to the context structure
 * @param argv Array of command line arguments
 * @param envp Array of environment variables
 * @param argc Number of command line arguments
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
static int	init_heredoc_context(t_pipex **context, char **argv, char **envp,
		int argc)
{
	*context = malloc(sizeof(t_pipex));
	if (!*context)
		return (EXIT_FAILURE);
	ft_memset(*context, 0, sizeof(t_pipex));
	(*context)->env_vars = envp;
	(*context)->outfile_path = argv[argc - 1];
	(*context)->is_heredoc = 1;
	(*context)->limiter = ft_strdup(argv[2]);
	(*context)->is_child = 0;
	(*context)->cleaned = 0;
	(*context)->cmd_count = argc - 4;
	(*context)->pipe_count = (*context)->cmd_count - 1;
	return (EXIT_SUCCESS);
}

/**
 * @brief Sets up pipes for heredoc mode
 *
 * @param context Pointer to the pipex context structure
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
static int	setup_heredoc_pipes(t_pipex *context)
{
	int	i;

	context->input_missing = 0;
	context->pipes = malloc(sizeof(int) * 2 * context->pipe_count);
	if (!context->pipes)
	{
		free(context->limiter);
		free(context);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (i < context->pipe_count)
	{
		if (pipe(context->pipes + (i * 2)) < 0)
		{
			free(context->limiter);
			free(context->pipes);
			free(context);
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

/**
 * @brief Handles the here_doc special case
 *
 * @param context Double pointer to the context structure
 * @param argv Array of command line arguments
 * @param envp Array of environment variables
 * @param argc Number of command line arguments
 * @return Exit code from executing the commands
 */
static int	handle_heredoc_case(t_pipex **context, char **argv, char **envp,
		int argc)
{
	int	exit_code;

	exit_code = 0;
	if (argc < 6)
	{
		write(STDERR_FILENO,
			"Usage for here_doc: ./pipex here_doc LIMITER cmd1 cmd2 file\n",
			60);
		return (exit_code);
	}
	if (init_heredoc_context(context, argv, envp, argc) == EXIT_FAILURE)
		return (exit_code);
	if (setup_heredoc_pipes(*context) == EXIT_FAILURE)
		return (exit_code);
	handle_heredoc(*context);
	exit_code = handle_processes(*context, argv);
	free_context(*context);
	return (exit_code);
}

/**
 * @brief Main function of the pipex program
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @param envp Array of environment variables
 * @return Exit code from program execution
 */
int	main(int argc, char **argv, char **envp)
{
	t_pipex	*context;
	int		exit_code;

	exit_code = 0;
	if (argc < 5)
		return (print_usage(exit_code));
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		return (handle_heredoc_case(&context, argv, envp, argc));
	context = init_context(argc, argv, envp);
	if (!context)
		return (EXIT_FAILURE);
	exit_code = handle_processes(context, argv);
	free_context(context);
	return (exit_code);
}

// here_doc
// ./pipex_bonus here_doc END "tr a-z A-Z" "cat" result.txt
// cat << END | tr a-z A-Z | cat > result.txt
//
//
// #!/bin/bash
//
// # Colors
// GREEN="\033[32m"
// RED="\033[31m"
// RESET="\033[0m"
//
// PASS="[${GREEN}PASS${RESET}]"
// FAIL="[${RED}FAIL${RESET}]"
//
// PIPEX=./pipex_bonus
// INFILE=infile_bonus
// OUTFILE=outfile_bonus
//
// total=0
// success=0
//
// # Prepare input
// echo -e "orange\nbanana\napple\nlemon" > $INFILE
//
// function test_case() {
// 	total=$((total+1))
// 	desc=$1
// 	cmd="$2"
// 	expected_code=$3
//
// 	eval "$cmd" > /dev/null 2>&1
// 	actual=$?
//
// 	if [ "$actual" -eq "$expected_code" ]; then
// 		printf "$PASS %-60s (exit: %s)\n" "$desc" "$actual"
// 		success=$((success+1))
// 	else
// 		printf "$FAIL %-60s (exit: %s, expected: %s)\n" \
//		"$desc" "$actual" "$expected_code"
// 	fi
// }
//
// echo -e "\n🌟 PIPEX BONUS – MULTI-COMMAND TEST SUITE\n"
//
// # Bash : < infile_bonus sort | grep a | wc -l > outfile_bonus
// # Pipex: ./pipex_bonus infile_bonus "sort" "grep a" "wc -l" outfile_bonus
// test_case "3 commands: sort | grep a | wc -l" \
// 	"$PIPEX $INFILE \"sort\" \"grep a\" \"wc -l\" $OUTFILE" 0
//
// # Bash : < infile_bonus sort | tr a-z A-Z | grep O > outfile_bonus
// # Pipex: 
// ./pipex_bonus infile_bonus "sort" "tr a-z A-Z" "grep O" outfile_bonus
// test_case "3 commands: sort | uppercase | grep O" \
// 	"$PIPEX $INFILE \"sort\" \"tr a-z A-Z\" \"grep O\" $OUTFILE" 0
//
// # Bash : < infile_bonus grep orange | cat | cat | wc -c > outfile_bonus
// # Pipex: 
// ./pipex_bonus infile_bonus "grep orange" "cat" "cat" "wc -c" outfile_bonus
// test_case "4 commands: grep | cat | cat | wc -c" \
// 	"$PIPEX $INFILE \"grep orange\" \"cat\" \"cat\" \"wc -c\" $OUTFILE" 0
//
// # Bash : < infile_bonus grep banana | grep apple | wc -l > outfile_bonus
// # Pipex: 
// ./pipex_bonus infile_bonus "grep banana" "grep apple" "wc -l" outfile_bonus
// test_case "2 filters: grep banana | grep apple" \
// 	"$PIPEX $INFILE \"grep banana\" \"grep apple\" \"wc -l\" $OUTFILE" 0
//
// # Bash : < infile_bonus grep kiwi | wc -l > outfile_bonus
// # Pipex: ./pipex_bonus infile_bonus "grep kiwi" \"wc -l\" $OUTFILE
// test_case "grep returns nothing (expect wc 0 lines)" \
// 	"$PIPEX $INFILE \"grep kiwi\" \"wc -l\" $OUTFILE" 0
//
// # Bash : < infile_bonus grep orange | false | wc -l > outfile_bonus
// # Pipex: 
// ./pipex_bonus infile_bonus "grep orange" "false" "wc -l" outfile_bonus
// test_case "middle command fails (false)" \
// 	"$PIPEX $INFILE \"grep orange\" \"false\" \"wc -l\" $OUTFILE" 0
//
// echo -e "\n📊 Result: $success out of $total tests passed."
//
// # Cleanup
// rm -f $INFILE $OUTFILE