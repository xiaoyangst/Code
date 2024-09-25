void mmap_cp(const char* src,const char * dst){
  int Rfd = open(src,O_RDONLY);
  if (Rfd < 0){
    error(1,errno,"open src");
  }

  int Wfd = open(dst,O_RDWR | O_CREAT | O_TRUNC,0666);
  if (Wfd < 0){
    error(1,errno,"open dst");
  }

  // 获取 src 文件大小，依据这个大小截断 dst文件
  struct stat srcMes;
  fstat(Rfd,&srcMes);
  off_t len = srcMes.st_size;
  ftruncate(Wfd,len);

  off_t cur = 0;
  while (cur < len){
    off_t actual_len = 0;
    if (len - cur > MMAP_SIZE){
      actual_len = MMAP_SIZE;
    } else{
      actual_len = len - cur;
    }

    // 映射
    void* srcAddr =  mmap(NULL,actual_len,PROT_READ,MAP_SHARED,Rfd,0);
    if (srcAddr == MAP_FAILED){
      error(1,errno,"mmap src");
    }

    void* dstAddr = mmap(NULL,actual_len,PROT_WRITE | PROT_READ,MAP_SHARED,Wfd,0);
    if (dstAddr == MAP_FAILED){
      error(1,errno,"mmap dst");
    }

    // 零拷贝
    memcpy(dstAddr,srcAddr,actual_len);

    // 解除映射
    munmap(dstAddr,actual_len);
    munmap(srcAddr,actual_len);

    cur += actual_len;
  }

  close(Rfd);
  close(Wfd);
}