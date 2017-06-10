#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <asm/uaccess.h>
MODULE_AUTHOR("@pumpkin031");
MODULE_DESCRIPTION("うんこもりもり森鴎外");
MODULE_LICENSE("Dual MIT/GPL");

const char* const MODNAME = "moriogai";
const char const MORIOGAI[] = "うんこもりもり森鴎外\n";
static dev_t dev_id; //device id
static struct class* class;
static struct cdev c_dev; //character device

static int cdev_open(struct inode* inode,struct file* filp){
	return 0; //何もしない
}

static int cdev_release(struct inode* inode,struct file* filp){
	return 0; //何もしない
}

//読み出されるときに呼ばれる関数
static ssize_t cdev_read(struct file* filp,char* user, size_t count,loff_t* offset){

	//返すサイズ
	ssize_t ret = sizeof(MORIOGAI) > count ? count : sizeof(MORIOGAI);

	//ユーザー空間にコピーする
	copy_to_user(user,MORIOGAI,ret);
	
	return ret;
}

static ssize_t cdev_write(struct file* filp,const char* buf,size_t count,loff_t* offset){
	return 0; //何もしない
}

static struct file_operations moriogai_fops ={
	.owner		=	THIS_MODULE,
	.open		=	cdev_open,
	.release	=	cdev_release,
	.read		=	cdev_read,
	.write		=	cdev_write,
	//.ioctl		=	NULL,
};

static int moriogai_init(void){
	int ret;

	//キャラクタデバイス番号を取得する
	ret = alloc_chrdev_region(&dev_id,0,1,MODNAME);

	if(ret<0){
		pr_warn("moriogai: うんこもりもり森鴎外をロードできません。");
		return ret;
	}
	
	//classを作成する
	class = class_create(THIS_MODULE,MODNAME);
	if(IS_ERR(class)){
		pr_warn("moriogai: クラスを作成できません。");
		goto destroy_chrdev;
	}

	//キャラクタデバイスを初期化する
	cdev_init(&c_dev, &moriogai_fops);
	c_dev.owner = THIS_MODULE;

	//キャラクタデバイスを追加する
	ret = cdev_add(&c_dev,dev_id,1);
	if(ret < 0){
		pr_warn("moriogai: キャラクタデバイスを追加できません。");
		goto destroy_class;
	}

	//デバイスを作成する
	if(device_create(class,NULL,dev_id,NULL,MODNAME) == NULL){
		pr_warn("moriogai: device_create()に失敗しました。");
		goto destroy_class;
	}


	pr_info("moriogai: うんこもりもり森鴎外がロードされました。\n");
	return 0;
destroy_class:
	class_destroy(class);

destroy_chrdev:
	unregister_chrdev_region(dev_id,1);
	
	return -1;
}


static void moriogai_exit(void){
	//デバイスを解放
	device_destroy(class,dev_id);
	
	//クラスを解放
	class_destroy(class);

	//キャラクタデバイスを解放
	unregister_chrdev_region(dev_id,1);
	pr_info("moriogai: 5000兆円欲しかった...\n");
}

module_init(moriogai_init);
module_exit(moriogai_exit);
