#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Question Question;
struct Question {
    // Variable that defines type of question :: 1 for True/False , 2 for Single Answer Or Multi Answer
    int type ;
    // setting max count of question text string
    char text[512];
    // array of answer (string) , for True False we use only emlemnts of array
    char answer_1[256];
    char answer_2[256];
    char answer_3[256];
    
    // Points for each answer , for True False we use only emlemnts of array 
    int points[3];
};

Question parseFileLine(char * question_line){
   Question question_to_return;

    char * question_text = strtok(question_line,":");
    char * question_type = strtok(NULL,":");
    // Cast string Number to int so we can use it better 
    int type = atoi(question_type);
    
    strncpy(question_to_return.text,question_text,sizeof(question_to_return.text));
    question_to_return.type = type;

    if(type==1){
        // if type of question == 1 , True False Question Than we don't need to read Answer Options
        char * true_points = strtok(NULL,":");
        char * false_points = strtok(NULL,":");

        question_to_return.points[0] = atoi(true_points);
        question_to_return.points[1] = atoi(false_points);
    }else {
        char * answer_1 = strtok(NULL,":");
        char * answer_2 = strtok(NULL,":");
        char * answer_3 = strtok(NULL,":");
        char * point_1 = strtok(NULL,":");
        char * point_2 = strtok(NULL,":");
        char * point_3 = strtok(NULL,"\n");;
        strncpy(question_to_return.answer_1,answer_1,sizeof(question_to_return.answer_1));
        strncpy(question_to_return.answer_2,answer_2,sizeof(question_to_return.answer_2));
        strncpy(question_to_return.answer_3,answer_3,sizeof(question_to_return.answer_3));        
        question_to_return.points[0] = atoi(point_1);
        question_to_return.points[1] = atoi(point_2);
        question_to_return.points[2] = atoi(point_3);
    }
    return question_to_return;
}

void printQuestionData(Question q){
    printf("%s ?\t",q.text);
    if(q.type==1){
        printf("\n\t1) true\n");
        printf("\t2) false\n");        
    }else {
        if(q.type==2){
            printf("\t(Single Answer Question)\n");
        }else {
            printf("\t(Multi Answer Question)\n");
        }
        printf("\t1) %s\n",q.answer_1);
        printf("\t2) %s\n",q.answer_2);    
        printf("\t3) %s\n",q.answer_3); 
    }
}


int main() {
    Question questions[512];
    FILE * myfile;
    printf("Questionaire Exam : \n");
    myfile = fopen("questionnaire.txt","r");
    if(myfile==NULL){
        printf("File Doesn't Exist please use Question Creater app to make one.");
        exit(1);
    }
    char firstName[256],familyName[256];

    // Reading Student Name 
    printf("Please Enter Your First Name :: ");
    fgets(firstName,sizeof(firstName),stdin);
    firstName[strcspn(firstName, "\n")] = 0;

    printf("Please Enter Your Family Name :: ");
    fgets(familyName,sizeof(familyName),stdin);
    familyName[strcspn(familyName, "\n")] = 0;


    printf("hellow %s Press Enter To continue to your exam \n",firstName);
    getchar();


    char fileName[128] = "Responses_";
    char * separter = "_";
    char * extension = ".txt";

    strcat(fileName,familyName);
    strcat(fileName,separter);
    strcat(fileName,firstName);
    strcat(fileName,extension);


    FILE * answersFile = fopen(fileName,"w");
    char question_line[1024]; 
    char question_line_clone[1024];
    int MaxScore=0;
    int studentSCore=0;

    // Reading Question File Line By Line 
    
    while ((fgets(question_line,1024,myfile))!=NULL)
    {
        // system("cls");
        printf("\nAnswer With 1 to 3 \n");
        strncpy(question_line_clone,question_line,sizeof(question_line));

        Question q = parseFileLine(question_line_clone);

        // Removing Last \n new line from string to use it again when writing to responses file
        question_line[strcspn(question_line, "\n")] = 0;


        printQuestionData(q);

        int answers[3];

        // Case True/False Question Or single Answer We only read One Answer 
        if(q.type==1 || q.type ==2){
            do{
                printf("Your Answer :: ");
                char buffer[8];
                fgets(buffer,sizeof(buffer),stdin);
                answers[0] = atoi(buffer);
            }while(answers[0]<1 && answers[0]>3);

            studentSCore += q.points[answers[0]-1];

            for(int i=0;i<3;i++){
                if(q.points[i]>0){
                    MaxScore += q.points[i]; 
                    if(answers[0]==(i+1)){
                        printf("You get it Right \t ");
                    }else {
                        printf("You get it Wrong \t ");                    
                    }
                    printf("The Right Answer Is :: %d\t",i+1);
                    break;
                }
            }  
            strcat(question_line,"\t:\t");
            sprintf(question_line, "%s%d",question_line,answers[0]);
        }
        else {
            // Case Multi Answer 

            do{
                printf("Your First Answer  :: ");
                char buffer[8];
                fgets(buffer,sizeof(buffer),stdin);
                answers[0] = atoi(buffer);

                printf("Your Second Answer  :: ");
                fgets(buffer,sizeof(buffer),stdin);
                answers[1] = atoi(buffer);       

            }while((answers[0] <1 && answers[0] >3 )||(answers[1] >3 && answers[1] <1));


            studentSCore = studentSCore + q.points[answers[0]-1] + q.points[answers[1]-1];


            printf("Right Answers Are :: ");
            for(int i=0;i<3;i++){
                if(q.points[i]>0){
                    MaxScore += q.points[i]; 
                    if(answers[0]==(i+1) || answers[1]==(i+1)){
                        printf("\t You got this option right\t");
                    }else {
                        printf("\t Forget this option\t");
                    }
                    printf(" : %d ,\t",i+1);
                }
            }
            strcat(question_line,"\t:\t");
            sprintf(question_line, "%s%d",question_line,answers[0]);
            strcat(question_line,"\t:\t");
            sprintf(question_line, "%s%d",question_line,answers[1]);
        }
        
        strcat(question_line,"\t\n\t"); 
        fputs(question_line,answersFile);
        printf("\nnext Question press Enter");
        getchar();
    }
    system("cls");
    printf("%s %s Your Score Is : %d/%d",familyName,firstName,studentSCore,MaxScore);
    fclose(answersFile);
}
