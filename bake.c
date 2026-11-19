#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET_MAX_CMDS 8

struct target
{
	char *name;
	// TODO: laziness
	char *cmds[TARGET_MAX_CMDS];
};

static void parse_target_line(struct target *target, const char *line)
{
	char accum[80];
	int naccum = 0;
	const char *head = line;

	/* search for colon */
	while (*head != ':')
	{
		if (*head == '\0')
		{
			fprintf(stderr, "bake: missing colon in target: %s\n", line);
			return;
		}

		accum[naccum++] = *head;
		head++;
	}
	accum[naccum] = '\0';

	target->name = strdup(accum);
}

static void add_target_command(struct target *target, const char *line)
{
	char **cmd = target->cmds;
	int n = 0;
	while (*cmd != NULL)
	{
		if (n == TARGET_MAX_CMDS)
		{
			fprintf(stderr, "bake: '%s': targets can only have 8 commands\n", target->name);
			return;
		}
		n++;
		cmd++;
	}

	// Add the command without the leading tab and trailing newline
	*cmd = strndup(line + 1, strlen(line) - 2);
}

static void target_run(struct target *target)
{
	printf("] BAKING '%s'\n", target->name);

	for (char **cmd = target->cmds; *cmd != NULL; cmd++)
	{
		printf("%s\n", *cmd);
		system(*cmd);
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;
	char *line = NULL;
	size_t nline = 0;
	struct target target = { 0 };

	fp = fopen("Makefile", "r");
	if (!fp)
	{
		perror("bake: Makefile: ");
		return EXIT_FAILURE;
	}

	while (getline(&line, &nline, fp) > 0)
	{
		if (line[0] == '\t')
		{
			add_target_command(&target, line);
		}
		else
		{
			parse_target_line(&target, line);
		}
	}

	fclose(fp);

	target_run(&target);

	return EXIT_SUCCESS;
}
