#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef NDEBUG
#define EXIT_IF_NULL(token_name,type)\
if(!token_name){\
    fprintf(stderr,"Unable to get pointer of type '" #type "' for token '" #token_name "' at line %d for file %s. Exiting program.\n",__LINE__,__FILE__);\
    exit(EXIT_FAILURE);\
}((void)0)
#else
#define EXIT_IF_NULL(token_name,type) ((void)0)
#endif
_Pragma("GCC diagnostic push")
_Pragma("GCC diagnostic ignored \"-Wstringop-truncation\"")
void replace_str(char** strptr, const char* replace, const char* with){
    char* new_strptr=(char*)(malloc(sizeof(char)));
    EXIT_IF_NULL(new_strptr,char*);
    size_t strptr_i=0;
    size_t new_strptr_i=0;
    const size_t replace_len=strlen(replace);
    const size_t with_len=strlen(with);
    char current_char;
    char* replace_p;
    while((current_char=*(*strptr+strptr_i))){
        if(!(replace_p=strchr(*strptr+strptr_i,*replace))) break;
        const size_t non_match_len=replace_p-*strptr-strptr_i;
        new_strptr=realloc(new_strptr,sizeof(char)*(new_strptr_i+non_match_len));//Strings that don't match replace.
        EXIT_IF_NULL(new_strptr,char*);
        strncpy(new_strptr+new_strptr_i,*strptr+strptr_i,non_match_len);
        new_strptr_i+=non_match_len;
        strptr_i+=non_match_len;
        if(strncmp(*strptr+strptr_i,replace,replace_len)){//First letter may not match the same word (!=0)
            new_strptr=realloc(new_strptr,(sizeof(char)*new_strptr_i+1));//To stop strchr from looping, add first character.
            EXIT_IF_NULL(new_strptr,char*);
            new_strptr[new_strptr_i++]=(*strptr)[strptr_i++];
            continue; 
        }
        new_strptr=realloc(new_strptr,sizeof(char)*(new_strptr_i+with_len));
        EXIT_IF_NULL(new_strptr,char*);
        strncpy(new_strptr+new_strptr_i,with,with_len);
        new_strptr_i+=with_len;//Increment string index by appropriate lengths.
        strptr_i+=replace_len;
    }
    const size_t last_chars_len=strchr(*strptr+strptr_i,'\0')-*strptr-strptr_i;
    new_strptr=realloc(new_strptr,sizeof(char)*(new_strptr_i+last_chars_len+1));
    EXIT_IF_NULL(new_strptr,char*);
    strcpy(new_strptr+new_strptr_i,*strptr+strptr_i);//Null-terminate with strcpy.
    free(*strptr);
    *strptr=new_strptr;//Change freed pointer to new pointer.
}
_Pragma("GCC diagnostic pop")
char* three_string_join(const char* str1,const char* str2,const char* str3){
    char* this=malloc(sizeof(char)*(strlen(str1)+strlen(str2)+strlen(str3)+1));
    EXIT_IF_NULL(this,char*);
    strcpy(this,str1); strcat(this,str2); strcat(this,str3);
    return this;
}
void add_begin(char** strptr,const char* begin_str){
    char* new_strptr=malloc(sizeof(char)*(strlen(*strptr)+strlen(begin_str)+1));
    EXIT_IF_NULL(new_strptr,char*);
    strcpy(new_strptr,begin_str);
    strcat(new_strptr,*strptr);
    free(*strptr);
    *strptr=new_strptr;
}
void add_end(char** strptr,const char* end_str){
    char* new_strptr=malloc(sizeof(char)*(strlen(*strptr)+strlen(end_str)+1));
    EXIT_IF_NULL(new_strptr,char*);
    strcpy(new_strptr,*strptr);
    strcat(new_strptr,end_str);
    free(*strptr);
    *strptr=new_strptr;
}
_Bool remove_begin_if(char** strptr,const char* has_this){
    const size_t has_this_len=strlen(has_this);
    if(**strptr==has_this[0]&&!strncmp(*strptr,has_this,has_this_len)){
        char* new_strptr=malloc(sizeof(char)*(strlen(*strptr)-has_this_len+1));
        EXIT_IF_NULL(new_strptr,char*);
        strcpy(new_strptr,(*strptr)+has_this_len);
        free(*strptr);
        *strptr=new_strptr;
        return 1;
    }
    return 0;
}
_Bool remove_end_if(char** strptr,const char* has_this){
    const size_t has_this_len=strlen(has_this),strptr_len=strlen(*strptr);
    if(*(*strptr+strptr_len-has_this_len)==has_this[0]&&!strncmp(*strptr+strptr_len-has_this_len,has_this,has_this_len)){
        *strptr=realloc(*strptr,sizeof(char)*(strptr_len-has_this_len+1));
        (*strptr)[strptr_len-has_this_len]='\0';
        return 1;
    }
    return 0;
}
char* read_file(const char* file_input){
    FILE* f_obj;
    char* f_str;
    f_obj=fopen(file_input,"r");
    if(!f_obj) return 0;
    fseek(f_obj,0,SEEK_END);
    size_t str_len=ftell(f_obj);
    rewind(f_obj);
    f_str=malloc(sizeof(char)*(str_len+1));//To include '\0'
    EXIT_IF_NULL(f_str,char*);
    fread(f_str,str_len,1,f_obj);
    f_str[str_len]='\0';
    fclose(f_obj);
    return f_str;
}
_Bool write_file(const char* file_output,const char* str_output){
    FILE* f_obj;
    f_obj=fopen(file_output,"w");
    if(!f_obj) return 0;
    size_t wrote=fwrite(str_output,strlen(str_output),1,f_obj);
    fclose(f_obj);
    return (_Bool)wrote;
}
int main(int argc,char** argv){
    if(argc!=5&&argc!=6){
        fprintf(stderr,"Usage: Appends characters from beginning to end of a file for use with other programs.\n"
            "Outputs to same file name appending with .output\n"
            "Syntax: input_file_name -(a/r) -bBeginCharacters -eEndingCharacters -BE(0/1)(0/1)\n"
            "Meaning: a/r means add or remove characters at beginning and end for each new line.\n"
            "-b -e means the characters to add/remove from the beginning/ending surrounded by quotes.\n"
            "-b and -e can also be empty. -BE means to append characters from the\n"
            "(B)eginning and (E)nding of file (Default 11 = true true)\n"
            "Example: glsl -a -b'\"' -e'\\n\"' adds \" to the beginning and \\n\" to the end for each line.\n"
            "-BE11 is the default, which also adds the characters at the beginning and ending of the file. It outputs the file to glsl.output\n"
        );
        return 1;
    }
    _Bool do_add,modify_at_begin=1,modify_at_end=1;
    if(strcmp("-a",argv[2])&&strcmp("-r",argv[2])){
        fprintf(stderr,"Second argument needs to be -a or -r.\n");
        return 1;
    }else if(!strcmp("-a",argv[2])) do_add=1;
    else do_add=0;
    if(argc==6){
        if(strncmp("-BE",argv[5],3)){
            fprintf(stderr,"Fifth argument needs to be -BE00, -BE01, -BE10, or -BE11.\n");
            return 1;
        }else if(strlen(argv[5])!=5){
            fprintf(stderr,"Fifth argument needs to be -BE00, -BE01, -BE10, or -BE11.\n");
            return 1;
        }
        switch(argv[5][3]){
            case '0': modify_at_begin=0; break;
            case '1': modify_at_begin=1; break;
            default: fprintf(stderr,"Fifth argument needs to be -BE00, -BE01, -BE10, or -BE11.\n"); return 1;
        }
        switch(argv[5][4]){
            case '0': modify_at_end=0; break;
            case '1': modify_at_end=1; break;
            default: fprintf(stderr,"Fifth argument needs to be -BE00, -BE01, -BE10, or -BE11.\n"); return 1;
        }
    }
    char* begin_str,* end_str,* file_name_str,* file_name_output_str,* file_str;
    if(strncmp("-b",argv[3],2)){
        fprintf(stderr,"Third argument needs to be -b\'chars\', where characters are surrounded by quotes (optional).\n"); return 1;
    }else if(strncmp("-e",argv[4],2)){
        fprintf(stderr,"Fourth argument needs to be -e\'chars\', where characters are surrounded by quotes (optional).\n"); return 1;
    }
    size_t begin_str_len=strchr(argv[3],'\0')-argv[3]-2,end_str_len=strchr(argv[4],'\0')-argv[4]-2;
    begin_str=malloc(sizeof(char)*(begin_str_len+1));
    end_str=malloc(sizeof(char)*(end_str_len+1));
    file_name_str=malloc(sizeof(char)*(strlen(argv[1])+1));
    file_name_output_str=malloc(sizeof(char)*(strlen(argv[1])+7+1));
    EXIT_IF_NULL(begin_str,char*);
    EXIT_IF_NULL(end_str,char*);
    EXIT_IF_NULL(file_name_str,char*);
    EXIT_IF_NULL(file_name_output_str,char*);
    strcpy(begin_str,argv[3]+2);
    strcpy(end_str,argv[4]+2);
    strcpy(file_name_str,argv[1]);
    strcpy(file_name_output_str,argv[1]);
    strcat(file_name_output_str,".output");
    printf("Options used for: %s - %s %s at the beginning and %s at the end at each line.\n"
        "The beginning of the file will%s be %s with %s and the ending of the file will%s be %s with %s.\n"
        "Outputting to %s.\n"
        ,file_name_str,do_add?"Appending":"Removing",begin_str,end_str
        ,modify_at_begin?"":" not",do_add?"appended":"removed",begin_str,modify_at_end?"":" not",do_add?"appended":"removed",end_str,file_name_output_str
    );
    file_str=read_file(file_name_str);
    if(!file_str){
        fprintf(stderr,"Error reading file %s. Closing program.\n",file_name_str);
        free(begin_str);
        free(end_str);
        free(file_name_str);
        free(file_name_output_str);
        return 1;
    }
    char* joined_chars=three_string_join(end_str,"\n",begin_str);
    if(do_add){
        replace_str(&file_str,"\n",joined_chars);
        if(modify_at_begin) add_begin(&file_str,begin_str);
        if(modify_at_end) add_end(&file_str,end_str);
    }else{
        replace_str(&file_str,joined_chars,"\n");
        if(modify_at_begin){
            _Bool rem_b_success=remove_begin_if(&file_str,begin_str);
            if(!rem_b_success) printf("Unable to find %s at beginning of file.\n",begin_str);
        }
        if(modify_at_end){
            _Bool rem_e_success=remove_end_if(&file_str,end_str);
            if(!rem_e_success) printf("Unable to find %s at end of file.\n",end_str);
        }
    }
    if(!write_file(file_name_output_str,file_str)){
       fprintf(stderr,"File not written at %s due to errors.\n",file_name_output_str);
    }
    free(file_str);
    free(joined_chars);
    free(begin_str);
    free(end_str);
    free(file_name_str);
    free(file_name_output_str);
    return 0;
}
