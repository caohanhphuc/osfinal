#ifndef _VFS_H
#define _VFS_H

#define MAXFTSIZE 	10000

typedef fd_t int;
typedef dir_t int;

enum {OREAD, OWRITE, RDWR, APPEND};

typedef struct vnode {
  int vid;
  char name[255];
  //need to update size every time read or write to file
  //int size;
  struct vnode *parent;
  vector<struct vnode&> *children;
  int permissions;
  int type; //0 for normal files, 1 for directory
  int fat_ptr; //fat pointer for normal files, child index for directory
} vnode_t;

typedef struct {
  int index;
  //int user;
  vnode_t* vn;
  int offset;
  int flag;
} ft_entry;

typedef struct {
  int uid;
  int size;
  int mode;
} stat_t;

extern ft_entry ftable[MAXFTSIZE];
extern int numFiles;
//initialize all entries to have -1 integers & NULL for vnode



#endif //_VFS_H
