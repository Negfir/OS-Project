#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <sys/time.h>
#include <psapi.h>
#include <unistd.h>


static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

void init(){
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;

    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double getCurrentValue(){
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) +
              (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}



int main ()
{

    FILE* f = fopen("output.txt", "r");
    FILE *fptr1;
    FILE *fptr2;
    fptr1 = fopen("part1.txt", "w");
    fptr2 = fopen("part2.txt", "w");
    if(fptr1 == NULL)
    {
        printf("Error!");
        exit(1);
    }
    if(fptr2 == NULL)
    {
        printf("Error!");
        exit(1);
    }
    int number = 0;
   // int sum = 0; /* the sum of numbers in the file */
int a=0;
    while( fscanf(f, "%d,", &number) > 0 ) // parse %d followed by ','
    {a++;
        if (a>500000) {
            fprintf(fptr1, "%d\n", number);
        }
        else{
            fprintf(fptr2, "%d\n", number);
        }

        //printf("%d\n", number); // instead of sum you could put your numbers in an array
    }
    fclose(fptr1);
    fclose(fptr2);
    fclose(f);

    STARTUPINFO si = {};
    si.cb = sizeof si;

    STARTUPINFO si2 = {};
    si2.cb = sizeof si2;

    PROCESS_INFORMATION pi = {};
    PROCESS_INFORMATION pi2 = {};
   // const TCHAR* target = _T();

    clock_t t;
    t = clock();

    ///////////////////////
    /////Process 1 ////////
    //////////////////////

    char cmdArgs[] = "run.exe part1.txt 1";
    const TCHAR* target = _T("C:\\Users\\negfi\\CLionProjects\\func\\run.exe");

    if ( !CreateProcess(target, cmdArgs, 0, FALSE, 0, 0, 0, 0, &si, &pi) )
    {
        printf( "CreateProcess failed ");
    }
    else
    {
        DWORD dwExitCode = NULL;
        GetExitCodeProcess( pi.hProcess, &dwExitCode );
        printf("ret is %x\n", dwExitCode);
        printf("Waiting on process for 5 seconds..\n" );
        WaitForSingleObject(pi.hProcess, INFINITE);
        /*
        if ( TerminateProcess(pi.hProcess, 0) ) // Evil
            cout << "Process terminated!";
        */
        if ( PostThreadMessage(pi.dwThreadId, WM_QUIT, 0, 0) ) // Good
            printf("Request to terminate process has been sent!");


        init();
        double d = getCurrentValue();
       // printf("proc 1 is %f\n",&d);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }


    ///////////////////////
    /////Process 2 ////////
    //////////////////////

    char cmdArgs2[] = "run.exe part2.txt 2";
    const TCHAR* target2 = _T("C:\\Users\\negfi\\CLionProjects\\func\\run.exe");

    if ( !CreateProcess(target2, cmdArgs2, 0, FALSE, 0, 0, 0, 0, &si2, &pi2) )
    {
        printf( "CreateProcess failed ");
    }
    else
    {
        DWORD dwExitCode = NULL;
        GetExitCodeProcess( pi2.hProcess, &dwExitCode );
        printf("ret is %x\n", dwExitCode);
        printf("Waiting on process for 5 seconds..\n" );
        WaitForSingleObject(pi2.hProcess, INFINITE);
        /*
        if ( TerminateProcess(pi.hProcess, 0) ) // Evil
            cout << "Process terminated!";
        */
        if ( PostThreadMessage(pi2.dwThreadId, WM_QUIT, 0, 0) ) // Good
            printf("Request to terminate process has been sent!");

        init();
        double d = getCurrentValue();
       // printf("proc2 is %f\n",&d);
        CloseHandle(pi2.hProcess);
        CloseHandle(pi2.hThread);
    }

    t = clock() - t;
    double time_taken = ((double) t) / CLOCKS_PER_SEC; // in seconds

    printf("Total %f seconds to execute \n", time_taken);



        init();
        double d = getCurrentValue();
        printf("CPU Utilization is: %f percent\n", d);



//    HINSTANCE hProcHandle = GetModuleHandle(NULL);  // get the current process handle
//    PROCESS_MEMORY_COUNTERS_EX memory; // output will go here.
//
//    GetProcessMemoryInfo(hProcHandle, &memory, sizeof(memory));

    return 0;
}