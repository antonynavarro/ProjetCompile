char my_getchar(void);
int my_getint(void);
void my_putchar(char);
void my_putint(int);

// Entry point
__attribute__((naked, noreturn)) void _start(void) {
    // Lors de l'exécution du programme, les arguments de la ligne de commande
    // se trouve dans la pile, le nombre d'argument, suivi de l'adresse de
    // chaque argument. La fonction `main` suivant la convention d'appel, il
    // faut lui donner dans `rdi` ce nombre et dans `rsi` l'adresse de la case
    // dans la pile contenant l'adresse du premier argument.

    // Syntaxe AT&T
    asm("xor %ebp, %ebp\n"
        "mov (%rsp), %edi\n"  // Argc: [rsp]
        "lea 8(%rsp), %rsi\n" // Argv: rsp + 8
        "xor %rax, %rax\n"    //
        "call main\n"         //
        "mov %eax, %edi\n"    // exit(main retval)
        "mov $60, %eax\n"     //
        "syscall\n");         //
}

// Write wrapper
static int write(char *buf, int size) {
    int rax = 1;
    asm volatile("syscall"
                 : "+a"(rax)
                 : "D"(1), "S"(buf), "d"(size)
                 : "rcx", "r11");
    return rax;
}

// Macro d'affichage d'une chaine littérale
#define WRITE_STR(s) write("" s "", sizeof s - 1)

// strlen
static int len(char *buf) {
    int i = 0;
    while (*buf++)
        i++;
    return i;
}

// Affichage d'un entier sur 8 chiffres hexadécimaux
static char hex[] = "0123456789abcdef";
static void dump_hex(unsigned int v) {
    static char s[10];
    s[0] = '0';
    s[1] = 'x';
    for (int i = 0; i < 8; i++) {
        s[i + 2] = hex[v >> 28];
        v <<= 4;
    }
    write(s, 10);
}

// atoi sans échec
static int atoi(char *s) {
    int ret = 0;
    while (*s) {
        ret = ret * 10 + *s - '0';
        s++;
    }
    return ret;
}

// Appel my_getchar `n` fois et affiche la somme des retours
static void test_getchar(int n) {
    WRITE_STR("GETCHAR: ");
    int r = 0;
    for (int i = 0; i < n; i++)
        r += my_getchar();
    dump_hex(r);
    WRITE_STR("\n");
}

// Affiche une chaine de caractère via des appels à my_putchar
static void test_putchar(char *s) {
    WRITE_STR("PUTCHAR: ");
    for (; *s; s++)
        my_putchar(*s);
    WRITE_STR("\n");
}

// Appel my_getint `n` fois et affiche la somme des retours
static void test_getint(int n) {
    WRITE_STR("GETINT: ");
    int r = 0;
    for (int i = 0; i < n; i++)
        r += my_getint();
    dump_hex(r);
    WRITE_STR("\n");
}

// Affiche un entier via my_putint
static void test_putint(int n) {
    WRITE_STR("PUTINT: ");
    my_putint(n);
    WRITE_STR("\n");
}

int main(int argc, char *argv[]) {
    // Affichage des arguments
    WRITE_STR("CLI: ");
    for (int i = 0; argv[i]; i++) {
        char *arg = argv[i];
        write(arg, len(arg));
        WRITE_STR(" ");
    }
    WRITE_STR("\n");

    if (argc < 2) {
        WRITE_STR("usage: prog [-c N] [-C text] [-i N] [-I N]\n");
        return 1;
    }

    for (int i = 1; argv[i]; i++) {
        char *arg = argv[i];
        if (arg[0] != '-' || !argv[i + 1])
            continue;
        for (int j = 1; arg[j]; j++) {
            switch (arg[j]) {
            case 'c':
                test_getchar(atoi(argv[++i]));
                break;
            case 'C':
                test_putchar(argv[++i]);
                break;
            case 'i':
                test_getint(atoi(argv[++i]));
                break;
            case 'I':
                test_putint(atoi(argv[++i]));
                break;
            }
        }
    }
    return 0;
}