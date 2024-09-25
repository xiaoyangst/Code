void copyFile(const char* src,const char* dst){
  int Rfd = open(src,O_RDONLY);
  if (Rfd < 0){
    error(1,errno,"open src");
  }

  int Wfd = open(dst,O_WRONLY | O_CREAT, 0666);
  if (Wfd < 0){
    error(1,errno,"open dst");
  }

  char buf[MAXSIZE] = {0};
  long len = 0;
  while((len = read(Rfd,buf,MAXSIZE)) != 0){
    write(Wfd,buf,len);
  }
  close(Rfd);
  close(Wfd);
}