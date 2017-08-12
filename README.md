# ralink-spec-api
Ralink Specific API for Ralink Proprietary WLAN driver to work properly with OpenWRT/LEDE without too many migration

It provides the following API:
  procRegDir
  ra_mtd_write_nm
  ra_mtd_read_nm
  [ optional ]
    ra_mtd_write
    ra_mtd_read
    ra_sw_nat_hook_tx
    ra_sw_nat_hook_rx
    ppe_dev_register_hook
    ppe_dev_unregister_hook
