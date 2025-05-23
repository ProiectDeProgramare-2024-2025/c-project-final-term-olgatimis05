#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Add this for srand(time(NULL))

#define MAX_PLAYERS 100
#define MAX_QUESTIONS 15
#define MAX_NAME_LENGTH 50

typedef struct
{
    char name[MAX_NAME_LENGTH];
    float score;
} Player;

typedef struct
{
    char question[255];
    char options[4][255];
    int correct_answer;
} Question;

Player leaderboard[MAX_PLAYERS];
int num_players = 0;

Question questions[MAX_QUESTIONS];
int current_question = 0;

void header();
void add();
void display();
void delete();
void play_game();
void use_lifeline(int question_index, int *answered_correctly);
void show_leaderboard();
void show_history(char *player_name);
void menu(int op);
void load_questions();
void save_player_score(char *player_name, float score);

void header()
{
    system("cls");
    printf("--------------\n");
    printf("1 - Play Game\n");
    printf("2 - View Leaderboard\n");
    printf("3 - View Game History\n");
    printf("0 - Exit\n");
    printf("--------------\n");
}

void add()
{
    system("cls");
    printf("Not implemented please press enter");
    getchar();
    getchar();
}

void display()
{
    system("cls");
    printf("Not implemented please press enter");
    getchar();
    getchar();
}

void delete()
{
    system("cls");
    printf("Not implemented please press enter");
    getchar();
    getchar();
}

// Add these function prototypes
void load_leaderboard();
void save_leaderboard();
void sort_leaderboard();

/**void show_leaderboard()
{
    system("cls");
    printf("\nLeaderboard:\n");
    printf("Rank  Name                 Score\n");
    printf("----------------------------------\n");
    int i;
    for (i = 0; i < num_players; i++)
    {
        printf("%-5d %-20s %.2f points\n", i + 1, leaderboard[i].name, leaderboard[i].score);
    }
    printf("\n");

    printf("Press Enter to continue...");
    getchar();
    getchar();
}
*/

void show_leaderboard()
{
    system("cls");
    printf("\nLeaderboard:\n");
    printf("Rank  Name                 Score\n");
    printf("----------------------------------\n");

    int i;
    for (i = 0; i < num_players; i++)
    {
        if (i == 0)
        {
            printf("%-5d \033[0;32m%-20s %.2f\033[0m points\n", i + 1, leaderboard[i].name, leaderboard[i].score);
        }

        else if (i == num_players - 1)
        {
            printf("%-5d \033[0;31m%-20s %.2f\033[0m points\n", i + 1, leaderboard[i].name, leaderboard[i].score);
        }

        else
        {
            printf("%-5d %-20s %.2f points\n", i + 1, leaderboard[i].name, leaderboard[i].score);
        }
    }
    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}



void show_history(char *player_name)
{
    system("cls");
    printf("\nGame History for %s:\n", player_name);

    FILE *file = fopen("leaderboard.txt", "r");
    if (file == NULL)
    {
        printf("No game history available.\n");
        printf("\nPress Enter to continue...");
        getchar();
        return;
    }

    char line[256];
    int found = 0;
    float best_score = 0;

    // First pass to find the best score
    while (fgets(line, sizeof(line), file))
    {
        char *name = strtok(line, ",");
        char *score_str = strtok(NULL, "\n");

        if (name && score_str && strcmp(name, player_name) == 0)
        {
            float score = atof(score_str);
            if (score > best_score)
            {
                best_score = score;
            }
            found = 1;
        }
    }

    if (found)
    {
        printf("Best Score: %.2f points\n\n", best_score);

        // Display all entries in the leaderboard for this player
        int i;
        for (i = 0; i < num_players; i++)
        {
            if (strcmp(leaderboard[i].name, player_name) == 0)
            {
                printf("Rank %d: %.2f points\n", i + 1, leaderboard[i].score);
            }
        }
    }
    else
    {
        printf("No history found for player %s.\n", player_name);
    }

    fclose(file);

    printf("\nPress Enter to continue...");
    getchar();
}

void play_game()
{
    system("cls");
    float score = 0.0;
    int answered_correctly = 1;
    int i;

    for (i = 0; i < MAX_QUESTIONS; i++)
    {
        system("cls");
        current_question = i;

        printf("\nQuestion %d: %s\n", i + 1, questions[i].question);

        // Offer the 50/50 lifeline before answering
        printf("Do you want to use 50/50 lifeline for this question? (y/n): ");
        char lifeline_choice;
        getchar(); // Clear the newline
        scanf("%c", &lifeline_choice);

        if (lifeline_choice == 'y' || lifeline_choice == 'Y')
        {
            use_lifeline(i, &answered_correctly);
            score -= 0.5; // Deduct 0.5 points for using the 50/50 lifeline

            if (answered_correctly)
            {
                score += 1.0;
            }
            else
            {
                break; // Exit if the player answers incorrectly
            }
        }
        else
        {
            int j;
            for (j = 0; j < 4; j++)
            {
                printf("%d. %s\n", j + 1, questions[i].options[j]);
            }

            // Now prompt the player for their answer
            printf("Your answer (1-4): ");
            int answer;
            scanf("%d", &answer);

            if (answer == questions[i].correct_answer)
            {
                score += 1.0;
                printf("Correct!\n");
            }
            else
            {
                printf("Incorrect!\n");
                answered_correctly = 0;
                break; // Exit if the player answers incorrectly
            }
        }
    }
    system("cls");

    if (answered_correctly)
    {
        printf("Congratulations! You finished the game with %.2f points.\n", score);
    }
    else
    {
        printf("You lost. Your score is %.2f points.\n", score);
    }

    // Save player score
    char player_name[MAX_NAME_LENGTH];
    printf("Enter your name for the leaderboard: ");
    getchar();
    fgets(player_name, MAX_NAME_LENGTH, stdin);
    player_name[strcspn(player_name, "\n")] = '\0';

    save_player_score(player_name, score);
}

void use_lifeline(int question_index, int *answered_correctly)
{
    system("cls");
    printf("\nQuestion %d: %s\n", question_index + 1, questions[question_index].question);
    printf("50/50 lifeline activated!\n");

    int correct_answer = questions[question_index].correct_answer;
    int incorrect_answers[3];
    int index = 0;

    // Identify the incorrect answers
    int i;
    for (i = 0; i < 4; i++)
    {
        if (i != correct_answer - 1)
        {
            incorrect_answers[index++] = i;
        }
    }

    // Randomly remove 2 incorrect answers
    int removed[2] = {rand() % 3, rand() % 3};
    while (removed[0] == removed[1])
    {
        removed[1] = rand() % 3;
    }

    // Print the remaining two options (correct answer and one random incorrect one)
    int remaining_incorrect = incorrect_answers[removed[0]];
    int remaining[2];
    int correct_position;

    // Determine the display order and new correct answer position
    if (rand() % 2 == 0)
    {
        remaining[0] = correct_answer - 1;
        remaining[1] = remaining_incorrect;
        correct_position = 1; // Correct answer is option 1
    }
    else
    {
        remaining[0] = remaining_incorrect;
        remaining[1] = correct_answer - 1;
        correct_position = 2; // Correct answer is option 2
    }

    // Display the two remaining options
    for (i = 0; i < 2; i++)
    {
        printf("%d. %s\n", i + 1, questions[question_index].options[remaining[i]]);
    }

    // Get player's answer
    printf("Your answer (1-2): ");
    int answer;
    scanf("%d", &answer);

    // Check if the answer is correct
    if (answer == correct_position)
    {
        printf("Correct!\n");
        *answered_correctly = 1;
    }
    else
    {
        printf("Incorrect!\n");
        *answered_correctly = 0;
    }
}

void save_player_score(char *player_name, float score)
{
    // Always add the new score to the leaderboard
    strcpy(leaderboard[num_players].name, player_name);
    leaderboard[num_players].score = score;
    num_players++;

    // Sort and save the leaderboard
    sort_leaderboard();
    save_leaderboard();
}

// Sort leaderboard by score (highest to lowest)
void sort_leaderboard()
{
    int i, j;
    Player temp;

    for (i = 0; i < num_players - 1; i++)
    {
        for (j = 0; j < num_players - i - 1; j++)
        {
            if (leaderboard[j].score < leaderboard[j + 1].score)
            {
                // Swap players
                temp = leaderboard[j];
                leaderboard[j] = leaderboard[j + 1];
                leaderboard[j + 1] = temp;
            }
        }
    }
}

// Save leaderboard to file
void save_leaderboard()
{
    FILE *file = fopen("leaderboard.txt", "w");
    if (file == NULL)
    {
        printf("Error opening leaderboard file for writing!\n");
        return;
    }

    int i;
    for (i = 0; i < num_players; i++)
    {
        fprintf(file, "%s,%.2f\n", leaderboard[i].name, leaderboard[i].score);
    }

    fclose(file);
}

// Load leaderboard from file
void load_leaderboard()
{
    FILE *file = fopen("leaderboard.txt", "r");
    if (file == NULL)
    {
        // File doesn't exist yet, that's okay
        return;
    }

    num_players = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && num_players < MAX_PLAYERS)
    {
        char *name = strtok(line, ",");
        char *score_str = strtok(NULL, "\n");

        if (name && score_str)
        {
            strcpy(leaderboard[num_players].name, name);
            leaderboard[num_players].score = atof(score_str);
            num_players++;
        }
    }

    fclose(file);

    // Sort the leaderboard after loading
    sort_leaderboard();
}

void load_questions()
{
    FILE *file = fopen("questions.txt", "r");
    if (file == NULL) {
        printf("Error opening questions file! Make sure 'questions.txt' exists in the same directory.\n");
        return;
    }

    char line[256];
    int q_index = 0;

    while (q_index < MAX_QUESTIONS && fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';

        // Read question
        strcpy(questions[q_index].question, line);

        // Read 4 options
        int i;
        for (i = 0; i < 4; i++) {
            if (fgets(line, sizeof(line), file)) {
                line[strcspn(line, "\n")] = '\0';
                strcpy(questions[q_index].options[i], line);
            }
        }

        // Read correct answer
        if (fgets(line, sizeof(line), file)) {
            questions[q_index].correct_answer = atoi(line);
        }

        q_index++;
    }

    fclose(file);

    // If we couldn't load enough questions, print a warning
    if (q_index < MAX_QUESTIONS) {
        printf("Warning: Only loaded %d questions out of %d required.\n", q_index, MAX_QUESTIONS);
    }
}

void exit_game()
{
    char user_input[10];

    // Ask the user if they want to exit
    printf("\nType 'exit' to exit the game or press any key to return to the main menu: ");
    getchar(); // Clear the newline
    fgets(user_input, sizeof(user_input), stdin);
    user_input[strcspn(user_input, "\n")] = '\0'; // Remove the newline at the end

    if (strcmp(user_input, "exit") == 0)
    {
// Clear the screen depending on the operating system
#if defined(_WIN32) || defined(_WIN64)
        system("cls"); // Windows
#else
        system("clear"); // Unix/Linux/macOS
#endif
        printf("Exiting the game. Goodbye!\n");
        exit(0); // Exit the program
    }
    else
    {
        printf("\nReturning to the main menu...\n");
    }
}

void menu(int op)
{
    switch (op)
    {
    case 1:
        play_game();
        break;
    case 2:
        show_leaderboard();
        break;
    case 3:
    {
        char player_name[MAX_NAME_LENGTH];
        printf("Enter player name to view history: ");
        getchar();
        fgets(player_name, MAX_NAME_LENGTH, stdin);
        player_name[strcspn(player_name, "\n")] = '\0';
        show_history(player_name);
    }
    break;
    default:
        printf("EXIT!\n");
    }
}

int main()
{
    int option;

    // Initialize random seed
    srand(time(NULL));

    // Load questions and leaderboard
    load_questions();
    load_leaderboard();

    do
    {
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        menu(option);
    } while (option > 0 && option <= 3);

    return 0;
}
