const static struct usb_dev known_good[] =
{
/* {0x8087, 0x09fa, 0x0000} */ /* Intel Sensor Hub. Bust on USB autosusped */
{0x8087, 0x07dc, 0x0000},
{0x8087, 0x8001, 0x0009},	/* EHCI/XHCI built in hub */
{0x8087, 0x0a2a, 224},
{0x8087, 0x0a2b, 224},
/* BYT-T additions from T100 - need to verify they are all BYT-T systems */
{0x0b05, 0x17e0, 0x0000},
{0x0b95, 0x7720, 0x00ff},
{0x1d6b, 0x0002, 0x0009},
{0x1d6b, 0x0003, 0x0009},
/* BYT-T additions end */
/* Dell XPS 13 9360 (KBL) additions */
{0x04f3, 0x2234, 0x0000},	/* Elan HID-Touchscreen, needed for >PkgC8 */
/* Dell XPS 13 9360 (KBL) additions end */
{0xFFFF, 0xFFFF, 0} };

