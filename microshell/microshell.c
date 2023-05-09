#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>


int ft_strlen(char *str)
{
    int i = 0;
    while(str[i])
        i++;
    return(i);
}
void cd (char **cmd_text)
{
	int a;
	int i = 0;
	if(cmd_text[1] == NULL)
	{
		write(2,"error: cd: bad arguments\n",26);
		return;
	}else if(cmd_text[1][0] == '-')
		{
			write(2,"error: cd: bad arguments\n",26);
			return;
		}
	a = chdir(cmd_text[1]);
	if(a == -1)
	{
		write(2,"cd: cannot change directory to ",32);
		while(cmd_text[1][i])
		{
			write(2,&cmd_text[1][i],1);
			i++;
		}
		write(2,"\n",1);
		return ;
	}
}
void *ft_calloc(int size ,int len)
{
    void *str = malloc(size * len);
    int i = 0;
    char *help;
    help = (char *)str;
    while(i < size * len)
        help [i++] = '\0';
    return(str);
}
int len2d(char **str)
{
    int i = 0;
    while(str[i])
        i++;
    return i;
}

char **ft_strjoin2d(char **table,char *new)
{
    char **str2d;
    int len ;
    int i = 0;
    if(table == NULL)
    {
        str2d  = ft_calloc(sizeof(char *) ,2);
        *str2d = new;
        return (str2d);
    }
    len = len2d(table);
    str2d = ft_calloc(sizeof(char *),len + 2);
    while(table[i])
    {
        str2d[i] = table[i];
        i++;
    }

    str2d[i] = new;
    return (str2d);

}
void close_pipe(int **pipes,int lenOperator,char **tableOperator)
{
    int i = 0;
    while(i < lenOperator)
    {
        if(strcmp(tableOperator[i],"|") == 0)
        {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }
        i++;
    }
}
void transfer_cmd(char **argv ,char **env)
{
    int i = 1;
    int j = 0;
    int lenOperator = 0;
    char ***tableCmd = NULL;
    char **tableOperator = NULL;
    while(argv[i])
    {
        if(strcmp(argv[i],"|") == 0 || strcmp(argv[i],";") == 0)
            lenOperator++;
        i++;
    }
    tableCmd = ft_calloc(sizeof(char**) ,lenOperator + 2);
    tableOperator = ft_calloc(sizeof(char*) ,lenOperator + 1);
    i = 1;
    while(argv[i])
    {
        while(argv[i] && strcmp(argv[i],"|") != 0 && strcmp(argv[i],";") != 0)
        {
            tableCmd[j] = ft_strjoin2d(tableCmd[j],argv[i]);
            i++;    
        }
        j++;
        if(argv[i])
            i++;
    }
    i = 1;
    j = 0;
    int lenpipe = 0;
    while(argv[i])
    {
        if(strcmp(argv[i],"|") == 0 || strcmp(argv[i],";") == 0)
        {
            tableOperator[j++] = argv[i];
            if(strcmp(argv[i],"|") == 0)
                lenpipe++;
        }
        i++;
    }
    i = 0;
    j = 0;
    int id;
    int **pipes = ft_calloc(sizeof(int*), lenOperator);
    while(i < lenOperator)
    {
        if(strcmp(tableOperator[i],"|") == 0)
        {
            pipes[i] = ft_calloc(sizeof(int),2);
            pipe(pipes[i]);
        }
        i++;
    }
    i = 0;
    while(i <= lenOperator)
    {
        if(tableCmd[i] != NULL)
        {
            if(strcmp(tableCmd[i][0],"cd") == 0)
			{
				if(tableOperator[0] == NULL)
					cd(tableCmd[i]);
				else if(strcmp(tableOperator[i],"|") != 0)
					cd(tableCmd[i]);
				else if(i != 0  && tableOperator[i - 1] != NULL)
					{
						if(strcmp(tableOperator[i - 1],"|") != 0)
						cd(tableCmd[i]);
					}
			}else
            {

                id = fork();
                if(id == 0)
                {
                    if(i > 0)
                    {
                        if(tableOperator[i - 1] != NULL)
                        if(strcmp(tableOperator[i - 1],"|") == 0)
                        {
                                dup2(pipes[i -1] [0],0);
                        }

                    }
                    if(tableOperator[i] != NULL)
                    if(strcmp(tableOperator[i],"|") == 0)
                            {
                                dup2(pipes[i][1],1);
                            }
                    close_pipe(pipes,i,tableOperator);
                    execve(tableCmd[i][0],tableCmd[i],env);
                    write(2,"error: cannot execute ",23);
                    write(2,tableCmd[i][0],ft_strlen(tableCmd[i][0]));
                    write(2,"\n",1);
                    exit(0);
                }
            }
        }
        if(tableOperator[i] != NULL)
        {
            if(strcmp(tableOperator[i],";") == 0)
            {
                close_pipe(pipes,i,tableOperator);
                while( waitpid(0,NULL,0) != -1);
            }
        }
        i++;
    }

    close_pipe(pipes,lenOperator,tableOperator);
    while( waitpid(0,NULL,0) != -1);
}

int main(int argc , char **argv,char ** env)
{
    if (argc == 1)
        return(0);
    transfer_cmd(argv,env);

}