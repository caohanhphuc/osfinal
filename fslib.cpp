#include "vfs.hpp"

//record current vnode visited in shell
//make tree root global in shell
vnode_t *root;

//have a find function to get vnode pointer as helper for f_open, f_remove, other funcs with directories
Vnode* findVnode(vnode_t *vn, string path){
  
}


fd_t f_open(vnode_t *vn, string path, int flag){
  int inRoot = 0;
  vector<string> names = new vector<string>();
  //parse path by "/" or "\", save names in vector
  //if first elem parsed == "~" set inRoot to 1 & not add it to vector
  parseLine(path, names); //check if parseLine func parses by \ or /

  //int curadd;
  int numchild;
  
  int start = 0;
  //create g_vnode *current to store currently visited vnode
  if (names[i].compare("~") == 0){
    start = 1;
    //curadd = BLOCKSIZE * rootreg;
    //lseek(g_disk_fd, curadd, SEEK_SET);
    //read(g_disk_fd, current, sizeof(Vnode));
    current = root;
  } else {
    //lseek(g_disk_fd, BLOCKSIZE * current->fatPtr);
  }
  
  //follow path
  int fileExist = 0;
  for (int i = start; i < names.size(); i++){
    int validDir = 0;
    if (current->type == 0 && current->size > 0){
      int numchild = current->size;
      int curPtr = current->fatPtr;
      while (curPtr != -1){
	for (int j = 0; j < 7; i++){
	  if (numchild == 0) break;
	  numchild--;
	  Vnode* newnode;
	  lseek(g_disk_fd, BLOCKSIZE * curPtr, SEEK_SET);
	  read(g_disk_fd, newnode, sizeof(Vnode)); //change read in size
	  //need to rearrange fields in Vnode
	  if (names[j].compare(newnode->name) == 0){
	    *(current->children).add(newnode);
	    current = newnode;
	    validDir = 1;
	    break;
	  }
	}
	curPtr = g_FAT_table[curPtr];
      }
    } else {
      //not a directory
      if (i == names.size()-1){
	//found file, file's vnode stored in current
	fileExist = 1;
	break;
      } else {
	//print error message
	return -1;
      }
    }
  }

  //return error if read a non existing file
  if (fileExist == 0 && (flag == OREAD || flag == RDWR)){
    //set error message / #
    return -1;
  }

  if (fileExist == 0){
    //create new file (new inode etc)
    //need to: edit permisssion, traverse for fat_ptr
    int newBlock = -1;
    for (int i = 0; i < FATSIZE; i++){
      if (g_FAT_table[i] == -2){ //agree on indices in FAT for empty blocks
	newBlock = i;
	break;
      }
    }
    
    if (newBlock == -1){
      //set error message -- ran out of data blocks
      return -1;
    }

    //do we need id??
    Vnode *newVnode = (Vnode*) malloc (sizeof(Vnode));
    newVnode->name = names.back();
    //uid, gid, permission, timestamp
    newVnode->size = 0;
    newVnode->parent = current;
    *(newVnode->children) = new vector<Vnode*>*();
    newVnode->type = 0;
    newVnode->fatPtr = newBlock;
    *(current->children).push_back(&newVnode);
  }

  //add file to filetable
  //traverse from beginning or last inserted index?
  int i;
  for (i = 0; i < MAXFTSIZE; i++){
    if (ftable[i].index == -1){
      ftable[i].index = i;
      ftable[i].vn = curr;
      if (flag == APPEND){
	//set offset to end of file
      } else {
	ftable[i].offset = 0; //set offset to beginning
      }
      ftable[i].flag = flag;
      break;
    }
  }
  if (i >= MAXFTSIZE){
    //error message -- file table full
    return -1;
  }

  return i;
}


size_t f_read(vnode_t *vn, void *data, size_t size, int num, fd_t fd);

size_t f_write(vnode_t *vn, void *data, size_t size, int num, fd_t fd);


int f_close(vnode_t *vn, fd_t fd){
  if (fd < 0 || fd >= MAXFTSIZE){
    //error message
    return -1;
  }
  if (ftable[fd].index != fd){
    //error message
    return -1;
  }

  //is this necessary??
  if (f_rewind(vn, fd) == -1){
    //error message
    return -1;
  }
  
  //insert empty entry into file table
  ftable[fd].index = -1;
  ftable[fd].vn = NULL;
  ftable[fd].offset = -1;
  ftable[fd].flag = -1;

  return 0;
}

//need disk design to implement f_seek
int f_seek(int offset, int whence, fd_t fd);

int f_rewind(fd_t fd){
  if (fd < 0 || fd >= MAXFTSIZE){
    //error message
    return -1;
  }
  if (ftable[fd].index != fd){
    //error message
    return -1;
  }

  ftable[fd].offset = 0;
  return 0;
}

int f_stat(struct stat_t *buf, fd_t fd){
  if (fd < 0 || fd >= MAXFTSIZE){
    //error message
    return -1;
  }
  if (ftable[fd].index != fd){
    //error message
    return -1;
  }

  (*buf) = {ftable[fd].user, ftable[fd].vn->size, ftable[fd].flag};
  return 0;
}

int f_remove(vnode_t *vn, const char *filename){
  //check if being opened
  //delete vnode
  //mark data blocks as available
}


//same as f_open, except for flags
//should we return a vnode instead of an int dir_t
dir_t f_opendir(vnode_t *vn, const char *filename){
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
  for (int i = 0; i < names.size(); i++){
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

  //fix return type/ val
  return 0;
}


struct dirent_t* f_readdir(vnode_t *vn, dir_t* dirp);
int f_closedir(vnode_t *vn, dir_t* dirp);
int f_mkdir(vnode_t *vn, const char *filename, int mode);
int f_rmdir(vnode_t *vn, const char *filename);
int f_mount(vnode_t *vn, const char *type, const char *dir, int flags, void *data);
int f_umount(vnode_t *vn, const char *dir, int flags);
