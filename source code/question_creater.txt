#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Question Question;

struct Question {
    // Variable that defines type of question :: 1 for True/False , 2 for Single Answer , 3 for Multi Answer
    int type ;
    // setting max count of question text string
    char text[512];
    // array of answer (string) , for True False we won't use this array
    char answer_1[256];
    char answer_2[256];
    char answer_3[256];
    
    // Points for each answer , for True False we use only emlemnts of array 
    int points[3];
};



int main() {
    int question_count = 0;
    printf("Question Txt file Creater \n");
    printf("How Many Question Does File Contains :: ");
    /*Question Count should greater or equals to 1 */
    do {
        char count[8];
        fgets(count,sizeof(count),stdin);
        question_count = atoi(count);
    }while(question_count<=0);


    Question questions[question_count];

    // Reading Question from user Keyboard
    for(int i = 0; i<question_count;i++){

        char type[8];
        char text[512];

        system("cls");

        do {
            printf("Question Type : 1) True/false , 2) Single Answer , 3) Multi Answer  \n");
            fgets(type,sizeof(type),stdin);
            questions[i].type = atoi(type);
        }while(questions[i].type<1 || questions[i].type>3);

        printf("Question Text ::  ");
        fgets(text,sizeof(text),stdin);
        text[strcspn(text, "\n")] = 0;
        strncpy(questions[i].text,text,sizeof(text));

        char answer[256];
        char point[8];
        
        if(questions[i].type == 1){
            // Case of true false question we don't need to read Answers , just Points for true and false
            cond:
            printf("Only One Answer Can have positive Point : \n");
            printf("Question Points : How Many Points for true answer :  \n");
            fgets(point,sizeof(point),stdin);
            questions[i].points[0] = atoi(point);
            printf("Question Points : How Many Points for false answer :  \n");
            fgets(point,sizeof(point),stdin);
            questions[i].points[1] = atoi(point);
            if(questions[i].points[0]>0 &&questions[i].points[2]>0 ){
                goto cond;
            }

        }else {
            printf("Answers Option 1 ::  ");
            fgets(answer,sizeof(answer),stdin);
            answer[strcspn(answer, "\n")] = 0;
            strncpy(questions[i].answer_1,answer,sizeof(answer));

     
            printf("Answers Option 2 ::  ");
            fgets(answer,sizeof(answer),stdin);
            answer[strcspn(answer, "\n")] = 0;
            strncpy(questions[i].answer_2,answer,sizeof(answer));

            printf("Answers Option 3 ::  ");
            fgets(answer,sizeof(answer),stdin);
            answer[strcspn(answer, "\n")] = 0;
            strncpy(questions[i].answer_3,answer,sizeof(answer));

            pointsCond:
            for(int h=0;h<3;h++){
                printf("Point For Option %d  :  ",h+1);
                fgets(point,sizeof(point),stdin);
                questions[i].points[h] = atoi(point);
            }

            // For Any Type Question At least One Question should have a positive points

            if(questions[i].points[0]<=0 && questions[i].points[1]<=0 && questions[i].points[2]<=0){
                printf("At least One Answer Must have positive Point : \n");
                goto pointsCond;
            }
            if(questions[i].points[0]>0 && questions[i].points[1]>0 && questions[i].points[2]>0){
                printf("All Answers Can't be Positive at same time : \n");
                goto pointsCond;
            }
            // single Answer Question Can't have many answers with positibe points
            if(questions[i].type == 1 || questions[i].type == 2 ){
                if(
                    (questions[i].points[0]>0 && questions[i].points[1]>0) ||
                    (questions[i].points[0]>0 && questions[i].points[2]>0 )||
                    (questions[i].points[1]>0 && questions[i].points[2]>0 )
                ){
                    printf("single Answer Question Can't have many answers with positibe points : \n");
                    goto pointsCond;
                }
            }

            // For Multi Answers Question Two Question should have a positive points
            if( questions[i].type == 3 ){
               if(  (questions[i].points[0]<=0 && questions[i].points[1]<=0)|| 
                    (questions[i].points[0]<=0 && questions[i].points[2]<=0)|| 
                    (questions[i].points[1]<=0 && questions[i].points[2]<=0)
                   ){
                    printf("Multi Answers Question Two Question should have a positive points : \n");
                    goto pointsCond;
               }
            }



        }
    }


    system("cls");

    // Writing Question on txt files

    FILE * Question_file = fopen("questionnaire.txt","w") ;


    for(int i = 0; i< question_count;i++){  

        char question_write[1024]="";

        strcat(question_write,questions[i].text);
        strcat(question_write,"\t:\t");
        sprintf(question_write, "%s%d",question_write,questions[i].type);
        strcat(question_write,"\t:\t");

        if(questions[i].type == 1){
            // Writing anwsers Point for true false questions
            // concatenation string with int
            sprintf(question_write, "%s%d",question_write,questions[i].points[0]);
            strcat(question_write,"\t:\t");
            sprintf(question_write, "%s%d",question_write,questions[i].points[1]);
            // strcat(question_write,"\t;");
        }else {
            // Writing anwsers
            strcat(question_write,questions[i].answer_1);
            strcat(question_write,"\t:\t");
            strcat(question_write,questions[i].answer_2);
            strcat(question_write,"\t:\t");
            strcat(question_write,questions[i].answer_3);
            strcat(question_write,"\t:\t");
            
            for(int j=0;j<3;j++){
                sprintf(question_write, "%s%d",question_write,questions[i].points[j]);
                if(j<2){
                    strcat(question_write,"\t:\t");
                }
            }
        }
        
        strcat(question_write,"\n");
        fputs(question_write,Question_file);
    }

    fclose(Question_file);





}