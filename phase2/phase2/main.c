#include <ncurses.h>
#include <stdio.h>
#include <string.h>

#define LINE_NUMBER_WIDTH 5
#define MAX_LEN 1024
#define WINDOW_HEIGHT LINES-1
#define WINDOW_WIDTH COLS
#define FILE_NAME_HEIGHT 1
#define FILE_NAME_WIDTH COLS - LINE_NUMBER_WIDTH

const char path[150] = "/Users/armin/Desktop/root/file1.txt";
int line_lengths[1024];

void insert_mode(unsigned long int *cur_y,unsigned long int *cur_x){
    char ch;
    char buffer[MAX_LEN];

    curs_set(2); // Makes the cursor blink
    while ((ch = getch()) != 27) // 27 is the ASCII code for the ESC key
    {
        switch(ch)
        {
            case 'h':
                if (*cur_x <= LINE_NUMBER_WIDTH) {
                    (*cur_y)--;
                    (*cur_x) = line_lengths[*cur_y] + 1;
                }
                else
                    (*cur_x)--;
                break;
            case 'j':
                    (*cur_y)--;
                break;
            case 'k':
                    (*cur_y)++;
                break;
            case 'l':
                if (*cur_x >= line_lengths[*cur_y]) {
                    *cur_x = LINE_NUMBER_WIDTH;
                    (*cur_y)++;
                }
                else
                    (*cur_x)++;
                
                break;
            default:
                line_lengths[*cur_y]++;
                char str[50]="\0";
                str[0] = ch;
                mvaddstr( (*cur_y) + 1, (*cur_x) + LINE_NUMBER_WIDTH, str);
                (*cur_x)++;
            break;
              
                
            (*cur_x) = ((*cur_x) + COLS) % COLS;
            (*cur_y) = ((*cur_y) + LINES) % LINES;
                
            if ((*cur_x) - LINE_NUMBER_WIDTH >= strlen(buffer)) {
                (*cur_x) = LINE_NUMBER_WIDTH ;
                (*cur_y)++;
            }
            if ((*cur_x) > line_lengths[(*cur_y)]) {
                (*cur_x) = line_lengths[(*cur_y)] ;
            }

            move((*cur_y), (*cur_x));
        }
        
    }
    curs_set(0); // Turns off the cursor
}




int main()
{
    FILE *fp;
    char buffer[1024];
    unsigned long int line_number = 1, cursor_x = 0, cursor_y = 0;

    // Open the file
    fp = fopen(path, "r");
    if (!fp) {
        printf("Error: unable to open file \n");
        return 1;
    }

    // Initialize ncurses
    initscr();
    noecho();
    cbreak();
    start_color();
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    
    
    attron(A_BOLD);

    // Read the file contents and display them in a window
    while (fgets(buffer, sizeof(buffer), fp)) {
        char line_number_str[16];
        sprintf(line_number_str, "%*lu ", LINE_NUMBER_WIDTH - 1, line_number++);
        attron(COLOR_PAIR(1));
        mvaddstr(getcury(stdscr), 0, line_number_str);
        attroff(COLOR_PAIR(1));
        addstr(buffer);
    }

    // Fill in unused lines with a pair of tildes
    for (int y = getcury(stdscr) + 1; y < LINES-2; y++) {
        mvaddstr(y, 2, "~~");
    }

    // Turn off bold attribute and wait for a keypress
    attroff(A_BOLD);
    curs_set(1);

    // Move the cursor to the first position
    move(0, LINE_NUMBER_WIDTH);
    
    // Get the length of each line

    int line_count = 0;
    int char_count = LINE_NUMBER_WIDTH;
    rewind(fp);
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        if (ch == '\n') {
            line_lengths[line_count] = char_count - 1 ;
            line_count++;
            char_count = LINE_NUMBER_WIDTH;
        }
        char_count++;
    }
    line_lengths[0]++;


    // Process keypresses and update the cursor position
    char name[50] = "file1";
    mvprintw(WINDOW_HEIGHT - FILE_NAME_HEIGHT, 0, "%s", "file1");
    mvprintw(WINDOW_HEIGHT - FILE_NAME_HEIGHT, (int)strlen(name)+1, "(%s)", "saved");
    
    int x, y, height, width;
    char str[100];

    height = 3;
    width = 30;
    y = LINES - height;
    x = (COLS - width) / 2;

    
    while (1) {
        int c = getch();
        if (c == 'h') {
            if (cursor_x <= LINE_NUMBER_WIDTH) {
                cursor_y--;
                cursor_x = line_lengths[cursor_y] + 1;
            }
            else
                cursor_x--;

        }
        else if (c == 'l') {
            if (cursor_x >= line_lengths[cursor_y]) {
                cursor_x = LINE_NUMBER_WIDTH;
                cursor_y++;
            }
            else
                cursor_x++;
        }
        else if (c == 'k') {
            cursor_y++;
        }
        else if (c == 'j') {
            cursor_y--;
        }
        else if (c == 'i') {
            insert_mode(&cursor_y, &cursor_x);
        }
        else if (c == 'q') {
            break;
        }
        cursor_x = (cursor_x + COLS) % COLS;
        cursor_y = (cursor_y + LINES) % LINES;
        if (cursor_x - LINE_NUMBER_WIDTH >= strlen(buffer)) {
            cursor_x = LINE_NUMBER_WIDTH ;
            cursor_y++;
        }
        if (cursor_x > line_lengths[cursor_y]) {
            cursor_x = line_lengths[cursor_y] ;
        }

        move(cursor_y, cursor_x);

    }

    // Clean up ncurses and close the file
    endwin();
    fclose(fp);

    return 0;
}



