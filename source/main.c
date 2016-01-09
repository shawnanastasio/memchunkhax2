#include <3ds.h>

#include <stdio.h>

#include "memchunkhax2.h"

void testService(const char *name) {
  Handle amHandle = 0;
  Result res;

  // Call am:u service
  res = srvGetServiceHandleDirect(&amHandle, name);
  printf("%s init1 result/handle: res=%lu handle=%lu\n", name, res, amHandle);
  if(amHandle) {
      svcCloseHandle(amHandle);
  }
}

int main(int argc, char **argv) {
    //mode
    //1 = only execute memchunkhax2
    //2 = only execute service call
    //3 = execute memchunkhax2 and service call afterwards
    //4 = execute all
    u8 mode = 4;
    u8 success;

    const char* service = "am:u";

    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    if (mode == 1) {
      // Run the exploit
      success = execute_memchunkhax2();
      printf("Exploit returned: %s\n", success ? "Success!" : "Failure.");
    }

    if (mode == 2) {
      testService(service);
    }

    if (mode == 3) {
      // Run the exploit
      success = execute_memchunkhax2();
      printf("Exploit returned: %s\n", success ? "Success!" : "Failure.");

      testService(service);
    }

    if (mode == 4) {
      // This one should fail
      testService(service);

      // Run the exploit
      success = execute_memchunkhax2();
      printf("Exploit returned: %s\n", success ? "Success!" : "Failure.");

      // This one hopefully won't
      testService(service);
    }

    printf("Press START to reboot.\n");
    printf("Press SELECT to (try to) exit.\n");


    u8 debugFirst = 1;
    while(aptMainLoop()) {
      hidScanInput();

      if (debugFirst == 1) {
        printf("entered loop");
        debugFirst = 0;
      }

      if(hidKeysDown() & KEY_SELECT) {
        break;
      }

      if(hidKeysDown() & KEY_START) {
        //Reboot console
        aptOpenSession();
        APT_HardwareResetAsync();
        aptCloseSession();
      }

      gfxFlushBuffers();
      gfxSwapBuffers();
      gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
