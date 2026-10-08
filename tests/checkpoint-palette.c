// Exercise the production RAM scan and palette converter. CPU/audio scans are
// irrelevant to this latch and are isolated below; no game data is required.
#include "../src/burn/drv/capcom/cps_mem.c"
#include <stdio.h>

INT32 Cps2DisableQSnd = 1;
UINT16 *ZBuf;
UINT8 CpsRecalcPal;
UINT8 *CpsRom, *CpsZRom;
UINT32 nCpsRomLen, nCpsZRomLen;

UINT8 *BurnMalloc(INT32 size) { return malloc(size); }
void _BurnFree(void *pointer) { free(pointer); }
void EEPROMScan(INT32 action, INT32 *minimum) { (void)action; (void)minimum; }
INT32 SekScan(INT32 action) { (void)action; return 0; }
INT32 QsndScan(INT32 action) { (void)action; return 0; }

static UINT8 checkpoint[0x80000];
static size_t position, checkpoint_size;
static int restoring;

static INT32 scan_area(struct BurnArea *area)
{
   assert(position + area->nLen <= sizeof(checkpoint));
   if (restoring) {
      assert(position + area->nLen <= checkpoint_size);
      memcpy(area->Data, checkpoint + position, area->nLen);
   } else {
      memcpy(checkpoint + position, area->Data, area->nLen);
   }
   position += area->nLen;
   return 0;
}
INT32 (__cdecl *BurnAcb)(struct BurnArea *) = scan_area;

static void load_checkpoint(void)
{
   restoring = 1;
   position = 0;
   CpsRecalcPal = 0;
   assert(CpsAreaScan(ACB_VOLATILE | ACB_WRITE, NULL) == 0);
   assert(position == checkpoint_size);
   assert(CpsRecalcPal == 1);
}

int main(void)
{
   UINT8 expected_latch[0x2000];
   UINT32 expected_colors[0xc00];
   size_t i;
   assert(AllocateMemory() == 0);
   assert(CpsPalInit() == 0);
   nCpsPalCtrlReg = 0x0a;
   CpsReg[nCpsPalCtrlReg] = 0x3f;
   // The hardware latch must survive independently of its DMA source RAM.
   memset(CpsRam90, 0xa5, 0x30000);
   for (i = 0; i < sizeof(expected_latch); ++i)
      expected_latch[i] = (UINT8)(i * 37 + 11);
   memcpy(CpsSavePal, expected_latch, sizeof(expected_latch));
   CpsPalUpdate(CpsSavePal);
   memcpy(expected_colors, CpsPal, sizeof(expected_colors));
   assert(expected_colors[0] != 0);
   assert(CpsAreaScan(ACB_VOLATILE | ACB_READ, NULL) == 0);
   checkpoint_size = position;

   memset(CpsSavePal, 0, sizeof(expected_latch));
   memset(CpsRam90, 0, 0x30000);
   memset(CpsPal, 0, sizeof(expected_colors));
   load_checkpoint();
   assert(memcmp(CpsSavePal, expected_latch, sizeof(expected_latch)) == 0);
   assert(CpsRam90[0] == 0xa5);
   CpsPalUpdate(CpsSavePal);
   assert(memcmp(CpsPal, expected_colors, sizeof(expected_colors)) == 0);

   // New memory starts with a zero latch, as in a cold browser instance.
   CpsPalExit();
   CpsMemExit();
   assert(AllocateMemory() == 0);
   assert(CpsPalInit() == 0);
   assert(CpsSavePal[0] == 0);
   load_checkpoint();
   assert(memcmp(CpsSavePal, expected_latch, sizeof(expected_latch)) == 0);
   CpsPalUpdate(CpsSavePal);
   assert(memcmp(CpsPal, expected_colors, sizeof(expected_colors)) == 0);
   CpsPalExit();
   CpsMemExit();
   puts("CPS2 latched palette checkpoint round-trip and fresh-instance restore passed");
   return 0;
}
