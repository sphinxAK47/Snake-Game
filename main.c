#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // Include conio.h for kbhit() and getch()
#include <time.h>
#include <unistd.h>  // Include unistd.h for usleep()

int HEIGHT = 20;
int WIDTH = 100;

struct Coordinates
{
    int coord_x;
    int coord_y;
    int food_x;
    int food_y;
    int tail_x[100];
    int tail_y[100];  
};

struct Stats
{
int snake_length;
int score;
int flag;
int gameover;
int pause;
int lives;
};

void game_over(struct Stats *s1)
{
    system("CLS");
    printf("\nGame Over!\nYour score: %d", s1->score);
    highscores(s1);
    s1->gameover = 0;
    system("pause");
    system("CLS");
}

void highscores(struct Stats *s1)
{
    char user[20];
    printf("\nEnter your username: ");
    scanf("%s", user);

    FILE *file1;
    file1 = fopen("highscores.txt", "a");

    if (file1 == NULL)
        printf("Failed to open file1\n");
    
    fprintf(file1, "%s: %d\n", user, s1->score);
    fclose(file1);
}

void draw_board(struct Coordinates *c1, struct Stats *s1)
{
    system("CLS");

    for (int i = 0; i < HEIGHT; i++) 
    {
        for (int j = 0; j < WIDTH; j++) 
        {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) 
                printf("#");
            
            else if (i == c1->coord_y && j == c1->coord_x)
            {
                printf(">");
            }

            else if (i == c1->food_y && j == c1->food_x)
                printf("0");

            else 
            {
                int isTail = 0;
                for (int k = 0; k < s1->snake_length; k++) 
                {
                    if (c1->tail_x[k] == j && c1->tail_y[k] == i) 
                    {
                        printf("o");
                        isTail = 1;
                        break;
                    }
                }
                if (!isTail)
                    printf(" ");
            }
        }
        printf("\n");
    }

    for (int i=0; i<40; i++)
    {
        if (i == 32)
            printf("\nScore: %d \nLives %d", s1->score, s1->lives);
        else
            printf(" ");
    }
}

void spawn_food(struct Coordinates *c1)
{
    c1->food_x = 1 + rand() % (WIDTH - 2);
    c1->food_y = 1 + rand() % (HEIGHT - 2);
}

void scoring(struct Coordinates *c1, struct Stats *s1)
{
    if (c1->coord_x == c1->food_x && c1->coord_y == c1->food_y)
    {
        s1->score += 10;
        s1->snake_length++;
        spawn_food(c1);
    }
}

void movement_input(struct Stats *s1)
{
    if (kbhit())
    {
        switch (getch())
        {
            case 'w':
                if (s1->flag != 3)  // Prevent moving in the opposite direction
                    s1->flag = 1;
                break;
            case 'a':
                if (s1->flag != 4)
                    s1->flag = 2;
                break;
            case 's':
                if (s1->flag != 1)
                    s1->flag = 3;
                break;
            case 'd':
                if (s1->flag != 2)
                    s1->flag = 4;
                break;
            case 'p':
                if (!s1->pause)
                    s1->pause = 1;  // s1->pause the game
                break;
            case 'r':
                s1->pause = 0;  // Resume the game
                break;
            case 'q':
                s1->flag = 7;
                break;
        }
    }
}

void game(struct Coordinates *c1, struct Stats *s1)
{
    if (!s1->pause)
        {
        int prev_x = c1->tail_x[0];
        int prev_y = c1->tail_y[0];
        int temp_x, temp_y;
        c1->tail_x[0] = c1->coord_x;
        c1->tail_y[0] = c1->coord_y;

        for (int i = 1; i < s1->snake_length; i++) 
        {
            temp_x = c1->tail_x[i];
            temp_y = c1->tail_y[i];
            c1->tail_x[i] = prev_x;
            c1->tail_y[i] = prev_y;
            prev_x = temp_x;
            prev_y = temp_y;
        }

        switch (s1->flag)
        {
            case 1:
                c1->coord_y--;
                break;
            case 2:
                c1->coord_x--;
                break;
            case 3:
                c1->coord_y++;
                break;
            case 4:
                c1->coord_x++;
                break;
            case 7:
                game_over(s1);
                break;
        }

        if (c1->coord_x <= 0 || c1->coord_x >= WIDTH - 1 || c1->coord_y <= 0 || c1->coord_y >= HEIGHT - 1)
        {
            s1->lives--;
            c1->coord_x = WIDTH / 2;
            c1->coord_y = HEIGHT /2;
        }
        for (int i = 1; i < s1->snake_length; i++) 
        {
            if (c1->coord_x == c1->tail_x[i] && c1->coord_y == c1->tail_y[i])
            {
                s1->lives--;
                c1->coord_x = WIDTH / 2;
                c1->coord_y = HEIGHT /2;
            }
        }

        if (s1->lives <= 0)
            game_over(s1);

    }
}

int main()
{
    struct Coordinates c1;


    srand((unsigned)time(NULL));

    struct Stats s1;
    s1.snake_length = 1;
    s1.score = 0;
    s1.gameover = 1;
    s1.pause = 0;
    s1.lives = 10;

    int wait = 5000;
    char line[50];

    int x = 0;
    int y = 0;

while (x == 0)
{
    printf("Would you like to:\n1.Play Game\n2.View Highscores\n");
    scanf("%d", &x);

    if (x == 2)
    {
        FILE *file2 = fopen("highscores.txt", "r");
        while (fgets(line, sizeof(line), file2))
        {
            printf("%s", line);
        }

        fclose(file2);
        x = 0;
        system("pause");
        system("CLS");
    }

    if (x == 1)
    {
        printf("1.Easy\n2.Medium\n3.Hard\n");
        scanf("%d", &y);
        s1.gameover = 1;

        WIDTH = 100;

        if (y == 2)
        {
            WIDTH = 75;
            s1.lives = 5;
        }

        if (y == 3)
        {
            WIDTH = 50;
            s1.lives = 3;
        }

        spawn_food(&c1);
        c1.coord_x = WIDTH / 2;
        c1.coord_y = HEIGHT / 2;

        while (s1.gameover == 1)
        {
            usleep(wait);
            draw_board(&c1, &s1);
            movement_input(&s1);
            game(&c1, &s1);
            scoring(&c1, &s1);

            // if (!s1.gameover)
            //     break;
        }

        x = 0;
        s1.gameover = 1;
        s1.lives = 3;
        s1.snake_length = 1;
        s1.score = 0;
        s1.gameover = 1;
        s1.pause = 0;
        s1.lives = 10;
    }
}
    return 0;
}
