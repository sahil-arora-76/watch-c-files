#include <string.h>
#include <stdio.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <stdlib.h>
#define HOST_NAME_MAX 600
#define EVENT_NUM 12  
 
char *event_str[EVENT_NUM] =
{
	"IN_ACCESS",
	"IN_MODIFY",
	"IN_ATTRIB",
	"IN_CLOSE_WRITE",
	"IN_CLOSE_NOWRITE",
	"IN_OPEN",
	"IN_MOVED_FROM",
	"IN_MOVED_TO",
	"IN_CREATE",
	"IN_DELETE",
	"IN_DELETE_SELF",
	"IN_MOVE_SELF"
};
 

char *get_dir() 
{
    char *buff  = getcwd(NULL, 0); 
    if (buff == NULL) 
    {
        fprintf(stderr, "Not able to fetch the current working dir\n"); 
        return NULL;
    } else 
    {
        return buff;
    }
}

void execute(char *name) 
{
    FILE *fp; 
    char *args = "-Wall";
    char buffer[400]; 
    char command[200];

    system("clear");
    sprintf(command,  "gcc %s %s", name, args);
    fp = popen(command, "r"); 
    fgets(buffer, sizeof(buffer), fp);
    printf("\033[31m%s\n\033[39m", buffer); 
    pclose(fp);
    return;
}

int check(char *s) 
{
    char *ret; 
    
    ret =  strstr(s, ".c");
    if (ret) 
    {
        return 1; 
    } else 
    {
        return 0; 
    }
}

int main(int argc, char **argv) 
{
    int fd;
	int wd;
	int len;
	int nread;
	char buf[BUFSIZ];
	struct inotify_event *event;
	int i;
    int c; 
    char *path;
    char *dir = get_dir(); 
    
    if (dir == NULL)
    {
        return -1;
    }
	if (argc < 2)
	{
		fprintf(stderr, "%s name of c file\n", argv[0]);
		return -1;
	}
    c = check(argv[1]); 
    if (c == 0) 
    {
        fprintf(stderr, "only c files can be watched\n"); 
        return -1;
    }
    path = malloc(strlen(dir) +  strlen(argv[1]) + 2);
    sprintf(path, "%s/%s", dir, argv[1]);
    printf("%s\n", path);
    free(dir);
    fd = inotify_init();
	if (fd < 0)
	{
		fprintf(stderr, "inotify_init failed\n");
		return -1;
	}
	wd = inotify_add_watch(fd, path, IN_ALL_EVENTS);
	if (wd < 0)
	{
		fprintf(stderr, "inotify_add_watch %s failed\n", argv[1]);
		return -1;
	}
 
    system("clear");
	buf[sizeof(buf) - 1] = 0;
	while ((len = read(fd, buf, sizeof(buf) - 1)) > 0)
	{
		nread = 0;
		while (len > 0)
		{
			event = (struct inotify_event *)&buf[nread];
			for (i = 0; i < EVENT_NUM; i++)
			{
				if ((event->mask >> i) & 1)
				{
                    if (strcmp(event_str[i], "IN_MODIFY") == 0) 
                    {
                        execute(argv[1]);
                    }
				}
			}
			nread = nread + sizeof(struct inotify_event) + event->len;
			len = len - sizeof(struct inotify_event) - event->len;
		}
	}
    free(path);
	return 0;
}

