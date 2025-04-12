#include <unistd.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>

int main() {
	int sv_in[2];  // pour stdin de l'enfant
	int sv_out[2]; // pour stdout de l'enfant

	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv_in) == -1) {
		perror("socketpair in");
		exit(1);
	}
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, sv_out) == -1) {
		perror("socketpair out");
		exit(1);
	}

	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		exit(1);
	} else if (pid == 0) {
		// Enfant
		close(sv_in[1]);
		close(sv_out[0]);
		dup2(sv_in[0], 0);   // stdin
		dup2(sv_out[1], 1);  // stdout
		close(sv_in[0]);
		close(sv_out[1]);

		char *args[] = { (char *)"./script.php", NULL };
		execve("./script.php", args, NULL);
		perror("execve");
		exit(1);
	} else {
		// Parent
		close(sv_in[0]);
		close(sv_out[1]);

		std::string msg = "Prenom=COUCOU&Nom=MAXXX\n";
        write(sv_in[1], msg.c_str(), msg.size());
		close(sv_in[1]);

		char buffer[1024];
		ssize_t count = read(sv_out[0], buffer, sizeof(buffer) - 1);
		if (count > 0) {
			buffer[count] = '\0';
			std::cout << "Parent received:\n" << buffer;
		}
		close(sv_out[0]);
		waitpid(pid, NULL, 0);
	}
}
