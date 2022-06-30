#include<string.h>
#include<stdio.h>
#include <stdlib.h>

#define   MAX_LENGTH_OF_SYMBOL_TABLE 100            
#define   MAX_LENGTH_OF_WORD  100        
       

char cur_w =' ';             
int row_num=1,char_num=0;   


char KT[][10]={"int","char","float","main","double","case", "for","if","auto","else","do","while","void","static", "return","break","struct","const","union","switch","typedef","enum"};
char PT[][10]={">=","<=","==",">","<","=","+","-","*","/","{","}","(",")","[", "]" , ",",";", "++","--", "&","!="};

int cur_index = (sizeof(KT)+sizeof(PT))/sizeof(KT[0]);

typedef struct{
	char* str;
	int index;
} Token;

Token IT[MAX_LENGTH_OF_SYMBOL_TABLE];
Token CHART[MAX_LENGTH_OF_SYMBOL_TABLE];
Token CONSTT[MAX_LENGTH_OF_SYMBOL_TABLE];
Token ST[MAX_LENGTH_OF_SYMBOL_TABLE];


int IT_index=0,CHART_index=0,CONSTT_index=0,ST_index=0;

int is_keyword(char * str);
int is_old_symbol(char * str,Token * table,int * index);
int is_in_PT(char * str);
int is_letter(char c);
int is_digit(char c);
int analyzer(FILE *f_pin,FILE *f_pout);


int main(){
   char in_fn[25]="demo.c",out_fn[25]="token.txt";
   FILE * fpin,* fpout;  
   printf("Please enter a file name (default is dome.c):");
   int i=0;
   char temp;
   while((temp=getchar())!='\n'){
	   if(i==0){
		   int j=0;
		   while (in_fn[j]!=0)
		   {
			   in_fn[j]=0;
		   }
		   
	   }
	   in_fn[i]=temp;
	   i++;
   }
   printf("%s\n",in_fn);
   printf("Please enter a save file name (default is token.txt):");
   i=0;
   while((temp=getchar())!='\n'){
	   if(i==0){
		   int j=0;
		   while (out_fn[j]!=0)
		   {
			   out_fn[j]=0;
		   }
		   
	   }
	   
	   out_fn[i]=temp;
	   i++;
   }
   printf("%s\n",out_fn);
   fpin=fopen(in_fn,"r");
   fpout=fopen(out_fn,"w");

   int result = analyzer(fpin,fpout);
   fclose(fpin);
   fclose(fpout);
   if(result==1){
		printf("done!\n");
   }else{
	    printf("error in %d row %d character !\n",row_num,char_num);
   }

	
   printf("\nSYMBOL TABLE\n");
   printf("IT\n");
   for(int i=0;i<IT_index;i++){
	   printf("symbol:%s   token:%d\n",IT[i].str,IT[i].index);
   }
   printf("\nST\n");
   for(int i=0;i<ST_index;i++){
	   printf("symbol:%s   token:%d\n",ST[i].str,ST[i].index);
   }
   printf("\nCHART\n");
   for(int i=0;i<CHART_index;i++){
	   printf("symbol:%s   token:%d\n",CHART[i].str,CHART[i].index);
   }
   printf("\nCONSTT\n");
   for(int i=0;i<CONSTT_index;i++){
	   printf("symbol:%s   token:%d\n",CONSTT[i].str,CONSTT[i].index);
   }

  
   printf("\nplease enter any key to exit.");
   scanf("%c",&temp);
   return 0;
}




int is_keyword(char * str){
   int i;
   for(i=0;i<sizeof(KT)/sizeof(KT[0]);i++){
     if((strcmp(str,KT[i]))==0){
		 return i;
	 }
   }
	
	return -1;
}

int is_in_PT(char * str){
   int i;
   for(i=0;i<sizeof(PT)/sizeof(PT[0]);i++){
     if((strcmp(str,PT[i]))==0){
		 return i+sizeof(KT)/sizeof(KT[0]);
	 }
   }
	
	return -1;
}

int is_old_symbol(char * str,Token * table,int* index){
   int i;
   for(i=0;i<(*index);i++){
     if((strcmp(str,table[i].str))==0){
		 return i;
	 }
   }
	table[(*index)].str = (char *) malloc(sizeof(str));
	strcpy(table[(*index)].str,str);
	table[(*index)].index = ++cur_index;
	(*index)++;
	return cur_index;
}


int is_letter(char c)
{
   if(((c<='z')&&(c>='a'))||((c<='Z')&&(c>='A'))) 
		return 1;
    else
		return 0;
}


int is_digit(char c){
    if(c>='0'&&c<='9') 
   		return 1;
    else 
   		return 0;
}


int analyzer(FILE *fpin,FILE *fpout){

    char cur_word[MAX_LENGTH_OF_WORD];
    int i=0;

    while( (cur_w=fgetc(fpin)) !=EOF){
		char_num++;
		if(cur_w==' '||cur_w=='\t'){

		}
		else if(cur_w=='\n'){
			row_num++;
			char_num=0;
		}
		else if(is_letter(cur_w)){
			while(is_letter(cur_w) | is_digit(cur_w) | cur_w=='_'){
				if((cur_w<='Z')&&(cur_w>='A'))     
					cur_w=cur_w+32;   //ignore case
					cur_word[i]=cur_w;
					i++;
					cur_w=fgetc(fpin);
					char_num++;
				}
				//go back to the previous character
				fseek(fpin,-1L,SEEK_CUR);
				char_num--;
				cur_word[i]='\0';
				i=0;
				int keyword_num = is_keyword(cur_word);

				if (keyword_num!=-1){   
					fprintf(fpout,"<%d>,",keyword_num);
				}else{
					fprintf(fpout,"<%d>,",is_old_symbol(cur_word,IT,&IT_index));
				} 
		}
		else if(is_digit(cur_w)){
			while(is_digit(cur_w)|cur_w=='.'){
				cur_word[i]=cur_w;
				i++;
				cur_w=fgetc(fpin);	
				char_num++;  
			}
			//go back to the previous character
			fseek(fpin,-1L,SEEK_CUR);
			char_num--;
			cur_word[i]='\0';
			i=0;
			fprintf(fpout,"<%d>,",is_old_symbol(cur_word,CONSTT,&CONSTT_index));
		}
		else if(cur_w=='"'){
			do{
				cur_word[i]=cur_w;
				i++;
				cur_w=fgetc(fpin);
				char_num++;
			}while(cur_w!='"');
			
			if(cur_w=='"'){
				cur_word[i]='"';
				cur_word[++i]='\0';
				i=0;
				fprintf(fpout,"<%d>,",is_old_symbol(cur_word,ST,&ST_index));
			}else{
				return -1;
			}
			
		}
		else if(cur_w=='\''){
			do{
				cur_word[i]=cur_w;
				i++;
				cur_w=fgetc(fpin);
				char_num++;
			}while(cur_w!='\'');
			if(i==2){
				cur_word[i]='\'';
				cur_word[++i]='\0';
				i=0;
				fprintf(fpout,"<%d>,",is_old_symbol(cur_word,CHART,&CHART_index));
			}else{
				return -1;
			}
			
		}
		else{
			int is_pt;
			cur_word[i]=cur_w;
			cur_w=fgetc(fpin);
			if(cur_w==EOF){
				fseek(fpin,-1L,SEEK_CUR);
				cur_word[++i]='\0';
				is_pt = is_in_PT(cur_word);
				if(is_pt>-1){
					fprintf(fpout,"<%d>,",is_pt);
					i=0;
					return 1;
				}else{
					return -1;
				}
			}
			char_num++;
			cur_word[++i]=cur_w;
			cur_word[++i]='\0';
			is_pt = is_in_PT(cur_word);
			if(is_pt>-1){
				fprintf(fpout,"<%d>,",is_pt);
				i=0;
			}else{
				fseek(fpin,-1L,SEEK_CUR);
				char_num--;
				cur_word[--i]='\0';
				is_pt = is_in_PT(cur_word);
				if(is_pt>-1){
					fprintf(fpout,"<%d>,",is_pt);
					i=0;
				}else{
					return -1;
				}

			}
			
		}
	}
	return 1;
		 
}
