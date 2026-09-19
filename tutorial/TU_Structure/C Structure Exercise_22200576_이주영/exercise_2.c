#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char building_name[100];
    int room_number;
    char room_name[100];
}Handong;

int main() {
    Handong room1, room2, room3;
   
    Handong* room1Pt = &room1;
    Handong* room2Pt = &room2;
    Handong* room3Pt = &room3;


    strcpy(room1.building_name, "Newton");
    room1.room_number   = 109;
    strcpy(room1.room_name, "iiLAB");

    strcpy(room2.building_name, "Newton");
    room2.room_number = 118;
    strcpy(room2.room_name, "Control-Lab");

    strcpy(room3.building_name, "Newton");
    room3.room_number = 119;
    strcpy(room3.room_name, "SW-Lab");
    
    // using structure
    printf("1. using structure\n");
    printf("%s building, room %4d is %s\n", room1.building_name, room1.room_number, room1.room_name);
    printf("%s building, room %4d is %s\n", room2.building_name, room2.room_number, room2.room_name);
    printf("%s building, room %4d is %s\n", room3.building_name, room3.room_number, room3.room_name);

    printf("\n\n");

    // using pointer structure
    printf("2. using pointer structure\n");
    printf("%s building, room %4d is %s\n", room1Pt->building_name, room1Pt->room_number, room1Pt->room_name);
    printf("%s building, room %4d is %s\n", room2Pt->building_name, room2Pt->room_number, room2Pt->room_name);
    printf("%s building, room %4d is %s\n", room3Pt->building_name, room3Pt->room_number, room3Pt->room_name);

    printf("\n\n");

    printf("room1 address=%x ,  roomPt = %x\n", (unsigned)&room1, (unsigned)room1Pt);
    printf("room1 address=%x ,  roomPt = %x\n", (unsigned)&room2, (unsigned)room2Pt);
    printf("room1 address=%x ,  roomPt = %x\n", (unsigned)&room3, (unsigned)room3Pt);

    return 0;
}