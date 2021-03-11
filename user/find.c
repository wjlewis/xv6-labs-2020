#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

char *
strippath(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // Move p to the final slash (/), or one char before path
  for(p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  strcpy(buf, p);
  return buf;
}

void
find(char *path, char *filename, uint8 firstrun)
{
  int fd;
  struct dirent de;
  struct stat st;
  char buf[512], *p;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    exit(1);
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    exit(1);
  }

  switch(st.type){
  case T_FILE:
    if(firstrun){
      fprintf(2, "find: path must refer to a directory\n");
      exit(1);
    }

    if(strcmp(strippath(path), filename) == 0){
      printf("%s\n", path);
    }
    break;

  case T_DIR:
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;

      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      strcpy(p, de.name);

      find(buf, filename, 0);
    }
    break;
  }

  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    fprintf(2, "Usage: find <dir> <name>\n");
    exit(1);
  }

  if(argc == 2){
    find(".", argv[1], 1);
    exit(0);
  }

  find(argv[1], argv[2], 1);
  exit(0);
}
