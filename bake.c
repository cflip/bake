#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TARGET_MAX_CMDS 8

struct target
{
	struct target *next;
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

static void parse_makefile(struct target **first_target, FILE *fp)
{
	struct target *curtarget = *first_target;
	char *line = NULL;
	size_t nline = 0;

	while (getline(&line, &nline, fp) > 0)
	{
		switch (line[0])
		{
		case '\t':
			add_target_command(curtarget, line);
			break;
		case '#':
		case '\n':
			break;
		default:
			if (curtarget == NULL)
			{
				curtarget = malloc(sizeof(struct target));
				parse_target_line(curtarget, line);
				*first_target = curtarget;
			}
			else
			{
				curtarget->next = malloc(sizeof(struct target));
				parse_target_line(curtarget->next, line);
				curtarget = curtarget->next;
			}

			break;
		}
	}

}

int main(int argc, char *argv[])
{
	FILE *fp;

	struct target *first_target = NULL;

	fp = fopen("Makefile", "r");
	if (!fp)
	{
		perror("bake: Makefile: ");
		return EXIT_FAILURE;
	}

	parse_makefile(&first_target, fp);

	fclose(fp);

	printf("LIST OF TARGETS:\n");
	for (struct target *cur = first_target; cur != NULL; cur = cur->next)
	{
		printf("%s\n", cur->name);
	}

	if (first_target != NULL)
	{
		target_run(first_target);
	}

	return EXIT_SUCCESS;
}
