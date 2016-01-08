#include <3ds.h>

#include <stdio.h>

#include "memchunkhax2.h"

int main(int argc, char **argv) {
    //mode
    //1 = only execute memchunkhax2
    //2 = only execute service call
    //3 = execute memchunkhax2 and service call afterwards
    //4 = execute all
    u8 mode = 3;

    Handle amHandle = 0;
    Result res;
    u8 success;

    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);

    if (mode == 1) {
      // Run the exploit
      success = execute_memchunkhax2();
      printf("Exploit returned: %s\n", success ? "Success!" : "Failure.");
    }

    if (mode == 2) {
      // Call am:u service
      res = srvGetServiceHandleDirect(&amHandle, "am:u");
      printf("am:u init1 result/handle: res=%lu handle=%lu\n", res, amHandle);
      if(amHandle) {
          svcCloseHandle(amHandle);
      }
    }

    if (mode == 3) {
      // Run the exploit
      success = execute_memchunkhax2();
      printf("Exploit returned: %s\n", success ? "Success!" : "Failure.");

      // Call am:u service
      res = srvGetServiceHandleDirect(&amHandle, "am:u");
      printf("am:u init2 result/handle: res=%lu handle=%lu\n", res, amHandle);
      if(amHandle) {
          svcCloseHandle(amHandle);
      }
    }

    if (mode == 4) {
      // This one should fail
      res = srvGetServiceHandleDirect(&amHandle, "am:u");
      printf("am:u init1 result/handle: res=%lu handle=%lu\n", res, amHandle);
      if(amHandle) {
          svcCloseHandle(amHandle);
      }

      // Run the exploit
      success = execute_memchunkhax2();
      printf("Exploit returned: %s\n", success ? "Success!" : "Failure.");

      // This one hopefully won't
      res = srvGetServiceHandleDirect(&amHandle, "am:u");
      printf("am:u init2 result/handle: res=%lu handle=%lu\n", res, amHandle);
      if(amHandle) {
          svcCloseHandle(amHandle);
      }
    }

    printf("Press START to exit.\n");

    while(aptMainLoop()) {
        hidScanInput();
        if(hidKeysDown() & KEY_START) {
            break;
        }

        gfxFlushBuffers();
        gfxSwapBuffers();
        gspWaitForVBlank();
    }

    gfxExit();
    return 0;
}
