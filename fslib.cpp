#include "vfs.hpp"

//record current vnode visited in shell
//make tree root global in shell
vnode_t *root;

//have a find function to get vnode pointer as helper for f_open, f_remove, other funcs with directories

fd_t f_open(string path, int flag){
  int inRoot = 0;
  Vnode* currFile;
  vector<string> names = new vector<string>();
  //parse path by "/" or "\", save names in vector
  //if first elem parsed == "~" set inRoot to 1 & not add it to vector
  parseLine(path, names); //check if parseLine func parses by \ or /

  int numchild;  
  int start = 0;
  if (names[i].compare("~") == 0){
    start = 1;
    current = root;
  }
  
  //follow path
  int fileExist = 0;
  for (int i = start; i < names.size(); i++){
    int validDir = 0;
    if (current->type == 1 && current->size > 0){
      int numchild = current->size;
      int curPtr = current->fatPtr;
      while (curPtr != USMAX){
	for (int j = 0; j < 7; i++){
	  if (numchild == 0) break;
	  numchild--;

	  char nodename[255];
	  lseek(g_disk_fd, BLOCKSIZE * curPtr + j * 288, SEEK_SET);
	  read(g_disk_fd, nodename, 255);
	  if (names[j].compare(nodename) == 0){
	    Vnode* newnode = (Vnode*) malloc (sizeof(Vnode));
	    read(g_disk_fd, newnode, sizeof(Vnode - 8));
	    newnode->parent = current;
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
	currFile = current;
	current = current->parent; //reset current to the parent dir
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
    //need to: edit permisssion, traverse for fat_ptr
    int newBlock = -1;
    for (int i = 0; i < FATSIZE; i++){
      if (g_FAT_table[i] == USMAX){ //agree on indices in FAT for empty blocks
	newBlock = i;
	break;
      }
    }
    
    if (newBlock == -1){
      //set error message -- ran out of data blocks
      return -1;
    }

    //do we need id??
    currFile = (Vnode*) malloc (sizeof(Vnode));
    newVnode->name = names.back();
    //uid, gid, permission, timestamp
    newVnode->size = 0;
    newVnode->parent = current;
    newVnode->type = 0;
    newVnode->fatPtr = newBlock;
    //write to file
  }

  //add file to filetable
  int idx = getNextIndex();
  if (idx < 0){
    //error message -- file table full
    return -1;
  }
  g_file_table.addFileEntry(FtEntry(idx, currFile, 0, flag));
  
  /* int i;
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
  */

  return i;
}


size_t f_read(vnode_t *vn, void *data, size_t size, int num, fd_t fd);

size_t f_write(vnode_t *vn, void *data, size_t size, int num, fd_t fd);


int f_close(fd_t fd){
  if (fd < 0 || fd >= MAXFTSIZE){
    //error message
    return -1;
  }
  
  if (ftable[fd].index != fd){
    //error message
    return -1;
  }

  free(ftable[fd].vnode);
  
  //insert empty entry into file table
  g_file_table.removeFileEntry(FtEntry());

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
