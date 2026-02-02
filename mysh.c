#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
//#include <limits.h>

#define CMD_SIZE 256
#define MAX_ARGS 20

int main(void)
{
    char buf[CMD_SIZE];
    char *args[MAX_ARGS];

    for ( ;; )
    {
        char cwd[PATH_MAX];
        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            continue;
        }

        printf("%s$mysh> ", cwd);

        if (fgets(buf, sizeof(buf), stdin) == NULL)
	{
            break;
        }

        size_t length = strlen(buf);
        if (length > 0 && buf[length - 1] == '\n')
	{
            buf[length - 1] = '\0';
	}

        if (strlen(buf) == 0)
	{
            continue;
	}

        int i = 0;
        char *token = strtok(buf, " ");

        while (token != NULL && i < MAX_ARGS - 1)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }

        args[i] = NULL;

        if (strcmp(args[0], "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(args[0], "pwd") == 0)
        {
            printf("%s\n", cwd);
        }
        else if (strcmp(args[0], "cd") == 0)
        {
            if (args[1] == NULL)
            {
                printf("cd missing arg\n");
            }
            else
            {
                if (chdir(args[1]) != 0)
		{
                    perror("Failed to cd");
		}
            }
        }
        else
        {
            pid_t pid = fork();

            if (pid == 0)
            {
                execvp(args[0], args);
                perror("Failed to execute command");
                exit(1);
            }
            else if (pid > 0)
            {
                wait(NULL);
            }
            else
            {
                perror("Failed to create process");
            }
        }
    }

    return 0;
}
