int main(){
   char in_fn[25]="demo.c",out_fn[25]="token.txt";
   FILE * fpin,* fpout;  
   printf("Please enter a file name (default is dome.c):");
   int i=0;
   char temp;
   while((temp=getchar())!='n'){
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
   while((temp=getchar())!='n'){
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

   int result = analyse(fpin,fpout);
   fclose(fpin);
   fclose(fpout);
   if(result==1){
		printf("done!\n");
   }else{
	    printf("error in %d row %d character !\n",row_num,char_num);
   }
  
   printf("please enter any key to exit.");
   scanf("%c",&temp);
   return 0;
}