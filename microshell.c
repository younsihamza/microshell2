#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

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
void transfer_cmd(char **argv ,char **env)
{
    int i = 1;
    int j = 0;
    int lenOperator = 0;
    char ***tableCmd = NULL;
    char **tableOperator = NULL;
    while(argv[i])
    {
        if(strcmp(argv[i],"|") == 0 ||strcmp(argv[i],";") == 0)
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
    int _**pipes = ft_calloc(sizeof(int*),lenpipe );
    while(i < lenpipe)
    {
        pipes[i] =malloc(sizeof(int)*2);
        pipe(pipes[i]);
    }
    int indexPipe = 0;
    while(i <= lenOperator)
    {
        if(tableCmd[i] != NULL)
        {
            id = fork();
            if(id == 0)
            {
                //write(2,"hamza\n",6);
                if(tableOperator[i]!=NULL)
                    if(strcmp(tableOperator[i],"|") == 0)
                        {
                            dup2(pipes[i],1);
                            close(fd[1]);
                            close(fd[0]);
                        }
                if(i > 0)
                {
                    if(tableOperator[i - 1]!=NULL)
                        if(strcmp(tableOperator[i - 1],"|") == 0)
                        {
                            //dup2(fd[0],0);
                            close(dup2(fd[0],0));
 
                        }
                }
                execve(tableCmd[i][0],tableCmd[i],env);
                exit(0);
            }
        }

        waitpid(0,NULL,0);
        if(strcmp(tableOperator[i],"|") == 0)
            indexPipe++;
        i++;
    }

}

int main(int argc , char **argv,char ** env)
{
    int i = 0;
    if (argc == 1)
        return(0);
    transfer_cmd(argv,env);

}