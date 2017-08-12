/* This package aims to provide ralink specific API for OpenWRT/LEDE,
 * in this way, the MTK Propriertary WLAN driver can be used with the
 * orignal OpenWRT/LEDE
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/version.h>
#include <linux/mtd/mtd.h>

struct proc_dir_entry *procRegDir;
#if defined (RA_HW_NAT_API)
typedef void VOID;
int (*ra_sw_nat_hook_tx)(VOID *skb);
int (*ra_sw_nat_hook_rx)(VOID *skb);
#if defined (RA_HW_NAT_WIFI_NEW_ARCH_API)
void (*ppe_dev_register_hook) (VOID  *dev);
void (*ppe_dev_unregister_hook) (VOID  *dev);
#endif /* RA_HW_NAT_WIFI_NEW_ARCH_API */
#endif /* RA_HW_NAT_API */

static int __ra_mtd_write(struct mtd_info *mtd, loff_t to, size_t len, const u_char *buf)
{
	int ret;
	size_t rdlen, wrlen;
	struct erase_info ei;
	u_char *bak = NULL;
	
	if (len > mtd->erasesize) {
		return -E2BIG;
	}

	bak = kmalloc(mtd->erasesize, GFP_KERNEL);
	if (bak == NULL) {
		return -ENOMEM;
	}

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36)
	ret = mtd->read(mtd, 0, mtd->erasesize, &rdlen, bak);
#else
	ret = mtd_read(mtd, 0, mtd->erasesize, &rdlen, bak);
#endif

	if (ret != 0) {
		kfree(bak);
		return ret;
	}
	if (rdlen != mtd->erasesize)
		printk("warning: ra_mtd_write: rdlen is not equal to erasesize\n");

	memcpy(bak + to, buf, len);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36)
	ret = mtd->erase(mtd, &ei);
#else
	ret = mtd_erase(mtd, &ei);
#endif
	if (ret != 0) {
		kfree(bak);
		return ret;
	}

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36)
	ret = mtd->write(mtd, 0, mtd->erasesize, &wrlen, bak);
#else
	ret = mtd_write(mtd, 0, mtd->erasesize, &wrlen, bak);
#endif
	if (wrlen != mtd->erasesize)
		printk("warning: ra_mtd_write: wrlen is not equal to erasesize\n");
	kfree(bak);
	
	return ret;
}
/*
 * Flash API: ra_mtd_read, ra_mtd_write
 * Arguments:
 *   - num: specific the mtd number
 *   - to/from: the offset to read from or written to
 *   - len: length
 *   - buf: data to be read/written
 * Returns:
 *   - return -errno if failed
 *   - return the number of bytes read/written if successed
 */
#ifdef RA_MTD_RW_BY_NUM_API
int ra_mtd_write(int num, loff_t to, size_t len, const u_char *buf)
{
	int ret = -1;
	struct mtd_info *mtd;

	mtd = get_mtd_device(NULL, num);
	if (IS_ERR(mtd))
		return (int)mtd;
	ret = __ra_mtd_write(mtd, to, len, buf);
	put_mtd_device(mtd);
	
	return ret;
}
#endif

int ra_mtd_write_nm(char *name, loff_t to, size_t len, const u_char *buf)
{
	int ret = -1;
	struct mtd_info *mtd;

	mtd = get_mtd_device_nm(name);
	if (IS_ERR(mtd))
		return (int)mtd;
	ret = __ra_mtd_write(mtd, to, len, buf);
	put_mtd_device(mtd);
	
	return ret;
}
static int __ra_mtd_read(struct mtd_info *mtd, loff_t from, size_t len, u_char *buf)
{
	int ret;
	size_t rdlen;
	
#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,36)
	ret = mtd->read(mtd, from, len, &rdlen, buf);
#else
	ret = mtd_read(mtd, from, len, &rdlen, buf);
#endif

	if (rdlen != len)
		printk("warning: ra_mtd_read: rdlen is not equal to len\n");
	
	return ret;
}
#ifdef RA_MTD_RW_BY_NUM_API
int ra_mtd_read(int num, loff_t from, size_t len, u_char *buf)
{
	int ret;
	struct mtd_info *mtd;

	mtd = get_mtd_device(NULL, num);
	if (IS_ERR(mtd))
		return (int)mtd;
	ret = __ra_mtd_read(mtd, from, len, buf);
	put_mtd_device(mtd);
	
	return ret;
}
#endif

int ra_mtd_read_nm(char *name, loff_t from, size_t len, u_char *buf)
{
	int ret;
	struct mtd_info *mtd;

	mtd = get_mtd_device_nm(name);
	if (IS_ERR(mtd))
		return (int)mtd;
	ret = __ra_mtd_read(mtd, from, len, buf);
	put_mtd_device(mtd);
	
	return ret;
}

#ifdef RA_MTD_RW_BY_NUM_API
EXPORT_SYMBOL(ra_mtd_write);
EXPORT_SYMBOL(ra_mtd_read);
#endif
EXPORT_SYMBOL(ra_mtd_write_nm);
EXPORT_SYMBOL(ra_mtd_read_nm);

static __init int ra_spec_api_init(void)
{
	printk("Ralink specific API registered\n");
	
	return 0;
}
static __exit void ra_spec_api_exit(void)
{
	printk("Ralink specific API unregistered\n");
}

module_init(ra_spec_api_init);
module_exit(ra_spec_api_exit);
MODULE_DESCRIPTION("Ralink specific API");
MODULE_LICENSE("GPL");