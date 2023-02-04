//
//  main.c
//  project mabani
//
//  Created by Armin on 1/21/23.
//


#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
const char cache_path[150] = "/Users/armin/Desktop/memo/cache.txt";
const char cache_backup_path[150] = "/Users/armin/Desktop/memo/cache backup.txt";
const char files_link_path[150] = "/Users/armin/Desktop/memo/fileslinks.txt";
const char clipboard_path[150] = "/Users/armin/Desktop/memo/clipboard.txt";
const char grep_path[150] = "/Users/armin/Desktop/memo/grep.txt";
const char grepnames_path[150] = "/Users/armin/Desktop/memo/grepnames.txt";
const char tree_path[150] = "/Users/armin/Desktop/memo/tree.txt";
const char findfile_path[150] = "/Users/armin/Desktop/memo/find.txt";
long int found_characters[100] = {-1};
int found_count_char_uni;

char *strremove(char *str, const char *sub) {
    size_t len = strlen(sub);
    if (len > 0) {
        char *p = str;
        while ((p = strstr(p, sub)) != NULL) {
            memmove(p, p + len, strlen(p + len) + 1);
        }
    }
    return str;
}

char *strshift(char *str, int length){
    for (int i=length; i<strlen(str); i++) {
        str[i-length] = str[i];
    }
    str[strlen(str)-length] = '\0';
    return str;
}

long long int myAtoi(char* str)
{
    long long int res = 0;
    for (int i = 0; str[i] != '\0'; ++i)
        res = res * 10 + str[i] - '0';
    return res;
}

int readline(FILE *f, char *buffer, size_t len){
   char c;
   int i;

   memset(buffer, 0, len);

   for (i = 0; i < len; i++)
   {
      c = fgetc(f);

      if (!feof(f))
      {
         if (c == '\r')
            buffer[i] = 0;
         else if (c == '\n')
         {
            buffer[i] = 0;

            return i+1;
         }
         else
            buffer[i] = c;
      }
      else
      {
         return -1;
      }
   }

   return -1;
}

void create(char path_remainder[150] , char entry_save[1000]){

    char path[200] = "/Users/armin/Desktop";
    char path_save[200] = "/Users/armin/Desktop";
    const char backslash[]= "/";

    char * arg1 = strtok(path_remainder, "/");
    strcat(path, backslash);
    strcat(path, arg1);

    int check = mkdir(path,0777);
    if (check == 0) {
        printf("%s Directory was made succesfully\n",arg1);
    }
    else
        printf("%s Directory already exist\n",arg1);
    
    while (1) {
        arg1 = strtok(NULL, "/");
        //checks whether the word has .txt at the end of it or not. an if the condition was true then it would make a directory (if it wasn't already existing)
        if (arg1 && strstr(arg1, ".txt") == NULL) {
            strcat(path, backslash);
            strcat(path, arg1);
            check = mkdir(path,0777);
            if (check == 0) {
                printf("%s Directory was made succesfully\n",arg1);
            }
            else
                printf("%s Directory already exist\n",arg1);
        }
        //if there was a .txt in the word, then it'll make a textfile with that name
        else if(arg1 && strstr(arg1, ".txt") != NULL){
            const char sub[] ="createfile --file ";
            strremove(entry_save, sub);
            strcat(path_save, entry_save);
            FILE* my_file = fopen(path_save, "r");
            if (my_file==NULL) {
                my_file = fopen(path_save, "a");
                printf("File was made succesfully\n");
            }
            else{
                printf("File already exists\n");
            }
            fclose(my_file);
            
        }
        else
            break;
    }
}


void create_with_space(char path[150]){
    char path_with_space [150] = "\0";
    
    //for starts from 2 because the first two characters in path are " and /
    for (int i = 2; i < strlen(path); i++) {
        
        if(path[i] == 47 || path[i] == 34){
            char path_from_desk [200] = "/Users/armin/Desktop/";
            strcat(path_from_desk, path_with_space);
            if (path[i] == 47) {
                int check = mkdir(path_from_desk,0777);
                if (check == 0) {
                    printf("Directory was made succesfully\n");
                }
                else
                    printf("Directory already exist\n");
            }
            else if(path[i] == 34){
                FILE* my_file = fopen(path_from_desk, "r");
                if (my_file==NULL) {
                    my_file = fopen(path_from_desk, "a");
                    printf("File was made succesfully\n");
                }
                else{
                    printf("File already exists\n");
                }
                fclose(my_file);
                break;
            }
        }
        
        path_with_space[i-2] = path[i];
            
    }
    
}


void write_to_cache_file(FILE *file){
    FILE *cache_file = fopen(cache_path, "w");
    char ch;
    while ((ch=fgetc(file)) != EOF) {
        fprintf(cache_file, "%c" ,ch);
    }
    fclose(cache_file);
}
void write_to_cache_backup_file(FILE *file){
    FILE *cache_file = fopen(cache_backup_path, "w");
    char ch;
    while ((ch=fgetc(file)) != EOF) {
        fprintf(cache_file, "%c" ,ch);
    }
    fclose(cache_file);
}


void insert(char* file_path, char* insert_string , int line_pos, int inline_pos){
    FILE *check_file = fopen(file_path, "r");
    if (check_file!=NULL) {
        printf("File opened\n");
        write_to_cache_file(check_file);
        fclose(check_file);
        FILE *insert_file = fopen(file_path, "w");
        FILE *cache_file = fopen(cache_path, "r");
        char ch;
        int count_line = 1;
        int count_pos = 0;
        int check_string_insert = 1;
        
        while ((ch=fgetc(cache_file)) != EOF || ((( line_pos == 1 )) && ( inline_pos == 0 ))) {
            
            if(ch == '\n' && count_line == line_pos && count_pos == inline_pos && check_string_insert){
                for (int i=0; i<strlen(insert_string); i++) {
                    if (insert_string[i] == '\\') {
                        i++;
                        if (insert_string[i] == 'n') {
                            fprintf(insert_file,"%c",'\n');
                            i++;
                        }
                        else if (insert_string[i] == 't') {
                            fprintf(insert_file,"%c",'\t');
                            i++;
                        }
                        
                    }
                    fprintf(insert_file,"%c",insert_string[i]);
                }
                printf("string was succesfully inserted\n");
                check_string_insert = 0;
            }
            else if (ch == '\n'){
                count_line++;
            }
            else if(count_line == line_pos && check_string_insert){
                if(count_pos == inline_pos){
                    //fprintf(insert_file, "%s", insert_string);
                    for (int i=0; i<strlen(insert_string); i++) {
                        if (insert_string[i] == '\\') {
                            i++;
                            if (insert_string[i] == 'n') {
                                fprintf(insert_file,"%c",'\n');
                                i++;
                            }
                            else if (insert_string[i] == 't') {
                                fprintf(insert_file,"%c",'\t');
                                i++;
                            }
                            
                        }
                        fprintf(insert_file,"%c",insert_string[i]);
                    }
                    printf("string was succesfully inserted\n");
                    check_string_insert = 0;
                }
                count_pos++;
            }
            if (ch == EOF) {
                break;
            }
            fprintf(insert_file,"%c",ch);
        }
        if (check_string_insert) {
            printf("string couldn't be inserted\n");
        }
        fclose(insert_file);
        fclose(cache_file);
        
    }
    else{
        printf("File doesn't exist or couldn't be opened\n");
        fclose(check_file);
    }
    
}


void delete(char* file_path , int line_pos, int inline_pos , int size , char direction){

    if (size<0) {
        printf("size must be greater than zero\n");
        return;
    }
    FILE *check_file = fopen(file_path, "r");
    if (check_file!=NULL) {
        
        write_to_cache_file(check_file);
        fclose(check_file);
        FILE *cache_file = fopen(cache_path, "r");
        char ch;
        int count_line = 1;
        int count_pos = 0;
        int counter_size = 0;
        int start_operatation_delete_f = 0;
        if (direction == 'f') {
            FILE *remove_file = fopen(file_path, "w");
            
            while ((ch=fgetc(cache_file)) != EOF) {
                if(ch == '\n' && count_line == line_pos && count_pos == inline_pos && start_operatation_delete_f==0){
                    start_operatation_delete_f = 1;
                }
                else if (count_line == line_pos && start_operatation_delete_f==0){
                    if(count_pos == inline_pos){
                        start_operatation_delete_f = 1;
                    }
                    else
                        fprintf(remove_file,"%c",ch);
                        count_pos++;
                    
                }
                else if (ch == '\n' && start_operatation_delete_f==0) {
                    count_line++;
                    fprintf(remove_file,"%c",ch);
                }
                else if (start_operatation_delete_f==1 && counter_size<(size - 1)){
                    counter_size++;
                }
                else
                    fprintf(remove_file,"%c",ch);
            }
            fclose(remove_file);
            if (start_operatation_delete_f == 1) {
                printf("string was removed succesfully\n");
            }
            else
                printf("string couldn't be removed\n");
            
            
        }
        else if (direction == 'b'){
            int position[1000] = {0};
            int line[1000] = {0};
            int pix = 1;
            int check_back = 0;
            count_line = 1;
            count_pos = 0;
            
            while ( (ch=fgetc(cache_file)) != EOF ) {
                if(ch == '\n' && count_line == line_pos && count_pos == inline_pos && start_operatation_delete_f==0){
                    check_back = 1;
                    position[pix] = count_pos;
                    line[pix] = count_line + 1;
                    break;
                }
                else if(count_line == line_pos && count_pos == inline_pos){
                    check_back = 1;
                    position[pix] = count_pos - 1;
                    line[pix] = count_line;
                    break;
                }
                
                if (ch == EOF) {
                    break;
                }
                if (ch == '\n') {
                    count_line++;
                    count_pos = 0;
                }
                if (count_line == line_pos && inline_pos == 0) {
                    check_back = 1;
                    position[pix] = 0;
                    line[pix] = count_line;
                    break;
                }
                position[pix] = count_pos;
                line[pix] = count_line;
                pix++;
                count_pos++;
            }
            
            
            if (check_back && size < pix) {
                fclose(cache_file);
                delete(file_path, line[pix - size + 1], position[pix - size + 1], size, 'f');
                
            }
            else
                printf("out of bound\n");
        }
        else
            printf("Invalid Notation it should be either b (backward) or f (forward)\n");

        fclose(cache_file);
        
        
        
    }
    else{
        printf("File doesn't exist or couldn't be opened\n");
        fclose(check_file);
    }
        
}


void copy(char* file_path , int line_pos, int inline_pos , int size , char direction){
    
    if (size<0) {
        printf("size must be greater than zero\n");
        return;
    }
    FILE *check_file = fopen(file_path, "r");
    FILE *clipbaord_file = fopen(clipboard_path, "w");
    if (check_file!=NULL) {

        char ch;
        int count_line = 1;
        int count_pos = 0;
        int counter_size = 0;
        int start_operatation_copy_f = 0;
        if (direction == 'f') {
            
            while ((ch=fgetc(check_file)) != EOF) {
                if(ch == '\n' && count_line == line_pos && count_pos == inline_pos && start_operatation_copy_f==0){
                    start_operatation_copy_f = 1;
                    fprintf(clipbaord_file, "%c", ch);
                }
                else if (count_line == line_pos && start_operatation_copy_f==0){
                    if(count_pos == inline_pos){
                        start_operatation_copy_f = 1;
                        fprintf(clipbaord_file, "%c", ch);
                    }
                    else
                        count_pos++;
                    
                }
                else if (ch == '\n' && start_operatation_copy_f==0) {
                    count_line++;
                }
                else if (start_operatation_copy_f==1 && counter_size<(size - 1)){
                    fprintf(clipbaord_file, "%c", ch);
                    counter_size++;
                }
                
            }
            
            if (start_operatation_copy_f == 1) {
                printf("string was copied succesfully\n");
            }
            else
                printf("string couldn't be copied\n");
            
        }
        else if (direction == 'b'){
            int position[1000] = {0};
            int line[1000] = {0};
            int pix = 1;
            int check_back = 0;
            count_line = 1;
            count_pos = 0;
            
            while ( (ch=fgetc(check_file)) != EOF ) {
                if(ch == '\n' && count_line == line_pos && count_pos == inline_pos && start_operatation_copy_f==0){
                    check_back = 1;
                    position[pix] = count_pos;
                    line[pix] = count_line + 1;
                    break;
                }
                else if(count_line == line_pos && count_pos == inline_pos){
                    check_back = 1;
                    position[pix] = count_pos - 1;
                    line[pix] = count_line;
                    break;
                }
                
                if (ch == EOF) {
                    break;
                }
                if (ch == '\n') {
                    count_line++;
                    count_pos = 0;
                }
                if (count_line == line_pos && inline_pos == 0) {
                    check_back = 1;
                    position[pix] = 0;
                    line[pix] = count_line;
                    break;
                }
                position[pix] = count_pos;
                line[pix] = count_line;
                pix++;
                count_pos++;
            }
            
            
            if (check_back && size < pix) {
                fclose(check_file);
                fclose(clipbaord_file);
                copy(file_path, line[pix - size + 1], position[pix - size + 1], size, 'f');
                
            }
            else
                printf("out of bound\n");
        }
        else
            printf("Invalid Notation it should be either b (backward) or f (forward)\n");

        fclose(check_file);
        fclose(clipbaord_file);
        
        
        
    }
    else{
        printf("File doesn't exist or couldn't be opened\n");
        fclose(check_file);
    }
    
    fclose(check_file);
    fclose(clipbaord_file);
}


void find(char* file_path , char* find_string, int at_activate , int all_activate , int byword_activate , int count_activate){
    FILE *file = fopen(file_path, "r");
    if (file!=NULL) {
        
        //insert the whole file in a string to proceed for finding the sub string
        char ch;
        char file_string[100000] = "\0";
        int check_wild = 1;
        
        for (int i=0; (ch=fgetc(file)) != EOF; i++) {
            file_string[i] = ch;
        }
        
        for (int i=0; i < strlen(find_string); i++) {
            if (find_string[i] == '\\' && find_string[i+1] == '*') {
                check_wild = 0;
                strremove(find_string, "\\");
                break;
            }
        }
        
        int wildcard_back = 0 ;
        int wildcard_forward = 0 ;
        if (check_wild && find_string[0] == '*') {
            wildcard_back = 1;
            strshift(find_string, 1);
        }
        if (check_wild && find_string[strlen(find_string)-1] == '*') {
            wildcard_forward = 1;
            find_string[strlen(find_string)-1] = '\0';
        }
        
        for (int i = 0 ; i<100; i++) {
            found_characters[i] = 0;
        }
        found_characters[0] = -1;
        
        long int found_byword[100] = {-1};
        int found_count_char = 0;
        int found_count_word = 0;
            
        while (strstr(file_string, find_string)!=NULL) {
            long int ans = strstr(file_string, find_string) - file_string;
            
            if ( (*(strstr(file_string, find_string)-1) == ' ' || *(strstr(file_string, find_string)-1) == '\0' || *(strstr(file_string, find_string)-1) == '\n') && (*(strstr(file_string, find_string)+strlen(find_string)) == ' ' || *(strstr(file_string, find_string)+strlen(find_string)) == '\0')) {
                found_count_word = 0;
                for (int i=0; i<ans; i++) {
                    if (file_string[i] == ' ' || file_string[i] == '\n') {
                        found_count_word++;
                    }
                }
                *(strstr(file_string, find_string)+strlen(find_string)/2) = '~';
                found_byword[found_count_char] = found_count_word+1;
                found_characters[found_count_char] = ans;
                found_count_char++;
                
            }
            else if (wildcard_back && (*(strstr(file_string, find_string)+strlen(find_string)) == ' ' || *(strstr(file_string, find_string)+strlen(find_string)) == '\0')){
                long int i = ans-1;
                while (i>=0) {
                    if (file_string[i] == ' ') {
                        break;
                    }
                    else
                        i--;
                }
                found_count_word = 0;
                for (int x=0; x<i+1; x++) {
                    if (file_string[x] == ' ' || file_string[x] == '\n') {
                        found_count_word++;
                    }
                }
                *(strstr(file_string, find_string)+strlen(find_string)/2) = '~';
                found_byword[found_count_char] = found_count_word+1;
                found_characters[found_count_char] = i+1;
                found_count_char++;
                
            }
            else if (wildcard_forward && (*(strstr(file_string, find_string)-1) == ' ' || *(strstr(file_string, find_string)-1) == '\0' || *(strstr(file_string, find_string)-1) == '\n' )){
                *(strstr(file_string, find_string)+strlen(find_string)/2) = '~';
                found_count_word = 0;
                for (int i=0; i<ans; i++) {
                    if (file_string[i] == ' ' || file_string[i] == '\n') {
                        found_count_word++;
                    }
                }
                
                found_byword[found_count_char] = found_count_word+1;
                found_characters[found_count_char] = ans;
                found_count_char++;
                
            }
            else if(strstr(file_string, find_string)!=NULL){
                *(strstr(file_string, find_string)+strlen(find_string)/2) = '~';
            }
             
        }
        FILE *find_ouput_file = fopen(findfile_path, "w");
        
        found_count_char_uni = found_count_char;
        if (at_activate != 0 && all_activate == 0) {
            if (at_activate<=found_count_char && byword_activate==0) {
                printf("the %d sub-string is at %ld characters\n",at_activate,found_characters[at_activate-1]);
                fprintf(find_ouput_file, "the %d sub-string is at %ld characters\n",at_activate,found_characters[at_activate-1]);
            }
            else if (at_activate<=found_count_char && byword_activate==1) {
                printf("the %d sub-string is at %ld characters\n",at_activate,found_byword[at_activate-1]);
                fprintf(find_ouput_file, "the %d sub-string is at %ld characters\n",at_activate,found_byword[at_activate-1]);
            }
            else{
                printf("-1\nthere are not enough sub-strings in given file or this sub string doesn't exist\n");
                fprintf(find_ouput_file, "-1\nthere are not enough sub-strings in given file or this sub string doesn't exist\n");
            }
        }
        else if (at_activate == 0 && all_activate == 1 && byword_activate==0){
            printf("the sub-strings are at %ld, ",found_characters[0]);
            fprintf(find_ouput_file, "the sub-strings are at %ld, ",found_characters[0]);
            for (int i=1; found_characters[i] != 0; i++) {
                printf("%ld ," , found_characters[i]);
                fprintf(find_ouput_file, "%ld ," , found_characters[i]);
            }
            printf("characters\n");
            fprintf(find_ouput_file, "characters\n");
        }
        else if (at_activate == 0 && all_activate == 1 && byword_activate==1){
            printf("the sub-strings are at %ld, ",found_byword[0]);
            fprintf(find_ouput_file, "the sub-strings are at %ld, ",found_byword[0]);
            for (int i=1; found_byword[i] != 0; i++) {
                printf("%ld ," , found_byword[i]);
                fprintf(find_ouput_file, "%ld ," , found_byword[i]);
            }
            printf("words\n");
            fprintf(find_ouput_file, "words\n");
        }
        else if(count_activate){
            printf("found %d sub-strings in given file\n",found_count_char);
            fprintf(find_ouput_file, "found %d sub-strings in given file\n",found_count_char);
        }
        
        fclose(find_ouput_file);
    }
    else{
        printf("File doesn't exist or couldn't be opened\n");
        fclose(file);
    }
}


void tree(int depth, int cnt, const char *str , FILE * tree_file) {
    DIR *folder = opendir(str);
    if (depth < 0)
        return;
    if (folder == NULL) {
        return;
    }
    else {
        struct dirent *entry;
        while ((entry = readdir(folder))!=NULL) {
            if (*entry->d_name == '.')
                continue;
            for (int i = 0; i < cnt; i++){
                printf("  ");
                fprintf(tree_file, "  ");
            }
                
            printf(" ├──");
            fprintf(tree_file, " ├──");
            printf("%s\n", entry->d_name);
            fprintf(tree_file, "%s\n", entry->d_name);
            char cur_dir[300] = "\0";
            strcpy(cur_dir, str);
            const char backslash[]= "/";
            strcat(cur_dir, backslash);
            strcat(cur_dir, entry->d_name);
            tree(depth - 1, cnt + 1, cur_dir,tree_file);
        }
    }
}


void indent(char *path){
    char input[1000] = "\0";
    
    FILE *check_file = fopen(path, "r");
    if (check_file != NULL) {
        write_to_cache_file(check_file);
        fclose(check_file);
        FILE* cache_file = fopen(cache_path, "r");
        FILE* indent_file = fopen(path, "w");
        char ch;
        
        for (int i=0; (ch = fgetc(cache_file)) != EOF; i++) {
            input[i] = ch;
        }
        
        char output[1000] = "\0";
        int outputIndex = 0;
        int indent_lvl = 0;
        
        long int len = strlen(input);
        int spaces = 0;
        int first_of_line = 1;
        for (int i = 0; i < len; i++) {
            if (input[i] == ';') {
                output[outputIndex++] = input[i];
                output[outputIndex++] = '\n';
                first_of_line = 1;
            }
            else if(input[i] == '{'){
                if(first_of_line){
                    for (int i=0; i<indent_lvl; i++) {
                        output[outputIndex++] = '\t';
                    }
                }
                indent_lvl++;
                output[outputIndex++] = input[i];
                output[outputIndex++] = '\n';
                first_of_line = 1;
            }
            else if(input[i] == '}'){
                indent_lvl--;
                for (int i=0; i<indent_lvl; i++) {
                    output[outputIndex++] = '\t';
                }
                output[outputIndex++] = input[i];
                output[outputIndex++] = '\n';
                first_of_line = 1;
            }
            else if (input[i] == ' ' && first_of_line) {
                spaces++;
            }
            else if (input[i] != ' ' && input[i] != ';' && input[i] != '{' && input[i] != '}' && first_of_line) {
                first_of_line = 0;
                for (int i=0; i<indent_lvl; i++) {
                    output[outputIndex++] = '\t';
                }
                output[outputIndex++] = input[i];
            }
            else {
                first_of_line = 0;
                output[outputIndex++] = input[i];
            }
        }

        fprintf(indent_file, "%s" , output);

        fclose(cache_file);
        fclose(indent_file);
        printf("file was indented succesfully\n");

    }
    else{
        fclose(check_file);
        printf("file couldn't be opened\n");
    }
}


void print_diff_word(int line_num, char *word1, char *word2) {
    if (strcmp(word1, "\n")!=0 && strcmp(word2, "\n")!=0) {
        printf("words that are diffrent: <<%s>> and <<%s>>\n", word1, word2);
    }
}


void compare(char *path1 , char *path2){
    FILE *fp1 = fopen(path1, "r");
    if (!fp1) {
        printf("Failed to open file 1\n");
        return;
    }

    FILE *fp2 = fopen(path2, "r");
    if (!fp2) {
        printf("Failed to open file 2\n");
        return;
    }

    int line_num = 1;
    char line1[256], line2[256];
    while ((fgets(line1, sizeof(line1), fp1)) && (fgets(line2, sizeof(line2), fp2))) {
        if (strcmp(line1, line2) != 0) {
            char* line1_with = strtok(line1, "\n");
            char* line2_with = strtok(line2, "\n");
            printf("----------------------------------------------------------------\n");
            printf("Line %d:\n%s\n%s\n", line_num, line1_with, line2_with);

            int i = 0, j = 0;
            long int len1 = strlen(line1), len2 = strlen(line2);
            while (i < len1 && j < len2) {
                // Find the start of a word
                while (i < len1 && line1[i] == ' ') i++;
                while (j < len2 && line2[j] == ' ') j++;
                if (i == len1 || j == len2) break;

                // Find the end of a word
                int start1 = i, start2 = j;
                while (i < len1 && line1[i] != ' ') i++;
                while (j < len2 && line2[j] != ' ') j++;

                // Compare the words
                if (i - start1 != j - start2 || strncmp(&line1[start1], &line2[start2], i - start1) != 0) {
                    // Create a null-terminated string for the word
                    line1[i] = '\0';
                    line2[j] = '\0';
                    print_diff_word(line_num, &line1[start1], &line2[start2]);
                }
            }
        }
        line_num++;
    }

    if (fgets(line1, sizeof(line1), fp1)) {
        printf("----------------------------------------------------------------\n");
        printf("File 1 has more lines:\n%s\n", line1);
    } else if (fgets(line2, sizeof(line2), fp2)) {
        printf("----------------------------------------------------------------\n");
        printf("File 2 has more lines:\n%s\n", line2);
    }
    printf("----------------------------------------------------------------\n");
    fclose(fp1);
    fclose(fp2);

}


void get_operation_createfile(char * operator, char * entry_save){
    operator = strtok(NULL, " ");
    
    if (strcmp(operator, "--file") == 0) {
        operator = strtok(NULL, " ");
        char path[150] = "\0";
        strcat(path, operator);
        //checks if the path starts with " (ascii 34)
        if (path[0] == 34) {
            const char sub[] ="createfile --file ";
            strremove(entry_save, sub);
            create_with_space(entry_save);
        }
        else
            create(path , entry_save);
        
    }
    else
        printf("Invalid Notation\n");
    
}

char cat_string[10000];

void get_operation_cat(char * operator, char * entry_save){
    
    operator = strtok(NULL, " ");
    
    if (strcmp(operator, "--file") == 0) {
        operator = strtok(NULL, " ");
        const char sub[] ="cat --file ";
        strremove(entry_save, sub);
        char * cat_path = strtok(entry_save, "\"");
        char path[200] = "/Users/armin/Desktop";
        strcat(path, cat_path);
        
            FILE* cat_file;
        
            char ch;
            int i=0;
            cat_file = fopen(path, "r");
         
            if ( cat_file == NULL) {
                printf("file can't be opened \n");
            }
            else{
                printf("content of this file are: \n");
                printf("-----------------------------------------------\n");
                // Printing what is written in file character by character using loop.
                do {
                    ch = fgetc(cat_file);
                    // the \377 is for the autocorrect that mac has so I had to delete that
                    if (ch != '\377') {
                        printf("%c", ch);
                        cat_string[i] = ch;
                        i++;
                    }
                    
                } while (ch != EOF);
                fclose(cat_file);
                printf("\n-----------------------------------------------\n");
            }
            
    }
    
    else
        printf("Invalid Notation\n");
    
}


void get_data_for_insert(char * operator, char * entry_save){
    operator = strtok(NULL, " ");
    
    
    if (strcmp(operator, "--file") == 0) {
        
        const char sub[] ="insertstr --file ";
        strremove(entry_save, sub);
        int white_space_for_path;
        char cat_path[1500] = "\0";
        
        //make path even with spaces
        if(entry_save[0] == '\"'){
            for (int i=1;entry_save[i] != '\"'; i++) {
                cat_path[i-1] = entry_save[i];
            }
            strremove(entry_save, cat_path);
            white_space_for_path=1;
        }
        else{
            for (int i=0 ; i<strlen(entry_save) ; i++) {
                cat_path[i] = entry_save[i];
                if (entry_save[i+2]=='-' && entry_save[i+3]=='-' ) {
                    break;
                }
            }
            strremove(entry_save, cat_path);
            white_space_for_path=0;
        }
        char path[200] = "/Users/armin/Desktop";
        strcat(path, cat_path);
        
        //sorts the string to start with --str...
        if (white_space_for_path) {
            strshift(entry_save, 3);
        }
        else{
            strshift(entry_save, 1);
        }
        
        if (entry_save[0]=='-' && entry_save[1]=='-' && entry_save[2]=='s' && entry_save[3]=='t' && entry_save[4]=='r') {
            strshift(entry_save, 6);
            char insert_string[1500] = "\0";
            
            //white-space for path string
            if (entry_save[0] == '\"') {
                strshift(entry_save, 1);
                for (int i=0; i<strlen(entry_save); i++) {
                    if (entry_save[i] == '\"' && entry_save[i-1] != '\\') {
                        break;
                    }
                    insert_string[i] = entry_save[i];
                }
                strremove(entry_save, insert_string);
                strshift(entry_save, 2);
            }
            else{
                for (int i=0; i<strlen(entry_save); i++) {
                    if (entry_save[i] == ' ') {
                        break;
                    }
                    insert_string[i] = entry_save[i];
                }
                strremove(entry_save, insert_string);
                strshift(entry_save, 1);
            }
            
            if (entry_save[0]=='-' && entry_save[1]=='-' && entry_save[2]=='p' && entry_save[3]=='o' && entry_save[4]=='s') {
                strshift(entry_save, 6);
                char line_no[5] = "\0";
                //char inline_no[5] = "\0";
                for (int i=0; i<strlen(entry_save); i++) {
                    if (entry_save[i] == ':') {
                        break;
                    }
                    line_no[i] = entry_save[i];
                }
                
                //strremove(entry_save, line_no);
                strshift(entry_save, (int)strlen(line_no));
                if (entry_save[0] == ':') {
                    strshift(entry_save, 1);
                    int line_pos = (int)myAtoi(line_no);
                    int inline_pos = (int)myAtoi(entry_save);
                    
                    insert(path, insert_string, line_pos, inline_pos);
                }
                else
                    printf("Invalid Notation (probably on <line>:<pos>)\n");

            }
            else
                printf("Invalid Notation (probably on --pos)\n");
            
        }
        else
            printf("Invalid Notation (probably on --str)\n");
        
    }
    else
        printf("Invalid Notation (probably on --file)\n");
}


void get_data_for_remove(char * operator,char * entry_save){
    operator = strtok(NULL, " ");
    
    if (strcmp(operator, "--file") == 0) {
        const char sub[] ="removestr --file ";
        strremove(entry_save, sub);
        int white_space_for_path;
        char cat_path[1500] = "\0";
        
        //white-space for path string
        if(entry_save[0] == '\"'){
            for (int i=1;entry_save[i] != '\"'; i++) {
                cat_path[i-1] = entry_save[i];
            }
            strremove(entry_save, cat_path);
            white_space_for_path=1;
        }
        else{
            for (int i=0 ; i<strlen(entry_save) ; i++) {
                cat_path[i] = entry_save[i];
                if (entry_save[i+2]=='-' && entry_save[i+3]=='-' ) {
                    break;
                }
            }
            strremove(entry_save, cat_path);
            white_space_for_path=0;
        }
        char path[200] = "/Users/armin/Desktop";
        strcat(path, cat_path);
        
        //sorts the string to start with --pos...
        if (white_space_for_path) {
            strshift(entry_save, 3);
        }
        else{
            strshift(entry_save, 1);
        }
        
        if (entry_save[0]=='-' && entry_save[1]=='-' && entry_save[2]=='p' && entry_save[3]=='o' && entry_save[4]=='s') {
            strshift(entry_save, 6);
            char line_no[5] = "\0";
            char inline_no[5] = "\0";
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == ':') {
                    break;
                }
                line_no[i] = entry_save[i];
            }
            
            //strremove(entry_save, line_no);
            strshift(entry_save, (int)strlen(line_no));
            if (entry_save[0] == ':') {
                strshift(entry_save, 1);
                for (int i=0; i<strlen(entry_save); i++) {
                    if (entry_save[i] == ' ') {
                        break;
                    }
                    inline_no[i] = entry_save[i];
                }
                int line_pos = (int)myAtoi(line_no);
                int inline_pos = (int)myAtoi(inline_no);
                strshift(entry_save, (int)strlen(inline_no)+1);
                if (entry_save[0]=='-' && entry_save[1]=='s' && entry_save[2]=='i' && entry_save[3]=='z' && entry_save[4]=='e'){
                    strshift(entry_save, 6);
                    char sizestr[5]="\0";
                    for (int i=0; i<strlen(entry_save); i++) {
                        if (entry_save[i] == ' ') {
                            break;
                        }
                        sizestr[i] = entry_save[i];
                    }
                    int size = (int)myAtoi(sizestr);
                    strshift(entry_save, (int)strlen(sizestr)+1);
                    if (entry_save[0]=='-'){
                        char remove_direction = '\0';
                        if (entry_save[1]=='b') {
                            remove_direction = 'b';
                            delete(path , line_pos , inline_pos , size , remove_direction);
                        }
                        else if(entry_save[1]=='f'){
                            remove_direction = 'f';
                            delete(path , line_pos , inline_pos , size , remove_direction);
                        }
                        else
                            printf("Invalid Notation it should be either b (backward) or f (forward)\n");
                        
                    }
                    else
                        printf("Invalid Notation (probably on -b/f)\n");
                    
                }
                else
                    printf("Invalid Notation (probably on -size)\n");
                
            }
            else
                printf("Invalid Notation (probably on <line>:<pos>)\n");
            
        }
        else
            printf("Invalid Notation (probably on --pos)\n");
        
    }
    else
        printf("Invalid Notation (probably on --file)\n");
    
}


void get_data_for_copy(char * operator,char * entry_save){
    operator = strtok(NULL, " ");
    
    if (strcmp(operator, "--file") == 0) {
        const char sub[] ="copystr --file ";
        strremove(entry_save, sub);
        int white_space_for_path;
        char cat_path[1500] = "\0";
        
        //white-space for path string
        if(entry_save[0] == '\"'){
            for (int i=1;entry_save[i] != '\"'; i++) {
                cat_path[i-1] = entry_save[i];
            }
            strremove(entry_save, cat_path);
            white_space_for_path=1;
        }
        else{
            for (int i=0 ; i<strlen(entry_save) ; i++) {
                cat_path[i] = entry_save[i];
                if (entry_save[i+2]=='-' && entry_save[i+3]=='-' ) {
                    break;
                }
            }
            strremove(entry_save, cat_path);
            white_space_for_path=0;
        }
        char path[200] = "/Users/armin/Desktop";
        strcat(path, cat_path);
        
        //sorts the string to start with --pos...
        if (white_space_for_path) {
            strshift(entry_save, 3);
        }
        else{
            strshift(entry_save, 1);
        }
        
        if (entry_save[0]=='-' && entry_save[1]=='-' && entry_save[2]=='p' && entry_save[3]=='o' && entry_save[4]=='s') {
            strshift(entry_save, 6);
            char line_no[5] = "\0";
            char inline_no[5] = "\0";
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == ':') {
                    break;
                }
                line_no[i] = entry_save[i];
            }
            
            //strremove(entry_save, line_no);
            strshift(entry_save, (int)strlen(line_no));
            if (entry_save[0] == ':') {
                strshift(entry_save, 1);
                for (int i=0; i<strlen(entry_save); i++) {
                    if (entry_save[i] == ' ') {
                        break;
                    }
                    inline_no[i] = entry_save[i];
                }
                int line_pos = (int)myAtoi(line_no);
                int inline_pos = (int)myAtoi(inline_no);
                strshift(entry_save, (int)strlen(inline_no)+1);
                if (entry_save[0]=='-' && entry_save[1]=='s' && entry_save[2]=='i' && entry_save[3]=='z' && entry_save[4]=='e'){
                    strshift(entry_save, 6);
                    char sizestr[5]="\0";
                    for (int i=0; i<strlen(entry_save); i++) {
                        if (entry_save[i] == ' ') {
                            break;
                        }
                        sizestr[i] = entry_save[i];
                    }
                    int size = (int)myAtoi(sizestr);
                    strshift(entry_save, (int)strlen(sizestr)+1);
                    if (entry_save[0]=='-'){
                        char remove_direction = '\0';
                        if (entry_save[1]=='b') {
                            remove_direction = 'b';
                            copy(path, line_pos, inline_pos, size, 'b');
                        }
                        else if(entry_save[1]=='f'){
                            remove_direction = 'f';
                            copy(path, line_pos, inline_pos, size, 'f');
                        }
                        else
                            printf("Invalid Notation it should be either b (backward) or f (forward)\n");
                        
                    }
                    else
                        printf("Invalid Notation (probably on -b/f)\n");
                    
                }
                else
                    printf("Invalid Notation (probably on -size)\n");
                
            }
            else
                printf("Invalid Notation (probably on <line>:<pos>)\n");
            
        }
        else
            printf("Invalid Notation (probably on --pos)\n");
        
    }
    else
        printf("Invalid Notation (probably on --file)\n");
    
}


void get_data_for_cut(char * operator,char * entry_save){
    operator = strtok(NULL, " ");
    
    if (strcmp(operator, "--file") == 0) {
        const char sub[] ="cutstr --file ";
        strremove(entry_save, sub);
        int white_space_for_path;
        char cat_path[1500] = "\0";
        
        //white-space for path string
        if(entry_save[0] == '\"'){
            for (int i=1;entry_save[i] != '\"'; i++) {
                cat_path[i-1] = entry_save[i];
            }
            strremove(entry_save, cat_path);
            white_space_for_path=1;
        }
        else{
            for (int i=0 ; i<strlen(entry_save) ; i++) {
                cat_path[i] = entry_save[i];
                if (entry_save[i+2]=='-' && entry_save[i+3]=='-' ) {
                    break;
                }
            }
            strremove(entry_save, cat_path);
            white_space_for_path=0;
        }
        char path[200] = "/Users/armin/Desktop";
        strcat(path, cat_path);
        
        //sorts the string to start with --pos...
        if (white_space_for_path) {
            strshift(entry_save, 3);
        }
        else{
            strshift(entry_save, 1);
        }
        
        if (entry_save[0]=='-' && entry_save[1]=='-' && entry_save[2]=='p' && entry_save[3]=='o' && entry_save[4]=='s') {
            strshift(entry_save, 6);
            char line_no[5] = "\0";
            char inline_no[5] = "\0";
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == ':') {
                    break;
                }
                line_no[i] = entry_save[i];
            }
            
            //strremove(entry_save, line_no);
            strshift(entry_save, (int)strlen(line_no));
            if (entry_save[0] == ':') {
                strshift(entry_save, 1);
                for (int i=0; i<strlen(entry_save); i++) {
                    if (entry_save[i] == ' ') {
                        break;
                    }
                    inline_no[i] = entry_save[i];
                }
                int line_pos = (int)myAtoi(line_no);
                int inline_pos = (int)myAtoi(inline_no);
                strshift(entry_save, (int)strlen(inline_no)+1);
                if (entry_save[0]=='-' && entry_save[1]=='s' && entry_save[2]=='i' && entry_save[3]=='z' && entry_save[4]=='e'){
                    strshift(entry_save, 6);
                    char sizestr[5]="\0";
                    for (int i=0; i<strlen(entry_save); i++) {
                        if (entry_save[i] == ' ') {
                            break;
                        }
                        sizestr[i] = entry_save[i];
                    }
                    int size = (int)myAtoi(sizestr);
                    strshift(entry_save, (int)strlen(sizestr)+1);
                    if (entry_save[0]=='-'){
                        char remove_direction = '\0';
                        if (entry_save[1]=='b') {
                            remove_direction = 'b';
                            copy(path, line_pos, inline_pos, size, 'b');
                            delete(path, line_pos, inline_pos, size, 'b');
                        }
                        else if(entry_save[1]=='f'){
                            remove_direction = 'f';
                            copy(path, line_pos, inline_pos, size, 'f');
                            delete(path, line_pos, inline_pos, size, 'f');
                        }
                        else
                            printf("Invalid Notation it should be either b (backward) or f (forward)\n");
                        
                    }
                    else
                        printf("Invalid Notation (probably on -b/f)\n");
                    
                }
                else
                    printf("Invalid Notation (probably on -size)\n");
                
            }
            else
                printf("Invalid Notation (probably on <line>:<pos>)\n");
            
        }
        else
            printf("Invalid Notation (probably on --pos)\n");
        
    }
    else
        printf("Invalid Notation (probably on --file)\n");
    
}


void get_data_for_find(char * operator,char * entry_save){
    operator = strtok(NULL, " ");
    
    if (strcmp(operator, "--str") == 0) {
        const char sub[] ="find --str ";
        strremove(entry_save, sub);
        
        char find_string[1500] = "\0";
        
        //white-space for find string
        if (entry_save[0] == '\"') {
            strshift(entry_save, 1);
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == '\"' && entry_save[i-1] != '\\') {
                    break;
                }
                find_string[i] = entry_save[i];
            }
            strremove(entry_save, find_string);
            strshift(entry_save, 2);
        }
        else{
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == ' ') {
                    break;
                }
                find_string[i] = entry_save[i];
            }
            strremove(entry_save, find_string);
            strshift(entry_save, 1);
        }
        
        if (entry_save[0]=='-' && entry_save[1]=='-' && entry_save[2]=='f' && entry_save[3]=='i' && entry_save[4]=='l' && entry_save[5]=='e') {
            strshift(entry_save, 7);
            int white_space_for_path;
            char cat_path[1500] = "\0";
            
            //white-space for path string
            if(entry_save[0] == '\"'){
                for (int i=1;entry_save[i] != '\"'; i++) {
                    cat_path[i-1] = entry_save[i];
                }
                strremove(entry_save, cat_path);
                white_space_for_path=1;
            }
            else{
                for (int i=0 ; i<strlen(entry_save) ; i++) {
                    cat_path[i] = entry_save[i];
                    if (entry_save[i+1]==' ') {
                        break;
                    }
                }
                strremove(entry_save, cat_path);
                white_space_for_path=0;
            }
            char path[200] = "/Users/armin/Desktop";
            strcat(path, cat_path);
            
            //sorts the string to start with features...
            if (white_space_for_path) {
                strshift(entry_save, 3);
            }
            else{
                strshift(entry_save, 1);
            }
            
            int byword_activate = 0;
            int at_activate = 0;
            int all_activate = 0;
            int count_activate = 0;
            
            // see what features are activated
            if (strstr(entry_save, "-byword") != NULL) {
                byword_activate = 1;
                strremove(entry_save, "-byword");
            }
            if (strstr(entry_save, "-count") != NULL) {
                count_activate = 1;
                strremove(entry_save, "-count");
            }
            if (strstr(entry_save, "-all") != NULL) {
                all_activate = 1;
                strremove(entry_save, "-all");
            }
            if (strstr(entry_save, "-at") != NULL) {
                strremove(entry_save, "-at");
                strremove(entry_save, " ");
                at_activate = (int)myAtoi(entry_save);
            }
            
            //check features compatibility
            if (count_activate==0 && all_activate==0 && byword_activate==0 && at_activate==0) {
                find(path, find_string, 1, 0, 0, 0);
            }
            else if (count_activate==1 && all_activate==0 && byword_activate==0 && at_activate==0) {
                find(path, find_string, 0, 0, 0, 1);
            }
            else if (count_activate==0 && all_activate==0 && at_activate!=0){
                find(path, find_string, at_activate, 0, byword_activate, 0);
            }
            else if (count_activate==0 && all_activate==1 && at_activate==0){
                find(path, find_string, 0, 1, byword_activate, 0);
            }
            else
                printf("Invalid command combination\n");
            
        }
        else
            printf("Invalid Notation (probably on --file)\n");
             
    }
    else
        printf("Invalid Notation (probably on --str)\n");
    
}


void replace(char * str, char * substr , long int index){
    
    char save[1000] = "\0";
    long int checkpoint = 0;
    for (long int i=index; i<strlen(str); i++) {
        checkpoint = i;
        if(str[i] == ' ' || str[i] == '\n' || str[i] == EOF){
            break;
        }
    }
    for (long int i=checkpoint; i<strlen(str); i++) {
        save[i-checkpoint] = str[i];
    }
    str[index]= '\0';
    strcat(str, substr);
    strcat(str, save);
}


void get_data_for_replace(char * operator,char * entry_save){
    operator = strtok(NULL, " ");
    
    if (strcmp(operator, "--str1") == 0) {
        const char sub[] ="replace --str1 ";
        strremove(entry_save, sub);
        
        char find_string[1500] = "\0";
        
        //white-space for find string
        if (entry_save[0] == '\"') {
            strshift(entry_save, 1);
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == '\"' && entry_save[i-1] != '\\') {
                    break;
                }
                find_string[i] = entry_save[i];
            }
            strremove(entry_save, find_string);
            strshift(entry_save, 2);
        }
        else{
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == ' ') {
                    break;
                }
                find_string[i] = entry_save[i];
            }
            strremove(entry_save, find_string);
            strshift(entry_save, 1);
        }
        
        char insert_string[1500] = "\0";
        if (entry_save[0]=='-'  && entry_save[1]=='-'  && entry_save[2]=='s'  && entry_save[3]=='t'  && entry_save[4]=='r'  && entry_save[5]=='2' ) {
            strshift(entry_save, 7);
            if (entry_save[0] == '\"') {
                strshift(entry_save, 1);
                for (int i=0; i<strlen(entry_save); i++) {
                    if (entry_save[i] == '\"' && entry_save[i-1] != '\\') {
                        break;
                    }
                    insert_string[i] = entry_save[i];
                }
                strremove(entry_save, insert_string);
                strshift(entry_save, 2);
            }
            else{
                for (int i=0; i<strlen(entry_save); i++) {
                    if (entry_save[i] == ' ') {
                        break;
                    }
                    insert_string[i] = entry_save[i];
                }
                strremove(entry_save, insert_string);
                strshift(entry_save, 1);
            }
            
            if (entry_save[0]=='-'  && entry_save[1]=='-'  && entry_save[2]=='f'  && entry_save[3]=='i'  && entry_save[4]=='l'  && entry_save[5]=='e' ) {
                strshift(entry_save, 7);
                int white_space_for_path;
                char cat_path[1500] = "\0";
                
                //white-space for path string
                if(entry_save[0] == '\"'){
                    for (int i=1;entry_save[i] != '\"'; i++) {
                        cat_path[i-1] = entry_save[i];
                    }
                    strremove(entry_save, cat_path);
                    white_space_for_path=1;
                }
                else{
                    for (int i=0 ; i<strlen(entry_save) ; i++) {
                        cat_path[i] = entry_save[i];
                        if (entry_save[i+1]==' ') {
                            break;
                        }
                    }
                    strremove(entry_save, cat_path);
                    white_space_for_path=0;
                }
                char path[200] = "/Users/armin/Desktop";
                strcat(path, cat_path);
                
                //sorts the string to start with features...
                if (white_space_for_path) {
                    strshift(entry_save, 3);
                }
                else{
                    strshift(entry_save, 1);
                }
                
                int at_activate = 0;
                int all_activate = 0;

                // see what features are activated
                
                
                if (strstr(entry_save, "-all") != NULL) {
                    all_activate = 1;
                    strremove(entry_save, "-all");
                }
                if (strstr(entry_save, "-at") != NULL) {
                    strremove(entry_save, "-at");
                    strremove(entry_save, " ");
                    at_activate = (int)myAtoi(entry_save);
                }
                //open the file to copy the file to cache and string
                FILE *check_file = fopen(path, "r");
                if (check_file!=NULL) {
                    write_to_cache_file(check_file);
                    fclose(check_file);
                    FILE *cache_file = fopen(cache_path, "r");
                    
                    char file_string[10000] = "\0";
                    char ch;
                    for (int i=0; (ch = getc(cache_file)) != EOF ; i++) {
                        file_string[i] = ch;
                    }
                    
                    fclose(cache_file);
                    
                    int check_replace = 0;
                    
                    if ( all_activate==0 && at_activate==0) {
                        find(path, find_string, 1, 0, 0, 0);
                        if (found_characters[0] != -1) {
                            replace(file_string, insert_string, found_characters[0]);
                            check_replace = 1;
                        }
                    }
                    else if ( all_activate==0 && at_activate!=0){
                        find(path, find_string, at_activate, 0, 0, 0);
                        if (found_characters[at_activate-1] != -1) {
                            replace(file_string, insert_string, found_characters[at_activate-1]);
                            if (at_activate<=found_count_char_uni) {
                                check_replace = 1;
                            }
                        }

                    }
                    else if ( all_activate==1 && at_activate==0){
                        find(path, find_string, 0, 1, 0, 0);
                        
                        if (found_characters[0] != -1) {
                            int i=1;
                            while(found_characters[i]!=0){
                                i++;
                            }
                            while (i>0) {
                                i--;
                                replace(file_string, insert_string, found_characters[i]);
                            }
                            check_replace = 1;
                        }
                    }
                    else
                        printf("Invalid command combination\n");
                    
                    
                    if (check_replace) {
                        FILE *replace_file = fopen(path, "w");
                        fprintf(replace_file, "%s" , file_string);
                        fclose(replace_file);
                        printf("string was replaced succesfully\n");
                    }
                    else
                        printf("string couldn't br replaced\n");
                    
                    
                }
                else
                    printf("File doesn't exist or couldn't be opened\n");
                    fclose(check_file);
                
            }
            else
                printf("Invalid Notation (probably on --file)\n");
        }
        else
            printf("Invalid Notation (probably on --str2)\n");
        
        
    }
    else
        printf("Invalid Notation (probably on --str1)\n");
}


void get_compare(char * entry_save){
    strshift(entry_save, 8);
    int white_space_for_path;
    char path1[1500] = "\0";
    
    //white-space for path string
    if(entry_save[0] == '\"'){
        for (int i=1;entry_save[i] != '\"'; i++) {
            path1[i-1] = entry_save[i];
        }
        strremove(entry_save, path1);
        white_space_for_path=1;
    }
    else{
        for (int i=0 ; i<strlen(entry_save) ; i++) {
            path1[i] = entry_save[i];
            if (entry_save[i+1]==' ') {
                break;
            }
        }
        strremove(entry_save, path1);
        white_space_for_path=0;
    }
    char path_1[200] = "/Users/armin/Desktop";
    strcat(path_1, path1);
    
    //sorts the string to start with features...
    if (white_space_for_path) {
        strshift(entry_save, 3);
    }
    else{
        strshift(entry_save, 1);
    }
    char path2[1500] = "\0";
    
    //white-space for path string
    if(entry_save[0] == '\"'){
        for (int i=1;entry_save[i] != '\"'; i++) {
            path2[i-1] = entry_save[i];
        }
        strremove(entry_save, path1);
    }
    else{
        for (int i=0 ; i<strlen(entry_save) ; i++) {
            path2[i] = entry_save[i];
            if (entry_save[i+1]==' ') {
                break;
            }
        }
        strremove(entry_save, path1);
        white_space_for_path=0;
    }
    char path_2[200] = "/Users/armin/Desktop";
    strcat(path_2, path2);
    
    compare(path_1, path_2);
}


void get_indent(char * entry_save){
    strshift(entry_save, 12);
    int white_space_for_path;
    char path[150] = "\0";
    
    //white-space for path string
    if(entry_save[0] == '\"'){
        for (int i=1;entry_save[i] != '\"'; i++) {
            path[i-1] = entry_save[i];
        }
        strremove(entry_save, path);
        white_space_for_path=1;
    }
    else{
        for (int i=0 ; i<strlen(entry_save) ; i++) {
            path[i] = entry_save[i];
            if (entry_save[i+1]==' ') {
                break;
            }
        }
        strremove(entry_save, path);
        white_space_for_path=0;
    }
    char indent_path[200] = "/Users/armin/Desktop";
    strcat(indent_path, path);
    
    indent(indent_path);
}

int count_grep = 0;

void grep(char * find , int c_activate , int l_activate){
    char path[150];
    count_grep = 0;
    FILE *links_file = fopen(files_link_path, "r");
    FILE *grep_file = fopen(grep_path, "w");
    FILE *grepnames_file = fopen(grepnames_path, "w");
    while (readline(links_file, path, 150) != -1) {
        int check_name=0;
        int w = 0;
        char name[50]="\0";
        for (int i = 0; i<(int)strlen(path); i++) {
            if (path[i] == '/') {
                w = 0;
            }
            name[w] = path[i+1];
            w++;
        }
        
        FILE *file=fopen(path, "r");
        
        char string_line[300];

        while (readline(file, string_line, 300) != -1) {
            if ( strstr(string_line, find) != NULL) {
                count_grep ++;
                check_name=1;
                fprintf(grep_file, "%s: %s\n",name,string_line);
            }
        }
        if (check_name) {
            fprintf(grepnames_file, "%s\n",name);
        }
    }
    fclose(grep_file);
    fclose(grepnames_file);
    
    
    if (c_activate == 0 && l_activate == 0) {
        FILE *grep_file_read = fopen(grep_path, "r");
        char x;
        while ((x = getc(grep_file_read))!= EOF) {
            printf("%c",x);
        }
        printf("\n");
        fclose(grep_file_read);
    }
    else if (c_activate==1 && l_activate == 0) {
        printf("%d\n",count_grep);
    }
    else if(c_activate==0 && l_activate == 1){
        FILE *grepnames_file_read = fopen(grepnames_path, "r");
        char x;
        while ((x = getc(grepnames_file_read))!= EOF) {
            printf("%c",x);
        }
        printf("\n");
        fclose(grepnames_file_read);
    }
    else
        printf("Invalid command combination\n");
    
    
    fclose(links_file);
}

int c_activate = 0;
int l_activate = 0;

void get_grep(char * operator ,char * entry_save){
    operator = strtok(NULL, " ");
    c_activate = 0;
    l_activate = 0;
    int q=0;
    int mode_string = 0;
    
    if (strcmp(operator, "-c") == 0) {
        c_activate = 1;
        q=1;
    }
    else if (strcmp(operator, "-l") == 0){
        l_activate = 1;
        q=1;
    }
    
    if (strcmp(operator, "--str") == 0 && q ==0 ){
        mode_string = 1;
        strshift(entry_save, 11);
    }
    else if(q==1){
        operator = strtok(NULL, " ");
        if (strcmp(operator, "--str") == 0 ){
            mode_string = 1;
            strshift(entry_save, 14);
        }
    }
    if (mode_string) {
        char find_string[1500] = "\0";
        
        //white-space for find string
        if (entry_save[0] == '\"') {
            strshift(entry_save, 1);
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == '\"' && entry_save[i-1] != '\\') {
                    break;
                }
                find_string[i] = entry_save[i];
            }
            strremove(entry_save, find_string);
            strshift(entry_save, 2);
        }
        else{
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == ' ') {
                    break;
                }
                find_string[i] = entry_save[i];
            }
            strremove(entry_save, find_string);
            strshift(entry_save, 1);
        }
        
        if (entry_save[0]=='-' && entry_save[1]=='-' && entry_save[2]=='f' && entry_save[3]=='i' && entry_save[4]=='l' && entry_save[5]=='e'&& entry_save[6]=='s'){
            strshift(entry_save, 8);
            FILE *links_file = fopen(files_link_path, "w");
            int link_check = 0;
            while (entry_save[0] != '\0') {
                link_check = 1;
                int white_space_for_path;
                char cat_path[1500] = "\0";
                
                //white-space for path string
                if(entry_save[0] == '\"'){
                    for (int i=1;entry_save[i] != '\"'; i++) {
                        cat_path[i-1] = entry_save[i];
                    }
                    strremove(entry_save, cat_path);
                    white_space_for_path=1;
                }
                else{
                    for (int i=0 ; i<strlen(entry_save) ; i++) {
                        cat_path[i] = entry_save[i];
                        if (entry_save[i+1]==' ') {
                            break;
                        }
                    }
                    strremove(entry_save, cat_path);
                    white_space_for_path=0;
                }
                char path[200] = "/Users/armin/Desktop";
                strcat(path, cat_path);
                
                //sorts the string to start with features...
                if (white_space_for_path) {
                    strshift(entry_save, 3);
                }
                else{
                    strshift(entry_save, 1);
                }
                
                fprintf(links_file, "%s" , path);
                fprintf(links_file, "%c" , '\n');

            }
            fclose(links_file);
            
            if(link_check){
                printf("links submitted\n");
                grep(find_string , c_activate , l_activate);
            }
            else{fclose(links_file);
                printf("no links were submitted\n");
            }
            
        }
        else
            printf("Invalid notation (prabablly on --files)\n");
    }
    else
        printf("Invalid notation (prabablly on --str)\n");
    
}


void get_undo(char * operator ,char * entry_save){
    operator = strtok(NULL, " ");
    
    if (strcmp(operator, "--file") == 0) {
        const char sub[] ="undo --file ";
        strremove(entry_save, sub);
        char * undo_path = strtok(entry_save, "\"");
        char path[200] = "/Users/armin/Desktop";
        strcat(path, undo_path);
        
        FILE *check_file = fopen(path, "r");
        if (check_file != NULL) {
            
            //backup cache
            FILE *cache_file = fopen(cache_path, "r");
            write_to_cache_backup_file(cache_file);
            fclose(cache_file);
            //backup file
            write_to_cache_file(check_file);
            fclose(check_file);
            
            FILE *backup_file = fopen(cache_backup_path, "r");
            FILE *undo_file = fopen(path, "w");
            char x;
            while ((x=fgetc(backup_file)) != EOF) {
                fprintf(undo_file, "%c" ,x);
            }
            fclose(backup_file);
            fclose(undo_file);
            
            printf("file was changed to past version\n");
        }
        else
            printf("file can't be opened \n");
    }
    else
        printf("Invalid Notation (probably on --file)\n");
}


void get_tree(char * entry_save){
    
    char *str = "/Users/armin/Desktop/root";
    FILE *tree_file = fopen(tree_path, "w");
    
    const char sub[] ="tree ";
    strremove(entry_save, sub);
    int depth = atoi(entry_save);
    
    if (depth == -1) {
        tree(100, 0, str,tree_file);
    }
    else if(depth > -1) {
        tree(depth, 0, str,tree_file);
    }
    else
        printf("depth can't be smaller than -1");
    
    fclose(tree_file);
}

void arman(char * operator , char * entry_save){
    
    char input[1000] = "\0";

    
    for (int i=0; i<strlen(entry_save); i++) {
        if (entry_save[i+1] == '=' && entry_save[i+2] == 'D') {
            input[i] = '\0';
            break;
        }
        else
            input[i] = entry_save[i];
    }
    strshift(entry_save, (int)strlen(input)+4);
    
    if (entry_save[0] == '-' && entry_save[1] == '-' && entry_save[2] == 'f' && entry_save[3] == 'i' && entry_save[4] == 'l' && entry_save[5] == 'e') {
        strshift(entry_save, 7);
        int white_space_for_path;
        char cat_path[1500] = "\0";
        
        //make path even with spaces
        if(entry_save[0] == '\"'){
            for (int i=1;entry_save[i] != '\"'; i++) {
                cat_path[i-1] = entry_save[i];
            }
            strremove(entry_save, cat_path);
            white_space_for_path=1;
        }
        else{
            for (int i=0 ; i<strlen(entry_save) ; i++) {
                cat_path[i] = entry_save[i];
                if (entry_save[i+2]=='-' && entry_save[i+3]=='-' ) {
                    break;
                }
            }
            strremove(entry_save, cat_path);
            white_space_for_path=0;
        }
        //sorts the string to start with --str...
        if (white_space_for_path) {
            strshift(entry_save, 3);
        }
        else{
            strshift(entry_save, 1);
        }
        char path[200] = "/Users/armin/Desktop";
        strcat(path, cat_path);
        
        if (entry_save[0]=='-' && entry_save[1]=='-' && entry_save[2]=='p' && entry_save[3]=='o' && entry_save[4]=='s') {
            strshift(entry_save, 6);
            char line_no[5] = "\0";
            //char inline_no[5] = "\0";
            for (int i=0; i<strlen(entry_save); i++) {
                if (entry_save[i] == ':') {
                    break;
                }
                line_no[i] = entry_save[i];
            }
            
            //strremove(entry_save, line_no);
            strshift(entry_save, (int)strlen(line_no));
            if (entry_save[0] == ':') {
                strshift(entry_save, 1);
                int line_pos = (int)myAtoi(line_no);
                int inline_pos = (int)myAtoi(entry_save);
                
                //start operations in order to get output
                if (strcmp(operator, "cat") == 0) {//
                    get_operation_cat(operator,input);
                    insert(path, cat_string, line_pos, inline_pos);
                    printf("Arman did his job perfectly\n");
                }
                else if (strcmp(operator, "find") == 0) {//
                    get_data_for_find(operator, input);
                    FILE *file_find =fopen(findfile_path, "r");
                    char z;
                    char string_find[1000] = "\0";
                    for (int i=0; (z = fgetc(file_find)) != EOF; i++) {
                        string_find[i] = z;
                    }
                    
                    fclose(file_find);
                    insert(path, string_find, line_pos, inline_pos);
                    printf("Arman did his job perfectly\n");
                }
                else if (strcmp(operator, "tree") == 0){//
                    get_tree(input);
                    FILE *file_tree =fopen(tree_path, "r");
                    char z;
                    char string_tree[1000] = "\0";
                    for (int i=0; (z = fgetc(file_tree)) != EOF; i++) {
                        string_tree[i] = z;
                    }
                    
                    fclose(file_tree);
                    insert(path, string_tree, line_pos, inline_pos);
                    printf("Arman did his job perfectly\n");
                }
                else if (strcmp(operator, "grep") == 0){//
                    get_grep(operator ,input);
                    if (c_activate == 0 && l_activate == 0) {
                        FILE *file_grep = fopen(grep_path, "r");
                        char z;
                        char string_grep[1000] = "\0";
                        for (int i=0; (z = fgetc(file_grep)) != EOF; i++) {
                            string_grep[i] = z;
                        }
                        printf("\n");
                        fclose(file_grep);
                        insert(path, string_grep, line_pos, inline_pos);
                        printf("Arman did his job perfectly\n");
                    }
                    else if (c_activate==1 && l_activate == 0) {
                        char str[10] = "\0";
                        sprintf(str,"%d", count_grep);
                        insert(path, str, line_pos, inline_pos);
                        printf("Arman did his job perfectly\n");
                    }
                    else if(c_activate==0 && l_activate == 1){
                        FILE *file_grep = fopen(grepnames_path, "r");
                        char z;
                        char string_grep[1000] = "\0";
                        for (int i=0; (z = fgetc(file_grep)) != EOF; i++) {
                            string_grep[i] = z;
                        }
                        printf("\n");
                        fclose(file_grep);
                        insert(path, string_grep, line_pos, inline_pos);
                        printf("Arman did his job perfectly\n");
                    }
                }
            }
            else
                printf("Invalid Notation (probably on <line>:<pos>)\n");
        }
        else
            printf("Invalid Notation (probably on --pos)\n");
    }
    else
        printf("Invalid notation (probablly on --file\n");
}


int main() {
    
    char entry[1000];
    while(1){
        scanf("%[^\n]%*c", entry);
        
        char entry_save[1000] = "\0";
        for (int i=0; i< strlen(entry); i++) {
            entry_save[i] = entry[i];
            
        }
        
        char * operator = strtok(entry, " ");
        
        if (strstr(entry_save, " =D ") != NULL) {
            arman(operator,entry_save);
        }
        else if (strcmp(operator, "createfile") == 0) {
            get_operation_createfile(operator,entry_save);
        }
        else if (strcmp(operator, "cat") == 0) {//
            get_operation_cat(operator,entry_save);
        }
        else if (strcmp(operator, "insertstr") == 0) {//
            get_data_for_insert(operator, entry_save);
        }
        else if (strcmp(operator, "removestr") == 0) {
            get_data_for_remove(operator, entry_save);
        }
        else if (strcmp(operator, "copystr") == 0) {//
            get_data_for_copy(operator, entry_save);
        }
        else if (strcmp(operator, "cutstr") == 0) {//
            get_data_for_cut(operator, entry_save);
        }
        else if (strcmp(operator, "find") == 0) {//
            get_data_for_find(operator, entry_save);
        }
        else if (strcmp(operator, "replace") == 0){
            get_data_for_replace(operator,entry_save);
        }
        else if (strcmp(operator, "tree") == 0){//
            get_tree(entry_save);
        }
        else if (strcmp(operator, "compare") == 0){//
            get_compare(entry_save);
        }
        else if (strcmp(operator, "auto-indent") == 0){//
            get_indent(entry_save);
        }
        else if (strcmp(operator, "undo") == 0){//
            get_undo(operator ,entry_save);
        }
        else if (strcmp(operator, "grep") == 0){//
            get_grep(operator ,entry_save);
        }
        else if (strcmp(operator, "exit") == 0){
            return 0;
        }
        else
            printf("Invalid Command\n");
    }
    
    return 0;
}
