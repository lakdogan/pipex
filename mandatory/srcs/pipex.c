/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lakdogan <lakdogan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:48:38 by lakdogan          #+#    #+#             */
/*   Updated: 2025/05/10 07:40:21 by lakdogan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex.h"

/**
 * @brief Creates child processes to handle command execution
 *
 * Creates a pipe and forks twice to create two child processes:
 * - The first child handles the input file and first command
 * - The second child handles the second command and output file
 * The function sets the is_child flag 
 * to prevent double freeing in children.
 *
 * @param context The pipex context containing execution information
 * @param child1 Pointer to store the first child's PID
 * @param child2 Pointer to store the second child's PID
 */
static void	create_children(t_pipex *context, pid_t *child1, pid_t *child2)
{
	if (pipe(context->ends) == -1)
		cleanup_and_exit(context, "pipe creation failed", 2);
	*child1 = fork();
	if (*child1 < 0)
		cleanup_and_exit(context, "fork failed", 3);
	if (*child1 == 0)
	{
		context->is_child = 1;
		setup_child(context);
		cleanup_and_exit(context, "child process error", 1);
	}
	*child2 = fork();
	if (*child2 < 0)
		cleanup_and_exit(context, "fork failed", 3);
	if (*child2 == 0)
	{
		context->is_child = 1;
		setup_parent(context);
		cleanup_and_exit(context, "parent process failed", 1);
	}
}

/**
 * @brief Waits for child processes to complete and determines exit code
 *
 * Closes pipe ends in the parent and waits for both child processes to finish.
 * The exit code is determined based on the second command's exit status,
 * following shell conventions for reporting exit codes.
 *
 * @param context The pipex context for resource cleanup
 * @param child1 PID of the first child process
 * @param child2 PID of the second child process
 * @return The appropriate exit code based on child process termination
 */
static int	wait_children(t_pipex *context, pid_t child1, pid_t child2)
{
	int	status1;
	int	status2;
	int	exit_code;

	close(context->ends[0]);
	close(context->ends[1]);
	if (waitpid(child1, &status1, 0) == -1)
		cleanup_and_exit(context, "waitpid failed", 1);
	if (waitpid(child2, &status2, 0) == -1)
		cleanup_and_exit(context, "waitpid failed", 1);
	if (WIFSIGNALED(status2))
		exit_code = 128 + WTERMSIG(status2);
	else if (WIFEXITED(status2))
		exit_code = WEXITSTATUS(status2);
	else
		exit_code = 1;
	return (exit_code);
}

/**
 * @brief Main process handler that creates children and manages their execution
 *
 * Creates child processes, waits for them to complete, and returns
 * the appropriate exit code.
 *
 * @param context The pipex context containing execution information
 * @return The exit code to be returned by the program
 */
int	handle_processes(t_pipex *context)
{
	pid_t	child1;
	pid_t	child2;
	int		exit_code;

	create_children(context, &child1, &child2);
	exit_code = wait_children(context, child1, child2);
	return (exit_code);
}

/**
 * @brief Main entry point for the pipex program
 *
 * Validates command line arguments, initializes the pipex context,
 * handles the creation and execution of processes, and ensures proper
 * resource cleanup before exit.
 *
 * @param argc Argument count
 * @param argv Array of argument strings
 * @param envp Array of environment variables
 * @return Exit code indicating success (0) or various error conditions
 */
int	main(int argc, char **argv, char **envp)
{
	t_pipex	*context;
	int		exit_code;

	if (argc != 5)
	{
		write(STDERR_FILENO, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 40);
		return (EXIT_FAILURE);
	}
	context = init_context(argv, envp);
	if (!context)
		return (EXIT_FAILURE);
	exit_code = handle_processes(context);
	free_context(context);
	return (exit_code);
}

//
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//            MANUAL TEST
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
// # Test 1: Basic text transformation and sorting
// # Converts lowercase to uppercase, then sorts the result
// # Create test file: echo -e "apple\nzebra\nbanana\nlemon" > infile.txt
// ./pipex infile.txt "tr a-z A-Z" "sort" result.txt
// < infile.txt tr a-z A-Z | sort > org_result.txt
//
// # Test 2: Pattern matching and counting
// # Finds lines containing "Lorem" and counts them
// # Create test file: 
//	echo -e "Lorem ipsum dolor\nHello world\nLorem again" > infile.txt
// ./pipex infile.txt "grep Lorem" "wc -l" outfile.txt
// < infile.txt grep Lorem | wc -l > org_outfile.txt
//
// # Test 3: Sorting and counting unique occurrences
// # Sorts lines alphabetically, then counts occurrences of each unique line
// # Create test file: 
//	echo -e "apple\nbanana\napple\norange\nbanana\nkiwi" > fruits.txt
// < fruits.txt sort | uniq -c > org_counts.txt
// ./pipex fruits.txt "sort" "uniq -c" counts.txt
//
// # Test 4: Field extraction and unique sorting
// # Extracts usernames from /etc/passwd and sorts them uniquely
// # Uses existing system file: /etc/passwd
// < /etc/passwd cut -d: -f1 | sort -u > org_users.txt
// ./pipex /etc/passwd "cut -d: -f1" "sort -u" users.txt
//
// # Test 5: Case conversion and line limiting
// # Converts uppercase to lowercase and shows only first 3 lines
// # Create test file: echo -e "HELLO\nWorld\nTEST\nExample" > mix.txt
// < mix.txt tr '[:upper:]' '[:lower:]' | head -n 3 > org_head3.txt
// ./pipex mix.txt "tr [:upper:] [:lower:]" "head -n 3" head3.txt
//
// # Test 6: String replacement with special characters
// # Replaces hyphens with forward slashes, then filters lines containing "05"
// # Create test file: 
//	echo -e "2023-05-15\n2023-06-20\n2023-05-30\n2023-07-10" > date.txt
// < date.txt sed 's/-/\//g' | grep 05 > org_result1.txt
// ./pipex date.txt "sed s/-/\\//g" "grep 05" result1.txt
//
// # Test 7: Using absolute paths to commands
// # Counts bytes in file, then counts the number of lines in that output
// # Reuses infile.txt created in Test 1
// < infile.txt /usr/bin/wc -c | /usr/bin/wc -l > org_absout.txt
// ./pipex infile.txt "/usr/bin/wc -c" "/usr/bin/wc -l" absout.txt
//
//
// ERROR TESTS
//
// # Test 8: Error handling for non-existent input file
// # Should handle gracefully when input file doesn't exist
// # No file creation needed (intentionally using non-existent file)
// < nofile.txt grep a | wc -l > out.txt
// ./pipex nofile.txt "grep a" "wc -l" out.txt
//
// # Test 9: Error handling for non-existent command
// # Should handle gracefully when command doesn't exist
// # Create test file: echo "Hello, world!" > hello.txt
// < hello.txt foobar | wc -l > out.txt
// ./pipex hello.txt "foobar" "wc -l" out.txt
//
// @@@@@@@@@@@@@@@@@@@@@@@@@@@
//         TEST SCRIPT
// @@@@@@@@@@@@@@@@@@@@@@@@@@@
//   Make or Make re then
//   Copy this script after #!/bin/bash, then put it into empty C-file,
//   cmd + a then cmd + / then cmd + a and last cmd + c
//   create a .sh file (test.sh)
//   chmod +x test.sh or chmod 755 test.sh
//   then go on that file and cmd + v
//   run it withing the same directory like the programm
//
// #!/bin/bash
//
// # Colors for output
// GREEN="\033[32m"
// RED="\033[31m"
// RESET="\033[0m"
//
// PASS="[${GREEN}PASS${RESET}]"
// FAIL="[${RED}FAIL${RESET}]"
//
// PIPEX=./pipex
// INFILE=infile_test
// OUTFILE=outfile_test
//
// total=0
// success=0
//
// # Create base dummy input
// echo -e "apple\nbanana\navocado\napricot" > $INFILE
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
//		 "$desc" "$actual" "$expected_code"
// 	fi
// }
//
// echo -e "\n🌐 PIPEX – BASH BEHAVIOR TEST SUITE\n"
//
// # Bash : bash -c "grep a < infile_test | wc -l"
// # Pipex: ./pipex infile_test "grep a" "wc -l" outfile_test
// test_case "Valid command (grep | wc)" \
// 	"$PIPEX $INFILE \"grep a\" \"wc -l\" $OUTFILE" 0
//
// # Bash : bash -c "doesnotexist < infile_test | wc -l"
// # Pipex: ./pipex infile_test "doesnotexist" "wc -l" outfile_test
// test_case "Invalid first command" \
// 	"$PIPEX $INFILE \"doesnotexist\" \"wc -l\" $OUTFILE" 0
//
// # Bash : bash -c "grep a < infile_test | nosuchcmd"
// # Pipex: ./pipex infile_test "grep a" "nosuchcmd" outfile_test
// test_case "Invalid second command" \
// 	"$PIPEX $INFILE \"grep a\" \"nosuchcmd\" $OUTFILE" 127
//
// # Bash : bash -c "nosuch1 < infile_test | nosuch2"
// # Pipex: ./pipex infile_test "nosuch1" "nosuch2" outfile_test
// test_case "Both commands invalid" \
// 	"$PIPEX $INFILE \"nosuch1\" \"nosuch2\" $OUTFILE" 127
//
// # Bash : bash -c "grep a < missingfile | wc -l"
// # Pipex: ./pipex missingfile "grep a" "wc -l" outfile_test
// test_case "Missing input file" \
// 	"$PIPEX missingfile \"grep a\" \"wc -l\" $OUTFILE" 0
//
// # Bash : bash -c "./nosuchcmd < infile_test | weqwec -l"
// # Pipex: ./pipex infile_test "./nosuchcmd" "weqwec -l" outfile_test
// test_case "Command with slash (./nosuchcmd)" \
// 	"$PIPEX $INFILE \"./nosuchcmd\" \"weqwec -l\" $OUTFILE" 127
//
// # Bash : bash -c "cat < infile_test | grep xyz"
// # Pipex: ./pipex infile_test "cat" "grep xyz" outfile_test
// test_case "cmd2 returns logical exit 1 (grep no match)" \
// 	"$PIPEX $INFILE \"cat\" \"grep xyz\" $OUTFILE" 1
//
// # Bash : bash -c "cat < infile_test | false"
// # Pipex: ./pipex infile_test "cat" "false" outfile_test
// test_case "cmd2 is 'false' (always returns 1)" \
// 	"$PIPEX $INFILE \"cat\" \"false\" $OUTFILE" 1
//
// ###############################
// # Extended real-world tests 
// ###############################
//
// # Bash : < infile.txt tr a-z A-Z | sort > org_result.txt
// # Pipex: ./pipex infile.txt "tr a-z A-Z" "sort" result.txt
// echo -e "apple\nzebra\nbanana\nlemon" > infile.txt
// test_case "Basic text transformation and sorting" \
// 	"$PIPEX infile.txt \"tr a-z A-Z\" \"sort\" result.txt" 0
//
// # Bash : < infile.txt grep Lorem | wc -l > org_outfile.txt
// # Pipex: ./pipex infile.txt "grep Lorem" "wc -l" outfile.txt
// echo -e "Lorem ipsum dolor\nHello world\nLorem again" > infile.txt
// test_case "Pattern matching and counting (grep | wc)" \
// 	"$PIPEX infile.txt \"grep Lorem\" \"wc -l\" outfile.txt" 0
//
// # Bash : < fruits.txt sort | uniq -c > org_counts.txt
// # Pipex: ./pipex fruits.txt "sort" "uniq -c" counts.txt
// echo -e "apple\nbanana\napple\norange\nbanana\nkiwi" > fruits.txt
// test_case "Sorting and counting unique lines (uniq -c)" \
// 	"$PIPEX fruits.txt \"sort\" \"uniq -c\" counts.txt" 0
//
// # Bash : < /etc/passwd cut -d: -f1 | sort -u > org_users.txt
// # Pipex: ./pipex /etc/passwd "cut -d: -f1" "sort -u" users.txt
// test_case "Extract usernames from /etc/passwd and sort uniquely" \
// 	"$PIPEX /etc/passwd \"cut -d: -f1\" \"sort -u\" users.txt" 0
//
// # Bash : < mix.txt tr '[:upper:]' '[:lower:]' | head -n 3 > org_head3.txt
// # Pipex: ./pipex mix.txt "tr '[:upper:]' '[:lower:]'" "head -n 3" head3.txt
// echo -e "HELLO\nWorld\nTEST\nExample" > mix.txt
// test_case "Convert to lowercase and limit lines (head)" \
// 	"$PIPEX mix.txt \"tr '[:upper:]' '[:lower:]'\" \"head -n 3\" head3.txt" 0
//
// # Bash : < date.txt sed 's/-/\//g' | grep 05 > org_result1.txt
// # Pipex: ./pipex date.txt "sed s/-/\\\/g" "grep 05" result1.txt
// echo -e "2023-05-15\n2023-06-20\n2023-05-30\n2023-07-10" > date.txt
// test_case "Replace hyphens and grep 05" \
// 	"$PIPEX date.txt \"sed s/-/\\\\\\\\\\//g\" \"grep 05\" result1.txt" 0
//
// # Bash : < infile.txt /usr/bin/wc -c | /usr/bin/wc -l > org_absout.txt
// # Pipex: ./pipex infile.txt "/usr/bin/wc -c" "/usr/bin/wc -l" absout.txt
// test_case "Use absolute paths for wc commands" \
// 	"$PIPEX infile.txt \"/usr/bin/wc -c\" \"/usr/bin/wc -l\" absout.txt" 0
//
// #######################
// # Error handling tests
// #######################
//
// # Bash : < nofile.txt grep a | wc -l > out.txt
// # Pipex: ./pipex nofile.txt "grep a" "wc -l" out.txt
// test_case "Error: non-existent input file" \
// 	"$PIPEX nofile.txt \"grep a\" \"wc -l\" out.txt" 0
//
// # Bash : < hello.txt foobar | wc -l > out.txt
// # Pipex: ./pipex hello.txt "foobar" "wc -l" out.txt
// echo "Hello, world!" > hello.txt
// test_case "Error: non-existent command" \
// 	"$PIPEX hello.txt \"foobar\" \"wc -l\" out.txt" 0
//
// ### Summary
//
// echo -e "\n📊 Result: $success out of $total tests passed."
//
// # Clean up
// rm -f $INFILE $OUTFILE infile.txt outfile.txt fruits.txt mix.txt date.txt \
// hello.txt absout.txt result.txt users.txt counts.txt head3.txt result1.txt