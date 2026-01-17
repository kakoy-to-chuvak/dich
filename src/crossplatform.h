#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H


#if defined( _WIN32 ) || defined( _WIN64 ) // system check
#       include <windows.h>
        
#       define CRP_sleep Sleep


#else // system check
#       include <unistd.h>


#       define CRP_sleep sleep


#endif // system check















#endif // CROSSPLATFORM_H