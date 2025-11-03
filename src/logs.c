#include "logs.h"


#ifdef __cplusplus
extern "C" {
#endif

// --- Потокобезопасность ---
#if defined _WIN32 || defined _WIN64
        static CRITICAL_SECTION         ___log_mutex___;
        int logs_mutex_inited = 0;
#	define log_mutex_lock()         { if (0==logs_mutex_inited) InitializeCriticalSection(&___log_mutex___); EnterCriticalSection(&___log_mutex___);}
#	define log_mutex_unlock()  LeaveCriticalSection(&___log_mutex___)
#else   
        static pthread_mutex_t          ___log_mutex___ = PTHREAD_MUTEX_INITIALIZER;
#	define log_mutex_lock()         pthread_mutex_lock(&___log_mutex___)
#	define log_mutex_unlock()  pthread_mutex_unlock(&___log_mutex___)
#endif


// --- Глобальные настройки ---
static FILE* log_file = NULL;
static int log_level = 0;
static int logs_enabled = 1;
static int colors_enabled = 0;

// массивы наименований
static LOG_COLOR level_colors[5] = {LOG_COLOR_White, LOG_COLOR_White, LOG_COLOR_Bright_White, LOG_COLOR_Bright_Yellow, LOG_COLOR_Bright_Red};
static char *level_names[5] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR"};

// API
FILE* Logs_SetFile_name(const char* filename) {
        log_mutex_lock();
        if (log_file && log_file != stdout && log_file != stderr) fclose(log_file);
        log_file = fopen(filename, "a");
        if (!log_file) log_file = stdout;
        log_mutex_unlock();
        return log_file;
}

FILE* Logs_SetFile_stream(FILE *stream) {
        log_mutex_lock();
        if (log_file && log_file != stdout && log_file != stderr) fclose(log_file);
        log_file = stream ? stream : stdout;
        log_mutex_unlock();
        return log_file;
}

void Logs_CloseFile() {
        log_mutex_lock();
        if (log_file && log_file != stdout && log_file != stderr) {
                fclose(log_file);
        }
	log_file = stdout;
        log_mutex_unlock();
}

void Logs_EnableColors(int enable) {
        colors_enabled = enable;
}
void Logs_EnableLogs(int enable) {
        logs_enabled = enable;
}

void Logs_SetLogLevel(int level) {
        log_level = level;
}



// --- Основные функции логирования ---
int __Logs_LogArgs(const char* level, LOG_COLOR color, const char* module, const char* file, int line, const char* message_format, va_list args) {
        log_mutex_lock();

        // NULL pointer check
        if ( 0 == logs_enabled || NULL == module || NULL == message_format ) return -1;
        if ( NULL==log_file ) log_file = stderr;

        // time to string
        char timebuf[20] = "-";
        time_t t = time(NULL);
        struct tm tm_info;
        #if defined _WIN32 || defined _WIN64
        localtime_s(&tm_info, &t);
        #else
        localtime_r(&t, &tm_info);
        #endif
        strftime(timebuf, 20, "%Y-%m-%d %H:%M:%S", &tm_info);

        // creating message
        char msgbuf[1024];
        vsnprintf(msgbuf, sizeof(msgbuf), message_format, args);
        va_end(args);

        char filename_buf[300];
        if ( file ) 
                sprintf_s(filename_buf, 300, " [%s:%d]", file, line);
        char color_buf[16] = "";
        if ( colors_enabled )
                sprintf_s(color_buf, 15, "\033[%im", color);

        // printing log
        int res = fprintf(log_file, "%s[%s] [%s]%s [%s] %s%s\n", color_buf, timebuf, level, filename_buf, module, msgbuf, colors_enabled ? "\033[0m" : "");
        fflush(log_file);

        log_mutex_unlock();
        return res;
}

int Logs_LogCustom(const char* level, LOG_COLOR color, const char* module, const char* file, int line, const char* message_format, ...) {
        va_list args;
        va_start(args, message_format);
        return __Logs_LogArgs(level, color, module, file, line, message_format, args);
}



int Logs_Log(int level, const char *module, const char *file, int line, const char *message_format, ...) {
        if ( level < log_level ) return 0;
        va_list args;
        va_start(args, message_format);
        int fixed_level = level < 5 ? level : 0;
        return __Logs_LogArgs(level_names[fixed_level], level_colors[fixed_level], module, file, line, message_format, args);
}



#ifdef __LOGS_TEST__
int main() {
        Logs_EnableColors(1);
        for ( int i = 0 ; i < 6 ; i++ ) {
                Logs_SetLogLevel(i);
                LogCustom_wf("CHANGE", LOG_COLOR_Bright_Green, "main", "new level: %i", i);
                for ( int j = 0 ; j < 5 ; j++ )
                        Log(j, "main", "test message %i %s", j, "something");
        }
        
        LogCustom("CUSTOM", LOG_COLOR_Bright_Cyan, "main", "test custom %i", 10);
        LogCustom_wf("CUSTOM", LOG_COLOR_Bright_Magenta, "main", "test custom %i", 10);

        // speed test
        Logs_SetLogLevel(-1);
        LogInfo("speed-test", "start speed test");
        Logs_SetFile("speed-test.logs");
        clock_t end;
        LogInfo("speed-test", "speed tets started");
        clock_t start = clock();
        for ( int i = 0 ; i < 1000000 ; i++ ) {
                LogTrace("speed-test", "random not so long string with random format: %s %i. Just random symbols: hunrnhgimurkigcl,hergh5e8m98xp4mh85yrgu589gu54vguv85g9845ugk0hergh5e8m98xp4mh85yrgu589gu54vguv85g9845ugk0hergh5e8m98xp4mh85yrgu589gu54vguv85g9845ugk0hergh5e8m98xp4mh85yrgu589gu54vguv85g9845ugk0hergh5e8m98xp4mh85yrgu589gu54vguv85g9845ugk0hergh5e8m98xp4mh85yrgu589gu54vguv85g9845ugk0hergh5e8m98xp4mh85yrgu589gu54vguv85g9845ugk0-x5,40-gu8809guv0p8og9v,jg", "random message", i);
        }
        end = clock();
        double result = ((double)(end-start))/CLOCKS_PER_SEC;
        Logs_SetFile(stdout);
        LogInfo("speed-test", "logged 1m logs in %f seconds. Spent %fms per log. Speed: %f logs per second", result, result/1000, 1000000/result);
        return 0;
}
#endif // __LOGS_TEST__

#ifdef __cplusplus
}
#endif