#ifndef _VFS_H
#define _VFS_H

#define MAX_FILE_TABLE_SIZE 	10000

typedef fd_t int;
typedef dir_t int;

enum {OREAD, OWRITE, RDWR, APPEND};

typedef struct vnode {
  int vid;
  char name[255];
  struct vnode *parent;
  vector<struct vnode&> *children;
  int permissions;
  int type; //0 for normal files, 1 for directory
  int fat_ptr; //fat pointer for normal files, child index for directory
} vnode_t;

typedef struct {
  int index;
  vnode_t* vn;
  int offset;
  int flag;
} ft_entry;

extern ft_entry ftable[MAX_FILE_TABLE_SIZE];








#endif //_VFS_H
