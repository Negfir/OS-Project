#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>

void wait ( int seconds )
{
    clock_t endwait;
    endwait = clock () + seconds * CLOCKS_PER_SEC ;
    while (clock() < endwait) {}
}

void ppp (  )
{
    printf("hiiiiiiiiii");
}


int main ()
{
    STARTUPINFO si,si0;
    PROCESS_INFORMATION pi,pi0;
    HWND themozilla;
    UINT change;
    int n = 0;


    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

    ZeroMemory( &si0, sizeof(si0) );
    si0.cb = sizeof(si0);
    ZeroMemory( &pi0, sizeof(pi0) );

//    if (!
//            CreateProcess
//                    (
//                            TEXT("c:\\WINDOWS\\system32\\cal.exe"),
//                            NULL,NULL,NULL,TRUE,
//                            CREATE_NEW_CONSOLE,
//                            NULL,NULL,
//                            &si,
//                            &pi
//                    )
//            )
//    {
//        printf("Unable to execute.");
//    }
//    else {
//        for (n = 30; n > 0; n--) {
//            printf("%d\n", n);
//           // wait(1);
//        }
//    }

    if (!
            CreateProcess
                    (
                            TEXT("C:\\Users\\negfi\\CLionProjects\\untitled1\\a.out"),
                            NULL,NULL,NULL,TRUE,
                            CREATE_NEW_CONSOLE,
                            NULL,NULL,
                            &si,
                            &pi
                    )
            )
    {
        printf("Unable to execute.");
    }
    for (n= 150; n>0; n--)
    {
        printf ("%d\n",n);
        //wait (1);
    }


    printf("jjjjjjj");
//
//
//    themozilla = FindWindow(
//            NULL, TEXT("Calculator"));
//    if(themozilla == NULL)
//        printf("bad");
//
//    DWORD exitCode;
//    change = GetExitCodeProcess(themozilla,&exitCode);
//
//    ExitProcess(
//            change
//    );

    return 0;
}