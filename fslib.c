#ifndef _FSLIB_H
#define _FSLIB_H

#include "vfs.h"

//record current vnode visited in shell
//make tree root global in shell
vnode_t *root;

fd_t f_open(vnode_t *vn, string path, int flag){
  int inRoot = 0;
  vector<string> names = new vector<string>();
  //parse path by "/" or "\", save names in vector
  //if first elem parsed == "~" set inRoot to 1 & not add it to vector
  vnode_t *curr;
  if (inRoot == 0){
    curr = vn;
  } else {
    curr = root;
  }

  //follow path
  for (int i = 0; i < names.size()-1; i++){
    int validDir = 0;
    for (int j = 0; j < (*(curr->children)).size(); j++){
      if ((*(curr->children))[j].name.compare(names[i]) == 0){
	validDir = 1;
	curr = (*(curr->children))[j];
	break;
      }
    }
    if (validDir == 0){
      //set error message / #
      //errno??
      return -1;
    }
  }

  //check if file exists
  int fileExist = 0;
  for (int i = 0; i < (*(curr->children)).size(); i++){
    if ((*(curr->children))[i].name.compare(names.back()) == 0){
      fileExist = 1;
      break;
    }
  }

  //return error if read a non existing file
  //do we allow multiple flags? no?
  if (fileExist == 0 && (flag == OREAD || flag == RDWR)){
    //set error message / #
    return -1;
  }

  if (fileExist == 0){
    //create new file (new inode etc)
  }

  //add file to filetable
  
  if (flag == OREAD){
  } else if (flag == OWRITE){
  } else if (flag == RDWR){
  } else if (flag == APPEND){
  } else {
  }
}
size_t f_read(vnode_t *vn, void *data, size_t size, int num, fd_t fd);
size_t f_write(vnode_t *vn, void *data, size_t size, int num, fd_t fd);
int f_close(vnode_t *vn, fd_t fd);
int f_seek(vnode_t *vn, int offset, int whence, fd_t fd);
int f_rewind(vnode_t *vn, fd_t fd);
int f_stat(vnode_t *vn, struct stat_t *buf, fd_t fd);
int f_remove(vnode_t *vn, const char *filename);
dir_t f_opendir(vnode_t *vn, const char *filename);
struct dirent_t* f_readdir(vnode_t *vn, dir_t* dirp);
int f_closedir(vnode_t *vn, dir_t* dirp);
int f_mkdir(vnode_t *vn, const char *filename, int mode);
int f_rmdir(vnode_t *vn, const char *filename);
int f_mount(vnode_t *vn, const char *type, const char *dir, int flags, void *data);
int f_umount(vnode_t *vn, const char *dir, int flags);

#endif //_FSLIB_H
