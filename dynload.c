extern void *getkernel32base();
extern int scmp(char*, char*, int);
extern void exit();

void mainCRTStartup()
{
    void *krnl32 = getkernel32base();
    long *dos_lfanew = (long*)((unsigned char*)krnl32 + 0x3C);

    void *nt_header = (void*)((unsigned char*)krnl32 + (*dos_lfanew));
    void *optional_header = (void*)((unsigned char*)nt_header + 0x18);
    void *data_directory = (void*)((unsigned char*)optional_header + 0x70);
    int *export_rva = (int*)data_directory;

    void *export_table = (void*)((unsigned char*)krnl32 + (*export_rva));

    int *number_of_names = (int*)((unsigned char*)export_table + 0x18);
    int *address_of_functions = (int*)((unsigned char*)export_table + 0x1C);
    int *address_of_names = (int*)((unsigned char*)export_table + 0x20);
    int *name_ordinals = (int*)((unsigned char*)export_table + 0x24);

    int *functions_rva = (int*)((unsigned char*)krnl32 + (*address_of_functions));
    int *names_rva = (int*)((unsigned char*)krnl32 + (*address_of_names));
    short *ordinals_rva = (short*)((unsigned char*)krnl32 + (*name_ordinals));


    char loadlib[13] = "LoadLibraryA";
    char getproc[15] = "GetProcAddress";
    char freelib[12] = "FreeLibrary";

    void *(*LoadLibrary)(char*) = 0;
    void *(*GetProcAddress)(void*, char*) = 0;
    int (*FreeLibrary)(void*) = 0;

    for (int i = 0; i < *number_of_names; ++i)
    {
        int name_rva = names_rva[i];
        char *func_name = (char*)((unsigned char*)krnl32 + name_rva);
        int ordinal = ordinals_rva[i];
        int func_rva = functions_rva[ordinal];

        if (!scmp(func_name, loadlib, 12))
            LoadLibrary = (void*(*)(char*))((unsigned char*)krnl32 + func_rva);

        if (!scmp(func_name, getproc, 14))
            GetProcAddress = (void*(*)(void*, char*))((unsigned char*)krnl32 + func_rva);
        
        if (!scmp(func_name, freelib, 12))
            FreeLibrary = (int(*)(void*))((unsigned char*)krnl32 + func_rva);
        

        if (LoadLibrary && GetProcAddress && FreeLibrary)
            break;
    }

    void *raylib = LoadLibrary("raylib.dll");

    void (*InitWindow)(int, int, const char*) = 
    (void(*)(int, int, const char*))GetProcAddress(raylib, "InitWindow");

    char (*WindowShouldClose)() = 
    (char(*)())GetProcAddress(raylib, "WindowShouldClose");

    void (*BeginDrawing)() = 
    (void(*)())GetProcAddress(raylib, "BeginDrawing");

    void (*EndDrawing)() = 
    (void(*)())GetProcAddress(raylib, "EndDrawing");

    void (*SetTargetFPS)(int) = 
    (void(*)(int))GetProcAddress(raylib, "SetTargetFPS");

    void (*CloseWindow)() = 
    (void(*)())GetProcAddress(raylib, "CloseWindow");

    InitWindow(800, 450, "Raylib");

    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        BeginDrawing();
        EndDrawing();
    }

    CloseWindow();

    FreeLibrary(raylib);
    exit();
}