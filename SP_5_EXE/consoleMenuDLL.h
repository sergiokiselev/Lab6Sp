

#ifdef consoleMenuDLL_EXPROTS
#define consoleMenuDLL_API __declspec(dllexport)
#else
#define consoleMenuDLL_API __declspec(dllimport)
#endif 


consoleMenuDLL_API void print_menu();