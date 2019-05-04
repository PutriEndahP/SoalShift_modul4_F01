#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <grp.h>
#include <pwd.h>

static const char *dirpath = "/home/putri/shift4";

void enkripsi(char* kata)
{
    char dasar[100] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
    for(int i=0; i<strlen(kata); i++){
        for(int j = 0 ; j<strlen(dasar); j++){
            if(kata[i] == dasar[j]){
                int indeks_baru = (j+17) % 94;
                kata[i] = dasar[indeks_baru];
                break;
            }
        }
    }
}

void dekripsi(char* kata)
{
    char dasar[100] = "qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
    for(int i=0; i<strlen(kata); i++){
        for(int j = 0 ; j<strlen(dasar); j++){
            if(kata[i] == dasar[j]){
                int indeks_baru = (j+(94-17)) % 94;
                kata[i] = dasar[indeks_baru];
                break;
            }
        }
    }
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    ///
	char fpath[1000];
	char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);
	sprintf(fpath, "%s%s",dirpath,sementara);
    ///
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{

	///
	char fpath[1000];

    char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);

	sprintf(fpath, "%s%s",dirpath,sementara);
	///
	int res = 0;

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat info;
        char cek[10000];
        strcpy(cek,fpath);
        strcat(cek,de->d_name);

        char file[1000];
		///
		if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0) dekripsi(de->d_name);
		///

        stat(cek,&info);

        
        struct passwd *user;
        user = getpwuid(info.st_uid);
        struct group *grup;
        grup = getgrgid(info.st_gid);
        if( (strcmp(user->pw_name,"chipset") == 0 || strcmp(user->pw_name,"ic_controller") == 0) && strcmp(grup->gr_name,"rusak") == 0){
          if((info.st_mode & R_OK)==0){
              char txt[10000] = "/filemiris.txt";
              enkripsi(txt);
              char pathtxt[100000];
              sprintf(pathtxt,"%s%s",dirpath,txt);

              FILE *filetxt;
              filetxt = fopen(pathtxt,"a+");

              char waktu[21];
			  time_t now = time(NULL);
			  strftime(waktu, 20, "%H:%M:%S %Y-%m-%d", localtime(&now));
			  char isi[1000];
              strcpy(isi,de->d_name);
              strcat(isi,"_");
              char iduser[1000];
              sprintf(iduser,"%d_%d",user->pw_uid,grup->gr_gid);
              strcat(isi,iduser);
              strcat(isi,"_");
              strcat(isi,waktu);

              fprintf(filetxt,"%s\n",isi);
              fclose(filetxt);
              remove(cek);
          }
        }
        else{
            struct stat st;
		    memset(&st, 0, sizeof(st));
		    st.st_ino = de->d_ino;
		    st.st_mode = de->d_type << 12;

            strcpy(file,de->d_name);
		    res = (filler(buf, file, &st, 0));
		    	if(res!=0) break;
        }
	}

	closedir(dp);
	return 0;
}


static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  	char fpath[1000];
	char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);
	
	sprintf(fpath, "%s%s",dirpath,sementara);
	int res = 0;
	int fd = 0 ;

    //char file[1000];
	//sprintf(file, "%s.bak", fpath);
	//rename(fpath,file);
    
	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char spath[1000];
	sprintf(spath,"%s",path);
	enkripsi(spath);
	
    // printf("%s\n",path);
    int length = strlen(path);
     if (strstr(path,"YOUTUBER") && length!=9)
    {
        mode = 0750;
    }
	// enkrip(path);
	sprintf(fpath,"%s%s",dirpath,spath);
	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
    char fpath[1000];
    char name[1000];

	sprintf(name,"%s",path);
    enkripsi(name);
	sprintf(fpath,"%s%s",dirpath,name);

	char newname[1000];
	char folder[1000] = "/Backup";
	enkripsi(folder);
	char folderdir[1000];
	sprintf(folderdir,"%s%s",dirpath,folder);
	//membuat folder backup
	mkdir(folderdir,0755);

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;
	printf("%s\n",buf);
	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);

	struct stat sd;
	if(stat(fpath,&sd)>-1 && strstr(path,".swp")==0){
		char t[1000];
		time_t now = time(NULL);
		char fname[1000];
		//ngambil timestampnya
		strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));
		dekripsi(name);
		//mendapatkan nama file keseluruhan
		sprintf(newname,"/Backup%s%s.ekstensi",name,t);
		enkripsi(newname);
		memset(fname,'\0',sizeof(fname));
		sprintf(fname,"%s%s",dirpath,newname);
		// printf("%s\n",fpath);
		pid_t child1;
		child1=fork();
		if(child1==0){
			//ngocpy isi file asli ke file backup
			execl("/bin/cp","/bin/cp",fpath,fname,NULL);
			exit(EXIT_SUCCESS);
		}
		else{
			wait(NULL);
		}

		return res;
	}
	
	return res;
}



static int xmp_rename(const char *from, const char *to)
{
	int res;

    ///
    char fpathfrom[1000];
	char sementarafrom[1000];
    sprintf(sementarafrom,"%s",from);

    enkripsi(sementarafrom);
	sprintf(fpathfrom, "%s%s",dirpath,sementarafrom);
    ///

	///
    char fpathto[1000];
	char sementarato[1000];
    sprintf(sementarato,"%s",to);

    enkripsi(sementarato);
	sprintf(fpathto, "%s%s",dirpath,sementarato);
    ///

	res = rename(fpathfrom, fpathto);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;
    //
    char fpath[1000];
	char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);

	sprintf(fpath, "%s%s",dirpath,sementara);
    //

	if (S_ISREG(mode)) {
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	} else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
    //
    char fpath[1000];
	char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);

	sprintf(fpath, "%s%s",dirpath,sementara);
    //

	res = truncate(fpath, size);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;
    char fpath[1000];
    char name[1000];
	sprintf(name,"%s",path);
	if(strstr(name,".swp")==0 && strstr(name,".gooutpustream")==0 && strstr(name,"/RecycleBin/") == 0){
		char folder[100000] = "/RecycleBin";
		enkripsi(folder);
		char fpath[1000];
    	sprintf(fpath,"%s%s", dirpath, folder);
		//membuatt folder recycle bin
		mkdir(fpath,0755);

		char t[1000];
		time_t now = time(NULL);
		//mendapatkan timestamp
		strftime(t, 22, "_%Y-%m-%d_%H:%M:%S", localtime(&now));

		char zip[1000];
		char fzip[1000];
		char fname[1000];
		memset(zip,0,sizeof(zip));
		memset(fzip,0,sizeof(fzip));
		memset(fname,0,sizeof(fname));
		//mendapatkan nama file lengkap
		sprintf(zip,"/RecycleBin%s_deleted_%s.zip",name,t);
		enkripsi(name);
		sprintf(fname,"%s%s",dirpath,name);
		enkripsi(zip);
		sprintf(fzip,"%s%s",dirpath,zip);
		pid_t child1;
		char dum[10000];
		dekripsi(name);
		sprintf(dum,"%s%s",dirpath,name);
		enkripsi(name);

		
		child1=fork();
		if(child1==0){
			//nge copy isi file asli ke file recycle bin
			execl("/bin/cp","/bin/cp",fname,dum,NULL);
			exit(EXIT_SUCCESS);
		}
		else{
			wait(NULL);
		}
		
		child1=fork();
		if(child1==0){
			//nge zip folder yg dihapus
			execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j",fzip,dum,NULL);
			exit(EXIT_SUCCESS);
		}
		else{
			wait(NULL);
		}
		remove(fname);

		char fback[1000] = "/Backup";
		enkripsi(fback);
		char foldbackp[1000];
		sprintf(foldbackp,"%s%s",dirpath,fback);
		DIR *dp;
		struct dirent *de;

		dp = opendir(foldbackp);
		while((de = readdir(dp))){
			if(strncmp(name+1,de->d_name,strlen(de->d_name)-29)==0){

				memset(fzip,0,sizeof(fzip));
				memset(zip,0,sizeof(zip));
				memset(fname,0,sizeof(fname));
				dekripsi(name);
				sprintf(zip,"/RecycleBin%s_deleted_%s.zip",name,t);
				sprintf(fname,"%s/%s",foldbackp,de->d_name);
				enkripsi(zip);
				enkripsi(name);
				sprintf(fzip,"%s%s",dirpath,zip);
				char dum[10000];
				dekripsi(de->d_name);
				sprintf(dum,"%s/%s",dirpath,de->d_name);
				enkripsi(de->d_name);				

				child1=fork();
				if(child1==0){
					execl("/bin/cp","/bin/cp",fname,dum,NULL);
					exit(EXIT_SUCCESS);
				}
				else{
					wait(NULL);
				}

				child1=fork();
				if(child1==0){
					execl("/usr/bin/zip","/usr/bin/zip","-q","-m","-j","-u",fzip,dum,NULL);
					exit(EXIT_SUCCESS);
				}
				else{
					wait(NULL);
				}
				remove(fname);
			}
		}
		dekripsi(name);
		memset(zip,0,sizeof(zip));
		memset(fzip,0,sizeof(fzip));
		memset(fname,0,sizeof(fname));
		sprintf(zip,"/RecycleBin%s_deleted_%s.zip",name,t);
		enkripsi(name);
		sprintf(fname,"%s%s",dirpath,name);
		enkripsi(zip);
		sprintf(fzip,"%s%s",dirpath,zip);
		memset(fname,0,sizeof(fname));
		strncpy(fname,fzip,strlen(fzip));
		strcat(fzip,".zip");
		rename(fzip,fname);
	}
	else{
    	enkripsi(name);
		sprintf(fpath, "%s%s",dirpath,name);
		res = unlink(fpath);
		if (res == -1)
			return -errno;
	}
	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;
    //
    char fpath[1000];
	char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);

	sprintf(fpath, "%s%s",dirpath,sementara);
    //

	res = rmdir(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
    char fpath[1000];
    char name[1000];
	char temp[1000];
	strcpy(temp,path);
	enkripsi(temp);
	sprintf(name, "%s%s", dirpath, temp);
	struct stat sfile;
	stat(name, &sfile);
	// sprintf(fpath,"%s%s",dirpath,path);
    int panjang = strlen(path);
    if(path[panjang-1]=='1' && path[panjang-2]=='z' && path[panjang-3]=='i' && S_ISDIR(sfile.st_mode)==0)
    {
        pid_t child_id;
		int status;
		child_id=fork();
		if(child_id == 0)
		{
			char *argv[5]={"zenity","--error","--text=\"File ekstensi iz1 tidak boleh diubah permissionnya.\n\"","--title=\"Warning!\"",NULL};
			execv("/usr/bin/zenity",argv);
		}
		else
		{
			while((wait(&status)) > 0);
		}
		
    }
    else
    {
		char spath[1000];
		sprintf(spath,"%s",path);
		enkripsi(spath);
		// char fpath[1000];
    
		sprintf(fpath,"%s%s",dirpath,spath);
        res = chmod(fpath, mode);
	    if (res == -1)
		    return -errno;
    }
    
	

	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
    //
    char fpath[1000];
	char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);

	sprintf(fpath, "%s%s",dirpath,sementara);
    //

	res = lchown(fpath, uid, gid);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
    //
    char fpath[1000];
	char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);

	sprintf(fpath, "%s%s",dirpath,sementara);
    //

	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
    //
    char fpath[1000];
	char sementara[1000];
    sprintf(sementara,"%s",path);

    enkripsi(sementara);

	sprintf(fpath, "%s%s",dirpath,sementara);
    //

	res = open(fpath, fi->flags);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) 
{

	(void) fi;

    int res;
    char fpath[1000];
	char spath[1000];
	sprintf(spath,"%s",path);
	int jum;
	int i=0;
	
	char *found;
	found=strstr(spath,"YOUTUBER");
    if(found)
    {
		int i=found-spath;
		while(spath[i] != '\0')
		{
			if(spath[i]=='/')
			jum++;
			i++;
		}
		if(jum<2)
		{
			mode = 0640;
       		strcat(spath,".iz1");
		}
    }
	enkripsi(spath);
    
	sprintf(fpath,"%s%s",dirpath,spath);
	// mode=0640;
    res = creat(fpath, mode);
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}

static void* pre_init(struct fuse_conn_info *conn)
{	//nama folder videos
        char folder[100000] = "/Videos";
		char folde1r[100000] = "/YOUTUBER";
		//nge enkripsi nama folder videos
		enkripsi(folder);
		enkripsi(folde1r);
		char fpath[1000];
	//ngegabungin dirpath sdan nama foldernya
    	sprintf(fpath,"%s%s", dirpath, folder);
		//buat folder videos
		mkdir(fpath,0755);
		memset(fpath,0,sizeof(fpath));
		sprintf(fpath,"%s%s", dirpath, folde1r);
		mkdir(fpath,0755);
		memset(fpath,0,sizeof(fpath));

		pid_t child1;
		child1=fork();
		if(child1==0){
			DIR *dp;
			struct dirent *de;
			dp = opendir(dirpath);
			//ngecek semua file yang ada di dirpath
			while((de = readdir(dp))){
				if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
					char ext[1000] = ".mkv";
					enkripsi(ext);
					if(strlen(de->d_name)>7 && strncmp(de->d_name+strlen(de->d_name)-8,ext,4)==0){

							char joined[1000];
							char video[1000] = "/Videos";
							enkripsi(video);
							sprintf(joined,"%s%s/",dirpath,video);
							//ngegabungin semua file yg harus digabungkan
							strncat(joined,de->d_name,strlen(de->d_name)-4);
							FILE* mainj;
							mainj = fopen(joined,"a+");
							FILE* need;
							char this[1000];
							sprintf(this,"%s/%s",dirpath,de->d_name);
							need = fopen(this,"r");
							int c;
							//ngegabungin isi video
							while(1) {
   								c = fgetc(need);
   								if( feof(need) ) {
   								   break;
   								}
   								fprintf(mainj,"%c",c);
   							}
							
					}
				}
			}
			exit(EXIT_SUCCESS);
		}

        (void) conn;
        return NULL;
}

static void post_destroy(void* private_data)
{
	char fpath[10000];
	char folder[100000] = "/Videos";
	enkripsi(folder);
    sprintf(fpath,"%s%s", dirpath, folder);

	DIR *dp;
	struct dirent *de;
	dp = opendir(fpath);
	//mengahpus semua file yang ada di dalam folder videos
	while((de = readdir(dp))){
		if(strcmp(de->d_name,".")!=0 && strcmp(de->d_name,"..")!=0){
			char file[1000];
			sprintf(file,"%s/%s",fpath,de->d_name);
			remove(file);
		}
	}
	rmdir(fpath);

	return;
}


static struct fuse_operations xmp_oper = {
	.init	= pre_init,
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
    .mkdir      = xmp_mkdir,
    .write      = xmp_write,
    .rename     = xmp_rename,
    .mknod      = xmp_mknod,
    .truncate   = xmp_truncate,
    .unlink     = xmp_unlink,
    .rmdir      = xmp_rmdir,
    .chmod      = xmp_chmod,
    .chown      = xmp_chown,
    .utimens    = xmp_utimens,
    .open       = xmp_open,
    .create     = xmp_create,
	.destroy	= post_destroy,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

