#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

//Node structure type for a binomial heap with max heap property.
typedef struct node{
	int key;
	struct node *parent;
	struct node *child;
	struct node *sibling;
	int degree;
	char name[50];
	char content[1000000];
}node;

//Creating Heap.
node* CreateHeap(){
	node *hdr=NULL;
	return hdr;
}

//Creating node with given key parameter.
node* CreateNode(int key){
	node *new_node=(node*)malloc(sizeof(node));
	new_node->key=key;
	return new_node;
}

//Links two BTs and creates new BT. 
void BH_Link(node *y,node *z){
	y->parent=z;
	y->sibling=z->child;
	z->child=y;
	z->degree++;
}

//Sorts root's of binomial heaps that are taken as parameters.
node* BH_Sort(node *hdr_1, node *hdr_2) {
    
	node* hdr =CreateHeap();
    node* temp1;
    node* temp2;
    
    if (hdr_1!=NULL && hdr_2!=NULL ) {
        if (hdr_1->degree <= hdr_2->degree)
            hdr = hdr_1;
        else if (hdr_1->degree > hdr_2->degree)
            hdr = hdr_2;
   }  
	else if(hdr_1!=NULL)   
        hdr = hdr_1;
    else 
        hdr = hdr_2;
    
	while (hdr_1 != NULL && hdr_2 != NULL) {
        if (hdr_1->degree < hdr_2->degree) {
            hdr_1 = hdr_1->sibling;
        } 
		else if (hdr_1->degree == hdr_2->degree) {
            temp1 = hdr_1->sibling;
            hdr_1->sibling = hdr_2;
            hdr_1 = temp1;
        } 
		else {
            temp2 = hdr_2->sibling;
            hdr_2->sibling = hdr_1;
            hdr_2 = temp2;
        }
    }
    return hdr;
}

//Merges the sorted BTs in BH_Sort function and creates new binomial heap. 
node* BH_Merge(node *hdr_1, node *hdr_2){
	
	node *hdr = BH_Sort(hdr_1,hdr_2);
	
	if(hdr == NULL)
	  return hdr;
	  	
	node *prev = NULL;
	node *x=hdr;
	node *next=x->sibling;
	
    while(next != NULL){
    	if ((x->degree != next->degree) || ((next->sibling != NULL) && (next->sibling)->degree == x->degree)) {
            prev=x;
            x=next;
        } 
		else{
            if (x->key >= next->key){
                x->sibling = next->sibling;
                BH_Link(next, x);
            } 
			else{
                if (prev == NULL)
                    hdr = next;
                else
                    prev->sibling = next;
				
				BH_Link(x, next);
                x = next;
            }
        }
        next= x->sibling;
    }
    return hdr;
}

//Insert a given node to given BH.
node* BH_Insert(node *hdr , node *newNode){	
	node *hdr_1 = CreateHeap();
	newNode->child=NULL;
	newNode->parent=NULL;
	newNode->sibling = NULL;
	newNode->degree = 0;
	hdr_1 = newNode;
	hdr = BH_Merge(hdr,hdr_1);	
	return hdr;
}

//This pre variable is also used in the BH_DeleteMax function after the FindMax funtion so as to it is global.
node *pre=NULL;
//Finding maximum node.
node *BH_FindMax(node *hdr){
	int max;
	max=hdr->key;
	node *x=hdr;
	node *maxNode=NULL;
	while(x->sibling!= NULL){
		if((x->sibling)->key > max){
		   max=(x->sibling)->key;	
			pre=x;
			maxNode=x->sibling;	
		}
		else
		   maxNode=x;    	
		x=x->sibling;
	}
	return maxNode;
}

/*This function inverts the children of deleted maximum node.This funtion is not necessary but it exits in this 
progrom for convenience.*/
node* BH_Invert(node* hdr) {
	if (hdr->sibling != NULL) {
       BH_Invert(hdr->sibling);
       (hdr->sibling)->sibling = hdr;
    } 
	else 
        return hdr; 
}

//Deleting the node including maximum key.
node* BH_DeleteMax(node* hdr) {
	
    node *hdr_1=NULL;
	node *hdr_2=NULL; 
	node *maxNode=BH_FindMax(hdr);
	
	if(pre==NULL){
		maxNode->sibling=hdr_2;
      if (maxNode->child != NULL) {
        hdr_1=BH_Invert(maxNode->child);
        (maxNode->child)->sibling = NULL;
      } 
	  else	
		 hdr_1=NULL;
	  hdr=BH_Merge(hdr_1,hdr_2);
	  return hdr;
	}
	
	else if(pre!=NULL && maxNode->sibling!=NULL){
		if (maxNode->child != NULL) {
          hdr_1=BH_Invert(maxNode->child);
          (maxNode->child)->sibling = NULL;
          node *temp=hdr_1;
		  while(temp->sibling!=NULL){
        	temp = temp->sibling;
		  }
		  temp->sibling=maxNode->sibling;     
	   } 
	   else
	      hdr_1=maxNode->sibling;
	   pre->sibling=NULL;
	   hdr_2=hdr;
	   hdr=BH_Merge(hdr_1,hdr_2);
	   return hdr;
	}
	else {
	    if (maxNode->child != NULL) {
          hdr_1=BH_Invert(maxNode->child);
          (maxNode->child)->sibling = NULL;
        } 
        else 
          hdr_1=NULL;  
        pre->sibling=NULL;
		hdr_2=hdr;
		hdr=BH_Merge(hdr_1,hdr_2);	
	    return hdr;
    } 
}

//Main function.
int main(void){

//Opening the folder named files.This folder should be in the same place with this program file.
DIR *folder;
folder = opendir("files");
struct dirent *entry;

if(folder == NULL){
	printf("Not proper folder to reading");
	exit(1);	
}   

else {
  
  char keyword[20];
  printf("Please enter the keyword: ");
  scanf("%s", &keyword);
  
  printf("Please enter the number of relevant documents you would like: ");
  int numberofRelevantDoc;
  scanf("%d", &numberofRelevantDoc);
  
  FILE *file;
  int simsc;	
  char fileLocation[50];
  int i =1;
  char word[100];
  node *maxNodes[numberofRelevantDoc];
  node *BH=CreateHeap();
  node *node;
  char content[1000000]; 
  
  while((entry=readdir(folder))){		
	if(i>2){
	  strcpy(fileLocation,"files//");
	  strcat(fileLocation,entry->d_name);
	  file = fopen(fileLocation,"r");
	  if(file==NULL)
      printf("File could not be opened!");
      else{
	    while(!feof(file)) {  
	      fscanf(file,"%s",word);
	      strcat(content,word);
	      strcat(content," ");
//strstr funtion is used for compare because some words in the documents have characters like "," , "." etc. at their endings.       
		  if(strstr(word,keyword)!=NULL)  
         	simsc++;	      
	    }
		node=CreateNode(simsc); 
	    strcpy(node->name,entry->d_name);
	    strcpy(node->content,content);
	    strcpy(content,"");
		BH=BH_Insert(BH,node);		
		simsc=0;
	  } 
	}
    i++;
  }	
  int j;
  printf("The relevance order is: ");
  for(j=0;j<numberofRelevantDoc;j++){
  	maxNodes[j]=BH_FindMax(BH);
  	if(maxNodes[j]->key==0){
  	  printf("\nThere is no another document including given keyword.");
	  numberofRelevantDoc=j;
	  break;
    }
	printf("%s(%d) ",maxNodes[j]->name,maxNodes[j]->key);
	BH=BH_DeleteMax(BH);
  }
  printf("\n\n");
  for(j=0;j<numberofRelevantDoc;j++){
    printf("%s(%d): %s\n\n\n",maxNodes[j]->name,maxNodes[j]->key,maxNodes[j]->content);
  }
}
return 0;
}
