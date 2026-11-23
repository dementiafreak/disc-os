#define B 0x000000AA
#define K 0x00000000
#define Y 0x00FFD700

struct EFI_TABLE_HEADER {
    unsigned long long Signature;
    unsigned int Revision;
    unsigned int HeaderSize;
    unsigned int CRC32;
    unsigned int Reserved;
};

struct EFI_INPUT_KEY {
    unsigned short ScanCode;
    unsigned short UnicodeChar;
};

struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    unsigned long long (*Reset)(struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This, unsigned char ExtendedVerification);
    unsigned long long (*ReadKeyStroke)(struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL* This, struct EFI_INPUT_KEY* Key);
    void* WaitForKey;
};

struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    void* Reset;
    unsigned long long (*OutputString)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, unsigned short* String);
    void* TestString;
    void* QueryMode;
    void* SetMode;
    unsigned long long (*SetAttribute)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, unsigned long long Attribute);
    unsigned long long (*ClearScreen)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This);
    void* SetCursorPosition;
    unsigned long long (*EnableCursor)(struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* This, unsigned char Visible);
};

struct EFI_RUNTIME_SERVICES {
    struct EFI_TABLE_HEADER Hdr;
    void* GetTime;
    void* SetTime;
    void* GetWakeupTime;
    void* SetWakeupTime;
    void* SetVirtualAddressMap;
    void* ConvertPointer;
    void* GetVariable;
    void* GetNextVariableName;
    void* SetVariable;
    void* GetNextHighMonotonicCount;
    void (*ResetSystem)(int ResetType, unsigned long long ResetStatus, unsigned long long DataSize, void* ResetData);
};

struct EFI_GUID {
    unsigned int Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char Data4[8];
};

struct EFI_BOOT_SERVICES {
    struct EFI_TABLE_HEADER Hdr;
    void* _pad[37];
    unsigned long long (*LocateProtocol)(struct EFI_GUID* Protocol, void* Registration, void** Interface);
};

struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION {
    unsigned int Version;
    unsigned int HorizontalResolution;
    unsigned int VerticalResolution;
    int PixelFormat;
    unsigned int PixelInformation[4];
    unsigned int PixelPerScanLine;
};

struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE {
    unsigned int MaxMode;
    unsigned int Mode;
    struct EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* Info;
    unsigned long long SizeOfInfo;
    unsigned long long FrameBufferBase;
    unsigned long long FrameBufferSize;
};

struct EFI_GRAPHICS_OUTPUT_PROTOCOL {
    void* QueryMode;
    void* SetMode;
    void* Bit;
    struct EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE* Mode;
};

struct EFI_SYSTEM_TABLE {
    struct EFI_TABLE_HEADER Hdr;
    char *FirmwareVendor;
    unsigned int FirmwareRevision;
    void* ConsoleInHandle;
    struct EFI_SIMPLE_TEXT_INPUT_PROTOCOL* ConIn;
    void* ConsoleOutHandle;
    struct EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL* ConOut;
    void* StdErr;
    void* StdErrHandle;
    struct EFI_RUNTIME_SERVICES* RuntimeServices;
    struct EFI_BOOT_SERVICES* BootServices;
};

int strcmp16(unsigned short* s1, unsigned short* s2) {
    while (*s1 != 0 && *s1 == *s2) {
        s1++;
        s2++;
    }

    if (*s1 == *s2) {
        return 1;
    } else {
        return 0;
    }
}

int startswith16(unsigned short* str, unsigned short* prefix) {
    while (*prefix != 0) {
        if (*str != *prefix) {
            return 0;
        }
        str++;
        prefix++;
    }
    return 1;
}

unsigned long long efi_main(void* ImageHandle, struct EFI_SYSTEM_TABLE* SystemTable) {

    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x1F);

    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

    SystemTable->ConOut->EnableCursor(SystemTable->ConOut, 1);

    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DDDDDD    IIIIIIIII  SSSSSSSS   CCCCCCC\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DDDDDDDD  IIIIIIIII  SSSSSSSS CCCCCCCCC\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DD    DD      I     SS        CC\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DD     DD     I     SSSSSSS   CC         ooooo   ssss   UEFI Firmware\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DD     DD     I       SSSSSSS CC        o     o s\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DD    DD      I            SS CC        o     o  sss    Version 1.1.0\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DDDDDDDD  IIIIIIIII SSSSSSSS  CCCCCCCCC o     o     s   Last Updated:\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DDDDDD    IIIIIIIII SSSSSSSS    CCCCCCC  ooooo  ssss    November 22, 2026\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n > ");

    struct EFI_INPUT_KEY Key;

    unsigned int count = 0;
    unsigned short cmd[128];

    static const unsigned int sprite[64] = {
        B, B, K, K, K, K, B, B,
        B, K, Y, Y, Y, Y, K, B,
        K, Y, K, Y, Y, K, Y, K,
        K, Y, Y, Y, Y, Y, Y, K,
        K, Y, K, Y, Y, Y, Y, K,
        K, Y, K, K, K, K, Y, K,
        B, K, Y, Y, Y, Y, K, B,
        B, B, K, K, K, K, B, B
    };

    while (1) {

        while (SystemTable->ConIn->ReadKeyStroke(SystemTable->ConIn, &Key) != 0) {
            __asm__ volatile("hlt");
        }

        unsigned short buf[] = {Key.UnicodeChar, 0};

        if (Key.UnicodeChar == u'\r') {
            cmd[count] = 0;
            SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");

            if (strcmp16(cmd, u"help")) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Available commands:\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"clear, echo, help, info, reboot, shutdown, smile, theme\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");

            } else if (strcmp16(cmd, u"clear")) {
                SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

            } else if (strcmp16(cmd, u"info")) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DDDDDD    IIIIIIIII  SSSSSSSS   CCCCCCC\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DDDDDDDD  IIIIIIIII  SSSSSSSS CCCCCCCCC\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DD    DD      I     SS        CC\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DD     DD     I     SSSSSSS   CC         ooooo   ssss   UEFI Firmware\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DD     DD     I       SSSSSSS CC        o     o s\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DD    DD      I            SS CC        o     o  sss    Version 1.1.0\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DDDDDDDD  IIIIIIIII SSSSSSSS  CCCCCCCCC o     o     s   Last Updated:\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"  DDDDDD    IIIIIIIII SSSSSSSS    CCCCCCC  ooooo  ssss    November 22, 2026\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");

            } else if (strcmp16(cmd, u"reboot")) {
                SystemTable->RuntimeServices->ResetSystem(0, 0, 0, 0);

            } else if (strcmp16(cmd, u"shutdown")) {
                SystemTable->RuntimeServices->ResetSystem(2, 0, 0, 0);

            } else if (strcmp16(cmd, u"smile")) {
                SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                struct EFI_GUID gop_guid = { 0x9042a9de, 0x23dc, 0x4a38, { 0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a } };
                struct EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = 0;
                SystemTable->BootServices->LocateProtocol(&gop_guid, 0, (void**)&gop);
                unsigned int* fb = (unsigned int*)gop->Mode->FrameBufferBase;

                unsigned int scale = 32;
                unsigned int start_y = (gop->Mode->Info->VerticalResolution - 8 * scale) / 2;
                unsigned int start_x = (gop->Mode->Info->HorizontalResolution - 8 * scale) / 2;

                if (start_x + 8 * scale <= gop->Mode->Info->HorizontalResolution && start_y + 8 * scale <= gop->Mode->Info->VerticalResolution) {
                    for (unsigned int y = 0; y < 8 * scale; y++) {
                        for (unsigned int x = 0; x < 8 * scale; x++) {
                            unsigned int color = sprite[(y / scale) * 8 + (x / scale)];
                            fb[(start_y + y) * gop->Mode->Info->PixelPerScanLine + (start_x + x)] = color;
                        }
                    }
                }
            } else if (startswith16(cmd, u"echo ")) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, cmd + 5);
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");

            } else if (startswith16(cmd, u"echo")) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");

            } else if (startswith16(cmd, u"theme ")) {
                if (strcmp16(cmd + 6, u"classic")) {
                    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x1F);
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                } else if (strcmp16(cmd + 6, u"bios")) {
                    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x71);
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                } else if (strcmp16(cmd + 6, u"retro")) {
                    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x0E);
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                } else if (strcmp16(cmd + 6, u"matrix")) {
                    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x0A);
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                } else if (strcmp16(cmd + 6, u"contrast")) {
                    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x0F);
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                } else if (strcmp16(cmd + 6, u"alert")) {
                    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x4E);
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                } else if (strcmp16(cmd + 6, u"ugly")) {
                    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x65);
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                } else if (strcmp16(cmd + 6, u"cyber")) {
                    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x0B);
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                } else if (strcmp16(cmd + 6, u"love")) {
                    SystemTable->ConOut->SetAttribute(SystemTable->ConOut, 0x4D);
                    SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

                } else if (strcmp16(cmd + 6, u"list")) {
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"alert - yellow on red\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"bios - blue on grey\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"classic - white on blue\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"contrast - white on black\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"cyber - cyan on black\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"love - pink on red\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"matrix - green on black\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"retro - yellow on black\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"ugly - magenta on brown\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");

                } else {
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Unknown theme! Type 'theme list' for a list of available themes\r\n");
                    SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
                }

            } else if (startswith16(cmd, u"force-panic ")) {
                if (strcmp16(cmd + 12, u"asm")) {
                    __asm__ volatile("ud2");
                } else if (strcmp16(cmd + 12, u"uefi-error")) {
                    SystemTable->RuntimeServices->ResetSystem(3, 0x8000000000000007, sizeof(u"DISCos Kernel Panic"), u"DISCos Kernel Panic");
                }

            } else if (count > 0) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Command not found! Type 'help' for a list of commands\r\n");
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
            }

            SystemTable->ConOut->OutputString(SystemTable->ConOut, u" > ");

            count = 0;

        } else if (Key.UnicodeChar == 0) {
            SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
            SystemTable->ConOut->OutputString(SystemTable->ConOut, u"Invalid key press! (Function keys not supported)\r\n");
            SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\r\n");
            SystemTable->ConOut->OutputString(SystemTable->ConOut, u" > ");
            count = 0;

        } else if (Key.UnicodeChar == u'\b'){

            if (count > 0) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, u"\b \b");
                count--;
            }

        } else {
            if (count < 127) {
                SystemTable->ConOut->OutputString(SystemTable->ConOut, buf);
                cmd[count] = Key.UnicodeChar;
                count++;
            }
        }
    }

}
