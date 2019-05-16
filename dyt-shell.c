#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

char *read_line() {
	char *line = NULL;
	size_t bufsize = 0;
	getline(&line, &bufsize, stdin);

	// Trimming newline character.
	line[strcspn(line, "\n")] = '\0';

	return line;
}

void shell_exit() {
	printf("\n");
	exit(0);
}

pid_t run(char *filename) {
	char *argv[] = {filename, NULL};
	pid_t pid = fork();
	
	if (pid == 0) {
		// Child process
		if (execvp(argv[0], argv) < 0) {
			printf("Exec error.\n");
		}
	} 	

	// Parent process
	return pid;

}

void join(pid_t pid) {
	int child_status;

	do {
		waitpid(pid, &child_status, WUNTRACED);
	} while (!WIFEXITED(child_status) && !WIFSIGNALED(child_status));
}

int main() {
	
	char *line = NULL;

	while (1) {
		printf("$ ");
		line = read_line();

		if (strlen(line) == 0) {
			shell_exit();
		}

		//TODO: Should probably check the return value of `run` before joining.
		join(run(line));

		free(line);
	}

}

