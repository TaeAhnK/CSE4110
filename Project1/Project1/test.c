#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define stunum 21

typedef struct {
    int year;
    int stuid;
    char major[10];
    int semester;
    char status[5];
    char name[10];
    char address[50];
    char mobile[15];
    float grade;
    char circle[15];
    char council;
    char fee;
}STUDENT;

int num, ber;

int callmain();

int callend();

void noone();

void change(STUDENT data[], int a, int b);

void swap(STUDENT data[]) {
    for (num = 0; num < stunum - 1; num++) {
        for (ber = num + 1; ber < stunum; ber++) {
            if (data[num].grade < data[ber].grade) {
                change(data, num, ber);
            }
        }
    }
}

void change(STUDENT data[], int a, int b) {
    STUDENT temp;
    temp = data[a];
    data[a] = data[b];
    data[b] = temp;
}

void printgrade(STUDENT data[], int a, int b) {
    for (num = a; num < b; num++) {
        printf("-----------------------------------------------------------\n");
        printf("|| 이름 | %-8s || 학번 | %-10d || 학점 | %-6.2f ||\n", data[num].name, data[num].stuid, data[num].grade);
    }
    printf("-----------------------------------------------------------\n");
}

void stuinfo(STUDENT data[]);

void deanslist(STUDENT data[]) {
    int m = 0;
    char M[10];
    STUDENT MAJOR[stunum] = { 0 };
    //printf("<%s>", MAJOR[7].address);
    printf("\n학과별 상위 35%% 학생 명단을 출력합니다.\n학과명을 입력하세요(보기 : 물리학, 수학, 컴퓨터공학, 경제학, 아텍, 유럽문화) : ");
    scanf("%s", M);
    //printf("%s", M);
    for (num = 0; num < stunum; num++) {
        if (strcmp(data[num].major, M) == 0) {
            MAJOR[m] = data[num];
            m += 1;
            printf("%s\n", data[num].name);
        }
    }
    printf("%d", m);
    if (m == 0) printf("\n해당 학과는 존재하지 않습니다.\n");
    else {
        swap(MAJOR);
        printf("\n\n%s 상위 35% 학생 명단입니다.");

    }
}

void scholarship(STUDENT data[]);

void election(STUDENT data[]);

void stucouncil(STUDENT data[]);

void warning(STUDENT data[]);

void youngest(STUDENT data[]);

int main() {
    int i = 2;
    int j = 0;
    int res;
    FILE* ifp;
    STUDENT data[stunum];
    ifp = fopen("STUDENTINFORMATION.txt", "r");
    if (ifp == NULL) {
        printf("input file open error!\n");
        return 1;
    }
    while (1) {
        if (j == stunum) break;
        fscanf(ifp, "%d %d %[^;] %*c %d %[^;] %*c %[^;] %*c", &data[j].year, &data[j].stuid, data[j].major, &data[j].semester, data[j].status, data[j].name);
        fscanf(ifp, "%*c %[^;] %*c %[^;] %*c %f %[^;] %*c %c %c", data[j].address, data[j].mobile, &data[j].grade, data[j].circle, &data[j].council, &data[j].fee);
        j++;
    }
    fclose(ifp);

    while (i == 2) {
        num = callmain();
        switch (num) {
        case 1: stuinfo(data); break;
        case 2: deanslist(data); break;
        case 3: scholarship(data); break;
        case 4: election(data); break;
        case 5: stucouncil(data); break;
        case 6: warning(data); break;
        case 7: youngest(data); break;
        default: printf("\n1에서 7 사이의 자연수를 입력해주세요.\n");
        }
        i = callend();
    }
    return 0;
}

int callmain() {
    //system("cls");
    printf("<메뉴>\n\n(1) 학생별 정보\n(2) 딘스 리스트\n(3) 성적 장학금\n(4) 선거인명부\n(5) 학생회\n(6) 학사 경고 대상자\n(7) 최연소 학생");
    printf("\n\n번호를 입력하세요 : ");
    scanf("%d", &num);
    return num;
}

int callend() {
    printf("\n\n돌아가시겠습니까?\n(1) 종료\n(2) 돌아가기");
    printf("\n\n번호를 입력하세요 : ");
    scanf("%d", &num);
    return num;
}

void noone() {
    printf("\n해당하는 학생이 존재하지 않습니다.\n");
    return;
}

void stuinfo(STUDENT data[]) {
    int id;
    ber = 0;
    printf("\n선택한 학생의 정보를 출력합니다.");
    printf("\n학번을 입력하세요 : ");
    scanf("%d", &id);
    for (num = 0; num < stunum; num++) {
        if (data[num].stuid == id) {
            printf("\n-----------------------------------------------------------------\n");
            printf("||  이름  | %-8s || 전공 | %-10s || 수료학기 | %-6d ||\n", data[num].name, data[num].major, data[num].semester);
            printf("-----------------------------------------------------------------\n");
            printf("|| 동아리 | %-8s || 주소 | %-31s ||\n", data[num].circle, data[num].address);
            printf("-----------------------------------------------------------------\n");
            printf("||  학점  | %-8.2f || 번호 | %-31s ||\n", data[num].grade, data[num].mobile);
            ber += 1;
        }
    }
    if (ber == 0) noone();
    else printf("-----------------------------------------------------------------\n");
    return;
}

void scholarship(STUDENT data[]) {
    int fifty, twenty;
    printf("\n장학금 대상자 명단입니다.\n");
    swap(data);
    /*for(num=0 ; num<stunum ; num++){
       for(ber=0 ; ber<stunum-num-1 ; ber++){
          if(data[ber].grade<data[ber+1].grade){
             change(data, ber, ber+1);
          }
       }
    }*/
    printf("\n50%% 장학금 수혜자(상위 5%%)\n");
    fifty = stunum * 0.05;
    printgrade(data, 0, fifty);
    /*for(num=0 ; num<fifty ; num++){
       printf("-----------------------------------------------------------\n");
       printf("|| 이름 | %-8s || 학번 | %-10d || 학점 | %-6.2f ||\n", data[num].name, data[num].stuid, data[num].grade);
    }
    printf("-----------------------------------------------------------\n");*/
    printf("\n20%% 장학금 수혜자(상위 20%%)\n");
    twenty = stunum * 0.2;
    printgrade(data, fifty, twenty);
    /*for(num=fifty ; num<twenty ; num++){
       printf("-----------------------------------------------------------\n");
       printf("|| 이름 | %-8s || 학번 | %-10d || 학점 | %-6.2f ||\n", data[num].name, data[num].stuid, data[num].grade);
    }
    printf("-----------------------------------------------------------\n");*/
    return;
}

void election(STUDENT data[]) {
    ber = 0;
    printf("\n선거가 가능한 학생의 명단입니다.\n\n");
    for (num = 0; num < stunum; num++) {
        if (strcmp(data[num].status, "재학") == 0 || data[num].fee == 'O') {
            printf("---------------------------------------------------------------\n");
            printf("|| 이름 | %-8s || 학번 | %-10d || 학적상태 | %-6s ||\n", data[num].name, data[num].stuid, data[num].status);
            ber += 1;
        }
    }
    if (ber == 0) noone();
    else printf("---------------------------------------------------------------\n");
    return;
}

void stucouncil(STUDENT data[]) {
    ber = 0;
    printf("\n학생회 명단입니다.\n\n");
    for (num = 0; num < stunum; num++) {
        if (data[num].council == 'O') {
            printf("---------------------------------------------------------------\n");
            printf("|| 이름 | %-8s || 학번 | %-10d || 학적상태 | %-6s ||\n", data[num].name, data[num].stuid, data[num].status);
            ber += 1;
        }
    }
    if (ber == 0) noone();
    else printf("---------------------------------------------------------------\n");
    return;
}

void warning(STUDENT data[]) {
    ber = 0;
    printf("\n학사경고 대상자 학생의 명단입니다. (학점이 2.00 미만)\n\n");
    for (num = 0; num < stunum; num++) {
        if (data[num].grade < 2.0) {
            printf("-----------------------------------------------------------\n");
            printf("|| 이름 | %-8s || 학번 | %-10d || 학점 | %-6.2f ||\n", data[num].name, data[num].stuid, data[num].grade);
            ber += 1;
        }
    }
    if (ber == 0) noone();
    else printf("-----------------------------------------------------------\n");
    return;
}

void youngest(STUDENT data[]) {
    printf("\n가장 나이가 어린 학생의 명단입니다.\n\n");
    ber = data[0].year;
    for (num = 1; num < stunum; num++) {
        if (data[num].year > ber) {
            ber = data[num].year;
        }
    }
    for (num = 0; num < stunum; num++) {
        if (data[num].year == ber) {
            printf("---------------------------------------------------------------\n");
            printf("|| 출생년도 | %-6d || 이름 | %-8s || 학번 | %-10d ||\n", data[num].year, data[num].name, data[num].stuid);
        }
    }
    printf("---------------------------------------------------------------\n");
}