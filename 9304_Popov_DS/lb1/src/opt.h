#pragma once

#include <getopt.h>



namespace opt {

    static const int THREAD_LAB  = 1;
    static const int PROCESS_LAB = 2;
    static const std::string helpStr = "How to use this program?:\n<app> <flags>\n\nFlags:\n-h, --help\tInfo about programm\n-t, --thread\tStart programm in thread mode\n-p, --process\tStart programm in process mode";


    int getOptForApp(const int& argc, char* const argv[])
    {
        if (argc == 1)
            return 0;

        const char* shortOption = "tph?";
        const struct option longOption[] = {
            {"thread",  no_argument, nullptr, 't'},
            {"process", no_argument, nullptr, 'p'},
            {"help",    no_argument, nullptr, 'h'}
        };

        int nextOpt = 0;

        while ((nextOpt = getopt_long(argc, argv, shortOption, longOption, nullptr)) != -1)
        {
            switch (nextOpt)
            {
            case 't': return THREAD_LAB;
            case 'p': return PROCESS_LAB;
            case 'h':
            case '?':
            default:
                break;
            }
        }

        return 0;
    }

}
