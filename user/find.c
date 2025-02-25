#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char* fmtname(char *path) // Lấy tên file từ đường dẫn đầy đủ
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void find(char *path, char *file)
{
  char buf[512], *p;
  int fd;
  struct dirent de; // Chứa thông tin cơ bản của một file/thư mục trong thư mục cha dưới dạng các entry
  struct stat st; // Chứa thông tin về tệp hay thư mục
  // Trường st.type cho biết T_FILE → File thường, T_DIR → Thư mục, T_DEVICE → Thiết bị.

  if((fd = open(path, O_RDONLY)) < 0){
    fprintf(2, "Find1: cannot open %s\n", path);
    return;
  } // Lệnh này dùng để kiểm tra xem có truy cập đc file/thư mục hay không 

  if(fstat(fd, &st) < 0){

    fprintf(2, "Find2: cannot stat %s\n", path);
    close(fd);
    return;
  } // Kiểm tra loại file(thư mục hay file) và lưu vào stat

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    if(strcmp(fmtname(path), file) == 0)
    {
        printf("%s\n", path);
    }
    break;
  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("Find: path too long\n");
      break;
    } // Kiểm tra độ dài đường dẫn có vượt quá giới hạn bộ nhớ đệm hay không
    // Nếu vượt quá thì lỗi và bỏ qua thư mục này

    //Tạo đường dẫn
    // Ví dụ Nếu path = "/home/user", thì sau bước này, buf = "/home/user/"
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';

    // Bắt đầu đọc từng entry
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        continue;
      memmove(p, de.name, DIRSIZ); // Copy tên file từ de.name vào p
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }
      find(buf, file);
    }
    break;
  }
  close(fd);
}

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("Failed to find command!\n");
        exit(0);
    }
    // Cú pháp find . a với . là thư mục chỉ định, a là tên file cần tìm kiếm
    memset(argv[2] + strlen(argv[2]), ' ', DIRSIZ - strlen(argv[2]));
    find(argv[1], argv[2]);
    exit(0);
}
