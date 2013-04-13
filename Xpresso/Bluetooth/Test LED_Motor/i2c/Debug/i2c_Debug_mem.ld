/*
 * GENERATED FILE - DO NOT EDIT
 * (C) Code Red Technologies Ltd, 2008-2013
 * Linker script memory definitions
 * Created from LinkMemoryTemplate
<<<<<<< HEAD
 * By LPCXpresso v5.1.2 [Build 2065] [2013-02-20]  on Sat Apr 13 18:15:14 EDT 2013)
=======
 * By LPCXpresso v5.1.2 [Build 2065] [2013-02-20]  on Sat Apr 13 18:24:11 EDT 2013)
>>>>>>> e9e22b181ada00525cbe676d08e56d3f10ec099a
*/

MEMORY
{
  /* Define each memory region */
  MFlash32 (rx) : ORIGIN = 0x0, LENGTH = 0x8000 /* 32k */
  RamLoc8 (rwx) : ORIGIN = 0x10000000, LENGTH = 0x2000 /* 8k */

}
  /* Define a symbol for the top of each memory region */
  __top_MFlash32 = 0x0 + 0x8000;
  __top_RamLoc8 = 0x10000000 + 0x2000;

