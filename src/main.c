# include "common.h"

int main(int argc, char const *argv[])
{
    int choice;
    int ch;
    
    init_menu ();
    while (1)
    {
        if (scanf ("%d", &choice) != 1)
        {
            printf("请输入一个有效的数字！\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
            continue;
        }
        while ((ch = getchar()) != '\n' && ch != EOF);
        switch (choice)
        {
            case 1:
                person_person ();
                break;
            case 2:
                replay_chess ();
                break;
            case 3:
                printf ("期待与您的下次相遇。\n");
                exit (0);
                break;
            default:
                printf ("无效选择,请再次输入： \n");
        }
    }
    return 0;
}
