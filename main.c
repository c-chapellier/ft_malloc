#include "src/ft_libc.h"
#include <stdlib.h>
#include <string.h>
#include <signal.h>

// #define VERIFY	1
#ifdef VERIFY
#define MALLOC	malloc	
#define REALLOC	realloc	
#define FREE	free
#else
#define MALLOC	ft_malloc
#define REALLOC	ft_realloc
#define FREE	ft_free
#endif

#define FAIL		RED "[FAIL]" RESET
#define SUCCESS		GREEN "[SUCCESS]" RESET
#define WARNING		YELLOW "[WARNING]" RESET

int main2();

void print_signal(int signal)
{
	switch (signal)
	{
	case SIGHUP:
		printf("SIGHUP");
		break ;
	case SIGINT:
		printf("SIGINT");
		break ;
	case SIGQUIT:
		printf("SIGQUIT");
		break ;
	case SIGILL:
		printf("SIGILL");
		break ;
	case SIGTRAP:
		printf("SIGTRAP");
		break ;
	case SIGABRT:
		printf("SIGABRT");
		break ;
#if  (defined(_POSIX_C_SOURCE) && !defined(_DARWIN_C_SOURCE))
	case SIGPOLL:
		printf("SIGPOLL");
		break ;
#else   /* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
	case SIGEMT:
		printf("SIGEMT");
		break ;
#endif  /* (!_POSIX_C_SOURCE || _DARWIN_C_SOURCE) */
	case SIGFPE:
		printf("SIGFPE");
		break ;
	case SIGKILL:
		printf("SIGKILL");
		break ;
	case SIGBUS:
		printf("SIGBUS");
		break ;
	case SIGSEGV:
		printf("SIGSEGV");
		break ;
	case SIGSYS:
		printf("SIGSYS");
		break ;
	case SIGPIPE:
		printf("SIGPIPE");
		break ;
	case SIGALRM:
		printf("SIGALRM");
		break ;
	case SIGTERM:
		printf("SIGTERM");
		break ;
	case SIGURG:
		printf("SIGURG");
		break ;
	case SIGSTOP:
		printf("SIGSTOP");
		break ;
	case SIGTSTP:
		printf("SIGTSTP");
		break ;
	case SIGCONT:
		printf("SIGCONT");
		break ;
	case SIGCHLD:
		printf("SIGCHLD");
		break ;
	case SIGTTIN:
		printf("SIGTTIN");
		break ;
	case SIGTTOU:
		printf("SIGTTOU");
		break ;
#if  (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
	case SIGIO:
		printf("SIGIO");
		break ;
#endif
	case SIGXCPU:
		printf("SIGXCPU");
		break ;
	case SIGXFSZ:
		printf("SIGXFSZ");
		break ;
	case SIGVTALRM:
		printf("SIGVTALRM");
		break ;
	case SIGPROF:
		printf("SIGPROF");
		break ;
#if  (!defined(_POSIX_C_SOURCE) || defined(_DARWIN_C_SOURCE))
	case SIGWINCH:
		printf("SIGWINCH");
		break ;
	case SIGINFO:
		printf("SIGINFO");
		break ;
#endif
	case SIGUSR1:
		printf("SIGUSR1");
		break ;
	case SIGUSR2:
		printf("SIGUSR2");
		break ;
	}
}

static void test_malloc_n(const char *name, int size)
{
	char *ft_str, *str;

	ft_str = MALLOC(size);
	str = malloc(size);
	if (str == NULL)
	{
		printf("real malloc failed\n");
		exit(-1);
	}
	else if (ft_str == NULL)
	{
		printf(FAIL " %s%d: %s != %s\n", name, size, ft_str, str);
		return ;
	}

	for (int i = 0; i < size - 1; ++i)
	{
		ft_str[i] = 'a' +(i < 20 ? i : 0);
		str[i] = 'a' + (i < 20 ? i : 0);
	}
	ft_str[size - 1] = '\0';
	str[size - 1] = '\0';

    // show_alloc_mem_ex();

	if (strcmp(ft_str, str) != 0)
	{
		printf(FAIL " %s%d: %.20s != %.20s\n", name, size, ft_str, str);
		FREE(ft_str);
		free(str);
	}
	else
	{
		printf(SUCCESS " %s%d : %.20s == %.20s\n", name, size, ft_str, str);
		// FREE(ft_str);
		free(str);
	}
}

void test_malloc_1(const char *name, int size)
{
	(void)size;
	test_malloc_n(name, 20);
	test_malloc_n(name, 30);
	test_malloc_n(name, 200);
	test_malloc_n(name, 300);
	test_malloc_n(name, 2000);
	test_malloc_n(name, 3000);
    // show_alloc_mem_ex();
}

void test_malloc_2(const char *name, int n)
{
	char *str;

	(void)n;
	str = MALLOC(0);
	if (str == NULL)
	{
		printf(FAIL " %s\n", name);
		return ;
	}
	printf(SUCCESS " %s\n", name);
}

void test_malloc_3(const char *name, int n)
{
	int m = 500;
	char *str[m];

	(void)n;
	for (int i = 0; i < m; ++i)
	{
		str[i] = MALLOC(99);
		if (str[i] == NULL)
		{
			printf(FAIL " %s\n", name);
			return ;
		}
		memset(str[i], 'a' + (i < 20 ? i : 0), 99);
	}
	// show_alloc_mem();
	for (int i = 0; i < m; ++i)
	{
		FREE(str[i]);
	}
	printf(SUCCESS " %s\n", name);
}

void test_free_1(const char *name, int n)
{
	char *str;

	(void)name;
	(void)n;
	str = MALLOC(10);
	if (str == NULL)
	{
		printf("real malloc error\n");
		exit(-1);
	}
	FREE(str);
	FREE(str);
}

void test_free_2(const char *name, int n)
{
	(void)n;
	FREE(NULL);
	printf(SUCCESS " %s\n", name);
}

void test_free_3(const char *name, int n)
{
	char *str = (char *)0xFEEDBEAF;

	(void)n;
	(void)name;
	FREE(str);
}

void test_free_4(const char *name, int n)
{
	char *str = (char *)0x1;

	(void)n;
	(void)name;
	FREE(str);
}

void test_realloc_1(const char *name, int n)
{
	char *str, *str2;

	(void)n;
   	str = (char *) MALLOC(15);
   	str2 = (char *) REALLOC(str, 25);
	if (str2 != str)
	{
   		printf(FAIL " %s: %p != %p -> a small change realloc memory\n", name, str, str2);
		FREE(str2);
		return ;
	}
	FREE(str2);
	printf(SUCCESS " %s: %p == %p -> a small change doesn't realloc memory\n", name, str, str2);
}

void test_realloc_2(const char *name, int n)
{
	int m = 1000;
	char *str, *str2;

	(void)n;
   	str = (char *) MALLOC(15);
   	str2 = (char *) REALLOC(str, 14 + m);
	if (str2 == str)
	{
   		printf(WARNING " %s: %p == %p -> a big change doesn't realloc memory\n", name, str, str2);
		FREE(str2);
		return ;
	}
	FREE(str2);
	printf(SUCCESS " %s: %p != %p -> a bif change realloc memory\n", name, str, str2);
}

void env_test(const char *name, int n, void test(const char *name, int n), int expected_signal)
{
	int pid;

	pid = fork();
	if (pid == 0)
	{
		dup2(1, 2);
		test(name, n);
		// char f[1024];
		// sprintf(f, "leaks %d > /dev/null", getpid());
		// if (system(f) != 0)
		// {
		// 	sprintf(f, "leaks %d", getpid());
		// 	printf(MAGENTA "[LEAKS] " RESET);
		// 	fflush(stdout);
		// 	system(f);
		// 	printf("\n");
		// }
		exit(0);
	}

	int status;
    if (waitpid(pid, &status, 0) == -1)
	{
        perror("waitpid() failed");
        exit(EXIT_FAILURE);
    }
	if (WIFSIGNALED(status))
	{
		int signal;
		
		signal = WTERMSIG(status);
		if (expected_signal == signal)
		{
			printf(SUCCESS " %s : signal encountered: ", name);
			print_signal(signal);
			printf("\n");
			return ;
		}
		printf(FAIL " %s : signal encountered: ", name);
		print_signal(signal);
		printf("\n");
		return ;
	}
    if (!WIFEXITED(status))
	{
		kill(pid, SIGKILL);
		printf(RED "Has not exited\n" RESET);
		return ;
    }
	if (expected_signal != 0)
	{
		printf(FAIL " %s : should have received: ", name);
		print_signal(expected_signal);
		printf("\n");
		return ;
	}
}

int main()
{
	env_test("test_malloc_1_", 0, test_malloc_1, 0);
	env_test("test_malloc_2", 0, test_malloc_2, 0);
	env_test("test_malloc_3", 0, test_malloc_3, 0);
	env_test("test_free_1", 0, test_free_1, SIGABRT);
	env_test("test_free_2", 0, test_free_2, 0);
	env_test("test_free_3", 0, test_free_3, SIGABRT);
	env_test("test_free_4", 0, test_free_4, SIGABRT);
	env_test("test_realloc_1", 0, test_realloc_1, 0);
	env_test("test_realloc_2", 0, test_realloc_2, 0);
	// system("leaks a.out");
    return (0);
}
