
#ifndef _VFS_H
#define _VFS_H

#define MAX_FILE_TABLE_SIZE 	10000


typedef fd_t int;
typedef dir_t int;



typedef struct vnode {
	int vnode_number;
	char name[255];
	struct vnode *parent;
	list* children;
	int permissions;
	int type;
	struct dos_fs {
		int fat_ptr;
	} dosfs;
} vnode_t;



typedef struct {
	int index;
	vnode_t* vn;
	int offset;
} ft_entry;

extern ft_entry g_file_table[MAX_FILE_TABLE_SIZE];








#endif //_VFS_H